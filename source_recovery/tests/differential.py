#!/usr/bin/env python3
"""Native recovered C versus bounded execution of original FLASH instructions."""
import ctypes as C,random,json,pathlib
from avr_oracle import Machine,FLASH,ROOT
rng=random.Random(0x504d12)
lib=C.CDLL(str(ROOT/'source_recovery/build/libpm_recovered.so'))
U8=C.c_uint8; U16=C.c_uint16; U32=C.c_uint32; P=C.c_void_p
READ=C.CFUNCTYPE(U8,P,U16); WRITE=C.CFUNCTYPE(None,P,U16,U8); IRQ=C.CFUNCTYPE(None,P,C.c_bool)
GET=C.CFUNCTYPE(U8,P); PUT=C.CFUNCTYPE(None,P,U8)
class Bus(C.Structure): _fields_=[('context',P),('read8',READ),('write8',WRITE),('flash8',READ),('irq',IRQ)]
class Stream(C.Structure): _fields_=[('context',P),('get',GET),('put',PUT)]
class Number(C.Structure): _fields_=[('value',U16),('terminator',U8),('error',C.c_bool)]

def bind(name,args,result=None):
 f=getattr(lib,name); f.argtypes=args; f.restype=result; return f
parse_integer=bind('pm_parse_integer',[C.POINTER(Stream)],Number)
parse_hex=bind('pm_parse_hex',[C.POINTER(Stream)],Number)
hex16=bind('pm_send_hex16',[C.POINTER(Stream),U16])
hex_digit=bind('pm_send_hex_digit',[C.POINTER(Stream),U8])
decimal=bind('pm_send_decimal',[C.POINTER(Stream),U16,C.c_bool,U8])
signed_scale=bind('pm_scale_signed',[U16,U16],U16); unsigned_scale=bind('pm_scale_unsigned',[U16,U16],U16)
next_char=bind('pm_console_next',[C.POINTER(Bus),C.c_bool],U8)
send_char=bind('pm_console_send',[C.POINTER(Bus),U8])

class IO:
 def __init__(self,values=None,queues=None):
  self.mem=dict(values or {}); self.queues={a:list(q) for a,q in (queues or {}).items()}; self.trace=[]; self.errors=[]; self.position=0; self.input=b''; self.output=[]
 def read(self,a):
  q=self.queues.get(a,[])
  if q: v=q.pop(0)
  else: v=self.mem.get(a,0x80 if a==0x08c2 else 0)
  self.trace.append(('read',a,v)); return v
 def write(self,a,v): self.trace.append(('write',a,v)); self.mem[a]=v
 def irq(self,v): self.trace.append(('irq',bool(v)))
 def get(self):
  if self.position>=len(self.input): raise AssertionError(('input exhausted',self.input))
  v=self.input[self.position]; self.position+=1; return v
 def put(self,v): self.output.append(v)
 def cget(self,_):
  try: return self.get()
  except Exception as exc: self.errors.append(str(exc)); return 13
 def bus(self):
  self.refs=[READ(lambda _,a:self.read(a)),WRITE(lambda _,a,v:self.write(a,v)),READ(lambda _,a:FLASH[a]),IRQ(lambda _,v:self.irq(v))]
  return Bus(None,*self.refs)
 def stream(self):
  self.refs=[GET(self.cget),PUT(lambda _,v:self.put(v))]; return Stream(None,*self.refs)
 def machine(self,stream=False):
  return Machine(self.read,self.write,self.irq,self.get if stream else None,self.put if stream else None)

counts={}
def check_parser(data,entry,func,name):
 io=IO(); io.input=data; m=io.machine(True).run(entry)
 ci=IO(); ci.input=data; s=ci.stream(); result=func(C.byref(s))
 actual=(result.value,result.terminator,bool(result.error),ci.position)
 expected=(m.r[20]|m.r[21]<<8,m.r[16],bool(m.f[0]),io.position)
 assert not ci.errors and actual==expected,(name,data,actual,expected,ci.errors)
 counts[name]=counts.get(name,0)+1
for i in range(65536):
 check_parser(str(i).encode()+b'\r',0x2634,parse_integer,'parse_integer')
for i in list(range(0,32770))+[65535,65536,99999,100000]:
 check_parser(b'-'+str(i).encode()+b'\r',0x2634,parse_integer,'parse_integer')
for data in [b'',b'-',b'--1',b'+1',b'65536',b'999999',b'-65536',b'-32769',b'-0',b'0'*255,b'0'*256,b'0'*257,b'-'+b'0'*254,b'0'*255+b'-1']:
 check_parser(data+b'\r',0x2634,parse_integer,'parse_integer')
for ch in range(256):
 for prefix in (b'',b'1',b'-1'):
  check_parser(prefix+bytes([ch])+b'\r\r',0x2634,parse_integer,'parse_integer')
for i in range(65536): check_parser(f'{i:04X}\r'.encode(),0x26ac,parse_hex,'parse_hex')
for ch in range(256):
 for prefix in (b'',b'F',b'FFFF'):
  check_parser(prefix+bytes([ch])+b'\r',0x26ac,parse_hex,'parse_hex')
