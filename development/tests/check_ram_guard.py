#!/usr/bin/env python3
"""A new uninitialized SRAM global must fail this no-CRT extension link."""
import json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];BASE=ROOT/'recovered_exact';BUILD=ROOT/'development/build'
source=BUILD/'forbidden_global.c';obj=BUILD/'forbidden_global.o'
source.write_text('volatile unsigned char forbidden_extension_global;\n')
subprocess.run(['avr-gcc','-mmcu=atxmega128a3u','-c',str(source),'-o',str(obj)],check=True)
objects=json.loads((BUILD/'link_objects.json').read_text())
args=['avr-gcc','-mmcu=atxmega128a3u','-nostdlib','-Wl,-T,'+str(BUILD/'development.ld'),*objects,str(obj),'-lgcc','-o',str(BUILD/'forbidden.elf')]
result=subprocess.run(args,cwd=BASE,text=True,capture_output=True)
assert result.returncode and 'unplaced code/data/runtime support' in result.stderr, result.stderr
# A forbidden .bss global also pulls CRT clear support, whose bounds are
# deliberately absent. Reject unrelated missing symbols as false positives.
missing=re.findall(r"undefined reference to `([^']+)'",result.stderr)
assert set(missing)<= {'__bss_start','__bss_end'},result.stderr
(BUILD/'ram_guard_rejection.txt').write_text(result.stderr)
print('PASS RAM guard: uninitialized global rejected by linker assertion')
