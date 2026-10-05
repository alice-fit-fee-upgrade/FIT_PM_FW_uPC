#!/usr/bin/env python3
"""Inventory the accepted exact baseline; no behavioral-equivalence acceptance."""
from collections import Counter
from pathlib import Path
import hashlib,json,re,subprocess
ROOT=Path(__file__).resolve().parents[2]
PROJECT=ROOT/'recovered_exact'
build=PROJECT/'build'
golden=(ROOT/'reference/flash_golden.bin').read_bytes()
rebuilt=(build/'flash_rebuilt.bin').read_bytes()
assert len(golden)==len(rebuilt)==0x22000
assert rebuilt==golden,'Cannot classify a differing image as the exact baseline'
manifest=json.loads((PROJECT/'manifest.json').read_text())
source=(PROJECT/'generated/application_exact.S').read_text()
entries=[(int(a,16)*2,name.strip()) for a,name in re.findall(
 r'^;[^\n]*FUN_code_[^\n]*\n\.org \((0x[0-9a-f]+) << 1\), 0xff\n([^:\n]+):',source,re.M)]
assert len(entries)==len({a for a,_ in entries})==89
accepted={row['address']:row for row in manifest['accepted']}
assert len(accepted)==len(manifest['accepted'])
assert set(accepted)<={a for a,_ in entries}
index=json.loads((ROOT/'docs/instruction_index.json').read_text())
code=set();boundaries=set()
for row in index:
 address=row['address'];data=bytes.fromhex(row['bytes'])
 assert golden[address:address+len(data)]==data,'stale instruction index'
 boundaries.update((address,address+len(data)))
 code.update(range(address,address+len(data)))
assert len(code)==10836
entry_addresses={address for address,_ in entries}
application_targets=set()
for row in index:
 if row['address']>=0x2916:
  continue
 text=row['instruction']
 if re.match(r'(call|rcall)\s',text) or row['address']<0x1e2:
  target=re.search(r';\s*0x([0-9a-f]+)',text);assert target,text
  address=int(target[1],16)
  if address<0x20000:application_targets.add(address)
assert application_targets<=entry_addresses,('unlisted original application entry',application_targets-entry_addresses)

compiler_code=set();helpers=set();functions=[]
for n,(address,name) in enumerate(entries):
 end=entries[n+1][0] if n+1<len(entries) else 0x2916
 row=accepted.get(address)
 state=row['classification'] if row else 'ASM_EXACT'
 assert state in ('C_BINARY_EXACT','C_WITH_EXACT_ASM_HELPER','ASM_EXACT')
 function={'address':address,'end_exclusive':end,'symbol':name,'classification':state}
 if row:
  assert row['symbol']==name and row['end_exclusive']==end
  region=set(range(address,end));assert region<=code
  compiler_code.update(region)
  helper=set()
  for part in row.get('asm_helper_ranges',[]):
   a,b=part['start'],part['end_exclusive']
   assert a in boundaries and b in boundaries and address<=a<b<=end
   part_bytes=set(range(a,b));assert not helper&part_bytes
   helper.update(part_bytes)
  assert (not helper)==(state=='C_BINARY_EXACT')
  helpers.update(helper)
  function.update(source=row['source'],compiler_profile=row['compiler_profile'],
                  compiler_generated_bytes=len(region)-len(helper),exact_asm_helper_bytes=len(helper))
 functions.append(function)
compiler_code-=helpers
# Expose target-specific compiler operations as a subset of C, rather than
# implying all compiler-generated source is portable C arithmetic/control flow.
avr_primitives={'cli','sei','nop','swap','bst','bld'}
primitive_code=set()
for instruction in index:
 if instruction['instruction'].split()[0] in avr_primitives:
  instruction_bytes=set(range(instruction['address'],instruction['address']+len(bytes.fromhex(instruction['bytes']))))
  if instruction_bytes<=compiler_code:primitive_code.update(instruction_bytes)
boot_rows=[row for row in index if row['address']>=0x20000]
boot_targets=set()
for row in boot_rows:
 text=row['instruction']
 if re.match(r'(call|rcall)\s',text) or row['address'] in (0x201a0,0x201dc,0x201e0):
  target=re.search(r';\s*0x([0-9a-f]+)',text);assert target,text
  boot_targets.add(int(target[1],16))
