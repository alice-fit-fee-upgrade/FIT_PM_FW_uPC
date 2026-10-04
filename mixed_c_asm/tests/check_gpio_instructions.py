#!/usr/bin/env python3
"""Independent architecture fixtures for the new low-I/O interpreter opcodes.
Synthetic fixtures are not counted as compiled-firmware differential cases.
"""
from cpu_state import CPU
class IO:
 def __init__(self,value): self.mem={0:value}; self.trace=[]
 def read(self,a): self.trace.append(('read',a)); return self.mem[a]
 def write(self,a,v): self.trace.append(('write',a,v)); self.mem[a]=v
class GPIOCPU(CPU):
 def io_read(self,a): return self.io.read(a) if a==0 else super().io_read(a)
 def io_write(self,a,v):
  if a==0: self.io.write(a,v)
  else: super().io_write(a,v)
cases=0
# Skip length must come from the skipped instruction, including four-byte STS.
for op in ('sbic','sbis'):
 for value in (0,2):
  for length in (2,4):
   skipped=('ldi','r16, 1',2,None) if length==2 else ('sts','0x0605, r16',4,None)
   program={0:(op,'0, 1',2,None),2:skipped,2+length:('ret','',2,None)}
   io=IO(value); cpu=GPIOCPU(program,io); cpu.r[16]=0x55
   cpu.f=[1]*8; cpu.run(0,10)
   skip=bool(value&2)==(op=='sbis')
   assert cpu.f==[1]*8 and cpu.sp==0x3fff
   if length==2: assert cpu.r[16]==(0x55 if skip else 1)
   else: assert (0x0605 in io.mem)==(not skip)
   cases+=1
for op in ('cbi','sbi'):
 for bit in range(8):
  for value in range(256):
   for sreg in (0,0xff,0x55,0xaa):
    io=IO(value); cpu=GPIOCPU({0:(op,f'0, {bit}',2,None),2:('ret','',2,None)},io)
    cpu.f=[(sreg>>b)&1 for b in range(8)]; flags=cpu.f.copy(); cpu.run(0,10)
    assert cpu.f==flags and cpu.sp==0x3fff
    expected=value|(1<<bit) if op=='sbi' else value&~(1<<bit)
    assert io.mem[0]==expected
    cases+=1
print('PASS low-I/O opcode architecture fixtures:',cases)
