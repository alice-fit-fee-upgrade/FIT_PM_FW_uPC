#!/usr/bin/env python3
"""Compile commented C verbatim; compare transitions with the golden AVR oracle.
All sites, deterministic boundary/random registers and all SREG bits. Additional
exhaustive byte cases cover each opcode/operand shape. This is transition coverage,
not all path coverage, not a timing/interrupt/peripheral hardware emulator.
"""
import pathlib,sys,json,re,hashlib,ctypes as C,subprocess,random,time
ROOT=pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import Machine,PROGRAM,FLASH
from generate_logical_comments import BEGIN,END,generate
BUILD=ROOT/'recovered_exact/build/logical_comments'
index=generate(write=False)
# Use the exact comment bodies, not an unrelated model implementation.
models=[]
for item in index:
    text=(ROOT/'recovered_exact'/item['source']).read_text()
    block=text.split(BEGIN,1)[1].split(END,1)[0]
    model=block[block.index('uint32_t '):].rstrip()
    assert hashlib.sha256(model.encode()).hexdigest()==item['model_sha256']
    models.append(model)
(BUILD/'models.c').write_text('#include "logical_c_runtime.h"\nstatic const uint8_t pm_golden_flash[] = {'+','.join(map(str,FLASH[:65536]))+'};\n'+'\n\n'.join(models))
subprocess.run(['cc','-std=c99','-O2','-Wall','-Wextra','-Werror','-fPIC','-shared','-I'+str(ROOT/'recovered_exact/tests'),str(BUILD/'models.c'),'-o',str(BUILD/'models.so')],check=True)
class State(C.Structure):
    _fields_=[('r',C.c_uint8*32),('sreg',C.c_uint8),('memory',C.c_uint8*65536),('stack',C.c_uint8*256),('depth',C.c_uint16),('call_depth',C.c_uint16),('calls',C.c_uint32*256),('pc',C.c_uint32),('trace',C.c_uint32*32),('trace_count',C.c_uint)]
lib=C.CDLL(str(BUILD/'models.so'));rng=random.Random(0x434f4d4d)
functions={}
for item in index:
    f=getattr(lib,item['model']);f.argtypes=[C.POINTER(State),C.c_uint32];f.restype=C.c_uint32;functions[item['model']]=f
class TransitionMachine(Machine):
    def io_read(self,a):return super().io_read(a) if a==0x3f else self.read(a)
    def io_write(self,a,v):
        if a==0x3f:super().io_write(a,v)
        else:self.write(a,v)

s=State();memory=bytes((a*71+19)&255 for a in range(65536));C.memmove(s.memory,memory,len(memory))
counts={};cases=0;start=time.monotonic()
def check(a,function,regs,sreg):
    global cases
    events=[];writes={}
    def read(address):
        value=writes.get(address,memory[address]);events.append((1<<24)|(address<<8)|value);return value
    def write(address,value):writes[address]=value;events.append((2<<24)|(address<<8)|value)
    def irq(value):events.append((3<<24)|int(value))
    m=TransitionMachine(read,write,irq);m.r=regs.copy();m.f=[(sreg>>i)&1 for i in range(8)];m.stack=[0x17,0x98];m.calls=[0x10204];m.pc=a
    s.r[:]=regs;s.sreg=sreg;s.depth=2;s.stack[0]=0x17;s.stack[1]=0x98;s.call_depth=1;s.calls[0]=0x10204;s.trace_count=0
    returned=function(C.byref(s),a)
    try:m.run(a,1)
    except AssertionError as e:
        if e.args[0][0]!='execution bound exceeded':raise
    op=PROGRAM[a][0]
    # Historical oracle leaves I unchanged on RETI; the architectural model must set it.
    if op=='reti':m.f[7]=1
    expected_sreg=sum(v<<i for i,v in enumerate(m.f))
    assert list(s.r)==m.r,(hex(a),op,'registers',regs,sreg,list(s.r),m.r)
    assert s.sreg==expected_sreg,(hex(a),op,'SREG',regs,sreg,s.sreg,expected_sreg)
    assert returned==m.pc,(hex(a),op,'next PC',returned,m.pc)
    assert list(s.stack[:s.depth])==m.stack,(hex(a),op,'byte stack')
    assert list(s.calls[:s.call_depth])==m.calls,(hex(a),op,'call continuations')
    assert list(s.trace[:s.trace_count])==events,(hex(a),op,'MMIO trace',list(s.trace[:s.trace_count]),events)
    for address,value in writes.items():
        assert s.memory[address]==value,(hex(a),'memory',address)
        s.memory[address]=memory[address]
    cases+=1;counts[op]=counts.get(op,0)+1

patterns=[0,1,0x7f,0x80,0xff]
for item in index:
    before=cases;f=functions[item['model']]
    for a in item['addresses']:
        for i in range(16):
            regs=[patterns[i%5]]*32 if i<5 else [rng.randrange(256) for _ in range(32)]
            # LPM only addresses representable by the original low FLASH addressing.
            check(a,f,regs,[0,1,2,4,8,16,32,64,128,255,0x55,0xaa,3,0x81,0x42,0x18][i])
    item['instruction_transition_cases']=cases-before
    item['status']='PASS_INSTRUCTION_TRANSITIONS'
    print('PASS',item['source'],item['instruction_transition_cases'],'cases',flush=True)
# Exhaust byte operands and all incoming SREG patterns on representative sites.
representatives={}
for item in index:
    for a in item['addresses']:
        op,args,*_=PROGRAM[a]
        shape=re.sub(r'r\d+','r',args)
        representatives.setdefault((op,shape),(a,functions[item['model']]))
for (op,shape),(a,f) in representatives.items():
    for value in range(256):
        regs=[(value+37*i)&255 for i in range(32)]
        check(a,f,regs,value)
print('ALL PASS logical C:',cases,'transition cases;',len(representatives),'operand shapes',flush=True)
report={'status':'PASS','golden_sha256':hashlib.sha256(FLASH).hexdigest(),'cases':cases,'instruction_sites':sum(len(x['addresses']) for x in index),'operand_shapes':len(representatives),'opcode_cases':counts,'sources':index,'scope':'Compiled commented C, all accepted function instruction sites: register/SREG/next-PC/abstract byte-stack/call-continuation/RAM-MMIO-IRQ traces. Deterministic boundary and random states; exhaustive byte/SREG values per operand shape. RETI I corrected architecturally against known historical oracle omission. Not exhaustive multi-register combinations, full-path coverage, real PC stack, cycle timing, asynchronous IRQ or physical peripheral validation.','runtime_sha256':hashlib.sha256((ROOT/'recovered_exact/tests/logical_c_runtime.h').read_bytes()).hexdigest(),'elapsed_seconds':round(time.monotonic()-start,3)}
(ROOT/'docs/logical_c_validation.json').write_text(json.dumps(report,indent=2)+'\n')

# Write explicit PASS labels only after successful validation and hash matching.
generate()
