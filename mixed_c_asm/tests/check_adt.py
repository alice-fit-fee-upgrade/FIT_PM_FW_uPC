#!/usr/bin/env python3
"""C transaction wrappers retain the original clock-producing byte ASM."""
import json,hashlib,random,argparse
from cpu_state import CPU,ROOT
from avr_oracle import PROGRAM,load_program
parser=argparse.ArgumentParser();parser.add_argument('--entry',action='append',type=lambda s:int(s,0),choices=(0x2598,0x25be,0x25ea))
entries=parser.parse_args().entry or [0x2598,0x25be,0x25ea]
PATH=ROOT/'mixed_c_asm/build/flash_mixed.bin';SHA=hashlib.sha256(PATH.read_bytes()).hexdigest();MIXED=load_program(PATH)
class IO:
 def __init__(self,reply,noise):self.reply=reply;self.noise=noise;self.index=0;self.trace=[]
 def read(self,a):
  assert a==0x608 and self.index<len(self.reply)*8,(hex(a),self.index)
  byte,bit=divmod(self.index,8);v=(self.noise[self.index%len(self.noise)]&0xfb)|(((self.reply[byte]>>(7-bit))&1)<<2)
  self.index+=1;self.trace.append(('read',a,v));return v
 def write(self,a,v):
  assert a in (0x605,0x606),(hex(a),v)
  self.trace.append(('write',a,v))
 def irq(self,v):raise AssertionError(('unexpected IRQ',v))
rng=random.Random(0x2598);templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
cases=0;maximum=0;rows=[]
def check(entry,command,value,reply,sreg,noise):
 global cases,maximum
 state=templates[cases&255].copy();state[16]=command;state[17]=value&255
 if entry==0x25be:state[18]=value>>8
 oi=IO(reply,noise);ni=IO(reply,noise);old=CPU(PROGRAM,oi);new=CPU(MIXED,ni)
 old.r=state.copy();new.r=state.copy();old.f=[(sreg>>b)&1 for b in range(8)];new.f=old.f.copy()
 old.run(entry,3000);new.run(entry,3000)
 ctx=(entry,command,value,reply,sreg,noise)
 assert old.r==new.r,('registers',ctx,old.r,new.r)
 assert old.f==new.f,('SREG',ctx,old.f,new.f)
 assert oi.trace==ni.trace,('CS/clock/data/GPIO ordering',ctx,oi.trace,ni.trace)
 assert old.sp==new.sp==0x3fff and oi.index==ni.index==len(reply)*8
 if entry==0x2598:assert new.r==state
 if entry==0x25be:assert new.r[20:22]==[reply[2],reply[1]]
 if entry==0x25ea:assert new.r[16]==0x10 and new.r[18]==0
 maximum=max(maximum,new.entry_sp-new.minimum_sp);cases+=1
for entry in entries:
 before=cases;count=2 if entry==0x2598 else 3 if entry==0x25be else 4
 if entry==0x2598:
  for command in range(256):
   for data in range(256):
    check(entry,command,data,[command^0xa5,data^0x5a],(command+data)&255,[command,data,0,255])
 elif entry==0x25be:
  for value in range(65536):
   check(entry,(value^(value>>8))&255,value,[(value>>8)^0xa5,value&255,(value&255)^0x5a],value&255,[value&255,value>>8,0,255])
 else:
  for reply in range(256):
   for sreg in range(256):check(entry,0,0,[0x55,0xaa,reply^0xff,reply],sreg,[reply,255,0,0x55])
 for sreg in range(256):
  for reply in (0,1,0x10,0x80,0xfe,0xff):
   check(entry,0xff,0xffff,[reply]*count,sreg,[0,255,sreg,reply])
 rows.append({'entry':entry,'cases':cases-before})
 print('PASS ADT wrapper',hex(entry),cases-before,'cases',flush=True)
assert hashlib.sha256(PATH.read_bytes()).hexdigest()==SHA,'image changed during test'
result={'cases':cases,'entries':rows,'all_sreg_patterns':256,'clock_primitive_retained':True,'all_registers_sreg_ordered_cs_clock_gpio_and_return_stack_equal':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':SHA,'limits':'functional interpreter, original within-byte ASM timing retained; no cycle/async IRQ/hardware validation'}
(ROOT/'mixed_c_asm/build/adt_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('ALL PASS ADT:',cases,'cases;',maximum,'additional stack bytes',flush=True)
