#!/usr/bin/env python3
"""Host-compiled production C against a line-level two-mux/INA I2C model."""
import ctypes as C,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];SRC=ROOT/'development/src';BUILD=ROOT/'development/build'
bridge=BUILD/'i2c_host_bridge.c'
bridge.write_text('''#include <stdint.h>
uint8_t (*rd)(uint16_t);void (*wr)(uint16_t,uint8_t);uint8_t (*enter)(void);void (*leave)(uint8_t);void (*delayfn)(void);
uint8_t pm_read8(uint16_t a){return rd(a);}void pm_write8(uint16_t a,uint8_t v){wr(a,v);}
uint8_t pm_irq_enter(void){return enter();}void pm_irq_leave(uint8_t v){leave(v);}void pm_i2c_delay(void){delayfn();}
''')
so=BUILD/'i2c_host.so'
subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-fPIC','-shared','-I'+str(SRC),str(SRC/'i2c_diag.c'),str(bridge),'-o',str(so)],check=True)
lib=C.CDLL(str(so))
READ=C.CFUNCTYPE(C.c_uint8,C.c_uint16);WRITE=C.CFUNCTYPE(None,C.c_uint16,C.c_uint8);ENTER=C.CFUNCTYPE(C.c_uint8);LEAVE=C.CFUNCTYPE(None,C.c_uint8);DELAY=C.CFUNCTYPE(None)
class Sample(C.Structure):_fields_=[('raw',C.c_uint16*6),('configuration',C.c_uint16),('mask70',C.c_uint8),('mask71',C.c_uint8)]
lib.pm_ina_read.argtypes=[C.c_uint8,C.POINTER(Sample)];lib.pm_ina_code.argtypes=[C.c_uint16];lib.pm_ina_code.restype=C.c_int16
class Bus:
 def __init__(self,case='ok'):
  self.case=case;self.mem={0x600:0xfb,0x604:0x12,0x6a0:9,0x6a4:1,0x6b7:0x10,0x684:4,0x688:10,0x628:64,0x620:0,15:0,14:0}
  if case=='busy':self.mem[15]=2
  if case=='pending':self.mem[15]=128
  if case=='off':self.mem[0x688]=0
  if case=='reset':self.mem[0x628]=0
  self.irq=128;self.slave=1;self.phase='IDLE';self.current=None;self.mask={0x70:2,0x71:8};self.reg=0;self.high=True;self.edges=[];self.readn=0;self.nacks=[];self.waits=0;self.starts=0;self.errors=[];self.received=[]
 def scl(self):return 0 if self.case=='timeout' or (self.case=='stretch' and self.starts>2) else int(not(self.mem[0x600]&2) or bool(self.mem[0x604]&2))
 def mastersda(self):return int(not(self.mem[0x6a0]&128) or bool(self.mem[0x6a4]&128))
 def sda(self):return self.mastersda()&self.slave
 def read(self,a):
  if a==0x608:return 16|(self.scl()<<1)
  if a==0x6a8:return self.sda()<<7
  return self.mem.get(a,0)
 def write(self,a,v):
  if a==15:self.mem[a]=v;return
  assert self.irq==128,('IRQ masked during I2C GPIO',a)
  assert a in (0x601,0x602,0x605,0x606,0x6a1,0x6a2,0x6a6,0x6b7),hex(a)
  oldc,oldd=self.scl(),self.sda()
  if a==0x6b7:self.mem[a]=v;return
  port=a&~31;off=a-port;mask=2 if port==0x600 else 128
  assert not v&~mask
  at=port+(4 if off>=4 else 0)
  if off in (1,5):self.mem[at]=self.mem.get(at,0)|v
  else:self.mem[at]=self.mem.get(at,0)&~v
  newc,newd=self.scl(),self.sda()
  if oldc==newc==1 and oldd!=newd:
   if not newd:self.start()
   else:self.stop()
  elif not oldc and newc:self.rise()
  elif oldc and not newc:self.fall()
 def start(self):
  self.starts+=1;self.phase='RX';self.bits=0;self.value=0;self.address=True;self.slave=1;self.current=None
 def stop(self):self.phase='IDLE';self.slave=1
 def accepted(self,v):
  self.received.append(v)
  if self.address:
   self.current=v>>1;self.reading=v&1;self.address=False
   if self.current==0x40:
    good=self.mask[0x70]==0 and self.mask[0x71] in (16,32,64) and self.case!='nack'
   else:good=self.current in self.mask
   return good
  if self.current in self.mask:self.mask[self.current]=v
  else:self.reg=v;self.high=True
  return True
 def nextbyte(self):
  if self.current in self.mask:return self.mask[self.current]
  words={0xfe:0x5449,0xff:0x3220,0:0x7127,1:0x0100,2:0x03e8,3:0xfff8,4:0x04b0,5:0x7ff8,6:0x0700}
  if self.case=='badid':words[0xfe]=0
  w=words.get(self.reg,0);v=w>>8 if self.high else w&255;self.high=not self.high;return v
 def send(self):self.tx=self.nextbyte();self.bits=0;self.phase='TX';self.slave=(self.tx>>7)&1
 def rise(self):
  if self.phase=='RX':
   self.value=(self.value<<1)|self.mastersda();self.bits+=1
   if self.bits==8:self.ack=self.accepted(self.value);self.phase='ACK_PREP'
  elif self.phase=='ACK':self.phase='ACK_DONE'
  elif self.phase=='TX':
   self.bits+=1
   if self.bits==8:self.phase='TX_LAST'
  elif self.phase=='MACK':self.ackmaster=not self.mastersda();self.nacks.append(not self.ackmaster);self.phase='MACK_DONE'
 def fall(self):
  if self.phase=='ACK_PREP':self.slave=int(not self.ack);self.phase='ACK'
  elif self.phase=='ACK_DONE':
   self.slave=1
   if self.ack and self.reading:self.send()
   else:self.phase='RX';self.bits=0;self.value=0
  elif self.phase=='TX':self.slave=(self.tx>>(7-self.bits))&1
  elif self.phase=='TX_LAST':self.slave=1;self.phase='MACK'
  elif self.phase=='MACK_DONE':
   if self.ackmaster:self.send()
   else:self.slave=1;self.phase='IDLE'
 def enter(self):old=self.irq;self.irq=0;return old
 def leave(self,v):self.irq=v
 def delay(self):self.waits+=1
 def attach(self):
  def safe(fn,default=None):
   def wrap(*args):
    try:return fn(*args)
    except Exception as e:self.errors.append(repr(e));return default
   return wrap
  self.refs=[READ(safe(self.read,0)),WRITE(safe(self.write)),ENTER(self.enter),LEAVE(self.leave),DELAY(self.delay)]
  for name,cb in zip(['rd','wr','enter','leave','delayfn'],self.refs):C.c_void_p.in_dll(lib,name).value=C.cast(cb,C.c_void_p).value
