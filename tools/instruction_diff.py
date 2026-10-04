#!/usr/bin/env python3
"""Instruction windows from canonical images; code/data classification is conservative."""
import sys,pathlib,subprocess,re,argparse

def decode(path):
    out=subprocess.check_output(['avr-objdump','-D','-b','binary','-m','avr:106',path],text=True)
    result={}
    for line in out.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s*((?:[0-9a-f]{2} )+)\s*(.*)',line)
        if m: result[int(m[1],16)]=(bytes.fromhex(m[2]),m[3])
    return result

def code(a,extra=()): return a<0x2916 or 0x201a0<=a<0x204e6 or any(start<=a<end for start,end in extra)

def classify(a,g,r,extra=()):
    if a<0x1e2: return 'H vector table'
    if not code(a,extra): return 'O data difference; inspect table/string boundaries'
    if any(start<=a<end for start,end in extra) and all(x==255 for x in g[0]): return 'F new code placed in erased golden FLASH'
    if r[0] and all(x==255 for x in r[0]): return 'A missing/erased region'
    gi=g[1].split(';')[0].strip(); ri=r[1].split(';')[0].strip()
    gm=gi.split()[0] if gi else ''; rm=ri.split()[0] if ri else ''
    if gm!=rm: return 'B wrong instruction (or O boundary ambiguity)'
    if gm in ('call','jmp'): return 'E CALL/JMP address difference; same opcode semantics, different target'
    if gm in ('rjmp','rcall') or gm.startswith('br'): return 'D relative branch target/relocation; same opcode, verify target semantics'
    if gi==ri: return 'N encoding alias; same decoded instruction'
    return 'C operand/immediate difference (verify register versus immediate)'

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('golden'); parser.add_argument('rebuilt')
    parser.add_argument('--code-range',action='append',default=[],help='additional byte code interval START:END (end exclusive)')
    args=parser.parse_args(); extra=[]
    for item in args.code_range:
        start,end=(int(v,0) for v in item.split(':'))
        if start>=end: parser.error('invalid code range')
        extra.append((start,end))
    gpath,rpath=args.golden,args.rebuilt; g=pathlib.Path(gpath).read_bytes(); r=pathlib.Path(rpath).read_bytes()
    if len(g)!=len(r): raise SystemExit('image sizes differ')
    gd,rd=decode(gpath),decode(rpath); seen=set(); count=0
    print('ADDRESS GOLDEN_BYTES REBUILT_BYTES GOLDEN_INSTRUCTION | REBUILT_INSTRUCTION | CLASS')
    for i in range(len(g)):
        if g[i]==r[i]: continue
        candidates=[a for a in (i&~1,(i&~1)-2) if a in gd and a<=i<a+len(gd[a][0])]; a=min(candidates) if candidates else i&~1
        if a in seen: continue
        seen.add(a); gg=gd.get(a,(g[a:a+2],'undecoded')); rr=rd.get(a,(r[a:a+len(gg[0])],'inside differently aligned instruction')); count+=1
        print(f'0x{a:06x} {gg[0].hex()} {rr[0].hex()} {gg[1]} | {rr[1]} | {classify(a,gg,rr,extra)}')
    print(f'Differing instruction/data windows: {count}')
