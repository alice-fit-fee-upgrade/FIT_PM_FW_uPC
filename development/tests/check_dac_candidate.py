#!/usr/bin/env python3
"""Execute original/full compiled DAC SPI driver and48-write updater.
Scripted SPI-ready/MMIO models, not analog outputs, cycle or async IRQ proof.
"""
import argparse,hashlib,json,random,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'source_recovery/tests'),str(ROOT/'mixed_c_asm/tests'),str(ROOT/'development/tools')]
from avr_oracle import PROGRAM,load_program
from cpu_state import CPU
from build import BUILD,symbols
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--smoke',action='store_true');p.add_argument('--sender-only',action='store_true');args=p.parse_args()
program=load_program(BUILD/'development.bin');names=symbols(BUILD/'development.elf')
assert 'pm_dac_patch_table' in names
class IO:
 def __init__(self,ram,delay):self.ram=ram;self.delay=delay;self.polls=0;self.trace=[];self.frames=[];self.active=None;self.cpu=None
 def read(self,a):
  if a in self.ram:v=self.ram[a]
  else:
   assert a==0x8c2,('unexpected read',hex(a))
   assert not self.cpu.f[7],'SPI read with IRQ enabled'
   self.polls+=1;v=0x2a|(128 if self.polls>self.delay else 0)
  self.trace.append(('R',a,v));return v
 def write(self,a,v):
  assert not self.cpu.f[7],'DAC SPI write with IRQ enabled'
  assert a in [0x8c0,0x8c3,0x645,0x646],('unexpected write',hex(a),v)
  if a==0x8c0:assert v==0xd5
  elif a==0x646:
   assert self.active is None and v in [1,2,4];self.active=[v,[]]
  elif a==0x8c3:
   assert self.active is not None;self.active[1].append(v);self.polls=0
  else:
   assert v==7 and self.active is not None and len(self.active[1])==3
   self.frames.append(self.active);self.active=None
  self.trace.append(('W',a,v))
 def irq(self,v):self.trace.append(('IRQ',bool(v)))
class BankCPU(CPU):
 def __init__(self,image,io,banks):self.banks=banks.copy();super().__init__(image,io);io.cpu=self
 def io_read(self,a):return self.banks[a] if a in self.banks else super().io_read(a)
 def io_write(self,a,v):
  if a in self.banks:self.banks[a]=v
  else:super().io_write(a,v)
rng=random.Random(0x44414343);templates=[[rng.randrange(256) for _ in range(32)] for _ in range(256)]
counts={'sender':0,'update':0};maximum={'sender':0,'update':0};old_maximum={'sender':0,'update':0};started=last=time.monotonic();stages=[]
def compare(entry,registers,flags,ram,delay,category):
 global last
 banks={a:(sum(registers)+a)&255 for a in range(0x38,0x3d)};machines=[];ios=[]
 for image in [PROGRAM,program]:
  io=IO(ram,delay);m=BankCPU(image,io,banks);m.r=registers.copy();m.f=[(flags>>b)&1 for b in range(8)]
  m.run(entry,limit=100000 if category=='update' else 2000);machines.append(m);ios.append(io)
 a,b=machines
 assert (a.r,a.f,a.banks)==(b.r,b.f,b.banks),(category,registers,flags,a.r,b.r,a.f,b.f)
 assert ios[0].trace==ios[1].trace,(category,registers,flags,ios[0].trace,ios[1].trace)
 assert a.sp==b.sp==0x3fff and not a.calls and not b.calls
 assert all(b.minimum_sp<addr<=0x3fff for addr in b.mem)
 assert len(ios[1].frames)==(48 if category=='update' else 1) and ios[1].active is None
 counts[category]+=1;maximum[category]=max(maximum[category],b.entry_sp-b.minimum_sp)
 old_maximum[category]=max(old_maximum[category],a.entry_sp-a.minimum_sp)
 if time.monotonic()-last>20:
  print('progress',counts,round(time.monotonic()-started,1),'seconds',flush=True);last=time.monotonic()
 return ios[1].frames

def sender(control,value,flags):
 r=templates[(control^value)&255].copy();r[16:18]=[value&255,value>>8];r[22]=control
 frames=compare(0x22aa,r,flags,{},value&3,'sender')
 group=control&0x30
 expected=[1 if group==0 else 2 if group==0x10 else 4,
           [((control<<4)&0xc0)|((control<<1)&6)|0x10,value>>8,value&255]]
 assert frames==[expected]
values=[0,1,127,128,255,256,32767,32768,65535] if args.smoke else range(65536)
for channel in [0,4,8]:
 for command in range(4):
  for value in values:sender(channel*4+command,value,value&255)
  print('PASS sender codewords, channel',channel,'output',command,flush=True)
# Every control byte (including non-application encodings), every SREG,
# two extreme data words. No claim of all256x65536 combinations.
for control in range(256):
 for flags in ([0,0xff] if args.smoke else range(256)):
  for value in [0,65535]:sender(control,value,flags)
print('PASS sender header/SREG matrix',counts['sender'],flush=True)
if not args.sender_only:
 for pattern in range(8):
  ram={a:((a*73)^(a>>1))&255 for a in range(0x2163,0x217b)}
  words=[]
  for n in range(48):
   v=[0,65535,0x7fff,0x8000,0x00ff,0xff00,20000,rng.randrange(65536)][(n+pattern)%8]
   words.append(v);ram[0x21cf+2*n]=v&255;ram[0x21d0+2*n]=v>>8
  for flags in ([0,0xff] if args.smoke else range(256)):
   r=templates[flags].copy()
   frames=compare(0x9de,r,flags,ram,pattern&3,'update')
   # Physical ordering: twelve channels, each threshold/CFDzero/ADCzero/delay.
   expected_headers=[(((channel*4+command)<<4)&0xc0)|(((channel*4+command)<<1)&6)|0x10 for channel in range(12) for command in [3,2,1,0]]
   assert [frame[1][0] for frame in frames]==expected_headers
  print('PASS48-frame update settings pattern',pattern,flush=True)
assert counts=={'sender':1132 if args.smoke else 917504,'update':0 if args.sender_only else(16 if args.smoke else 2048)}
report={'status':'SMOKE_ONLY' if args.smoke else 'PARTIAL_SENDER_ONLY' if args.sender_only else 'PASS_SIMULATION_MATRIX','cases':counts,'maximum_additional_stack_below_entry':maximum,'original_maximum_additional_stack_below_entry':old_maximum,
 'sender_coverage':('SMOKE: nine words x twelve representative headers; all256 controls x two SREG x two extreme words' if args.smoke else 'all65536 DAC words x four outputs x three representative CS groups; all256 control bytes x all256 SREG x two extreme words'),
 'update_coverage':('NOT_RUN' if args.sender_only else 'SMOKE: eight settings images x two SREG x48 writes' if args.smoke else 'eight settings images x all256 initial SREG; all48 writes each, exact ordered settings/calibration reads and SPI/IRQ trace'),
 'compared':'all32 GPR, all8 SREG, banks, physical3-byte call stack, complete SPI MMIO/IRQ traces, CS selection and frame bytes',
 'canonical_candidate_sha256':hashlib.sha256((BUILD/'development.bin').read_bytes()).hexdigest(),
 'limits':'NOT exhaustive over all settings/calibration RAM images, all header/data/state Cartesian products, timing, asynchronous IRQs, analog outputs or hardware. No DAC readback exists.',
 'hardware':'NOT_RUN; no board programming/reset in this pass'}
(BUILD/('dac_smoke.json' if args.smoke else 'dac_validation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(report['status'],counts,maximum,flush=True)
