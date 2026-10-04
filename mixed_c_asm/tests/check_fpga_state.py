#!/usr/bin/env python3
"""Exhaustive original versus compiled C state transition and ABI check."""
import json, random, hashlib, sys
from cpu_state import CPU, ROOT
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM, load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
class IO:
 def __init__(self,previous): self.mem={0x2159:previous,0:previous^0xa5}; self.trace=[]
 def read(self,a):
  v=self.mem[a]; self.trace.append(('read',a,v)); return v
 def write(self,a,v): self.mem[a]=v; self.trace.append(('write',a,v))
 def irq(self,v): self.trace.append(('irq',bool(v)))
class StateCPU(CPU):
 def io_read(self,a):
  return self.io.read(a) if a==0 else super().io_read(a)
 def io_bit_write(self,a,bit,value):
  assert a==0 and bit==1 and not value
  self.io.mem[a] &= ~(1<<bit)
  self.io.trace.append(('atomic_clear',a,bit))
rng=random.Random(0xa9c)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
maximum=0; cases=0
for entry in (0xa9c,0x167e):
 for previous in range(256):
  for sreg in range(256):
   state=templates[sreg].copy(); state[16]=(previous+sreg)&255
   oi=IO(previous); ni=IO(previous); old=StateCPU(PROGRAM,oi); new=StateCPU(MIXED,ni)
   old.r=state.copy(); new.r=state.copy()
   old.f=[(sreg>>b)&1 for b in range(8)]; new.f=old.f.copy()
   old.run(entry,1000); new.run(entry,1000)
   ctx=(entry,previous,sreg,state[16])
   assert old.r==new.r,('registers',ctx,old.r,new.r)
   assert old.f==new.f,('SREG',ctx,old.f,new.f)
   assert oi.mem==ni.mem and oi.trace==ni.trace,('RAM/GPIO ordering',ctx,oi.trace,ni.trace)
   assert old.sp==new.sp==0x3fff
   maximum=max(maximum,new.entry_sp-new.minimum_sp); cases+=1
 print('PASS FPGA state entry:',hex(entry),flush=True)
result={'cases':cases,'entries':[0xa9c,0x167e],'previous_state_values':256,'incoming_sreg_patterns':256,'all_registers_flags_memory_trace_return_stack_equal':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded functional interpreter; no cycle, asynchronous IRQ or physical hardware validation'}
(ROOT/'mixed_c_asm/build/fpga_state_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('PASS FPGA state:',cases,'cases;',maximum,'additional stack bytes',flush=True)
