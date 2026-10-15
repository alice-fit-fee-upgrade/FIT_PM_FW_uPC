#!/usr/bin/env python3
"""Decoded reset adapter ABI/counted delay; no electrical timing simulation."""
import json,random,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import Machine,load_program
sys.path.insert(0,str(ROOT/'development/tools'))
from build import BUILD,symbols
names=symbols(BUILD/'development.elf');linked=load_program(BUILD/'development.bin')
assert linked[0x1100][0]=='call' and linked[0x1100][3]==names['pm_adt_reset_ready']
loop=names['pm_adt_reset_delay_loop']
assert linked[loop-4][0]=='ldi' and '0xC0' in linked[loop-4][1]
assert linked[loop-2][0]=='ldi' and '0x12' in linked[loop-2][1]
assert [linked[loop+i][0] for i in (0,2,4)]==['subi','sbci','brne']
assert linked[loop+4][3]==loop
class Reset(Machine):
 def enter_call(self,address):
  if self.program[address-4][3]==names['adt7311_faults_clr']:
   self.r[16]=0x10;self.r[17]=0xa6;self.r[18]=0;self.f[:]=[(self.status>>i)&1 for i in range(8)]
   self.expected=(self.r.copy(),self.f.copy())
  super().enter_call(address)
rng=random.Random(600);cases=0
for status in [0,0xff,0x35,0x80]:
 for _ in range(4):
  p=dict(linked);p[names['adt7311_faults_clr']]=('ret','',2,None)
  m=Reset(lambda a:0,lambda a,v:None,lambda v:None,program=p);m.status=status
  m.r[:]=[rng.randrange(256) for _ in range(32)];m.run(names['pm_adt_reset_ready'])
  assert (m.r,m.f)==m.expected and not m.stack and not m.calls
  cases+=1
report={'status':'PASS','cases':cases,'counter':4800,'loop_cycles':19199,'minimum_cycles_including_counter_load':19201,'clock_hz':32000000,'scope':'decoded register/SREG preservation and counted startup delay; substituted legacy reset, no electrical proof'}
(BUILD/'adt_reset_ready_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS reset-ready adapter:',cases,'cases, >=600 us at 32 MHz')
