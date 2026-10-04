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
class AuditCPU(CPU):
 def push_byte(self,value):
  if getattr(self,'pc',None)==0x28ae:
   self.io.trace.append(('before_original_uart_sender',self.r[16],sum(v<<i for i,v in enumerate(self.f))))
  super().push_byte(value)
rng=random.Random(0x53544154)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
cases=0; maximum=0

def check(status,sreg,mode):
 global cases,maximum
 state=templates[cases&255].copy(); state[16]=status&255; state[17]=status>>8; flags=[(sreg>>b)&1 for b in range(8)]
 oi=IO(status,mode); ni=IO(status,mode); old=AuditCPU(PROGRAM,oi); new=AuditCPU(MIXED,ni)
 old.r=state.copy();new.r=state.copy();old.f=flags.copy();new.f=flags.copy()
 old.run(0x26f8,10000);new.run(0x26f8,10000)
 ctx=(status,sreg,mode)
 assert old.r==new.r,('registers',ctx,[(i,a,b) for i,(a,b) in enumerate(zip(old.r,new.r)) if a!=b])
 assert old.f==new.f,('flags',ctx,old.f,new.f)
 assert oi.mem==ni.mem and oi.trace==ni.trace,('memory/UART/IRQ',ctx)
 assert old.sp==new.sp==0x3fff
 assert new.r==state
 assert bytes(row[1] for row in ni.trace if row[0]=="before_original_uart_sender")==f"{status:04X}".encode()
 maximum=max(maximum,new.entry_sp-new.minimum_sp);cases+=1

for word in range(65536):check(word,word&255,'direct')
print('PASS all 65536 input words; cycled SREG',flush=True)
for mode in ('direct','queued','busy','wrap','full'):
 for word in (0,1,0xff,0x100,0x7fff,0x8000,0xabcd,0xffff):
  for sreg in range(256):check(word,sreg,mode)
 print('PASS HEX16 UART scenario:',mode,flush=True)
assert cases==75776
result={'cases':cases,'input_words':65536,'initial_sreg_patterns':256,'uart_scenarios':['direct','queued','busy','wrap','full'],'registers_sreg_memory_uart_irq_and_return_stack_equal':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded functional interpreter; scripted UART consumption; no cycle/async interrupt/hardware validation'}
(ROOT/'mixed_c_asm/build/hex16_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('ALL PASS HEX16:',cases,'cases;',maximum,'additional stack bytes',flush=True)
