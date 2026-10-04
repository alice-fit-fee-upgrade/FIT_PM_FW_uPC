#!/usr/bin/env python3
"""Actual original/compiled AVR comparisons: byte-channel wrap and calibration carry.
Includes out-of-range channels to preserve original behavior without sanitizing it.
"""
import sys, random, json, hashlib
from cpu_state import CPU, ROOT
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM, load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
class IO:
 def __init__(self,calibration): self.calibration=calibration; self.trace=[]; self.poll=0
 def read(self,a):
  if 0x2163<=a<=0x2262:
   value=(self.calibration >> (8*((a-0x2163)&1))) & 255
  else:
   assert a==0x08c2,('unexpected read',hex(a))
   self.poll+=1; value=128 if self.poll%2==0 else 0
  self.trace.append(('read',a,value)); return value
 def write(self,a,v): self.trace.append(('write',a,v))
 def irq(self,v): self.trace.append(('irq',bool(v)))
rng=random.Random(0x44414345)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
cases=0; maximum=0

def check(entry,value,channel,calibration):
 global cases,maximum
 state=templates[cases&255].copy(); state[20]=value&255; state[21]=value>>8; state[22]=channel
 flags=[(cases>>b)&1 for b in range(8)]
 old_io=IO(calibration); new_io=IO(calibration)
 old=CPU(PROGRAM,old_io); new=CPU(MIXED,new_io)
 old.r=state.copy(); new.r=state.copy(); old.f=flags.copy(); new.f=flags.copy()
 old.run(entry,500); new.run(entry,1000)
 context=(hex(entry),value,channel,calibration)
 assert old.r==new.r,('register mismatch',context,[(i,a,b) for i,(a,b) in enumerate(zip(old.r,new.r)) if a!=b])
 assert old.f==new.f,('SREG mismatch',context,old.f,new.f)
 assert old_io.trace==new_io.trace,('bus/IRQ mismatch',context,old_io.trace,new_io.trace)
 assert old.sp==new.sp==0x3fff
 if entry==0x20a6:
  expected=0x2163+((channel*2)&255)
  reads=[x[1] for x in new_io.trace if x[0]=='read' and x[1]!=0x08c2]
  assert reads==[expected,expected+1] and new.ptr('Z')==expected+1
 maximum=max(maximum,new.entry_sp-new.minimum_sp); cases+=1

edges=(0,1,127,128,255,256,0x7fff,0x8000,19999,20000,20001,0xffff)
for entry in (0x20a6,0x20d0,0x20ec,0x2104):
 for channel in range(256):
  for value in edges:
   for calibration in ((0,0x7fff,0x8000,0xffff) if entry==0x20a6 else (0,)):
    check(entry,value,channel,calibration)
 print(f'PASS DAC entry {entry:#06x}: all 256 channel bytes and boundary values',flush=True)
for calibration in range(65536): check(0x20a6,1234,calibration&255,calibration)
print('PASS all 65,536 calibration words; channel/SREG patterns cycled',flush=True)
assert cases==87040
result={'cases':cases,'all_channel_bytes':256,'calibration_words':65536,'boundary_inputs':list(edges),'calibration_scan_input':1234,'full_registers_sreg_spi_irq_and_return_stack_equal':True,'calibration_read_order_and_final_z_checked':True,'maximum_additional_stack':maximum,'mixed_bin_sha256':hashlib.sha256((ROOT/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest(),'limits':'functional bounded interpreter; scripted hardware; no cycle/async interrupt/hardware validation'}
(ROOT/'mixed_c_asm/build/dac_edges_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('ALL PASS DAC boundary/calibration cases:',cases,flush=True)
