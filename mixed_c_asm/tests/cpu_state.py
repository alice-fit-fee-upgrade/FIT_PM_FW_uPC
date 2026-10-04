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
 def invalid_read(self,a):
  if self.io is not None: return self.io.read(a)
  raise AssertionError(('unexpected nonstack read',hex(a)))
 def invalid_write(self,a,v):
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

