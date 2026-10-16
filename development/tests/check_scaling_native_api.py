#!/usr/bin/env python3
"""Numeric GNU API vs golden AVR unsigned scaler, full supported16-bit domains."""
import hashlib,json,random,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'source_recovery/tests'),str(ROOT/'mixed_c_asm/tests'),str(ROOT/'development/tools')]
from avr_oracle import PROGRAM,load_program
from cpu_state import CPU
from build import BUILD,symbols
program=load_program(BUILD/'development.bin');names=symbols(BUILD/'development.elf')
start=names['pm_dev_scale_unsigned'];rng=random.Random(0x554e5347)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
count=0;depth=0;begun=last=time.monotonic()
for coefficient in [0x020c,0x0272]:
 for value in range(65536):
  registers=templates[value&255].copy();registers[1]=0
  old=CPU(PROGRAM);old.r=registers.copy();old.r[18:22]=[coefficient&255,coefficient>>8,value&255,value>>8]
  old.run(0x214c,limit=100)
  new=CPU(program);new.r=registers.copy();new.r[22:26]=[value&255,value>>8,coefficient&255,coefficient>>8]
  new.run(start,limit=1000)
  assert old.r[16:18]==new.r[24:26],(coefficient,value)
  assert all(new.r[r]==registers[r] for r in list(range(2,18))+[28,29]) and new.r[1]==0
  assert new.sp==0x3fff and not new.calls
  assert all(new.minimum_sp<a<=0x3fff for a in new.mem)
  depth=max(depth,new.entry_sp-new.minimum_sp);count+=1
  if time.monotonic()-last>20:
   print('progress',count,'native cases',round(time.monotonic()-begun,1),'seconds',flush=True);last=time.monotonic()
 print('PASS unsigned GNU API all65536 inputs coefficient',hex(coefficient),flush=True)
assert count==131072
report={'status':'PASS','cases':count,'domain':'all65536 input words for each coefficient0x020c,0x0272',
 'contract':'GNU ABI numeric result, callee-saved GPRs, R1 zero, physical3-byte return stack, no MMIO/static RAM',
 'maximum_additional_stack_below_entry':depth,
 'canonical_candidate_sha256':hashlib.sha256((BUILD/'development.bin').read_bytes()).hexdigest(),
 'limits':'compiled AVR against original; not all2^32 argument pairs, cycle/async IRQ or hardware validation'}
(BUILD/'scaling_native_api_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS',count,'GNU API cases',flush=True)
