#!/usr/bin/env python3
"""Full AVR comparison of status polling with original 1600-iteration delay."""
import sys,json,random,hashlib
from cpu_state import CPU,ROOT
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM,load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
class IO:
 def __init__(self,ready,spi,delay,busy):
  self.data=[((ready+i*29)|1)&255 for i in range(busy)]+[ready]
  self.spi=spi;self.delay=delay;self.pos=0;self.poll=0;self.trace=[]
 def read(self,a):
  if a==0x0ac2:
   self.poll+=1;v=self.spi if self.poll>self.delay else ((self.poll*17)&127)
  else:
   assert a==0x0ac3 and self.pos<len(self.data),('unexpected data read',a,self.pos)
   v=self.data[self.pos];self.pos+=1
  self.trace.append(('read',a,v));return v
 def write(self,a,v):
  assert a in (0x0685,0x0686,0x0ac3)
  self.trace.append(('write',a,v))
  if a==0x0ac3:self.poll=0
 def irq(self,v):raise AssertionError(('unexpected IRQ change',v))
rng=random.Random(0x57414954)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
cases=0;maximum=0

def check(ready,spi,delay,busy,sreg):
 global cases,maximum
 state=templates[cases&255].copy();flags=[(sreg>>b)&1 for b in range(8)]
 oi=IO(ready,spi,delay,busy);ni=IO(ready,spi,delay,busy)
 old=CPU(PROGRAM,oi);new=CPU(MIXED,ni)
 old.r=state.copy();new.r=state.copy();old.f=flags.copy();new.f=flags.copy()
 old.run(0x173a,100000);new.run(0x173a,100000)
 ctx=(ready,spi,delay,busy,sreg)
 assert old.r==new.r,('registers',ctx,[(i,a,b) for i,(a,b) in enumerate(zip(old.r,new.r)) if a!=b])
 assert old.f==new.f,('flags',ctx,old.f,new.f)
 assert oi.trace==ni.trace and oi.pos==ni.pos==busy+1,('SPI trace',ctx)
 assert new.r[24]==ready and new.r[25]==0 and new.r[19]==spi
 assert old.sp==new.sp==0x3fff
 maximum=max(maximum,new.entry_sp-new.minimum_sp);cases+=1
patterns=(0,0xff,0x20,0x40,0x80,0xa0,0xc0,0xe0)
for ready in range(0,256,2):
 for flags in patterns:check(ready,0x80,0,0,flags)
print('PASS every even FLASH status byte and H/T/I flag combinations',flush=True)
for flags in range(256):check(0x12,0xa5,1,0,flags)
print('PASS all incoming SREG patterns',flush=True)
for spi in range(128,256):
 for flags in patterns:
  for delay in (0,1,3):check(0x82,spi,delay,0,flags)
print('PASS all ready SPI status bytes and failed-poll profiles',flush=True)
for flags in range(256):
 for busy in (1,2,4):check(flags&254,0xc7,2,busy,flags)
print('PASS 1/2/4 busy retry transactions through retained delay',flush=True)
assert cases==5120
result={'cases':cases,'incoming_sreg_patterns':256,'ready_flash_status_bytes':128,'ready_spi_status_bytes':128,'busy_retry_counts':[0,1,2,4],'original_timed_delay_bytes_unchanged':True,'original_delay_iterations':1600,'registers_flags_spi_and_return_stack_equal':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded functional interpreter; original delay encoding retained but no cycle/async interrupt/hardware validation'}
(ROOT/'mixed_c_asm/build/flash_wait_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('ALL PASS FLASH wait:',cases,'cases;',maximum,'additional stack bytes',flush=True)
