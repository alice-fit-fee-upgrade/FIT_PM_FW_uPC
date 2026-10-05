#!/usr/bin/env python3
"""Generate compilable, flag-aware C equivalents in every accepted source file.
The equivalent is a register-state transition model, not an ordinary GNU ABI
replacement. Its comments are compiled verbatim by check_logical_comments.py.
"""
import pathlib,sys,json,re,hashlib
ROOT=pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'source_recovery/tests'))
from avr_oracle import PROGRAM,FLASH
BEGIN='/* BEGIN COMPILED LOGICAL C EQUIVALENT'
END='END COMPILED LOGICAL C EQUIVALENT */'

def reg(p): return int(p[1:])
def expression(p): return f's->r[{reg(p)}]' if p.startswith('r') else str(int(p,0))
def addr(p):
    pre=p.startswith('-'); post=p.endswith('+'); base=p.strip('-+')
    off=0
    if '+' in base: base,k=base.split('+');off=int(k,0)
    low={'X':26,'Y':28,'Z':30}[base]
    lines=[]
    if pre:lines.append(f'pm_setpointer(s, {low}, pm_pointer(s, {low}) - 1);')
    lines.append(f'uint16_t address = pm_pointer(s, {low}) + {off};')
    if post:lines.append(f'pm_setpointer(s, {low}, address + 1);')
    return lines

