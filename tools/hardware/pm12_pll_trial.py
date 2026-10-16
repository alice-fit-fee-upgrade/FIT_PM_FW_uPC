#!/usr/bin/env python3
"""Explicit application-only PLL migration trial, with physical backup/rollback.

Requires prior software qualification and user-authorized PM12 programming.
Holds both board leases through programming, tests and FPGA SRAM restoration.
Never writes boot/EEPROM/fuses/user signature/PLL EEPROM/configuration flash.
The FPGA restoration is deliberately pinned to the current integrated image.
"""
import argparse
import contextlib
import fcntl
import hashlib
import json
import re
import signal
import subprocess
import sys
import time
from pathlib import Path

from pm12_console_regression import PORT, capture
from pm12_pll_readback import parse_pll_all
from pm12_power import wait_power_good, require_power_good

ROOT = Path(__file__).resolve().parents[2]
MEM = ['flash', 'boot', 'eeprom', 'fuse0', 'fuse1', 'fuse2', 'fuse4',
       'fuse5', 'lock', 'usersig', 'prodsig', 'signature']
BASE = ['avrdude', '-c', 'atmelice_pdi', '-p', 'x128a3u',
        '-P', 'usb:J41800043065', '-B', '10', '-u']
BIT_SHA = '5b9c5fb30722a49cb8fdeea524f7541e0b10dce1e575b015a5d5e43d6ff6d5d4'
VIVADO = '/srv/codex-hil-data/toolchains/amd/Xilinx/2025.2/Vivado/bin/'
DOCKER = ['docker', '-H', 'unix:///run/user/1001/docker-vivado.sock']


def sha(data):
    return hashlib.sha256(data).hexdigest()


