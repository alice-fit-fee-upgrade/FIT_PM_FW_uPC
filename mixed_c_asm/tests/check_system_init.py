#!/usr/bin/env python3
"""Actual linked C/ASM startup vs golden GPIO/DMA/PLL and ABI behavior."""
import json, hashlib, random
from cpu_state import CPU, ROOT
from avr_oracle import PROGRAM, load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
class IO:
 def __init__(self,dma_ready,spi_ready,dma_delay,spi_delay):
  self.dma_ready=dma_ready; self.spi_ready=spi_ready
  self.dma_delay=dma_delay; self.spi_delay=spi_delay
  self.dma_poll=0; self.spi_poll=0; self.trace=[]
 def read(self,a):
  if a==0x100:
   self.dma_poll+=1
   v=self.dma_ready if self.dma_poll>self.dma_delay else self.dma_ready|0x40
  elif a==0x8c2:
   self.spi_poll+=1
   v=self.spi_ready if self.spi_poll>self.spi_delay else self.spi_ready&0x7f
  else: raise AssertionError(('unexpected read',hex(a)))
  self.trace.append(('read',a,v)); return v
 def write(self,a,v):
  assert a in ALLOWED,('unexpected write',hex(a),v)
  self.trace.append(('write',a,v))
  if a==0x8c3:self.spi_poll=0
 def irq(self,v):self.trace.append(('irq',bool(v)))
ALLOWED={int(row[1].split(',')[0],0) for a,row in PROGRAM.items()
         if (0xd10<=a<0xddc or 0x2486<=a<0x24ce) and row[0]=='sts'}
rng=random.Random(0xd10); cases=0; maximum=0
states=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
def check(sreg,dma_ready,spi_ready,dma_delay,spi_delay):
 global cases,maximum
 oi=IO(dma_ready,spi_ready,dma_delay,spi_delay); ni=IO(dma_ready,spi_ready,dma_delay,spi_delay)
 old=CPU(PROGRAM,oi); new=CPU(MIXED,ni); state=states[cases&255].copy()
 old.r=state.copy(); new.r=state.copy(); old.f=[(sreg>>b)&1 for b in range(8)]; new.f=old.f.copy()
 old.run(0xd10,1000); new.run(0xd10,1000)
 ctx=(sreg,dma_ready,spi_ready,dma_delay,spi_delay)
 assert old.r==new.r,('registers',ctx,old.r,new.r)
 assert old.f==new.f,('SREG',ctx,old.f,new.f)
 assert oi.trace==ni.trace,('GPIO/DMA/SPI/IRQ ordering',ctx,oi.trace,ni.trace)
 assert old.sp==new.sp==0x3fff
 assert new.r[16:20]==[8,0x10,0,0]
 assert sum(v<<b for b,v in enumerate(new.f))==((sreg&0x61)|0x82)
 assert [row for row in ni.trace if row[0]=='irq']==[('irq',False),('irq',True)]
 maximum=max(maximum,new.entry_sp-new.minimum_sp); cases+=1
for sreg in range(256):
 for dma_delay in range(4):
  for spi_delay in range(4):check(sreg,0x23,0x95,dma_delay,spi_delay)
for ready in range(256):
 if ready&0x40:continue
 for sreg in range(256):check(sreg,ready,0x80|(sreg&127),sreg%4,(sreg>>2)%4)
result={'cases':cases,'all_sreg_patterns':256,'dma_ready_values':128,'spi_ready_values':128,'dma_and_spi_delay_values':[0,1,2,3],'registers_sreg_ordered_gpio_dma_spi_irq_and_return_stack_equal':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'functional interpreter, scripted DMA/SPI polling; no timeout added; no cycle/asynchronous IRQ/hardware validation'}
(ROOT/'mixed_c_asm/build/system_init_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('PASS system init:',cases,'cases;',maximum,'additional stack bytes',flush=True)