cases=0
for case,expected in [('ok',0),('busy',1),('pending',1),('off',8),('reset',4),('nack',2),('badid',7),('timeout',5),('stretch',5)]:
 b=Bus(case);b.attach();s=Sample();result=lib.pm_ina_read(4,C.byref(s));assert not b.errors,b.errors
 assert result==expected,(case,result,b.phase,b.received)
 assert b.mask=={0x70:2,0x71:8},(case,b.mask)
 assert not b.mem[15]&3 or case=='busy'
 if case=='ok':
  assert s.configuration==0x7127 and list(s.raw)==[0x100,0x3e8,0xfff8,0x4b0,0x7ff8,0x700]
  assert b.nacks and b.nacks[-1] and False in b.nacks,'MSB ACK / LSB NACK missing'
  assert b.starts>20,'repeated STARTs missing'
  assert b.mem[0x600]&2 and b.mem[0x604]&2 and not b.mem[0x6a0]&128
  assert b.mem[0x6b7]==0
 if case=='stretch':assert b.waits<2500 and b.mem[15]&64
 if case=='timeout':assert not b.mem[0x600]&2 and b.mem[15]&64
 print('PASS I2C',case,'result',result,'delays',b.waits);cases+=1
for raw in range(65536):
 expected=raw>>3;expected=expected-8192 if expected&4096 else expected
 assert lib.pm_ina_code(raw)==expected
print('PASS full signed conversion: 65536 raw words; I2C scenarios',cases)
