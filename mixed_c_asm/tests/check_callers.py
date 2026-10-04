#!/usr/bin/env python3
"""Exercise all four original users of both scalers through unchanged DAC SPI code.
Program-dependent compiler opcodes run to completion; no scaler stubs/hooks.
"""
import sys,pathlib,json,random
from cpu_state import CPU,ROOT
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM,load_program
MIXED=load_program(ROOT/'mixed_c_asm/build/flash_mixed.bin')
class IO:
 def __init__(self): self.trace=[]; self.status_reads=0
 def read(self,a):
  if 0x2163 <= a <= 0x217a:
   value=((a * 73) ^ (a >> 1)) & 255
   self.trace.append(('read',a,value)); return value
  assert a==0x08c2,('unexpected read',hex(a))
  self.status_reads+=1; value=128 if self.status_reads%2==0 else 0
  self.trace.append(('read',a,value)); return value
 def write(self,a,v): self.trace.append(('write',a,v))
 def irq(self,v): self.trace.append(('irq',bool(v)))
rng=random.Random(0x44414312)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
maximum=0; cases=0
for entry in (0x20ec,0x2104,0x20a6,0x20d0):
 for value in range(65536):
  state=templates[value&255].copy(); state[20]=value&255; state[21]=value>>8; state[22]=value%12
  flags=[(value>>b)&1 for b in range(8)]
  old_io=IO(); new_io=IO(); old=CPU(PROGRAM,old_io); new=CPU(MIXED,new_io)
  old.r=state.copy(); new.r=state.copy(); old.f=flags.copy(); new.f=flags.copy()
  old.run(entry,500); new.run(entry,1000)
  assert old.r==new.r and old.f==new.f,('DAC caller CPU state mismatch',hex(entry),value)
  assert old_io.trace==new_io.trace,('DAC caller bus/IRQ mismatch',hex(entry),value,old_io.trace,new_io.trace)
  assert old.sp==new.sp==0x3fff
  maximum=max(maximum,new.entry_sp-new.minimum_sp); cases+=1
 print(f'PASS original caller 0x{entry:04x}: all 65,536 inputs, cycled 12 channels',flush=True)
assert cases==262144 and maximum>=26
result={'cases':cases,'caller_entries':[0x20ec,0x2104,0x20a6,0x20d0],'signed_scaler_call_sites':[0x20f2,0x210a],'unsigned_scaler_call_sites':[0x20ac,0x20e0],'all_input_words_per_caller':65536,'cycled_channels':12,'registers_and_sreg_identical':True,'dac_spi_polling_write_irq_traces_identical':True,'maximum_additional_stack_below_caller_entry':maximum,'return_address_bytes':3,'limits':'scripted SPI-ready polling; no hardware/cycle/asynchronous interrupt validation'}
(ROOT/'mixed_c_asm/build/callers_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('ALL PASS caller composition:',cases,'cases;',maximum,'additional stack bytes',flush=True)
