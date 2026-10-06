#!/usr/bin/env python3
"""Decoded-instruction ABI tests; callee substitution, no timing/IRQ simulation."""
import json,random,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import Machine,load_program,PROGRAM
sys.path.insert(0,str(ROOT/'development/tools'))
from build import BUILD,symbols
names=symbols(BUILD/'development.elf');linked=load_program(BUILD/'development.bin')
assert linked[0x12ea][0]=='jmp' and linked[0x12ea][3]==names['pm_extension_entry']
assert linked[0x12ee][0]=='nop'

class BoundaryMachine(Machine):
    def __init__(self,program,character=None,callee=None,send=False,stream_status=None):
        self.ram={};self.bank={a:0 for a in range(0x38,0x3d)};self.callee=callee;self.send=send;self.observed=None
        def read_character():
            if stream_status is not None:self.f[:]=[(stream_status>>i)&1 for i in range(8)]
            return character
        super().__init__(lambda a:self.ram.get(a,0),lambda a,v:self.ram.__setitem__(a,v),lambda enabled:None,
                         stream=read_character if character is not None else None,program=program)
    def io_read(self,a): return self.bank[a] if a in self.bank else super().io_read(a)
    def io_write(self,a,value):
        if a in self.bank:self.bank[a]=value
        else:super().io_write(a,value)
    def enter_call(self,address):
        target=self.program[address-4][3]
        if target==self.callee:
            assert self.r[1]==0 and not any(self.bank.values()), 'GNU/bank normalization missing'
            self.observed=self.r[16]
            # Adversarial callee changes every GPR, SREG and extended bank.
            self.r[:]=[(i*19+7)&255 for i in range(32)];self.r[16]=0x5a
            self.f[:]=[1]*8
            for a in self.bank:self.bank[a]=0xa5
        super().enter_call(address)

rng=random.Random(12);cases=0
for first in range(256):
    if first==ord('@'):continue
    for status in [0,0xff,0x80,0x35]:
        baseline=dict(PROGRAM);baseline[0x12f4]=('ret','',2,None)
        development=dict(linked);development[0x12f4]=('ret','',2,None)
        regs=[rng.randrange(256) for _ in range(32)]
        machines=[];bank={a:rng.randrange(256) for a in range(0x38,0x3d)}
        for program in [baseline,development]:
            m=BoundaryMachine(program,character=first,stream_status=status);m.r[:]=regs;m.f[:]=[(status>>i)&1 for i in range(8)]
            m.bank.update(bank);m.run(0x12ea);machines.append(m)
        a,b=machines
        assert (a.r,a.f,a.ram,a.bank,a.stack,a.calls)==(b.r,b.f,b.ram,b.bank,b.stack,b.calls)
        cases+=1
# Native handler is substituted by RET plus adversarial clobbers at entry.
for status in range(256):
    program=dict(linked);callee=names['pm_extension_dispatch'];program[callee]=('ret','',2,None)
    m=BoundaryMachine(program,character=ord('@'),callee=callee,stream_status=status)
    regs=[rng.randrange(256) for _ in range(32)];m.r[:]=regs;m.f[:]=[(status>>i)&1 for i in range(8)]
    m.bank.update({a:rng.randrange(256) for a in m.bank});bank=m.bank.copy()
    # Reference point is after the displaced prologue + original first-byte read.
    ref=dict(PROGRAM);ref[0x12f4]=('ret','',2,None)
    expected=BoundaryMachine(ref,character=ord('@'),stream_status=status);expected.r[:]=regs;expected.f[:]=m.f;expected.run(0x12ea)
    m.run(0x12ea)
    assert m.r==expected.r and m.f==expected.f and m.bank==bank
    assert m.ram==expected.ram and not m.stack and not m.calls
    cases+=1
for function,target,send in [('pm_console_getc',0x283c,False),('pm_console_putc',0x28ac,True)]:
    for status in range(256):
        program=dict(linked);program[target]=('ret','',2,None)
        m=BoundaryMachine(program,callee=target,send=send)
        regs=[rng.randrange(256) for _ in range(32)];regs[1]=0;m.r[:]=regs
        m.f[:]=[(status>>i)&1 for i in range(8)];flags=m.f.copy()
        m.bank.update({a:rng.randrange(256) for a in m.bank});bank=m.bank.copy()
        m.run(names[function])
        assert all(m.r[i]==regs[i] for i in range(32) if i!=24)
        assert m.f==flags and m.bank==bank and not m.stack and not m.calls
        if send:assert m.observed==regs[24]
        else:assert m.r[24]==0x5a
        cases+=1
report={'status':'PASS','cases':cases,'scope':'decoded gate and ABI adapters; substituted adversarial callees; no timing or asynchronous IRQ model'}
(BUILD/'bridge_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS decoded ABI boundary:',cases,'cases')
