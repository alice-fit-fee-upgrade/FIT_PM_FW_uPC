#!/usr/bin/env python3
"""Split verified symbolic ASM around accepted C regions; never read golden bytes."""
from pathlib import Path
import json,re,subprocess,sys
ROOT=Path(__file__).resolve().parents[2]
PROJECT=ROOT/'recovered_exact'
manifest=json.loads((PROJECT/'manifest.json').read_text())
accepted=sorted(manifest['accepted'],key=lambda r:r['address'])
output=PROJECT/'generated';output.mkdir(exist_ok=True)
subprocess.run([sys.executable,str(ROOT/'tools/convert_analysis.py'),'--output',str(output/'application_exact.S')],check=True)
lines=(output/'application_exact.S').read_text().splitlines()
(PROJECT/'build').mkdir(exist_ok=True)
subprocess.run(['avr-gcc','-mmcu=atxmega128a3u','-I'+str(ROOT/'asm_analysis'),'-c',str(output/'application_exact.S'),'-o',str(PROJECT/'build/application_exact.o')],check=True)
original_symbols={}
for line in subprocess.check_output(['avr-nm','-n',str(PROJECT/'build/application_exact.o')],text=True).splitlines():
 m=re.match(r'([0-9a-f]+)\s+[tT]\s+(\S+)',line)
 if m:original_symbols[m[2]]=int(m[1],16)

orgs={}
for index,line in enumerate(lines):
 m=re.match(r'\s*\.org\s+\(?(0x[0-9a-fA-F]+)(\s*<<\s*1)?\)?\s*,',line)
 if m:orgs[int(m[1],16)*(2 if m[2] else 1)]=index
skips={}
last_end=0
for row in accepted:
 assert last_end<=row['address']<row['end_exclusive']<=manifest['application_end']
 start,end=orgs[row['address']],orgs[row['end_exclusive']]
 assert re.search(r'^'+re.escape(row['symbol'])+r':', '\n'.join(lines[start:end]),re.M)
 # All removed internal symbols are recreated by the linker at their exact offsets.
 row['labels']=[(name,address-row['address']) for name,address in original_symbols.items() if row['address']<=address<row['end_exclusive'] and name!=row['symbol']]
 skips[start]=(end,row['end_exclusive'])
 last_end=row['end_exclusive']
result=['.global '+name for name in manifest.get('asm_exports',[])];regions=[];base=0;index=0
while index<len(lines):
 if index in skips:
  end,next_base=skips[index]
  region_end=next(r['address'] for r in accepted if r['end_exclusive']==next_base)
  if base<region_end:
   regions.append({'start':base,'end_exclusive':region_end})
  base=next_base;index=end
  result.append(f'.section .application_{base:06x},"ax",@progbits')
  continue
 line=lines[index]
 if line.startswith('.section .application,'):
  line=f'.section .application_{base:06x},"ax",@progbits'
 elif re.match(r'\s*\.org\s+',line):
  m=re.match(r'\s*\.org\s+(.+),\s*(0x[0-9a-fA-F]+)\s*$',line)
  assert m,line
  line=f'.org ({m[1]}) - 0x{base:x}, {m[2]}'
 result.append(line);index+=1
regions.append({'start':base,'end_exclusive':manifest['application_end']})
(output/'application.S').write_text('\n'.join(result)+'\n')
sections=[]
for region in regions:
 name=f'.application_{region["start"]:06x}'
 sections.append((region['start'],name,name,region['end_exclusive']-region['start']))
for row in accepted:
 sections.append((row['address'],f'.c_{row["address"]:06x}',row['section'],row['end_exclusive']-row['address']))
link=[]
for address,name,input_section,size in sorted(sections):
 link.append(f' {name} 0x{address:x} : {{ KEEP(*({input_section})) }} > flash')
for row in accepted:
 for name,offset in row['labels']:
  link.append(f' {name} = ADDR(.c_{row["address"]:06x}) + 0x{offset:x};')
link+=[' .boot 0x20000 : { KEEP(*(.boot)) } > flash',
 ' .unexpected : { *(.text*) *(.data*) *(.rodata*) *(.bss*) *(COMMON) } > flash',
 ' /DISCARD/ : { *(.comment) *(.note*) }']
(output/'sections.ld').write_text('\n'.join(link)+'\n')
assertions=[]
for address,name,input_section,size in sorted(sections):
 assertions.append(f'ASSERT(SIZEOF({name}) == 0x{size:x}, "exact region size: {name}")')
assertions+=['ASSERT(SIZEOF(.boot) == 0x4e6, "boot size changed")',
 'ASSERT(SIZEOF(.unexpected) == 0, "unplaced code/data/runtime support")']
(output/'assertions.ld').write_text('\n'.join(assertions)+'\n')
(output/'regions.json').write_text(json.dumps({'asm_regions':regions,'accepted':accepted},indent=2)+'\n')
