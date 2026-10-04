"""Bounded functional interpreter of original AVR disassembly, NOT a timing model.
Unsupported instructions fail closed. Every decoded row originates in golden BIN.
Only stream hooks bypass original subroutines; MMIO routines execute real calls.
"""
import pathlib,re,subprocess
ROOT=pathlib.Path(__file__).resolve().parents[2]
FLASH=(ROOT/'reference/flash_golden.bin').read_bytes()
def load_program():
    text=subprocess.check_output(['avr-objdump','-D','-b','binary','-m','avr:106',str(ROOT/'reference/flash_golden.bin')],text=True)
    program={}
    for line in text.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s*((?:[0-9a-f]{2} )+)\s*(\S+)\s*(.*)',line)
        if not m: continue
        a=int(m[1],16); raw=bytes.fromhex(m[2]); operands=m[4].split(';')[0].strip()
        target=re.search(r';\s*0x([0-9a-f]+)',m[4])
        program[a]=(m[3],operands,len(raw),int(target[1],16) if target else None)
    return program
PROGRAM=load_program()

class Machine:
    def __init__(self,read,write,irq,stream=None,output=None):
        self.r=[0]*32; self.f=[0]*8; self.stack=[]; self.calls=[]
        self.read=read; self.write=write; self.irq=irq
        self.stream=stream; self.output=output; self.steps=0
    def reg(self,s): return int(s.strip()[1:])
    def ptr(self,p):
        n={'X':26,'Y':28,'Z':30}[p]; return self.r[n]|self.r[n+1]<<8
    def setptr(self,p,v):
        n={'X':26,'Y':28,'Z':30}[p]; self.r[n]=v&255; self.r[n+1]=(v>>8)&255
    def addr(self,p):
        p=p.strip(); post=p.endswith('+'); pre=p.startswith('-'); base=p.strip('-+')
        delta=0
        if '+' in base: base,offset=base.split('+'); delta=int(offset,0)
        if pre: self.setptr(base,self.ptr(base)-1)
        a=(self.ptr(base)+delta)&65535
        if post: self.setptr(base,a+1)
        return a
    def nzv(self,res,v):
        self.f[1]=int(res==0); self.f[2]=(res>>7)&1; self.f[3]=int(v); self.f[4]=self.f[2]^self.f[3]
    def add(self,a,b,carry=0):
        full=a+b+carry; v=full&255
        self.f[0]=int(full>255); self.f[5]=int((a&15)+(b&15)+carry>15)
        self.nzv(v,((~(a^b)&(a^v))&128)!=0); return v
    def sub(self,a,b,carry=0,chain=False):
        oldz=self.f[1]; full=a-b-carry; v=full&255
        self.f[0]=int(full<0); self.f[5]=int((a&15)-(b&15)-carry<0)
        self.nzv(v,((a^b)&(a^v)&128)!=0)
        if chain: self.f[1]&=oldz
        return v
    def run(self,start,limit=30000):
        self.pc=start
        while self.steps<limit:
            self.steps+=1
            op,args,n,target=PROGRAM[self.pc]; self.pc+=n
            p=[s.strip() for s in args.split(',')] if args else []
            if op in ('ret','reti'):
                if not self.calls: return self
                self.pc=self.calls.pop(); continue
            if op in ('call','rcall','jmp','rjmp'):
                dest=target if target is not None else int(args,0)
                if op in ('call','rcall'):
                    if dest in (0x2836,0x283c) and self.stream is not None:
                        self.r[16]=self.stream(); continue
                    if dest==0x28ac and self.output is not None:
                        self.output(self.r[16]); continue
                    self.calls.append(self.pc)
                self.pc=dest; continue
            if op.startswith('br'):
                conditions={'breq':(1,1),'brne':(1,0),'brcs':(0,1),'brcc':(0,0),'brlo':(0,1),'brsh':(0,0),'brlt':(4,1),'brge':(4,0),'brmi':(2,1),'brpl':(2,0),'brts':(6,1),'brtc':(6,0)}
                if op not in conditions: raise AssertionError(('unsupported branch',op))
                bit,value=conditions[op]
                if self.f[bit]==value: self.pc=target
                continue
            if op=='push': self.stack.append(self.r[self.reg(p[0])]); continue
            if op=='pop': self.r[self.reg(p[0])]=self.stack.pop(); continue
            if op in ('sbrc','sbrs','cpse'):
                if op=='cpse': skip=self.r[self.reg(p[0])]==self.r[self.reg(p[1])]
                else: skip=bool(self.r[self.reg(p[0])]&(1<<int(p[1],0)))==(op=='sbrs')
                if skip: self.pc+=PROGRAM[self.pc][2]
                continue
            if op in ('sei','cli'):
                self.f[7]=int(op=='sei'); self.irq(op=='sei'); continue
            if op in ('sec','clc','set','clt'):
                self.f[6 if op in ('set','clt') else 0]=int(op in ('sec','set')); continue
            if op=='nop': continue
            if op=='in':
                assert int(p[1],0)==0x3f
                self.r[self.reg(p[0])]=sum(v<<i for i,v in enumerate(self.f)); continue
            if op=='out':
                assert int(p[0],0)==0x3f
                value=self.r[self.reg(p[1])]; self.f=[(value>>i)&1 for i in range(8)]; continue
            if op in ('lds','ld','ldd','lpm'):
                d=self.reg(p[0]); a=int(p[1],0) if op=='lds' else self.addr(p[1])
                self.r[d]=FLASH[a] if op=='lpm' else self.read(a); continue
            if op in ('sts','st','std'):
                a=int(p[0],0) if op=='sts' else self.addr(p[0]); self.write(a,self.r[self.reg(p[1])]); continue
            if op in ('adiw','sbiw'):
                d=self.reg(p[0]); a=self.r[d]|self.r[d+1]<<8; k=int(p[1],0); v=(a+k if op=='adiw' else a-k)&65535
                self.r[d]=v&255; self.r[d+1]=v>>8; old=(a>>15)&1; new=(v>>15)&1
                self.f[0]=int((new==0 and old==1) if op=='adiw' else (new==1 and old==0))
                self.f[3]=int((old==0 and new==1) if op=='adiw' else (old==1 and new==0)); self.f[2]=new; self.f[4]=new^self.f[3]; self.f[1]=int(v==0); continue
            d=self.reg(p[0]); a=self.r[d]
            if op=='ldi': self.r[d]=int(p[1],0); continue
            if op=='mov': self.r[d]=self.r[self.reg(p[1])]; continue
            if op=='movw':
                s=self.reg(p[1]); self.r[d:d+2]=self.r[s:s+2]; continue
            if op in ('bst','bld'):
                bit=int(p[1],0)
                if op=='bst': self.f[6]=(a>>bit)&1
                else: self.r[d]=(a&~(1<<bit))|(self.f[6]<<bit)
                continue
            if op in ('mul','mulsu'):
                b=self.r[self.reg(p[1])]; aa=a if op=='mul' or a<128 else a-256; v=(aa*b)&65535
                self.r[0]=v&255; self.r[1]=v>>8; self.f[1]=int(v==0); self.f[0]=(v>>15)&1; continue
            if op=='swap': self.r[d]=((a<<4)|(a>>4))&255; continue
            if op in ('inc','dec'):
                v=(a+(1 if op=='inc' else -1))&255; self.nzv(v,v==(128 if op=='inc' else 127)); self.r[d]=v; continue
            if op=='neg':
                v=self.sub(0,a); self.f[5]=int(bool((v|a)&8)); self.r[d]=v; continue
            if op=='com': self.r[d]=a^255; self.nzv(a^255,False); self.f[0]=1; continue
            if op in ('lsr','ror','asr'):
                c=a&1; v=(a>>1)|((self.f[0]<<7) if op=='ror' else (a&128) if op=='asr' else 0)
                self.f[0]=c; self.nzv(v,((v>>7)&1)^c); self.r[d]=v; continue
            if op in ('and','andi','or','ori','eor'):
                b=int(p[1],0) if op.endswith('i') else self.r[self.reg(p[1])]
                v=(a&b) if op in ('and','andi') else (a|b) if op in ('or','ori') else a^b
                self.r[d]=v; self.nzv(v,False); continue
            if op in ('add','adc','sub','subi','sbc','sbci','cp','cpi','cpc'):
                b=int(p[1],0) if op in ('subi','sbci','cpi') else self.r[self.reg(p[1])]
                carry=self.f[0] if op in ('adc','sbc','sbci','cpc') else 0
                v=self.add(a,b,carry) if op in ('add','adc') else self.sub(a,b,carry,op in ('sbc','sbci','cpc'))
                if op not in ('cp','cpi','cpc'): self.r[d]=v
                continue
            raise AssertionError(('unsupported instruction',hex(self.pc-n),op,args))
        raise AssertionError(('execution bound exceeded',hex(self.pc),limit))
