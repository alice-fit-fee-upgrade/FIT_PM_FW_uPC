"""Known passive FPGA instrument for a caller already holding PM12+ICE leases.
Only SRAM loading and bounded, read-only GPIO/ADT console capture. No MCU writes.
"""
import csv
import hashlib
import json
import re
import subprocess
import time
from fpga_analyzer.run_capture import DOCKER, HERE, OLD, payload
from pm12_console_regression import capture
from pm12_power import wait_power_good, require_power_good

class PassiveAnalyzer:
    def __init__(self, directory):
        self.directory = directory.resolve()
        self.directory.mkdir(exist_ok=False)
        self.bit = HERE/'build/pm12_analyzer.bit'
        expected = json.loads((HERE.parents[2]/'docs/validation/fpga-analyzer-2026-10-15/report.json').read_text())['fpga_sha256']
        assert hashlib.sha256(self.bit.read_bytes()).hexdigest() == expected
        self.vector = self.directory/'analyzer.hex'
        self.bits = payload(self.bit, self.vector)
        template = (OLD/'load_passive_fpga.tcl').read_text()
        old_guard = 'if {[expr {("0x$baseline") & 0xffffffff}] != 0x00000000}'
        new_guard = 'if {[expr {("0x$baseline") & 0xffffffff}] == 0x504d5a03} {puts "LOAD_PASS analyzer already present"; pm12_close; exit}\nif {[expr {("0x$baseline") & 0xffffffff}] != 0 && [expr {("0x$baseline") & 0xffffffff}] != 0x504d5a02}'
        assert old_guard in template
        template = template.replace(old_guard, new_guard).replace('!= 0x504d5a02} {error "Loaded', '!= 0x504d5a03} {error "Loaded')
        self.loader = self.directory/'load.tcl'
        self.loader.write_text(template)

    def load(self, log):
        with log.open('w') as output:
            subprocess.run(DOCKER+[str(self.loader), '-tclargs', '210249A73E43', 'localhost:3121', str(self.vector), str(self.bits)], stdout=output, stderr=subprocess.STDOUT, timeout=145, check=True)
        assert 'LOAD_PASS' in log.read_text()

    def observe(self, uart, name, commands=None):
        dest = self.directory/name
        dest.mkdir(exist_ok=False)
        commands = commands or ['@TEMP', '@HW', 'RS']
        result = {'sensor_register_writes': '@TEMP RESTORE' in commands, 'commands': []}
        process = None
        try:
            result['power_ready'] = wait_power_good(uart, capture)
            with (dest/'capture.log').open('w') as log:
                process = subprocess.Popen(DOCKER+[str(HERE/'capture.tcl'), '-tclargs', '210249A73E43', 'localhost:3121', str(dest/'armed'), str(self.bits)], stdout=log, stderr=subprocess.STDOUT)
                deadline = time.monotonic()+30
                while not (dest/'armed').exists():
                    if process.poll() is not None or time.monotonic()>deadline:
                        raise RuntimeError('analyzer did not arm')
                    time.sleep(.05)
                result['power_recheck'] = require_power_good(uart, capture)
                for command in commands:
                    if command.startswith('@TEMP'):
                        result.setdefault('power_rechecks', []).append(require_power_good(uart, capture))
                    result['commands'].append(capture(uart, command))
                process.wait(timeout=90)
                assert process.returncode == 0
            text = (dest/'capture.log').read_text()
            assert 'CAPTURE_PASS' in text
            rate = float(re.search(r'^CLOCK (.+)$', text, re.M)[1])
            rows = [tuple(map(int, item)) for item in re.findall(r'^EVENT (\d+) (\d+) (\d+)$', text, re.M)]
            assert rows and all(b[0]==a[0]+1 and b[1]>a[1] for a,b in zip(rows,rows[1:]))
            with (dest/'events.csv').open('w') as f:
                writer = csv.writer(f, lineterminator='\n')
                writer.writerow(['event','cycles','seconds','SDA','SCL','RESET_N'])
                for i,t,v in rows: writer.writerow([i,t,t/rate,v&1,(v>>1)&1,(v>>2)&1])
            with (dest/'capture.vcd').open('w') as f:
                f.write('$timescale 1ns $end\n$scope module pm12 $end\n$var wire 1 ! SDA $end\n$var wire 1 " SCL $end\n$var wire 1 # RESET_N $end\n$upscope $end\n$enddefinitions $end\n')
                for i,t,v in rows:
                    f.write(f'#{round(t/rate*1e9)}\n')
                    for n,symbol in enumerate('!"#'): f.write(f'{(v>>n)&1}{symbol}\n')
            result.update(capture='PASS', events=len(rows), sample_hz=rate, adt_id_valid=any('REG=03 C3' in row['reply'] for row in result['commands']))
            return result
        except Exception as error:
            result['error'] = repr(error)
            raise
        finally:
            if process is not None and process.poll() is None:
                try: process.wait(timeout=15)
                except subprocess.TimeoutExpired:
                    process.terminate()
                    process.wait(timeout=5)
            (dest/'report.json').write_text(json.dumps(result, indent=2)+'\n')