assert len(boot_targets)==9
assert sum(row['instruction'].strip() in ('ret','reti') for row in boot_rows)==9

def symbols(path):
 values={}
 for line in subprocess.check_output(['avr-nm','-n',str(path)],text=True).splitlines():
  m=re.match(r'([0-9a-f]+)\s+\w\s+(\S+)',line)
  if m:values[m[2]]=int(m[1],16)
 return values
linked=symbols(build/'reconstructed.elf')
original_text={}
for line in subprocess.check_output(['avr-nm','-n',str(build/'application_exact.o')],text=True).splitlines():
 m=re.match(r'([0-9a-f]+)\s+[tT]\s+(\S+)',line)
 if m:original_text[m[2]]=int(m[1],16)
for name,address in original_text.items():
 assert linked.get(name)==address,('original application symbol moved',name,address)
original_all=dict(original_text)
for line in subprocess.check_output(['avr-nm','-n',str(build/'boot.o')],text=True).splitlines():
 m=re.match(r'([0-9a-f]+)\s+[tT]\s+(\S+)',line)
 if m:
  address=int(m[1],16)+0x20000;name=m[2]
  assert linked.get(name)==address,('original boot symbol moved',name,address)
  original_all[name]=address
assert len(original_all)==806
counts=Counter(row['classification'] for row in functions)
app_code={a for a in code if a<0x20000}
sha=lambda data:hashlib.sha256(data).hexdigest()
result={'acceptance':'canonical complete FLASH equality only','application_functions':89,
 'count_measure':'verified original entries, including 10 ISRs, main and shared formatter entries',
 'all_original_direct_application_call_targets_and_vector_destinations_listed':True,
 **{name:counts[name] for name in ('C_BINARY_EXACT','C_WITH_EXACT_ASM_HELPER','ASM_EXACT')},
 'bootloader_asm_count':len(boot_targets),'bootloader_function_addresses':sorted(boot_targets),
 'complete_executable_bytes':len(code),'application_executable_bytes':len(app_code),
 'boot_executable_bytes':len(code)-len(app_code),
 'compiler_generated_c_bytes':len(compiler_code),
 'compiler_generated_avr_primitive_bytes':len(primitive_code),
 'compiler_generated_other_c_bytes':len(compiler_code)-len(primitive_code),
 'avr_primitive_mnemonics':sorted(avr_primitives),'exact_asm_helper_bytes_inside_c_regions':len(helpers),
 'all_asm_bytes_including_helpers':len(code)-len(compiler_code),
 'C_percentage_all_executable':100*len(compiler_code)/len(code),
 'ASM_percentage_all_executable':100*(len(code)-len(compiler_code))/len(code),
 'C_percentage_application_executable':100*len(compiler_code)/len(app_code),
 'ASM_percentage_application_executable':100*(len(app_code)-len(compiler_code))/len(app_code),
 'coverage_measure':'instruction bytes; inline ASM helpers counted as ASM; data/padding excluded',
 'original_application_symbols_preserved':len(original_text),
 'all_original_text_symbols_preserved':len(original_all),
 'flash_size':len(golden),'golden_sha256':sha(golden),'rebuilt_sha256':sha(rebuilt),'differing_bytes':0,
 'compiler':subprocess.check_output(['avr-gcc','--version'],text=True).splitlines()[0],
 'source_sha256':{r['source']:sha((PROJECT/r['source']).read_bytes()) for r in manifest['accepted']},
 'application':functions}
(build/'function_inventory.json').write_text(json.dumps(result,indent=2)+'\n')
print(f'Application {len(functions)}: '+', '.join(f'{name}={counts[name]}' for name in ('C_BINARY_EXACT','C_WITH_EXACT_ASM_HELPER','ASM_EXACT')))
print(f'Boot ASM: {len(boot_targets)}; C instruction bytes: {len(compiler_code)}/{len(code)} ({100*len(compiler_code)/len(code):.4f}%); differing bytes: 0')
