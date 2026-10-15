#!/usr/bin/env python3
"""Symbolic relocation of the accepted timer ISR, with explicit control-flow edges.
No data blobs and no changes to the baseline. Widening branches changes timing;
this tool preserves instruction effects/control flow, not cycle counts.
"""
import json,re,subprocess
from pathlib import Path
from power_patch import SITES
START,END=0x49e,0x8e4
INVERSE={'breq':'brne','brne':'breq','brcs':'brcc','brcc':'brcs','brge':'brlt'}
SKIPS={'sbrs':'sbrc','sbrc':'sbrs','sbis':'sbic','sbic':'sbis'}
def decode(path):
    result={}
    for line in subprocess.check_output(['avr-objdump','-d',str(path)],text=True).splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s*((?:[0-9a-f]{2} )+)\s*(\S+)\s*(.*)',line)
        if not m:continue
        tail=m[4];target=re.search(r';\s*0x([0-9a-f]+)',tail)
        op=m[3];args=tail.split(';')[0].strip()
        if op in ('jmp','call') and target is None:
            target=re.match(r'0x([0-9a-f]+)',args)
        result[int(m[1],16)]=(op,args,len(bytes.fromhex(m[2])),int(target[1],16) if target else None,bytes.fromhex(m[2]))
    return result

def generate(base,build):
    program=decode(base); rows=[]
    def label(a):return f'pm_timer_at_{a:04x}'
    def dest(a):return label(a) if START<=a<END else f'0x{a:x}'
    # No outside control-flow entry may bypass the original ISR entry gate.
    for a,(op,args,n,target,raw) in program.items():
        if not START<=a<END and op in ('rjmp','jmp','rcall','call',*INVERSE) and target is not None:
            assert not START<target<END, ('external entry into ISR interior',a,target)
    lines=['/* Generated from the accepted source-built ISR; do not edit. */',
           '.section .text.pm_timer_relocated,"ax",@progbits',
           '.global pm_timer_relocated','pm_timer_relocated:']
    a=START
    while a<END:
        op,args,n,target,raw=program[a];nxt=a+n
        lines += [f'.global {label(a)}',label(a)+':',f'/* Original byte address 0x{a:04x}: {op} {args} */']
        if a==0x77e:
            lines += ['    push r16','    in r16,0x3f','    push r16','    cli',
                      '    in r16,0x0f','    andi r16,0x43','    brne .Ltemp_defer',
                      '    cbi 0x0f,7','    sbi 0x0f,1',
                      '    pop r16','    out 0x3f,r16','    pop r16','    rjmp .Ltemp_claimed',
                      '.Ltemp_defer:','    sbi 0x0f,7','    in r16,0x0e','    cpi r16,255',
                      '    breq .Ltemp_counted','    inc r16','    out 0x0e,r16','.Ltemp_counted:',
                      '    pop r16','    out 0x3f,r16','    pop r16','    jmp '+dest(0x8be),
                      '.Ltemp_claimed:']
        if a==0x788:
            lines += ['    cbi 0x0f,1']
        hooks={address:name for address,name in SITES if START<=address<END}
        lines += [f'.global pm_timer_instruction_{a:04x}',f'pm_timer_instruction_{a:04x}:']
        if a in hooks:
            assert op=='sts' and n==4
            emitted=['jmp '+hooks[a]]
        elif op in ('rjmp','jmp','rcall','call'):
            assert target is not None
            emitted=[('call' if op in ('rcall','call') else 'jmp')+' '+dest(target)]
        elif op in INVERSE:
            assert target is not None
            emitted=[INVERSE[op]+' 1f','jmp '+dest(target),'1:']
        elif op in SKIPS:
            skip=nxt+program[nxt][2]
            emitted=[SKIPS[op]+' '+args,'jmp '+dest(skip)]
        else:
            assert not op.startswith('br') and op not in ('cpse','icall','ijmp','eicall','eijmp'),op
            emitted=[op+' '+args]
        lines+=['    '+s for s in emitted]
        rows.append({'original':a,'symbol':label(a),'op':op,'size':n,'target':target})
        a=nxt
    assert a==END
    (build/'timer_relocated.S').write_text('\n'.join(lines)+'\n')
    (build/'timer_relocation.json').write_text(json.dumps(rows,indent=2)+'\n')
    return build/'timer_relocated.S'