@contextlib.contextmanager
def console():
    import serial
    uart = serial.Serial(port=None, baudrate=115200, timeout=.03,
                         write_timeout=1, rtscts=True, exclusive=True)
    uart.dtr = False
    uart.rts = True
    uart.port = PORT
    uart.open()
    try:
        uart.read(4096)
        yield uart
    finally:
        uart.rts = False
        uart.dtr = False
        uart.close()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', required=True, type=Path)
    parser.add_argument('--expected-running-sha256', required=True)
    parser.add_argument('--fpga-source', required=True, type=Path,
                        help='diagnostics/pm12 directory of qualified FPGA source')
    args = parser.parse_args()
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    candidate = (ROOT/'development/build/development.bin').read_bytes()
    assert len(candidate) == 0x22000
    software = json.loads((ROOT/'development/build/pll_legacy_validation.json').read_text())
    layout = json.loads((ROOT/'development/build/layout_report.json').read_text())
    assert software['status'] == 'PASS' and layout['development_sha256'] == sha(candidate)
    assert layout['golden_sha256'] == sha((ROOT/'reference/flash_golden.bin').read_bytes())
    fpga = args.fpga_source.resolve()
    bit = fpga/'tdc_buffers/generated/board/build/pm12_ipbus_adc.bit'
    assert sha(bit.read_bytes()) == BIT_SHA, 'unqualified FPGA restore image'
    sys.path[:0] = [str(fpga), str(fpga/'ipbus_adc')]
    from test_board import payload
    from client import Client
    bits = payload(bit, out/'fpga-payload.hex')
    tcl = (fpga/'adc/tools/load_fpga.tcl').read_text().replace(
        'source [file join [file dirname [info script]] common.tcl]',
        'source {'+str(fpga/'adc/tools/common.tcl')+'}').replace('0x504dad01', '0x504d5a04')
    (out/'restore-fpga.tcl').write_text(tcl)
    # Keep the alias in this repository; the bounded app view must be reviewable.
    conf = ROOT/'tools/hardware/pdi-application-only.conf'
    assert conf.is_file()
    report = {'status': 'IN_PROGRESS', 'candidate_sha256': sha(candidate),
              'source_commit': subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
              'fpga_bit_sha256': BIT_SHA, 'configuration_flash_written': False,
              'explicit_PLL_configuration_or_EEPROM_console_commands': False,
              'unchanged_startup_can_replay_PLL_RAM_profile': True, 'chip_erase': False,
              'software_qualification': software, 'layout': layout,
              'excluded': ['PLL EEPROM copy PC', 'PLL profile/reset changes',
                           'power/fault injection', 'clock frequency/jitter proof',
                           'asynchronous IRQ/whole-firmware timing proof']}
    container = None
    attempted = False
    pdi_touched = False
    old = None

    def save():
        (out/'report.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(command, log, timeout=180):
        print('RUN '+log.name, flush=True)
        with log.open('w') as stream:
            subprocess.run(command, stdout=stream, stderr=subprocess.STDOUT,
                           check=True, timeout=timeout)

    def reads(dest):
        return sum((['-U', m+':r:'+str(dest/(m+'.bin'))+':r'] for m in MEM), [])

    def image(dest):
        raw = (dest/'flash.bin').read_bytes()
        assert len(raw) <= 0x22000
        return raw.ljust(0x22000, b'\xff')

    def protected(dest):
        result = {m: (dest/(m+'.bin')).read_bytes() ==
                  (out/'before-memory'/(m+'.bin')).read_bytes() for m in MEM[1:]}
        assert all(result.values()), result
        return result

    def program(data, dest):
        dest.mkdir()
        app = dest/'application-full.bin'
        app.write_bytes(data[:0x20000])
        command = ['avrdude', '-C', '+'+str(conf), '-c', 'atmelice_pdi',
                   '-p', 'pm12_full_app', '-P', 'usb:J41800043065', '-B', '10', '-u',
                   '-U', 'app_full:w:'+str(app)+':r']
        run(command+reads(dest), dest/'program-readback.log')
        assert image(dest) == data, 'full canonical readback differs'
        return protected(dest)

    def fpga_identity():
        client = Client()
        try:
            identity = client.read(0, 3)
            assert identity == [0x504d1202, 0x10002, 0x4bbe9dfc], identity
            return identity
        finally:
            client.close()

    def restore_fpga(dest):
        nonlocal container
        # Qualify PGOOD promptly after PDI, before the slower FPGA reload.
        with console() as uart:
            report.setdefault('post_pdi_power_good', []).append(wait_power_good(uart, capture))
        if container is None:
            command = DOCKER+['run', '-d', '--rm', '--init', '--name',
                'pm12-pll-trial-'+str(int(time.time())),
                '--mount', 'type=bind,src=/dev/bus/usb,dst=/dev/bus/usb',
                '--mount', 'type=bind,src=/home/codex-hil,dst=/home/codex-hil',
                '--mount', 'type=bind,src=/srv/codex-hil-data,dst=/srv/codex-hil-data',
                'codex-vivado-runtime:ubuntu22.04', VIVADO+'hw_server',
                '-e', 'set jtag-port-filter 210249A73E43']
            container = subprocess.check_output(command, text=True, timeout=30).strip()
            time.sleep(1)
        run(DOCKER+['exec', '-e', 'LD_PRELOAD=/lib/x86_64-linux-gnu/libudev.so.1',
                    container, 'timeout', '150', VIVADO+'vivado', '-mode', 'batch',
                    '-nolog', '-nojournal', '-notrace', '-source', str(out/'restore-fpga.tcl'),
                    '-tclargs', '210249A73E43', 'localhost:3121',
                    str(out/'fpga-payload.hex'), str(bits)], dest/'fpga-reload.log', 170)
        assert 'LOAD_PASS' in (dest/'fpga-reload.log').read_text()
        return fpga_identity()

    def runtime(extended):
        result = {'extended': extended, 'commands': [], 'maps': []}
        # Retain partial captures even when a later acceptance assertion fails.
        report.setdefault('runtime_attempts', []).append(result)
        with console() as uart:
            result['power_good'] = wait_power_good(uart, capture)
            def read(command):
                if command.startswith(('@TEMP', '@INA', '@PLL')):
                    result.setdefault('power_rechecks', []).append(require_power_good(uart, capture))
                row = capture(uart, command)
                result['commands'].append(row)
                print(json.dumps(row), flush=True)
                assert row['reply'] and not row['reply'].startswith('ERR '), row
                return row['reply']
            assert read('@PING') == 'PONG\r\n'
            for _ in range(3):
                status = read('RS')
                assert 'Board power ON, OK' in status and ' NORMAL' in status and 'PLL LOCKED' in status, status
                pll = read('@PLL')
                assert re.fullmatch(r'CDCE62005 [0-9A-F]{8}\r\n', pll), pll
                if extended:
                    assert read('@PLL LEGACY') == pll
                    values = parse_pll_all(read('@PLL ALL'))
                    assert values[8] == int(pll.split()[1], 16)
                    result['maps'].append(values)
            result['adt'] = read('@TEMP ALL')
            assert 'ADT7311 REG=03 C3\r\n' in result['adt'], ('ADT ID failed', result['adt'])
            for route in [4, 5, 6]:
                # BUSY is an expected ownership result; bounded retries only.
                for attempt in range(4):
                    result.setdefault('power_rechecks', []).append(require_power_good(uart, capture))
                    row = capture(uart, '@INA '+str(route));result['commands'].append(row)
                    print(json.dumps(row), flush=True)
                    if row['reply'] != 'ERR BUSY\r\n': break
                    time.sleep(.1)
                assert 'CONFIG=7127' in row['reply'] and 'ERR ' not in row['reply'], row
            read('@CFG')
            read('@FPGA')
            assert read('@FPGATEST') == 'FPGA TEST PASS 32 RESTORED\r\n'
            if extended:
                assert all(all(m[r] == result['maps'][0][r] for r in range(8)) for m in result['maps'])
        return result

    def stop(signum, frame):
        raise InterruptedError('termination signal '+str(signum))

    with contextlib.ExitStack() as stack:
        for name in sorted(['pm12-210249A73E43', 'atmel-ice-J41800043065']):
            lock = stack.enter_context(open(str(Path.home()/'.cache'/(name+'.lock')), 'a+'))
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        signal.signal(signal.SIGTERM, stop)
        try:
            report['before_fpga'] = fpga_identity()
            report['before_runtime'] = runtime(False)
            save()
            before = out/'before-memory';before.mkdir()
            pdi_touched = True
            run(BASE+reads(before), before/'readback.log')
            assert (before/'signature.bin').read_bytes() == bytes.fromhex('1e9742')
            old = image(before)
            (before/'flash-full.bin').write_bytes(old)
            report['before_sha256'] = sha(old)
            assert sha(old) == args.expected_running_sha256, 'unexpected physical running firmware'
            assert old[0x20000:] == candidate[0x20000:], 'bootloader differs'
            save()
            attempted = True
            report['protected_unchanged'] = program(candidate, out/'candidate-memory')
            report['candidate_readback_sha256'] = sha(image(out/'candidate-memory'))
            report['after_fpga'] = restore_fpga(out/'candidate-memory')
            report['after_runtime'] = runtime(True)
            # Ignore only measured temperature/status, not configuration changes.
            def settings(reply):
                values = {int(k,16):int(v,16) for k,v in re.findall(r'ADT7311 REG=([0-9A-F]{2}) ([0-9A-F]+)', reply)}
                assert set(values) == set(range(8))
                return {str(r):values[r] for r in [1,3,4,5,6,7]}
            report['adt_configuration_before'] = settings(report['before_runtime']['adt'])
            report['adt_configuration_after'] = settings(report['after_runtime']['adt'])
            assert report['adt_configuration_before'] == report['adt_configuration_after'], 'ADT configuration changed across reset'
            report['status'] = 'PASS'
            report['candidate_left_running'] = True
        except BaseException as error:
            report['status'] = 'FAIL'
            report['error'] = repr(error)
            save()
            # Even an unsuccessful read-only PDI session resets the MCU/FPGA.
            try:
                if attempted:
                    report['rollback_protected'] = program(old, out/'rollback-memory')
                    report['rollback_sha256'] = sha(old)
                if pdi_touched:
                    dest = out/'rollback-runtime';dest.mkdir()
                    report['rollback_fpga'] = restore_fpga(dest)
                    report['rollback_runtime'] = runtime(False)
                report['candidate_left_running'] = False
            except BaseException as rollback:
                report['rollback_error'] = repr(rollback)
            raise
        finally:
            if container:
                try:
                    subprocess.run(DOCKER+['stop', container], check=True, timeout=30,
                                   stdout=subprocess.DEVNULL)
                except Exception as error:
                    report['private_server_stop_error'] = repr(error)
            save()


if __name__ == '__main__':
    main()
