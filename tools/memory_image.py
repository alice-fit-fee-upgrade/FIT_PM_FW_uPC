#!/usr/bin/env python3
"""Strict Intel HEX decoding; byte addresses, checksum and overlap validation."""
import pathlib, collections, argparse
FLASH_SIZE=0x22000

def read_hex(path):
    memory={}; types=collections.Counter(); base=0; ended=False
    for lineno,line in enumerate(pathlib.Path(path).read_text().splitlines(),1):
        if not line.strip(): continue
        if ended: raise ValueError('record after EOF')
        if not line.startswith(':'): raise ValueError(f'{path}:{lineno}: not HEX')
        b=bytes.fromhex(line[1:]); n=b[0]; addr=int.from_bytes(b[1:3],'big'); kind=b[3]
        if len(b)!=n+5 or sum(b)%256: raise ValueError(f'{path}:{lineno}: checksum/length')
        payload=b[4:-1]; types[kind]+=1
        if kind==0:
            for i,v in enumerate(payload):
                a=base+addr+i
                if a in memory and memory[a]!=v: raise ValueError('conflicting overlap')
                memory[a]=v
        elif kind==1:
            if n or addr: raise ValueError('invalid EOF')
            ended=True
        elif kind in (2,4):
            if n!=2 or addr: raise ValueError('invalid extended address')
            base=int.from_bytes(payload,'big') << (4 if kind==2 else 16)
        elif kind in (3,5):
            if n!=4: raise ValueError('invalid start record')
        else: raise ValueError(f'unsupported record {kind}')
    if not ended: raise ValueError('missing EOF')
    return memory,dict(types)

def canonical(memory,size,offset=0):
    result=bytearray([255])*size
    for a,v in memory.items():
        if not offset<=a<offset+size: raise ValueError(f'out of range {a:x}')
        result[a-offset]=v
    return bytes(result)

if __name__=='__main__':
    p=argparse.ArgumentParser(); p.add_argument('input'); p.add_argument('output'); p.add_argument('--size',type=lambda x:int(x,0),default=FLASH_SIZE); p.add_argument('--offset',type=lambda x:int(x,0),default=0); a=p.parse_args()
    pathlib.Path(a.output).write_bytes(canonical(read_hex(a.input)[0],a.size,a.offset))
