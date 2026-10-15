"""Execute linked instrumentation instructions; no asynchronous IRQ/timing claim."""
import sys,random
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'source_recovery/tests'));from avr_oracle import Machine,load_program
sys.path.insert(0,str(ROOT/'development/tools'));from build import symbols,BUILD
names=symbols(BUILD/'development.elf');linked=load_program(BUILD/'development.bin')
class Model(Machine):
 def __init__(self,program,ram):
  self.ram=dict(ram);self.writes=[]
  def write(a,v):
   self.writes.append((a,v))
   if a==0xa0c:self.ram[a]=self.ram.get(a,0)&~v
   else:self.ram[a]=v
   if a==0x685:self.ram[0x684]=self.ram.get(0x684,0)|v
   if a==0x686:self.ram[0x684]=self.ram.get(0x684,0)&~v
  super().__init__(lambda a:self.ram.get(a,0),write,lambda e:None,program=program)
 def io_read(self,a):
  return super().io_read(a) if a==0x3f else self.ram.get(a,0)
 def io_write(self,a,v):
  if a==0x3f:super().io_write(a,v)
  else:self.ram[a]=v
rng=random.Random(2026);cases=0
for status in range(256):
 for scenario in ['init','start','prehigh','pg','pending','timeout','fault','already_done']:
  memory={0x684:4,0x688:8,0xa2a:1,0xa20:0x39,0xa21:0x30,0xa0c:0}
  if scenario=='init':entry=names['pm_pg_init_hook'];stop=0x106e
  elif scenario in ['start','prehigh']:entry=names['pm_pg_start_hook'];stop=names["pm_timer_at_0666"];memory[0x688]=8 if scenario=='prehigh' else 0
  elif scenario in ['pg','pending']:entry=names['pm_pg_irq_hook'];stop=names['PORTE_INT0_vect_isr'];memory[0x688]=0 if scenario=='pending' else 8
  else:
   entry=names['pm_pg_timeout_hook'] if scenario=='timeout' else names['pm_pg_thermal_hook'];stop=names['pm_timer_at_0692'] if scenario=='timeout' else names['pm_timer_at_05ea']
   if scenario=='already_done':memory[0xa2a]=2
  program=dict(linked);program[stop]=('ret','',2,None)
  m=Model(program,memory);regs=[rng.randrange(256) for _ in range(32)]
  if scenario=='init':regs[16]=6
  elif scenario not in ['pg','pending']:regs[16]=4
  m.r[:]=regs;m.f[:]=[(status>>i)&1 for i in range(8)];flags=list(m.f)
  m.run(entry,limit=300)
  assert m.r==regs and m.f==flags and not m.stack and not m.calls,(scenario,status)
  assert all(0xa00<=a<=0xa3f or a in [0x800,0x685,0x686] for a,v in m.writes)
  if scenario=='init':assert m.ram[0xa2a]==0 and m.ram[0x800]==6
  elif scenario=='start':assert m.ram[0xa2a]==1 and m.ram[0xa00]==7 and m.ram[0xa20]==0
  elif scenario=='prehigh':assert m.ram[0xa2a]==4 and m.ram[0xa00]==0
  elif scenario=='pg':assert m.ram[0xa2a]==2 and m.ram[0xa28]==0x39 and m.ram[0xa29]==0x30 and m.ram[0xa00]==0
  elif scenario=='pending':assert m.ram[0xa2a]==1 and not m.writes
  elif scenario=='already_done':assert m.ram[0xa2a]==2 and m.writes==[(0x686,4)]
  else:assert m.writes[0]==(0x686,4) and m.ram[0xa2a]==(3 if scenario=='timeout' else 5) and m.ram[0xa28]==0x39
  cases+=1
print('PASS PG instrumentation:',cases,'decoded cases; flags/registers preserved, power cut precedes telemetry')
# Run the complete legacy timer ISR at the PG deadline against baseline and
# candidate. Both must cut EN and produce the same legacy state/registers.
baseline=load_program(ROOT/'reference/flash_golden.bin')
for pg in [0,8]:
 mem={0x2159:0,0x215b:2,0x215c:1,0x215d:0,0x2157:pg,0x684:4,0x688:10|pg,0xa2a:1,0xa20:42,0xa21:0}
 machines=[]
 for program in [baseline,linked]:
  # PG-success path invokes system_init; substitute the same RET for both.
  p=dict(program);p[names['system_init']]=('ret','',2,None)
  m=Model(p,mem);m.run(names['TCC0_OVF_vect_isr']);machines.append(m)
 a,b=machines
 assert a.r==b.r and a.f==b.f and a.stack==b.stack and a.calls==b.calls
 assert {k:v for k,v in a.ram.items() if not 0xa00<=k<=0xa3f}=={k:v for k,v in b.ram.items() if not 0xa00<=k<=0xa3f}
 if not pg:assert a.ram[0x684]&4==0 and b.ram[0xa2a]==3
 else:assert a.ram[0x684]&4 and a.ram[0x215b]==3
print('PASS original PG deadline paths: missing PG cuts EN; present PG retains original transition')
mem={0x2157:1,0x215b:2,0x215c:100,0x215d:0,0x684:4,0x688:14,0xa2a:1,0xa20:39,0xa21:0}
a=Model(baseline,mem);b=Model(linked,mem)
a.run(0xac);b.run(0xac)
assert a.r==b.r and a.f==b.f and a.stack==b.stack and a.calls==b.calls
assert {k:v for k,v in a.ram.items() if not 0xa00<=k<=0xa3f}=={k:v for k,v in b.ram.items() if not 0xa00<=k<=0xa3f}
assert b.ram[0xa2a]==2 and b.ram[0xa28]==39
print('PASS full PG GPIO ISR: original status transition unchanged, elapsed captured')
