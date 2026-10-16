#!/usr/bin/env python3
"""Composition through four original DAC callers; scripted SPI, not analog HW."""
import hashlib,json,random,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'source_recovery/tests'),str(ROOT/'mixed_c_asm/tests'),str(ROOT/'development/tools')]
from avr_oracle import PROGRAM,load_program
from cpu_state import CPU
from build import BUILD
program=load_program(BUILD/'development.bin')
class IO:
 def __init__(self):self.trace=[];self.reads=0
 def read(self,a):
  if 0x2163<=a<=0x217a:v=((a*73)^(a>>1))&255
  else:
   assert a==0x8c2,hex(a)
   self.reads+=1;v=128 if self.reads%2==0 else 0
  self.trace.append(('R',a,v));return v
 def write(self,a,v):self.trace.append(('W',a,v))
 def irq(self,v):self.trace.append(('IRQ',bool(v)))
class BankCPU(CPU):
 def __init__(self,program,io):self.banks={a:0 for a in range(0x38,0x3d)};super().__init__(program,io)
 def io_read(self,a):return self.banks[a] if a in self.banks else super().io_read(a)
 def io_write(self,a,v):
  if a in self.banks:self.banks[a]=v
  else:super().io_write(a,v)
rng=random.Random(0x43414c4c);count=maximum=0
entries=[0x20a6,0x20d0,0x20ec,0x2104]
values=[0,1,127,128,255,256,511,512,19999,20000,20001,32767,32768,65280,65534,65535]
for entry in entries:
 for value in values:
  for channel in range(12):
   for flags in [0,0x3f,0x80,0xff]:
    registers=[rng.randrange(256) for _ in range(32)];registers[20:23]=[value&255,value>>8,channel]
    machines=[];ios=[]
    for image in [PROGRAM,program]:
     io=IO();m=BankCPU(image,io);m.r=registers.copy();m.f=[(flags>>b)&1 for b in range(8)]
     m.run(entry,limit=2000);machines.append(m);ios.append(io)
    a,b=machines
    assert (a.r,a.f,a.banks)==(b.r,b.f,b.banks),(entry,value,channel,flags)
    assert ios[0].trace==ios[1].trace,(entry,value,channel,flags,'SPI/RAM/IRQ trace mismatch')
    assert a.sp==b.sp==0x3fff and not a.calls and not b.calls
    maximum=max(maximum,b.entry_sp-b.minimum_sp);count+=1
 print('PASS original DAC caller',hex(entry),'all12 channels x16 boundaries x4 SREG states',flush=True)
assert count==3072
report={'status':'PASS','cases':count,'original_callers':entries,'input_words':values,'channels':12,'initial_SREG':[0,63,128,255],
 'compared':'all32 GPRs, SREG, banks, original calibration RAM reads, SPI polling/write/IRQ traces, physical3-byte return stack',
 'maximum_additional_stack_below_caller_entry':maximum,
 'canonical_candidate_sha256':hashlib.sha256((BUILD/'development.bin').read_bytes()).hexdigest(),
 'limits':'boundary composition only; one deterministic calibration-RAM pattern and one polling pattern. Not exhaustive caller state/EEPROM domain, analog HW, cycle or async IRQ proof.'}
(BUILD/'scaling_callers_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS',count,'original caller compositions',flush=True)
