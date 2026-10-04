#!/usr/bin/env python3
"""Actual compiled ten-word FLASH-table PLL reset versus original execution."""
import json,hashlib,random
from cpu_state import CPU,ROOT
from avr_oracle import PROGRAM,load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
TABLE=(ROOT/'reference/flash_golden.bin').read_bytes()[0x2916:0x293e]
class IO:
 def __init__(self,ready,delay):self.ready=ready; self.delay=delay; self.poll=0; self.trace=[]
 def read(self,a):
  assert a==0x8c2,('unexpected read',hex(a))
  self.poll+=1; v=self.ready if self.poll>self.delay else self.ready&127
  self.trace.append(('read',a,v)); return v
 def write(self,a,v):
  assert a in (0x8c0,0x8c3,0x6a6,0x6a5,0x6a9,0x626),('unexpected write',hex(a),v)
  self.trace.append(('write',a,v))
  if a==0x8c3:self.poll=0
 def irq(self,v):self.trace.append(('irq',bool(v)))
rng=random.Random(0xddc); templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
cases=0; maximum=0
for ready in range(128,256):
 for sreg in range(256):
  state=templates[(ready+sreg)&255]; oi=IO(ready,sreg%4); ni=IO(ready,sreg%4)
  old=CPU(PROGRAM,oi); new=CPU(MIXED,ni)
  old.r=state.copy(); new.r=state.copy(); old.f=[(sreg>>b)&1 for b in range(8)]; new.f=old.f.copy()
  old.run(0xddc,10000); new.run(0xddc,10000)
  ctx=(ready,sreg)
  assert old.r==new.r==state,('registers',ctx,old.r,new.r,state)
  assert old.f==new.f,('SREG',ctx,old.f,new.f)
  assert oi.trace==ni.trace,('ordered table/SPI/IRQ effects',ctx,oi.trace,ni.trace)
  assert old.sp==new.sp==0x3fff
  assert sum(v<<b for b,v in enumerate(new.f))==((sreg&0x61)|0x82)
  assert [row for row in ni.trace if row[0]=='irq']==[('irq',False),('irq',True)]*10
  values=[row[2] for row in ni.trace if row[:2]==('write',0x8c3)]
  assert bytes(values)==TABLE
  maximum=max(maximum,new.entry_sp-new.minimum_sp); cases+=1
result={'cases':cases,'all_sreg_patterns':256,'spi_ready_values':128,'poll_delays':[0,1,2,3],'table_byte_range':[0x2916,0x293e],'transactions':10,'all_registers_sreg_table_values_spi_irq_and_return_stack_equal':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'bounded functional interpreter; LPM uses unchanged golden data; no cycle/asynchronous IRQ/hardware validation'}
(ROOT/'mixed_c_asm/build/pll_reset_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('PASS PLL control reset:',cases,'cases;',maximum,'additional stack bytes',flush=True)
