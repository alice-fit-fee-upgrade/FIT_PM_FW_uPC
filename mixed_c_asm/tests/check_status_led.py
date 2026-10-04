#!/usr/bin/env python3
"""Exhaustive original versus compiled C atomic GPIO/LED writes and ABI check."""
import json, random, hashlib, sys
from cpu_state import CPU, ROOT
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM, load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
class IO:
 def __init__(self,previous): self.mem={0:previous}; self.trace=[]
 def read(self,a):
  v=self.mem[a]; self.trace.append(('read',a,v)); return v
 def write(self,a,v): self.mem[a]=v; self.trace.append(('write',a,v))
 def irq(self,v): raise AssertionError(('unexpected IRQ',v))
class GPIOCPU(CPU):
 def io_read(self,a):
  return self.io.read(a) if a==0 else super().io_read(a)
 def io_write(self,a,v):
  if a==0: self.io.write(a,v)
  else: super().io_write(a,v)
 def io_bit_write(self,a,bit,value):
  assert a==0 and bit==0 and not value,('unexpected atomic bit operation',a,bit,value)
  self.io.mem[a] &= ~(1<<bit)
  self.io.trace.append(('atomic_clear',a,bit))
rng=random.Random(0xc7e)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
maximum=0; cases=0
for previous in range(256):
 for sreg in range(256):
  state=templates[sreg].copy(); state[16]=(previous+sreg)&255
  oi=IO(previous); ni=IO(previous); old=GPIOCPU(PROGRAM,oi); new=GPIOCPU(MIXED,ni)
  old.r=state.copy(); new.r=state.copy()
  old.f=[(sreg>>b)&1 for b in range(8)]; new.f=old.f.copy()
  old.run(0xc7e,1000); new.run(0xc7e,1000)
  ctx=(previous,sreg,state[16])
  assert old.r==new.r,('registers',ctx,old.r,new.r)
  assert old.f==new.f,('SREG',ctx,old.f,new.f)
  assert oi.mem==ni.mem and oi.trace==ni.trace,('RAM/GPIO ordering',ctx,oi.trace,ni.trace)
  assert old.sp==new.sp==0x3fff
  assert new.r[16]==1 and ni.mem[0]==previous&254
  assert ni.trace.count(('atomic_clear',0,0))==1
  maximum=max(maximum,new.entry_sp-new.minimum_sp); cases+=1
result={'cases':cases,'gpio_values':256,'incoming_sreg_patterns':256,'all_registers_flags_memory_trace_return_stack_equal':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded functional interpreter; no cycle, asynchronous IRQ or physical hardware validation'}
(ROOT/'mixed_c_asm/build/status_led_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('PASS status LED:',cases,'cases;',maximum,'additional stack bytes',flush=True)
