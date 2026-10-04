#!/usr/bin/env python3
"""Execute linked unsigned C adapter + ASM bridge, with original full CPU contract.
No C/native callback shortcuts. Physical three-byte return stack is modelled;
only dead stack scratch bytes may differ. No peripheral/IRQ or static-RAM writes.
"""
import sys,pathlib,json,random,time,hashlib
ROOT=pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import Machine,PROGRAM,load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
CALLER=0x20400

from cpu_state import CPU

rng=random.Random(0x41424912)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
count=0; max_depth=0; min_steps=10000; max_steps=0; stages=[]; started=time.monotonic()
def check(coefficient,value):
 global count,max_depth,min_steps,max_steps
 state=templates[count&255].copy(); state[18]=coefficient&255; state[19]=coefficient>>8
 state[20]=value&255; state[21]=value>>8; flags=[(count>>bit)&1 for bit in range(8)]
 old=CPU(PROGRAM); new=CPU(MIXED)
 old.r=state.copy(); new.r=state.copy(); old.f=flags.copy(); new.f=flags.copy()
 old.run(0x214c,600); new.run(0x214c,600)
 assert old.r==new.r,('register mismatch',hex(coefficient),hex(value),[(i,a,b) for i,(a,b) in enumerate(zip(old.r,new.r)) if a!=b])
 assert old.f==new.f,('SREG mismatch',hex(coefficient),hex(value),flags,old.f,new.f)
 assert new.sp==old.sp==0x3fff and not old.calls and not new.calls
 assert all(0x3fe7<=a<=0x3fff for a in new.mem),'unexpected live/static RAM mutation'
 depth=new.entry_sp-new.minimum_sp
 max_depth=max(max_depth,depth); min_steps=min(min_steps,new.steps); max_steps=max(max_steps,new.steps)
 count+=1

def stage(name,previous):
 stages.append({'name':name,'cases':count-previous})
 print(f'PASS {name}: {count-previous} cases; cumulative {count}; {time.monotonic()-started:.1f}s',flush=True)

for coefficient in (0,1,0xff,0x100,0x4188,0x8000,0xffff):
 before=count
 for value in range(65536): check(coefficient,value)
 stage(f'all inputs, coefficient 0x{coefficient:04x}',before)
for value in (0,1,0x7fff,0x8000,0xffff):
 before=count
 for coefficient in range(65536): check(coefficient,value)
 stage(f'all coefficients, input 0x{value:04x}',before)
before=count
for _ in range(10000): check(rng.randrange(65536),rng.randrange(65536))
stage('random input/coefficient/state',before)
assert count==796432
assert max_depth == 22
result={'cases':count,'stages':stages,'seed':'0x41424912','checked_registers':32,'checked_sreg_bits':8,'all_initial_sreg_values':256,'executed_code':'linked AVR opcodes, including pm_scale_unsigned_abi and bridge; no native/host C hooks','return_address_bytes':3,'entry_stack_pointer':0x3ffc,'returned_stack_pointer':0x3fff,'maximum_additional_stack_below_entry':max_depth,'minimum_stack_pointer':0x3ffc-max_depth,'mixed_instruction_steps_range':[min_steps,max_steps],'peripheral_reads_writes':0,'new_static_ram_bytes':0,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded custom functional interpreter; no asynchronous interrupt injection, cycle equivalence or physical device execution'}
(ROOT/'mixed_c_asm/build/unsigned_abi_result.json').write_text(json.dumps(result,indent=2)+'\n')
print(f'ALL PASS: {count} register/SREG/stack comparisons; {max_depth} additional stack bytes',flush=True)