def body(a,row):
    op,args,n,target=row; p=[x.strip() for x in args.split(',')] if args else []; nxt=a+n
    lines=[]; result=str(nxt)
    d=reg(p[0]) if p and re.fullmatch('r[0-9]+',p[0]) else None
    rd=f's->r[{d}]';val=expression(p[1]) if len(p)>1 and not op in ('ld','ldd','lpm','st','std') else None
    if op=='ldi' or op=='mov':lines=[f'{rd} = {val};']
    elif op=='movw':lines=[f'uint16_t pair = pm_pointer(s, {reg(p[1])});',f'pm_setpointer(s, {d}, pair);']
    elif op in ('add','adc','sub','subi','sbc','sbci','cp','cpi','cpc'):
        carry='pm_getflag(s, CARRY)' if op in ('adc','sbc','sbci','cpc') else '0'
        expr=f'pm_add(s, {rd}, {val}, {carry})' if op in ('add','adc') else f'pm_sub(s, {rd}, {val}, {carry}, '+('true' if op in ('sbc','sbci','cpc') else 'false')+')'
        lines=[('' if op in ('cp','cpi','cpc') else rd+' = ')+expr+';']
    elif op in ('and','andi','or','ori','eor'):
        symbol='&' if op in ('and','andi') else '|' if op in ('or','ori') else '^'
        lines=[f'{rd} {symbol}= {val};',f'pm_nzv(s, {rd}, false);']
    elif op in ('inc','dec'):
        lines=[f'{rd}'+('++;' if op=='inc' else '--;'),f'pm_nzv(s, {rd}, {rd} == '+('128' if op=='inc' else '127')+');']
    elif op=='com':lines=[f'{rd} = ~{rd};',f'pm_nzv(s, {rd}, false);','pm_flag(s, CARRY, true);']
    elif op=='neg':lines=[f'uint8_t old = {rd};',f'{rd} = pm_sub(s, 0, old, 0, false);',f'pm_flag(s, HALF, ({rd} | old) & 8);']
    elif op=='swap':lines=[f'{rd} = ({rd} >> 4) | ({rd} << 4);']
    elif op in ('lsr','asr','ror'):
        high='(pm_getflag(s, CARRY) << 7)' if op=='ror' else f'({rd} & 128)' if op=='asr' else '0'
        lines=[f'bool carry = {rd} & 1;',f'{rd} = ({rd} >> 1) | {high};','pm_flag(s, CARRY, carry);',f'pm_nzv(s, {rd}, !!({rd} & 128) ^ carry);']
    elif op in ('bst','bld'):
        bit=int(p[1],0)
        lines=[f'pm_flag(s, TRANSFER, {rd} & (1u << {bit}));'] if op=='bst' else [f'{rd} = ({rd} & ~(1u << {bit})) | (pm_getflag(s, TRANSFER) << {bit});']
    elif op in ('mul','mulsu'):
        aa=f'(int8_t){rd}' if op=='mulsu' else rd
        lines=[f'uint16_t product = ({aa}) * (int){val};','pm_setpointer(s, 0, product);','pm_flag(s, ZERO, product == 0);','pm_flag(s, CARRY, product & 0x8000);']
    elif op in ('adiw','sbiw'):
        sign='+' if op=='adiw' else '-'
        lines=[f'uint16_t old = pm_pointer(s, {d});',f'uint16_t value = old {sign} {val};',f'pm_setpointer(s, {d}, value);','bool old_negative = old & 0x8000, negative = value & 0x8000;', 'pm_flag(s, CARRY, '+('old_negative && !negative' if op=='adiw' else '!old_negative && negative')+');','pm_flag(s, OVERFLOW, '+('!old_negative && negative' if op=='adiw' else 'old_negative && !negative')+');','pm_flag(s, NEGATIVE, negative);','pm_flag(s, ZERO, value == 0);','pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));']
    elif op in ('lds','ld','ldd','lpm'):
        lines=[f'uint16_t address = {int(p[1],0)};'] if op=='lds' else addr(p[1])
        lines+=[f'{rd} = '+('pm_golden_flash[address]' if op=='lpm' else 'pm_read(s, address)')+';']
    elif op in ('sts','st','std'):
        lines=[f'uint16_t address = {int(p[0],0)};'] if op=='sts' else addr(p[0])
        lines += [f'pm_write(s, address, {expression(p[1])});']
    elif op=='in':lines=[f'{rd} = pm_io_read(s, {int(p[1],0)});']
    elif op=='out':lines=[f'pm_io_write(s, {int(p[0],0)}, {expression(p[1])});']
    elif op in ('cbi','sbi'):
        address=int(p[0],0); bit=int(p[1],0)
        lines=[f'uint8_t value = pm_io_read(s, {address});',f'pm_io_write(s, {address}, value '+ ('|' if op=='sbi' else '& ~')+f' (1u << {bit}));']
    elif op in ('cli','sei'):lines=[f'pm_irq(s, '+('true' if op=='sei' else 'false')+');']
    elif op in ('sec','clc'):lines=[f'pm_flag(s, CARRY, '+('true' if op=='sec' else 'false')+');']
    elif op=='push':lines=[f's->stack[s->depth++] = {rd};']
    elif op=='pop':lines=[f'{rd} = s->stack[--s->depth];']
    elif op in ('call','rcall','jmp','rjmp'):
        result=str(target if target is not None else int(args,0))
        if op in ('call','rcall'):lines=[f's->calls[s->call_depth++] = {nxt};']
    elif op in ('ret','reti'):
        result='s->calls[--s->call_depth]'
        if op=='reti':lines=['pm_flag(s, INTERRUPT, true);']
    elif op.startswith('br'):
        bit,expected={'breq':(1,1),'brne':(1,0),'brcs':(0,1),'brcc':(0,0),'brge':(4,0),'brlt':(4,1),'brmi':(2,1),'brpl':(2,0),'brtc':(6,0),'brts':(6,1)}[op]
        result=f'(pm_getflag(s, {bit}) == {expected}) ? {target} : {nxt}'
    elif op in ('sbrs','sbrc','sbis','sbic','cpse'):
        if op=='cpse':cond=f'{rd} == {val}'
        elif op in ('sbrs','sbrc'):cond=f'!!({rd} & (1u << {int(p[1],0)})) == '+('1' if op=='sbrs' else '0')
        else:cond=f'!!(pm_io_read(s, {int(p[0],0)}) & (1u << {int(p[1],0)})) == '+('1' if op=='sbis' else '0')
        result=f'({cond}) ? {nxt+PROGRAM[nxt][2]} : {nxt}'
    elif op!='nop':raise ValueError((a,op,args))
    return lines+[f'return {result};']

