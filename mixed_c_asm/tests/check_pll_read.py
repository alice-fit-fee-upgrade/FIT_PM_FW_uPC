#!/usr/bin/env python3
import json,hashlib,random
from cpu_state import CPU,ROOT
from avr_oracle import PROGRAM,load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
class IO:
 def __init__(self,value,ready,delay):self.value=value;self.ready=ready;self.delay=delay;self.poll=0;self.index=0;self.trace=[]
 def read(self,a):
  if a==0x8c2:
   self.poll+=1;v=self.ready if self.poll>self.delay else self.ready&127
  elif a==0x8c3:v=(self.value>>(self.index*8))&255;self.index+=1
  else:raise AssertionError(('read',hex(a)))
  self.trace.append(('read',a,v));return v
 def write(self,a,v):
  assert a in (0x8c0,0x8c3,0x6a6,0x6a5),(hex(a),v)
  self.trace.append(('write',a,v))
  if a==0x8c3:self.poll=0
 def irq(self,v):raise AssertionError(('unexpected IRQ',v))
rng=random.Random(0x24ce); templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
cases=0;maximum=0

def check(value,flags,ready,delay):
 global cases,maximum
 oi=IO(value,ready,delay);ni=IO(value,ready,delay);old=CPU(PROGRAM,oi);new=CPU(MIXED,ni)
 old.r=templates[cases&255].copy();new.r=old.r.copy();old.f=[(flags>>b)&1 for b in range(8)];new.f=old.f.copy()
 old.run(0x24ce,2000);new.run(0x24ce,2000)
 ctx=(value,flags,ready,delay)
 assert old.r==new.r,('registers',ctx,old.r,new.r)
 assert old.f==new.f,('flags',ctx,old.f,new.f)
 assert oi.trace==ni.trace,('SPI trace',ctx,oi.trace,ni.trace)
 assert old.sp==new.sp==0x3fff
 assert new.r[16:20]==[(value>>(i*8))&255 for i in range(4)]
 assert sum(v<<b for b,v in enumerate(new.f))==(flags&0xe1)|2
 assert [row[2] for row in ni.trace if row[:2]==('write',0x8c3)]==[0x8e,0,0,0,0,0,0,0]
 maximum=max(maximum,new.entry_sp-new.minimum_sp);cases+=1
for position in range(4):
 for byte in range(256):
  for flags in (0,1,0x20,0x40,0x80,0xa1,0xe1,0xff):check(byte<<(position*8),flags,0x95,flags%4)
for flags in range(256):
 for value in (0,0xffffffff,0x12345678,0x89abcdef,0x80000000,1,0x55aa55aa,0xaa55aa55):
  for delay in range(4):check(value,flags,0x95,delay)
for ready in range(128,256):
 for flags in range(256):check(rng.randrange(1<<32),flags,ready,flags%4)
result={'cases':cases,'all_sreg_patterns':256,'ready_values':128,'all_256_bytes_in_each_reply_position':True,'registers_sreg_ordered_spi_and_return_stack_equal':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded functional interpreter; no timing/asynchronous IRQ/hardware validation'}
(ROOT/'mixed_c_asm/build/pll_read_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('PASS PLL read:',cases,'cases;',maximum,'additional stack bytes',flush=True)
