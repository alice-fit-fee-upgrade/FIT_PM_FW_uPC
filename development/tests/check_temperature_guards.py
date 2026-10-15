#!/usr/bin/env python3
"""Original ISR bytes retained except explicit hooks; compare displaced release."""
import sys,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'development/tools'),str(ROOT/'source_recovery/tests')]
from build import BUILD,symbols
from avr_oracle import Machine,load_program
from power_patch import SITES
names=symbols(BUILD/'development.elf');new=load_program(BUILD/'development.bin');old=load_program()
golden=(ROOT/'reference/flash_golden.bin').read_bytes();actual=(BUILD/'development.bin').read_bytes()
allowed={a+i for a,_ in SITES for i in range(4)}|set(range(0x77e,0x782))|set(range(0x788,0x78c))
for a in range(0x49e,0x8e4):
 if a not in allowed:assert actual[a]==golden[a],hex(a)
cases=0
for flags in range(256):
 for sample in (0,4,255):
  for seen in (0,1,2,4,7):
   results=[]
   for program,start in [(old,0x788),(new,names['pm_temperature_release'])]:
    p=dict(program)
    for end in (0x78c,0x78e):p[end]=('ret','',2,None)
    class Model(Machine):
     def io_read(self,a):return super().io_read(a) if a==63 else 2
     def io_write(self,a,v):
      if a==63:super().io_write(a,v)
      else:assert a==15 and v==0
    stores={}
    def store(a,v):
     assert a in (0xa2c,0xa2e),hex(a)
     stores[a]=v
    m=Model(lambda a:seen if a==0xa2e else 0,store,lambda e:None,program=p)
    m.r[16]=0xa7;m.r[20]=sample;m.r[17]=123;m.f[:]=[(flags>>i)&1 for i in range(8)];m.run(start,limit=30)
    if program is new:assert stores=={0xa2c:sample&7,0xa2e:seen|(sample&7)},stores
    else:assert not stores
    results.append((m.r,m.f,m.pc))
   assert results[0]==results[1]
   cases+=1
report={'status':'PASS','mode':'original ISR body in place','release_flag_cases':cases,'thermal_trace':'disabled TCE0 CCC/D low bytes: last/sticky sensor alarm bits','temperature_gate_sites':[0x77e,0x788],'scope':'Original bytes outside explicit gates unchanged; displaced EOR/SBRS register/flag/branch outcomes match. Hardware test separate.'}
(BUILD/'timer_relocation_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS in-place timer guards:',report)
