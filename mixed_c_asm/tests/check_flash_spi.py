#!/usr/bin/env python3
"""Original versus linked AVR execution of hardware-SPI FLASH helpers."""
import sys,json,random,hashlib,argparse
from cpu_state import CPU,ROOT
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM,load_program
parser=argparse.ArgumentParser()
parser.add_argument('--entry',action='append',type=lambda s:int(s,0),choices=(0x1664,0x171e,0x1808,0x1710))
entries=parser.parse_args().entry or [0x1664,0x171e,0x1808,0x1710]
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
class IO:
 def __init__(self,status,delay):self.status=status;self.delay=delay;self.poll=0;self.trace=[]
 def read(self,a):
  assert a==0x0ac2,('unexpected read',hex(a))
  self.poll+=1;value=self.status if self.poll>self.delay else ((self.poll*17)&127)
  self.trace.append(('read',a,value));return value
 def write(self,a,v):
  assert a in (0x0666,0x0681,0x0685,0x0686,0x0ac0,0x0ac3),('unexpected write',hex(a),v)
  self.trace.append(('write',a,v))
  if a==0x0ac3:self.poll=0
 def irq(self,v):raise AssertionError(('unexpected IRQ change',v))
rng=random.Random(0x53504946)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
cases=0;maximum=0;rows=[]
def check(entry,command,flags,status,delay):
 global cases,maximum
 state=templates[cases&255].copy();state[16]=command
 oi=IO(status,delay);ni=IO(status,delay);old=CPU(PROGRAM,oi);new=CPU(MIXED,ni)
 old.r=state.copy();new.r=state.copy();f=[(flags>>b)&1 for b in range(8)];old.f=f.copy();new.f=f.copy()
 old.run(entry,1000);new.run(entry,1000)
 ctx=(hex(entry),command,flags,status,delay)
 assert old.r==new.r,('registers',ctx,[(i,a,b) for i,(a,b) in enumerate(zip(old.r,new.r)) if a!=b])
 assert old.f==new.f,('flags',ctx,old.f,new.f)
 assert oi.trace==ni.trace,('SPI/MMIO trace',ctx,oi.trace,ni.trace)
 assert old.sp==new.sp==0x3fff
 maximum=max(maximum,new.entry_sp-new.minimum_sp);cases+=1
for entry in entries:
 before=cases
 if entry==0x1664:
  for command in range(256):
   for flags in range(256):check(entry,command,flags,128,0)
 elif entry==0x171e:
  for status in range(128,256):
   for flags in range(256):
    for delay in (0,1,3):check(entry,0,flags,status,delay)
 else:
  for command in range(256):
   for flags in range(256):check(entry,command,flags,128|(command&127),flags%4)
 rows.append({'entry':entry,'cases':cases-before})
 print(f'PASS FLASH helper {entry:#06x}: {cases-before} cases',flush=True)
result={'cases':cases,'entries':rows,'registers_sreg_mmio_order_and_return_stack_equal':True,'incoming_sreg_patterns':256,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded functional interpreter; scripted hardware SPI polling; no cycle/async interrupt/hardware validation'}
(ROOT/'mixed_c_asm/build/flash_spi_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('ALL PASS FLASH helpers:',cases,'cases;',maximum,'additional stack bytes',flush=True)
