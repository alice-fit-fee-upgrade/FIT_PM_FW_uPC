#!/usr/bin/env python3
"""Execute linked AVR slow reads; model DOUT going low after final rising edge.
This is a protocol/ABI regression, not an electrical timing measurement.
"""
import json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'development/tools'),str(ROOT/'source_recovery/tests')]
from build import BUILD,symbols
from avr_oracle import Machine,load_program
names=symbols(BUILD/'development.elf');program=load_program(BUILD/'development.bin')
values=[0x80,0x50,0x0f99,0xc3,0x2301,5,0x1e01,0x0501]
class Sensor:
 def __init__(self):
  self.mem={0x684:4,0x688:10,0x600:0xfb,0x604:0xfb}
  self.frames=[];self.active=False;self.dout=0;self.bits=[];self.rx_bits=[];self.index=0
 def read(self,a):
  if a==0x608:return (self.mem[0x604]&~4)|(self.dout<<2)
  return self.mem.get(a,0)
 def write(self,a,v):
  if a not in (0x605,0x606):self.mem[a]=v;return
  before=self.mem[0x604]
  after=(before|v) if a==0x605 else before&(~v&255)
  self.mem[0x604]=after
  if before&16 and not(after&16):self.active=True;self.bits=[];self.rx_bits=[];self.index=0;self.dout=0
  if not self.active:return
  if (before&2) and not(after&2) and len(self.bits)>=8:
   self.dout=self.rx_bits[self.index] if self.index<len(self.rx_bits) else 0
  if not(before&2) and after&2:
   self.bits.append((after>>3)&1)
   if len(self.bits)==8:
    command=sum(bit<<(7-i) for i,bit in enumerate(self.bits))
    assert command&0xc7==0x40,hex(command)
    reg=(command>>3)&7;size=16 if reg in (2,4,6,7) else 8
    self.rx_bits=[(values[reg]>>(size-1-i))&1 for i in range(size)]
   elif len(self.bits)>8:
    self.index+=1
    # ADT7311 t10 permits DOUT to go LOW just after the last inactive edge.
    if self.index==len(self.rx_bits):self.dout=0
  if after&16:
   self.frames.append(self.bits.copy());self.active=False;self.dout=0
class Xmega(Machine):
 def io_read(self,a):return super().io_read(a) if a==0x3f else self.read(a)
 def io_write(self,a,v):
  if a==0x3f:super().io_write(a,v)
  else:self.write(a,v)
cases=0
for driver in ('pm_diag_temp','pm_diag_temp_slow'):
 for reg in range(8):
  sensor=Sensor();m=Xmega(sensor.read,sensor.write,lambda enabled:None,program=program)
  m.r[1]=0;m.r[24]=reg;m.r[22]=0;m.r[23]=0x23;m.f[7]=1
  m.run(names[driver],limit=20000)
  assert m.r[24]==0 and m.f[7]==1 and m.r[1]==0
  assert sensor.mem[0x2300]|(sensor.mem[0x2301]<<8)==values[reg],(reg,sensor.mem)
  assert len(sensor.frames)==1 and len(sensor.frames[0])==(24 if reg in (2,4,6,7) else 16)
  assert not any(sensor.frames[0][8:]),'read dummy bits must be zero'
  assert sensor.mem[0x604]==0xfb and not m.stack and not m.calls
  cases+=1
report={'status':'PASS','cases':cases,'scope':'actual linked AVR slow driver with command framing and DOUT-low after final rising edge; no electrical or real-time proof'}
(BUILD/'adt_sampling_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS linked AVR ADT sampling:',cases,'registers, including odd final data bits')
