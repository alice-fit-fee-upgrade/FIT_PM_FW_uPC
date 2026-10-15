#!/usr/bin/env python3
"""Compare private byte-cell ABI with golden under stable per-bit MISO.
Separate DOUT-release sampling tests exercise the intentionally improved timing.
"""
import json,random,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'development/tools'),str(ROOT/'source_recovery/tests')]
from build import BUILD,symbols
from avr_oracle import Machine,load_program
names=symbols(BUILD/'development.elf');new=load_program(BUILD/'development.bin');old=load_program()
assert new[0x2608][0]=='jmp' and new[0x2608][3]==names['pm_adt_shared_byte']
class Pins:
 def __init__(self,rx):self.out=0xeb;self.rx=rx;self.index=0;self.miso=0;self.sent=[]
 def read(self,a):
  assert a==0x608,hex(a)
  return (self.out&~4)|(self.miso<<2)
 def write(self,a,v):
  assert a in (0x605,0x606) and v in (2,8),(a,v)
  before=self.out;self.out=(self.out|v) if a==0x605 else self.out&(~v&255)
  if before&2 and not(self.out&2):
   assert self.index<8
   self.miso=(self.rx>>(7-self.index))&1;self.index+=1
  if not(before&2) and self.out&2:self.sent.append((self.out>>3)&1)
rng=random.Random(2608);cases=0
for tx in (0,1,0x40,0x50,0x60,0x78,0x80,0xff):
 for rx in (0,1,0x55,0x80,0xc3,0xff):
  for status in (0,0x35,0x80,0xff):
   regs=[rng.randrange(256) for _ in range(32)];regs[16]=tx;results=[]
   for program in (old,new):
    pins=Pins(rx);m=Machine(pins.read,pins.write,lambda e:None,program=program)
    m.r[:]=regs;m.f[:]=[(status>>i)&1 for i in range(8)];m.run(0x2608,limit=3000)
    assert m.r[16]==rx and pins.index==8 and pins.sent==[(tx>>(7-i))&1 for i in range(8)]
    assert not m.stack and not m.calls
    results.append((m.r,m.f,pins.out))
   assert results[0]==results[1],(tx,rx,status,results)
   cases+=1
report={'status':'PASS','cases':cases,'scope':'golden/private slow byte cell GPR/SREG/TX/RX agreement with stable MISO, no real-time or electrical proof'}
(BUILD/'adt_byte_contract_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS shared-clock ADT private ABI:',cases,'cases')
