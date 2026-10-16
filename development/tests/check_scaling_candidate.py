#!/usr/bin/env python3
"""Full16-bit application domains, actual linked AVR C+ASM vs original opcodes.
No host arithmetic callbacks, MMIO substitution or hardware programming.
"""
import argparse, hashlib, json, random, subprocess, sys, time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'source_recovery/tests'),str(ROOT/'mixed_c_asm/tests'),str(ROOT/'development/tools')]
from avr_oracle import PROGRAM,load_program
from cpu_state import CPU
from build import BUILD,symbols
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--smoke',action='store_true',help='quick development check, not qualification')
args=parser.parse_args()
new_program=load_program(BUILD/'development.bin');names=symbols(BUILD/'development.elf')
assert 'pm_scaling_patch_table' in names,'build with PM_SCALING_CANDIDATE=1'
# Static direct-call audit is not a proof about arbitrary indirect callers;
# the assembly coefficient guard retains original behavior for those too.
expected_calls={0x20ac:(0x214c,0x020c),0x20e0:(0x214c,0x0272),0x20f2:(0x2130,0x4188),0x210a:(0x2130,0x4188)}
actual_calls={a:row[3] for a,row in PROGRAM.items() if row[0] in ('rcall','call','jmp','rjmp') and row[3] in (0x2130,0x214c)}
assert actual_calls=={a:v[0] for a,v in expected_calls.items()},actual_calls
for address,(target,coefficient) in expected_calls.items():
 for offset,reg,value in [(-4,'r18',coefficient&255),(-2,'r19',coefficient>>8)]:
  row=PROGRAM[address+offset]
  assert row[0]=='ldi' and row[1].split(',')[0]==reg and int(row[1].split(',')[1],0)==value

rng=random.Random(0x5343414c)
templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
class BankCPU(CPU):
 def __init__(self,program,banks):
  self.banks=banks.copy();super().__init__(program)
 def io_read(self,a):return self.banks[a] if a in self.banks else super().io_read(a)
 def io_write(self,a,v):
  if a in self.banks:self.banks[a]=v
  else:super().io_write(a,v)
count=0;max_depth=0;stages=[];started=time.monotonic();last=started

def check(entry,coefficient,value,flags):
 global count,max_depth,last
 state=templates[count&255].copy();state[18:22]=[coefficient&255,coefficient>>8,value&255,value>>8]
 bank={a:((count*17+a)&255) for a in range(0x38,0x3d)}
 machines=[]
 for program in [PROGRAM,new_program]:
  m=BankCPU(program,bank);m.r=state.copy();m.f=[(flags>>i)&1 for i in range(8)]
  m.run(entry,limit=2000);machines.append(m)
 old,new=machines
 assert (old.r,old.f,old.banks)==(new.r,new.f,new.banks),(hex(entry),hex(coefficient),hex(value),hex(flags),old.r,new.r,old.f,new.f)
 assert old.sp==new.sp==0x3fff and not old.calls and not new.calls
 assert all(new.minimum_sp<a<=0x3fff for a in new.mem),'nonstack RAM write'
 depth=new.entry_sp-new.minimum_sp
 selected=coefficient in ((0x4188,) if entry==0x2130 else (0x020c,0x0272))
 assert (depth>2 if selected else depth==2),('incorrect C/fallback routing',entry,coefficient,depth)
 max_depth=max(max_depth,depth);count+=1
 if time.monotonic()-last>20:
  print('progress',count,'cases',round(time.monotonic()-started,1),'seconds',flush=True);last=time.monotonic()

def stage(name,before):
 stages.append({'name':name,'cases':count-before});print('PASS',name,count-before,'cases',flush=True)

values=[0,1,127,128,255,256,32767,32768,65535] if args.smoke else range(65536)
for entry,coeff in [(0x2130,0x4188),(0x214c,0x020c),(0x214c,0x0272)]:
 before=count
 for ti in [0,0x40,0x80,0xc0]:
  for value in values:check(entry,coeff,value,ti|(value&63))
 stage(f'entry{entry:04x} coefficient{coeff:04x}: '+('smoke values' if args.smoke else 'all65536 input words')+' x four T/I states',before)
# Every SREG value at arithmetic boundaries; six incoming arithmetic flags
# are overwritten in both original bodies, leaving only T/I as input state.
before=count
for entry,coeff in [(0x2130,0x4188),(0x214c,0x20c),(0x214c,0x272)]:
 for flags in range(256):
  for value in [0,127,128,255,256,32767,32768,65535]:check(entry,coeff,value,flags)
stage('all256 SREG values x arithmetic boundaries',before)
# Unsupported coefficients must use retained original code, not unchecked C.
before=count
coefficients=[0,1,255,256,0x20b,0x20c,0x20d,0x271,0x272,0x273,0x4187,0x4188,0x4189,65535] if args.smoke else range(65536)
for entry in [0x2130,0x214c]:
 for coeff in coefficients:
  for value in [0,65535]:check(entry,coeff,value,(coeff^value)&255)
stage(('smoke coefficients' if args.smoke else 'all65536 coefficients')+' x two boundary inputs: C selection/original fallback',before)
if not args.smoke:assert count==1054720
report={'status':'SMOKE_ONLY' if args.smoke else 'PASS_EXHAUSTIVE_APPLICATION_INPUT_DOMAINS',
 'direct_call_audit':{hex(a):{'target':hex(t),'coefficient':hex(c)} for a,(t,c) in expected_calls.items()},
 'compiler':subprocess.check_output(['avr-gcc','--version'],text=True).splitlines()[0],
 'golden_sha256':hashlib.sha256((ROOT/'reference/flash_golden.bin').read_bytes()).hexdigest(),
 'cases':count,'stages':stages,'maximum_additional_stack_below_entry':max_depth,
 'C_domains':{'signed':'coefficient0x4188; all65536 input words x all4 T/I combinations','unsigned':'coefficients0x020c,0x0272; each all65536 input words x all4 T/I combinations'},
 'checks':['all32 GPRs including dirty R1:R0','all8 SREG bits','extended-bank save/restore','three-byte return addresses and physical stack balance','no MMIO/IRQ/static RAM accesses','fallback route for other coefficients'],
 'canonical_candidate_sha256':hashlib.sha256((BUILD/'development.bin').read_bytes()).hexdigest(),
 'coverage_limit':'NOT exhaustive over all2^32 coefficient/input pairs or all arbitrary CPU states. Other coefficients retain original ASM. Bank tests model register restoration, not extended-address electrical bus behavior.',
 'hardware':'NOT_RUN; ADT startup regression blocks deployment; no programming by this test',
 'timing':'functional decoded instruction model; no cycle/async IRQ or total system stack proof'}
(BUILD/('scaling_smoke.json' if args.smoke else 'scaling_validation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(report['status'],count,'cases; max additional stack',max_depth,flush=True)
