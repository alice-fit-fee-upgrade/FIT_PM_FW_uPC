#!/usr/bin/env python3
"""Full register/SREG and UART/memory effects through actual linked AVR code."""
import sys,json,random,hashlib
from cpu_state import CPU,ROOT
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM,load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
class IO:
 def __init__(self,status,mode):
  self.mode=mode; self.trace=[]
  self.mem={0x2157:status,0x2002:0,0x2003:0,0x2004:1,0x0ba1:32,0x0ba3:0x54}
  if mode=='queued': self.mem[0x2004]=0
  if mode=='busy': self.mem[0x0ba1]=0
  if mode=='wrap': self.mem.update({0x2002:254,0x2003:254,0x2004:0})
  if mode=='full': self.mem.update({0x2002:0,0x2003:255,0x2004:0})
 def read(self,a):
  assert a in self.mem,('unexpected/uninitialized read',hex(a))
  v=self.mem[a]; self.trace.append(('read',a,v)); return v
 def write(self,a,v): self.mem[a]=v; self.trace.append(('write',a,v))
 def irq(self,v):
  self.trace.append(('irq',bool(v)))
  if v and self.mode=='full' and self.mem[0x2002]==((self.mem[0x2003]+1)&255):
   self.mem[0x2002]=(self.mem[0x2002]+1)&255
   self.trace.append(('scripted_tx_consumer',self.mem[0x2002]))
rng=random.Random(0x53544154)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
cases=0; maximum=0

def check(status,sreg,mode):
 global cases,maximum
 state=templates[cases&255].copy(); flags=[(sreg>>b)&1 for b in range(8)]
 oi=IO(status,mode); ni=IO(status,mode); old=CPU(PROGRAM,oi); new=CPU(MIXED,ni)
 old.r=state.copy();new.r=state.copy();old.f=flags.copy();new.f=flags.copy()
 old.run(0x211c,10000);new.run(0x211c,10000)
 ctx=(status,sreg,mode)
 assert old.r==new.r,('registers',ctx,[(i,a,b) for i,(a,b) in enumerate(zip(old.r,new.r)) if a!=b])
 assert old.f==new.f,('flags',ctx,old.f,new.f)
 assert oi.mem==ni.mem and oi.trace==ni.trace,('memory/UART/IRQ',ctx)
 assert old.sp==new.sp==0x3fff
 assert new.r[16]==status and new.f[0]==int((status&16)==0)
 maximum=max(maximum,new.entry_sp-new.minimum_sp);cases+=1

for status in range(256):
 for sreg in range(256):check(status,sreg,'direct')
print('PASS all 256 status bytes x all 256 SREG patterns, direct UART',flush=True)
for mode in ('queued','busy','wrap','full'):
 for status in range(256):
  for sreg in (0,1,0x40,0x80,0xff):check(status,sreg,mode)
 print('PASS status gate UART scenario:',mode,flush=True)
assert cases==70656
result={'cases':cases,'status_bytes':256,'initial_sreg_patterns':256,'uart_scenarios':['direct','queued','busy','wrap','full'],'registers_sreg_memory_uart_irq_and_return_stack_equal':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded functional interpreter; scripted UART consumption; no cycle/async interrupt/hardware validation'}
(ROOT/'mixed_c_asm/build/status_gate_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('ALL PASS status gate:',cases,'cases;',maximum,'additional stack bytes',flush=True)
