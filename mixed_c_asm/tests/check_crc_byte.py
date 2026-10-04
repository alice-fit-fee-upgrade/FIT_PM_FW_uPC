#!/usr/bin/env python3
"""Execute original and compiled CRC with full ABI, then original SPI stream caller."""
import sys,json,random,hashlib
from cpu_state import CPU,ROOT
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM,load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
rng=random.Random(0x43524342)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
cases=0; maximum=0

def check(crc,poly,byte,sreg):
 global cases,maximum
 state=templates[cases&255].copy()
 for reg,shift in ((18,0),(19,8),(16,16),(17,24)):state[reg]=(crc>>shift)&255
 for reg,shift in ((23,0),(24,8),(25,16),(26,24)):state[reg]=(poly>>shift)&255
 state[22]=byte; flags=[(sreg>>b)&1 for b in range(8)]
 old=CPU(PROGRAM);new=CPU(MIXED)
 old.r=state.copy();new.r=state.copy();old.f=flags.copy();new.f=flags.copy()
 old.run(0x17e4,2000);new.run(0x17e4,2000)
 ctx=(hex(crc),hex(poly),byte,sreg)
 assert old.r==new.r,('registers',ctx,[(i,a,b) for i,(a,b) in enumerate(zip(old.r,new.r)) if a!=b])
 assert old.f==new.f,('flags',ctx,old.f,new.f)
 assert old.sp==new.sp==0x3fff
 maximum=max(maximum,new.entry_sp-new.minimum_sp);cases+=1

states=(0,1,0x80000000,0xffffffff,0x01234567,0x89abcdef,0x08000000,0x04000000)
polys=(0,1,0xffffffff,0x04c11db7,0x80000000,0x01020304)
for crc in states:
 for poly in polys:
  for byte in range(256):
   for flags in (0,0xff,0x40,0x80):check(crc,poly,byte,flags)
print('PASS CRC all bytes x state/polynomial boundary combinations x flags',flush=True)
basis=(0,)+tuple(1<<bit for bit in range(32))
for crc in basis:
 for poly in basis:
  for byte in (1,2,4,8,16,32,64,128):check(crc,poly,byte,cases&255)
print('PASS CRC state/polynomial/input bit basis',flush=True)
for i in range(20000):check(rng.randrange(1<<32),rng.randrange(1<<32),rng.randrange(256),i&255)
assert cases==77864

class SPI:
 def __init__(self,data):self.data=list(data);self.trace=[];self.poll=0;self.pos=0
 def read(self,a):
  if a==0x0ac2:
   self.poll+=1;v=(0x80 if self.poll%2==0 else 0)|((self.poll*17)&0x7f)
  else:
   assert a==0x0ac3 and self.pos<len(self.data),('unexpected SPI read',a,self.pos)
   v=self.data[self.pos];self.pos+=1
  self.trace.append(('read',a,v));return v
 def write(self,a,v):
  assert a in (0x0685,0x0686,0x0ac3)
  self.trace.append(('write',a,v))
 def irq(self,v):raise AssertionError(('unexpected IRQ change',v))
stream_cases=0; stream_max=0
for length in (1,2,3,4,8,16,32):
 for start in (0,0xfffe,0xffff,0xfffff0,0xffffff):
  for repeat in range(8):
   state=templates[stream_cases&255].copy();end=(start+length-1)&0xffffff
   state[28]=start&255;state[29]=(start>>8)&255;state[30]=start>>16
   state[0]=end&255;state[1]=(end>>8)&255;state[2]=end>>16
   data=[rng.randrange(256) for _ in range(length)]
   oi=SPI(data);ni=SPI(data);old=CPU(PROGRAM,oi);new=CPU(MIXED,ni)
   old.r=state.copy();new.r=state.copy();flags=[((stream_cases>>b)&1) for b in range(8)]
   old.f=flags.copy();new.f=flags.copy();old.run(0x1772,100000);new.run(0x1772,100000)
   assert old.r==new.r and old.f==new.f,('CRC stream CPU',length,start,repeat,old.f,new.f,[(i,a,b) for i,(a,b) in enumerate(zip(old.r,new.r)) if a!=b])
   assert oi.trace==ni.trace and oi.pos==ni.pos==length,('CRC stream SPI',length,start)
   assert old.sp==new.sp==0x3fff
   stream_max=max(stream_max,new.entry_sp-new.minimum_sp);stream_cases+=1
assert stream_cases==280
result={'direct_cases':cases,'stream_caller_cases':stream_cases,'input_bytes':256,'state_and_polynomial_bits':32,'incoming_sreg_patterns':256,'checked_registers':32,'checked_sreg_bits':8,'direct_maximum_additional_stack':maximum,'stream_maximum_additional_stack':stream_max,'spi_stream_address_wraps_tested':True,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded functional interpreter; scripted SPI; no cycle/async interrupt/hardware validation'}
(ROOT/'mixed_c_asm/build/crc_byte_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('ALL PASS CRC:',cases,'direct;',stream_cases,'original SPI stream callers',flush=True)
