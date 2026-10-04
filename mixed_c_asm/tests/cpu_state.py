"""Three-byte-PC architectural stack adapter for the bounded AVR interpreter."""
import sys,pathlib
ROOT=pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import Machine
CALLER=0x20400

class CPU(Machine):
 def __init__(self,program,io=None):
  self.io=io
  super().__init__(self.invalid_read,self.invalid_write,self.invalid_irq,program=program)
  self.sp=0x3fff; self.mem={}; self._push_return(CALLER)
  self.entry_sp=self.sp; self.minimum_sp=self.sp; self.writes=0
 def io_read(self,a):
  if a==0x3d: return self.sp & 255
  if a==0x3e: return self.sp >> 8
  return super().io_read(a)
 def io_write(self,a,v):
  if a in (0x3d,0x3e):
   self.sp=((self.sp & 0xff00)|v) if a==0x3d else ((v<<8)|(self.sp & 255))
   assert 0x3f00 < self.sp <= 0x3fff,('invalid SP write',a,v,self.sp)
   self.minimum_sp=min(self.minimum_sp,self.sp)
   return
  super().io_write(a,v)
 def invalid_read(self,a):
  if 0x3f00 < a <= 0x3fff:
   assert a > self.sp and a in self.mem,('uninitialized/outside active stack read',hex(a),hex(self.sp))
   return self.mem[a]
  if self.io is not None: return self.io.read(a)
  raise AssertionError(('unexpected nonstack read',hex(a)))
 def invalid_write(self,a,v):
  if 0x3f00 < a <= 0x3fff:
   assert a > self.sp,('outside allocated stack write',hex(a),hex(self.sp))
   self.mem[a]=v; self.writes+=1; return
  if self.io is not None: return self.io.write(a,v)
  raise AssertionError(('unexpected nonstack write',hex(a),v))
 def invalid_irq(self,v):
  if self.io is not None: return self.io.irq(v)
  raise AssertionError(('unexpected interrupt enable change',v))
 def push_byte(self,value):
  assert 0x3f00<self.sp<=0x3fff,('stack outside isolated guard region',hex(self.sp))
  self.mem[self.sp]=value; self.sp-=1
  if hasattr(self,'minimum_sp'): self.minimum_sp=min(self.minimum_sp,self.sp); self.writes+=1
 def pop_byte(self):
  self.sp+=1
  assert self.sp<=0x3fff and self.sp in self.mem
  return self.mem[self.sp]
 def _push_return(self,address):
  word=address>>1
  for shift in (0,8,16): self.push_byte((word>>shift)&255)
 def _pop_return(self):
  word=0
  for _ in range(3): word=(word<<8)|self.pop_byte()
  return word<<1
 def enter_call(self,address):
  self._push_return(address); self.calls.append(address)
 def leave_call(self):
  actual=self._pop_return(); expected=self.calls.pop()
  assert actual==expected,('return address corrupted',hex(actual),hex(expected))
  return actual
 def finish_root_return(self):
  assert self._pop_return()==CALLER
  assert self.sp==0x3fff,'unbalanced entry/return stack'

