#!/usr/bin/env python3
"""Count verified original entry points, not every disassembly label.
Shared formatter entries are counted separately. Boot vector stubs are aliases.
"""
from pathlib import Path
import sys,re,json,subprocess
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM,load_program
baseline_path=ROOT/'mixed_c_asm/build/application_exact.S'
if not baseline_path.exists():
 subprocess.run([sys.executable,str(ROOT/'tools/convert_analysis.py'),'--output',str(baseline_path)],check=True)
baseline=baseline_path.read_text()
entries=[(int(a,16)*2,n.strip()) for a,n in re.findall(r'^;[^\n]*FUN_code_[^\n]*\n\.org \((0x[0-9a-f]+) << 1\), 0xff\n([^:\n]+):',baseline,re.M)]
assert len(entries)==len(set(a for a,_ in entries))==89
starts={a for a,_ in entries}
call_targets={v[3] for a,v in PROGRAM.items() if a<0x2916 and v[0] in ('call','rcall')}
assert call_targets<=starts,('unlisted application CALL target',call_targets-starts)
vectors=json.loads((ROOT/'docs/asm_audit.json').read_text())['vectors']
assert {PROGRAM[v['address']][3] for v in vectors}<=starts
mixed=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
symbols={}
for line in subprocess.check_output(['avr-nm','-n',str(ROOT/'mixed_c_asm/build/mixed.elf')],text=True).splitlines():
 m=re.match(r'([0-9a-f]+)\s+[tT]\s+(pm_\S+_bridge)',line)
 if m:symbols[int(m[1],16)]=m[2]
coverage=json.loads((ROOT/'docs/c_recovery_coverage.json').read_text())['regions']
rows=[]
for index,(address,name) in enumerate(entries):
 offset=8 if address==0x173a else 0
 op,args,length,target=mixed[address+offset]
 integrated=op=='jmp' and target in symbols
 recovered=integrated or any(row['start']<=address<row['end_exclusive'] for row in coverage)
 rows.append({'address':address,'name':name,'end_exclusive':entries[index+1][0] if index+1<len(entries) else 0x2916,'integrated_c':integrated,'retained_asm_prefix_bytes':offset if integrated else 0,'functional_c_recovered':recovered,'bridge':symbols.get(target) if integrated else None})
boot_targets={v[3] for a,v in PROGRAM.items() if 0x20000<=a<0x204e6 and v[0] in ('call','rcall')}
boot_bodies=boot_targets|{PROGRAM[a][3] for a in (0x201a0,0x201dc,0x201e0)}
assert len(boot_bodies)==9
boot_returns=[a for a,v in PROGRAM.items() if 0x20000<=a<0x204e6 and v[0] in ('ret','reti')]
assert len(boot_returns)==9
integrated=sum(r['integrated_c'] for r in rows); recovered=sum(r['functional_c_recovered'] for r in rows)
report={'measure':'original entry points, including interrupts/main/shared formatter entries; not independent source function bodies','application_entries':len(rows),'integrated_application_entries':integrated,'application_entries_not_integrated':len(rows)-integrated,'functional_c_application_entries':recovered,'application_entries_without_functional_c':len(rows)-recovered,'boot_procedures':len(boot_bodies),'boot_procedure_addresses':sorted(boot_bodies),'boot_integrated_entries':0,'all_entries_not_integrated':len(rows)-integrated+len(boot_bodies),'all_direct_application_call_targets_and_vector_destinations_listed':True,'application':rows}
output=ROOT/'mixed_c_asm/build/function_inventory.json';output.write_text(json.dumps(report,indent=2)+'\n')
print(f'Application: {len(rows)} entries; {integrated} integrated C; {len(rows)-integrated} not integrated; {recovered} functional C recovered. Boot: {len(boot_bodies)} ASM procedures.')
