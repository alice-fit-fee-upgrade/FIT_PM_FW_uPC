#!/usr/bin/env python3
"""A new uninitialized SRAM global must fail this no-CRT extension link."""
import shlex,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];BASE=ROOT/'recovered_exact';BUILD=ROOT/'development/build'
source=BUILD/'forbidden_global.c';obj=BUILD/'forbidden_global.o'
source.write_text('volatile unsigned char forbidden_extension_global;\n')
subprocess.run(['avr-gcc','-mmcu=atxmega128a3u','-c',str(source),'-o',str(obj)],check=True)
plan=subprocess.check_output(['make','-B','-n','build/reconstructed.elf'],cwd=BASE,text=True)
command=next(shlex.split(line) for line in plan.splitlines() if line.startswith('avr-gcc ') and ' -nostdlib ' in line)
original=[str(BASE/a) for a in command if a.endswith('.o')]
args=['avr-gcc','-mmcu=atxmega128a3u','-nostdlib','-Wl,-T,'+str(BUILD/'development.ld'),*original,
      str(BUILD/'extension.o'),str(BUILD/'legacy_bridge.o'),str(obj),'-lgcc','-o',str(BUILD/'forbidden.elf')]
result=subprocess.run(args,cwd=BASE,text=True,capture_output=True)
assert result.returncode and 'unplaced code/data/runtime support' in result.stderr, result.stderr
(BUILD/'ram_guard_rejection.txt').write_text(result.stderr)
print('PASS RAM guard: uninitialized global rejected by linker assertion')