def generate(write=True):
    manifest=json.loads((ROOT/'recovered_exact/manifest.json').read_text()); by_source={}
    for f in manifest['accepted']:by_source.setdefault(f['source'],[]).append(f)
    header_ops = {
        'legacy_flag_ops.h': {'eor', 'inc', 'dec'},
        'legacy_carry.h': {'rcall', 'brcs'},
        'legacy_word_ops.h': {'cpi', 'cpc', 'brge', 'brlt', 'st'},
        'legacy_console_call_c.h': {'rcall'},
        'legacy_cpu.h': {'cli', 'sei', 'nop', 'bst', 'bld'},
        'legacy_r16.h': {'ldi', 'lds', 'eor'},
        'legacy_r16_c.h': {'ldi', 'lds', 'eor'},
        'legacy_interrupt.h': {'push', 'in', 'pop', 'out', 'reti'},
        'legacy_interrupt_register_c.h': {'push', 'in', 'pop', 'out', 'reti'},
        'legacy_spi.h': {'sts', 'lds', 'sbrs', 'rjmp'},
        'legacy_spi_c.h': {'sts', 'lds', 'sbrs', 'rjmp'},
        'legacy_cli.h': {'rcall', 'brcs', 'and', 'brne', 'cpi', 'brge', 'brcc', 'ret'},
        'legacy_cli_guard_c.h': {'rcall', 'brcc', 'ret'},
    }
    for header in header_ops: by_source['src/'+header] = []
    validation_path=ROOT/'docs/logical_c_validation.json'
    previous=json.loads(validation_path.read_text()) if validation_path.exists() else {}
    validated={row['source']:row for row in previous.get('sources',[])}
    index=[]; models=[]
    for source, funcs in sorted(by_source.items()):
        path=ROOT/'recovered_exact'/source;name='pm_logical_'+path.stem
        code=[f'uint32_t {name}(PMLogical *s, uint32_t pc)', '{','    switch (pc) {']
        sites=[]
        if path.suffix == '.h':
            # Header operations are instantiated by accepted translation units.
            # One genuine golden site for each opcode/operand shape supplies an
            # explicit C version of the shared private-ABI primitives.
            shapes = {}
            for f in manifest['accepted']:
                for a in range(f['address'], f['end_exclusive'], 2):
                    if a not in PROGRAM: continue
                    op, args, _, _ = PROGRAM[a]
                    if op not in header_ops[path.name]: continue
                    shapes.setdefault((op, args), a)
            funcs = [{'address': a, 'end_exclusive': a+PROGRAM[a][2]} for a in sorted(shapes.values())]
        for f in sorted(funcs,key=lambda f:f['address']):
            for a in range(f['address'], f['end_exclusive'], 2):
                if a not in PROGRAM: continue
                sites.append(a);op,args,_,_=PROGRAM[a]
                code.append(f'    case 0x{a:04x}: {{ // {op} {args}'.rstrip())
                code += ['        '+line for line in body(a,PROGRAM[a])]
                code.append('    }')
        code+=['    default: return UINT32_MAX;','    }','}'];model='\n'.join(code)
        digest=hashlib.sha256(model.encode()).hexdigest()
        evidence=validated.get(source,{})
        passed=evidence.get('model_sha256')==digest and previous.get('status')=='PASS' and previous.get('golden_sha256')==hashlib.sha256(FLASH).hexdigest() and previous.get('runtime_sha256')==hashlib.sha256((ROOT/'recovered_exact/tests/logical_c_runtime.h').read_bytes()).hexdigest()
        status=('PASS_INSTRUCTION_TRANSITIONS: '+str(evidence['instruction_transition_cases'])+' file cases; shared exhaustive operand tests also passed.') if passed else 'PENDING: rerun make c-comment-check; never infer a pass from compilation alone.'
        note=(BEGIN+'\n * Validation: '+status+'\n * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.\n'
              ' * Compiled verbatim and differentially tested by tests/check_logical_comments.py.\n'
              ' * PASS applies only when docs/logical_c_validation.json matches this model hash.\n'
              ' * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.\n'
              ' * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.\n'
              ' * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.\n'
              ' * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.\n *\n'+model+'\n'+END+'\n')
        old=path.read_text();old=re.sub(re.escape(BEGIN)+r'.*?'+re.escape(END)+r'\n?', '',old,flags=re.S)
        if write: path.write_text(old.rstrip()+'\n\n'+note)
        models.append(model);index.append({'source':source,'model':name,'model_sha256':hashlib.sha256(model.encode()).hexdigest(),'addresses':sites})
    out=ROOT/'recovered_exact/build/logical_comments';out.mkdir(parents=True,exist_ok=True)
    if write: (out/'index.json').write_text(json.dumps(index,indent=2)+'\n')
    if write: (out/'models.c').write_text('#include "logical_c_runtime.h"\nstatic const uint8_t pm_golden_flash[] = {'+','.join(map(str,FLASH[:65536]))+'};\n'+'\n\n'.join(models))
    return index
if __name__=='__main__':generate()