print('PASS parser returns, partial-error values and consumed input',counts,flush=True)

def check_output(entry,func,args,value,name):
 io=IO(); m=io.machine(True); m.r[16]=value&255; m.r[17]=value>>8; m.run(entry)
 ci=IO(); s=ci.stream(); func(C.byref(s),*args)
 assert ci.output==io.output,(name,value,bytes(ci.output),bytes(io.output))
 counts[name]=counts.get(name,0)+1
for value in range(256): check_output(0x2720,hex_digit,[value],value,'hex_digit')
for value in range(65536): check_output(0x26f8,hex16,[value],value,'hex16')
values=sorted(set([0,1,9,10,99,100,999,1000,9999,10000,32767,32768,65535]+[rng.randrange(65536) for _ in range(3000)]))
for entry,signed,places in [(0x272e,True,2),(0x2736,True,1),(0x273e,True,3),(0x2746,True,0),(0x274e,False,0)]:
 for value in values: check_output(entry,decimal,[value,signed,places],value,'decimal')
print('PASS console emitted bytes including signed widths/decimal positions',counts,flush=True)

for entry,func,name,coefficient in [(0x2130,signed_scale,'signed_scale',0x4188),(0x214c,unsigned_scale,'unsigned_scale',0x0272)]:
 for value in range(65536):
  io=IO(); m=io.machine(); m.r[18]=coefficient&255; m.r[19]=coefficient>>8; m.r[20]=value&255; m.r[21]=value>>8; m.run(entry)
  expected=m.r[16]|m.r[17]<<8; actual=func(coefficient,value)
  assert actual==expected,(name,hex(coefficient),hex(value),hex(actual),hex(expected))
  counts[name]=counts.get(name,0)+1
for _ in range(4000):
 coefficient=rng.randrange(65536); value=rng.randrange(65536)
 for entry,func,name in [(0x2130,signed_scale,'signed_scale'),(0x214c,unsigned_scale,'unsigned_scale')]:
  io=IO(); m=io.machine(); m.r[18]=coefficient&255; m.r[19]=coefficient>>8; m.r[20]=value&255; m.r[21]=value>>8; m.run(entry)
  assert func(coefficient,value)==m.r[16]|m.r[17]<<8,(name,coefficient,value)
  counts[name]+=1
print('PASS fixed-point results including staged unsigned saturation',counts,flush=True)

def trace_case(entry,name,argtypes,args,regs,result_regs=None,values=None,queues=None,result_type=None):
 io=IO(values,queues); m=io.machine()
 for n,v in regs.items(): m.r[n]=v
 m.run(entry)
 ci=IO(values,queues); b=ci.bus(); func=bind(name,[C.POINTER(Bus)]+argtypes,result_type); result=func(C.byref(b),*args)
 assert io.trace==ci.trace,(name,args,'trace',next(((i,x,y) for i,(x,y) in enumerate(zip(io.trace,ci.trace)) if x!=y),None),len(io.trace),len(ci.trace))
 if result_regs is not None:
  expected=sum(m.r[n]<<(8*i) for i,n in enumerate(result_regs))
  assert result==expected,(name,args,'result',result,expected)
 counts[name]=counts.get(name,0)+1

# Eight input characters with bit7 set prove signed comparison behavior;
# RTS threshold intentionally writes full previous PORTF.OUT at exact occupancy.
for ch in range(256):
 for raw in (False,True):
  for head,tail,port in [(0,1,1),(63,20,0x31)]:
   h=(head+1)&63; values={0x2000:head,0x2001:tail,0x2005:0,0x06a4:port,0x2007+h:ch}
   trace_case(0x2836 if raw else 0x283c,'pm_console_next',[C.c_bool],[raw],{},[16],values,result_type=U8)
for head,tail in [(0,0),(255,255),(0,1),(5,3),(0,254)]:
 for ready in (0,1):
  for status in (0,32):
   trace_case(0x28ac,'pm_console_send',[U8],[0xa5],{16:0xa5},values={0x2002:head,0x2003:tail,0x2004:ready,0x0ba1:status,0x0ba3:0x20})
# Busy RX and full TX loops, externally advancing pointer simulates an ISR.
trace_case(0x283c,'pm_console_next',[C.c_bool],[False],{},[16],{0x2000:0,0x2001:1,0x2008:ord('z')},queues={0x2001:[0,1]},result_type=U8)
trace_case(0x28ac,'pm_console_send',[U8],[42],{16:42},values={0x2002:2,0x2003:0},queues={0x2002:[1,2]})
print('PASS UART MMIO, ring boundaries, flow-control and busy loops',flush=True)

for _ in range(100):
 value=rng.randrange(65536); cmd=rng.randrange(256); responses=[rng.randrange(256) for _ in range(32)]
 trace_case(0x2608,'pm_adt7311_byte',[U8],[cmd],{16:cmd},[16],queues={0x0608:responses[:8]},result_type=U8)
 trace_case(0x2598,'pm_adt7311_write8',[U8,U8],[cmd,value&255],{16:cmd,17:value&255},queues={0x0608:responses[:16]})
 trace_case(0x25be,'pm_adt7311_exchange16',[U8,U16],[cmd,value],{16:cmd,17:value&255,18:value>>8},[20,21],queues={0x0608:responses[:24]},result_type=U16)
 trace_case(0x25ea,'pm_adt7311_clear',[],[],{},queues={0x0608:responses})
