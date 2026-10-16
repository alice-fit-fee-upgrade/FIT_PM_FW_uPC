#!/usr/bin/env python3
"""Bounded UART PLL readback. No firmware/FPGA programming, PC, reset or EEPROM writes."""
import argparse
import contextlib
import fcntl
import json
import re
from pathlib import Path
from pm12_console_regression import PORT, capture
from pm12_power import wait_power_good, require_power_good

def parse_pll_all(reply):
    values = {}
    for line in reply.splitlines():
        match = re.fullmatch(r'CDCE62005 REG=([0-9A-F]{2}) ([0-9A-F]{8})', line)
        if not match:
            raise ValueError('unexpected PLL map reply: '+repr(line))
        reg, value = (int(x, 16) for x in match.groups())
        if reg > 8 or reg in values:
            raise ValueError('invalid/duplicate PLL register')
        values[reg] = value
    if set(values) != set(range(9)):
        raise ValueError('incomplete PLL register map')
    return values

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', required=True, type=Path)
    parser.add_argument('--all', action='store_true', help='requires firmware with @PLL ALL')
    args = parser.parse_args()
    if args.out.exists():
        raise FileExistsError(args.out)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    report = {'scope': 'UART read commands only; no reset, programming or EEPROM copy',
              'commands': [], 'maps': [], 'writes_to_configuration': False}
    import serial
    with contextlib.ExitStack() as stack:
        for name in sorted(['pm12-210249A73E43', 'atmel-ice-J41800043065']):
            lock = stack.enter_context(open(str(Path.home()/'.cache'/(name+'.lock')), 'a+'))
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        uart = serial.Serial(port=None, baudrate=115200, timeout=.03, write_timeout=1,
                             rtscts=True, exclusive=True)
        uart.dtr = False
        uart.rts = True
        uart.port = PORT
        uart.open()
        def read(command):
            row = capture(uart, command)
            report['commands'].append(row)
            print(json.dumps(row), flush=True)
            return row['reply']
        try:
            uart.read(4096)
            assert read('@PING') == 'PONG\r\n'
            report['power_good'] = wait_power_good(uart, capture)
            # GPIO-only view; records actual INIT/PROGRAM state without changing it.
            read('@CFG')
            for _ in range(3):
                report.setdefault('power_rechecks', []).append(require_power_good(uart, capture))
                assert 'PLL LOCKED' in read('RS')
                assert re.fullmatch(r'CDCE62005 [0-9A-F]{8}\r\n', read('@PLL'))
                if args.all:
                    report['maps'].append(parse_pll_all(read('@PLL ALL')))
            report['power_rechecks'].append(require_power_good(uart, capture))
            assert 'ADT7311 REG=03 C3\r\n' in read('@TEMP ALL')
            # No claim that status register8 is constant. Record configuration
            # changes separately; do not hide differences with guessed bit masks.
            if args.all:
                report['configuration_maps_equal'] = all(
                    all(v[r] == report['maps'][0][r] for r in range(8))
                    for v in report['maps'][1:])
            report['status'] = 'PASS_READBACK_ONLY'
        except Exception as error:
            report['status'] = 'FAIL'
            report['error'] = repr(error)
            raise
        finally:
            uart.rts = False
            uart.dtr = False
            uart.close()
            args.out.write_text(json.dumps(report, indent=2)+'\n')

if __name__ == '__main__':
    main()
