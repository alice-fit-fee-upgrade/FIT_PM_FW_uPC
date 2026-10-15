#!/usr/bin/env python3
"""Decoded ISR acquisition/defer and release for all SREG values; no timing claim."""
import sys,json,random
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import Machine,load_program
sys.path.insert(0,str(ROOT/'development/tools'))
from build import BUILD,symbols
names=symbols(BUILD/'development.elf');program=load_program(BUILD/'development.bin')
class Model(Machine):
 def __init__(self,owner):
  self.memory={15:owner,14:0,0x684:4,0x688:10}
  super().__init__(lambda a:self.memory.get(a,0),lambda a,v:self.memory.__setitem__(a,v),lambda e:None,program=program)
 def io_read(self,a):return super().io_read(a) if a==63 else self.memory.get(a,0)
 def io_write(self,a,v):
  if a==63:super().io_write(a,v)
  else:self.memory[a]=v
cases=0
for flags in range(256):
 for owner in (0,1,2,0x40,0x80,0x81):
  m=Model(owner);m.r[:]=[(i*17+flags)&255 for i in range(32)];m.f[:]=[(flags>>i)&1 for i in range(8)]
  before=list(m.r);beforeflags=list(m.f)
  pc=names['pm_timer_at_077e'];stop={names['pm_timer_instruction_077e'],names['pm_timer_at_08be']}
  m.program=dict(program)
  for endpoint in stop:m.program[endpoint]=('ret','',2,None)
  m.run(pc,limit=100);pc=m.pc-2
  assert m.r==before and m.f==beforeflags and not m.stack and not m.calls
  if owner&0x43:
   assert pc==names['pm_timer_at_08be'] and m.memory[15]==owner|0x80 and m.memory[14]==1
  else:
   assert pc==names['pm_timer_instruction_077e'] and m.memory[15]==2
   m.steps=0;m.program[names['pm_timer_instruction_0788']]=('ret','',2,None)
   m.run(names['pm_timer_at_0788'],limit=100);pc=m.pc-2
   assert pc==names['pm_timer_instruction_0788'] and not m.memory[15]&3
  cases+=1
print('PASS ISR mutex/defer:',cases,'SREG/ownership cases; preserved registers/flags, no spinning')

power_cases=0
for enabled in (0xf0,0xf4):
 for pins in range(256):
  if enabled&4 and pins&10==10:continue
  for flags in (0,0x80,0xff):
   for owner in (0,1,0x40,0x80):
    m=Model(owner);m.memory.update({0x684:enabled,0x688:pins,14:91})
    m.r[:]=[(i*17+flags)&255 for i in range(32)];m.f[:]=[(flags>>i)&1 for i in range(8)]
    before=list(m.r);beforeflags=list(m.f);beforemem=dict(m.memory)
    m.program=dict(program);m.program[names['pm_timer_at_08be']]=('ret','',2,None)
    # Entering the SPI continuation on a bad PGOOD is a failure, not a stub.
    m.run(names['pm_timer_at_077e'],limit=100)
    assert m.pc-2==names['pm_timer_at_08be']
    assert m.r==before and m.f==beforeflags and not m.stack and not m.calls
    assert m.memory==beforemem, 'unready ISR must not drive GPIO or alter mutex/defer'
    power_cases+=1
print('PASS ISR PGOOD skip:',power_cases,'unready GPIO/SREG/ownership cases; no shared-clock writes')