for reg in range(256):
 value=rng.randrange(65536); seed=rng.randrange(256)
 queues={0x08c2:[0,0,128]*4,0x08c3:[0x56,0x78]}
 trace_case(0x230e,'pm_fpga_write16',[U8,U16],[reg,value],{18:reg,16:value&255,17:value>>8},queues=queues)
 trace_case(0x2368,'pm_fpga_read16',[U8,U16,U8],[reg,value,seed],{18:reg,16:value&255,17:value>>8,21:seed},[16,17],queues=queues,result_type=U16)
for _ in range(100):
 value=rng.randrange(1<<32)
 trace_case(0x2486,'pm_pll_write32',[U32],[value],{16:value&255,17:(value>>8)&255,18:(value>>16)&255,19:value>>24},queues={0x08c2:[0,128]*4})
 trace_case(0x24ce,'pm_pll_read32',[],[],{},[16,17,18,19],queues={0x08c2:[0,128]*8,0x08c3:[18,52,86,120]},result_type=U32)
for control in range(256):
 value=rng.randrange(65536)
 trace_case(0x22aa,'pm_dac_send',[U8,U16],[control,value],{22:control,16:value&255,17:value>>8})
 trace_case(0x2174,'pm_ths_write24',[U8,U8,U8,U8],[control,value&255,value>>8,0xa5],{19:control,16:value&255,17:value>>8,18:0xa5})
 trace_case(0x2208,'pm_ths_read16',[U8,U8,U16],[control,0xa5,value],{19:control,16:0xa5,17:value&255,18:value>>8},[17,18],queues={0x0628:[rng.randrange(256) for _ in range(16)]},result_type=U16)
# Bulk BC read returns R9:R8, R11:R10, R13:R12, R15:R14 in wire order.
for _ in range(20):
 responses=[rng.randrange(256) for _ in range(8)]; io=IO(queues={0x08c3:responses}); m=io.machine().run(0x23ca)
 ci=IO(queues={0x08c3:responses}); b=ci.bus(); result=(U8*8)()
 f=bind('pm_fpga_read_bc',[C.POINTER(Bus),C.POINTER(U8)]); f(C.byref(b),result)
 assert io.trace==ci.trace
 assert list(result)==[m.r[n] for n in (9,8,11,10,13,12,15,14)]
 counts['pm_fpga_read_bc']=counts.get('pm_fpga_read_bc',0)+1
print('PASS peripheral wire bytes, sampled inputs and MMIO ordering',flush=True)
# Strings and CRLF are independently checked against actual LPM/RCALL execution.
crlf=bind('pm_send_crlf',[C.POINTER(Stream)])
check_output(0x281e,crlf,[],0,'crlf')
flash_string=bind('pm_send_flash_string',[C.POINTER(Bus),C.POINTER(Stream),U16])
for address in range(0x2962,0x2bdc):
    io=IO(); m=io.machine(True); m.setptr('Z',address); m.run(0x2826)
    ci=IO(); b=ci.bus(); bus_refs=ci.refs; stream=ci.stream()
    flash_string(C.byref(b),C.byref(stream),address)
    assert ci.output==io.output
    counts['flash_string']=counts.get('flash_string',0)+1
for _ in range(1000):
    head=rng.randrange(256); tail=rng.randrange(256); ready=rng.randrange(256)
    values={0x2002:head,0x2003:tail,0x2004:ready,0x0ba3:rng.randrange(256),0x2047+((head+1)&255):rng.randrange(256)}
    trace_case(0x0e88,'pm_console_dre',[],[],{},values=values)
    h=rng.randrange(64); t=rng.randrange(64)
    values={0x2000:h,0x2001:t,0x2005:rng.randrange(256),0x0ba1:rng.randrange(256),0x0ba0:rng.choice([13,rng.randrange(256)])}
    trace_case(0x0ee0,'pm_console_rxc',[],[],{},values=values)
for _ in range(20):
    values={a:rng.randrange(256) for a in range(0x2000,0x2443)}
    trace_case(0x08e4,'pm_fpga_settings_init',[],[],{},values=values)
    trace_case(0x098a,'pm_fpga_settings_reset',[],[],{},values=values)
    trace_case(0x0c96,'pm_system_deinit',[],[],{},values=values)
print('PASS IRQ bodies, settings sequence, shutdown, FLASH strings and CRLF',flush=True)
(ROOT/'source_recovery/build/differential_result.json').write_text(json.dumps({'seed':'0x504d12','cases':counts,'total_cases':sum(counts.values()),'scope':'functional values and MMIO/IRQ access order; no timing or original register/SREG ABI proof'},indent=2)+'\n')
print('ALL PASS',sum(counts.values()),'cases',flush=True)
