#!/usr/bin/env python3
"""Compare compiled settings C against original RAM/SPI/IRQ and ABI behavior.
"""
import random, json, hashlib, argparse
from cpu_state import CPU, ROOT
from avr_oracle import PROGRAM, load_program
MIXED=load_program(ROOT/"mixed_c_asm/build/flash_mixed.bin")
parser=argparse.ArgumentParser()
parser.add_argument("--entry",type=lambda s:int(s,0),action="append", choices=(0x08e4,0x098a))
entries=parser.parse_args().entry or [0x08e4,0x098a]
class IO:
 def __init__(self,rng,delay):
  self.mem={a:rng.randrange(256) for a in range(0x2160,0x2234)}
  self.mem[0x2441]=rng.randrange(256)
  self.delay=delay; self.poll=0; self.trace=[]
 def read(self,a):
  if a==0x08c2:
   self.poll+=1; value=0x95 if self.poll>self.delay else 0x15
  else: value=self.mem[a]
  self.trace.append(('read',a,value)); return value
 def write(self,a,v):
  assert a in (0x08c0,0x08c3,0x0665,0x0666),(hex(a),v)
  self.trace.append(('write',a,v))
  if a==0x08c3:self.poll=0
 def irq(self,v):self.trace.append(('irq',bool(v)))
rng=random.Random(0x8e4); cases=0; rows=[]
for entry,transactions,arithmetic in ((0x08e4,42,0x82),(0x098a,61,0x8c)):
 if entry not in entries:continue
 maximum=0; before=cases
 for pattern in ('random','zero','ones','address'):
  for delay in range(4):
   for sreg in range(256):
    io=IO(rng,delay); cpu=CPU(PROGRAM,io)
    if pattern!='random':
     io.mem={a:(0 if pattern=='zero' else 255 if pattern=='ones' else a&255) for a in io.mem}
    state=[rng.randrange(256) for _ in range(32)]; cpu.r=state.copy()
    cpu.f=[(sreg>>b)&1 for b in range(8)]
    other=IO(random.Random(0),delay); other.mem=io.mem.copy()
    rebuilt=CPU(MIXED,other); rebuilt.r=state.copy(); rebuilt.f=cpu.f.copy()
    cpu.run(entry,30000); rebuilt.run(entry,30000)
    assert cpu.r==rebuilt.r,('register difference',entry,sreg,delay,cpu.r,rebuilt.r)
    assert cpu.f==rebuilt.f,('flag difference',entry,sreg,delay,cpu.f,rebuilt.f)
    assert io.trace==other.trace,('ordered RAM/SPI/IRQ',entry,sreg,delay)
    assert cpu.sp==rebuilt.sp==0x3fff
    expected=state.copy()
    if entry==0x08e4:expected[28]=0xb7
    assert cpu.r==expected,('registers',entry,sreg,delay,cpu.r,expected)
    assert sum(v<<b for b,v in enumerate(cpu.f))==arithmetic|(sreg&0x40),('SREG',entry,sreg,delay,cpu.f)
    assert [v for op,*v in io.trace if op=='irq']==[[False],[True]]*transactions
    assert sum(row[:2]==('write',0x08c3) for row in io.trace)==transactions*4
    assert cpu.sp==0x3fff
    maximum=max(maximum,rebuilt.entry_sp-rebuilt.minimum_sp); cases+=1
 rows.append({'entry':entry,'cases':cases-before,'transactions':transactions,'maximum_additional_stack':maximum,'final_sreg_fixed_bits':arithmetic,'preserved_sreg_mask':0x40,'r28_output':0xb7 if entry==0x08e4 else 'preserved'})
result={'baseline_only':False,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'cases':cases,'entries':rows,'all_registers_sreg_irq_and_return_stack_contracts_verified':True,'limits':'actual linked AVR execution in bounded functional interpreter; no cycle, asynchronous IRQ or hardware validation'}
(ROOT/'mixed_c_asm/build/fpga_settings_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('PASS compiled AVR FPGA settings:',cases,'cases')
