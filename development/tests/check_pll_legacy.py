#!/usr/bin/env python3
"""Run complete old/new AVR routines with a stateful SPI model, not timing/IRQ injection."""
import json,random,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'source_recovery/tests'),str(ROOT/'development/tools')]
from avr_oracle import Machine,load_program
from build import BUILD,symbols
names=symbols(BUILD/'development.elf');old=load_program();new=load_program(BUILD/'development.bin')

class SpiMachine(Machine):
 def __init__(self,program,value,delay):
  self.bank={a:0 for a in range(0x38,0x3d)};self.value=value;self.delay=delay
  self.trace=[];self.count=0;self.poll=0;self.max_stack=0;self.irq_events=[]
  self.ctrl=0xd1;self.out=0xf0
  super().__init__(self.mmio_read,self.mmio_write,self.irq_events.append,program=program)
 def push_byte(self,v):
  super().push_byte(v);self.max_stack=max(self.max_stack,len(self.stack))
 def io_read(self,a):return self.bank[a] if a in self.bank else super().io_read(a)
 def io_write(self,a,v):
  if a in self.bank:self.bank[a]=v
  else:super().io_write(a,v)
 def mmio_read(self,a):
  if a==0x8c2:
   self.poll+=1;v=0x35|(0x80 if self.poll>self.delay else 0)
  elif a==0x8c3:
   assert 5<=self.count<=8
   v=(self.value>>(8*(self.count-5)))&255
  else:raise AssertionError(('unexpected MMIO read',hex(a)))
  self.trace.append(('R',a,v));return v
 def mmio_write(self,a,v):
  assert a in (0x8c0,0x8c3,0x6a5,0x6a6),(hex(a),v)
  if a==0x8c3:
   self.count+=1;self.poll=0
   assert v==(0x8e if self.count==1 else 0),('non-read SPI command',self.count,v)
  elif a==0x8c0:self.ctrl=v
  elif a==0x6a5:self.out|=v
  elif a==0x6a6:self.out&=~v
  self.trace.append(('W',a,v))

rng=random.Random(62005);cases=0;stack=0
for flags in range(256):
 for value,delay in [(0,0),(0xffffffff,1),(0x80009d38,3)]:
  regs=[rng.randrange(256) for _ in range(32)]
  # Nonzero legacy R1 and bank registers must be restored; CPU MMIO model
  # abstracts extended addressing, so no nonzero-bank bus behavior claim.
  bank={a:rng.randrange(256) for a in range(0x38,0x3d)}
  machines=[]
  for program in [old,new]:
   m=SpiMachine(program,value,delay);m.r[:]=regs;m.f[:]=[(flags>>i)&1 for i in range(8)];m.bank.update(bank)
   m.run(0x24ce,limit=4000);machines.append(m)
  a,b=machines
  assert (a.r,a.f,a.bank,a.trace,a.ctrl,a.out)==(b.r,b.f,b.bank,b.trace,b.ctrl,b.out),(flags,hex(value))
  assert not b.stack and not b.calls and not b.irq_events
  assert int.from_bytes(bytes(b.r[16:20]),'little')==value
  native=SpiMachine(new,value,delay);native.r[:]=regs;native.f[:]=[(flags>>i)&1 for i in range(8)];native.bank.update(bank)
  native.run(names['pm_pll_read_legacy_native'],limit=4000)
  assert int.from_bytes(bytes(native.r[22:26]),'little')==value
  assert all(native.r[r]==regs[r] for r in list(range(2,18))+[28,29])
  assert native.r[1]==regs[1] and native.bank==bank and native.trace==a.trace
  assert not native.stack and not native.calls and not native.irq_events
  stack=max(stack,b.max_stack);cases+=1
report={'status':'PASS','cases':cases,'GNU_diagnostic_adapter_cases':cases,'initial_SREG_values':256,'response_and_busy_poll_scenarios':3,
 'compared':['all32GPR','all8SREG','saved extended-bank registers','entire SPI/LE MMIO read/write trace','controller and LE final state','balanced data/call stack','no explicit CLI/SEI'],
 'maximum_push_stack_bytes':stack,'scope':'full original versus compiled linked C+ASM adapter; abstract SPI completion delays; no cycle, asynchronous IRQ, extended-address bus or electrical proof'}
(BUILD/'pll_legacy_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS original/compiled C PLL read:',cases,'cases; complete ABI and SPI traces; maximum push stack',stack)
