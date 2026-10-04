#!/usr/bin/env python3
"""Compare canonical memory, with separate non-erased and extent metrics."""
import argparse,hashlib,pathlib,sys

def report(g,r):
    if len(g)!=len(r): raise ValueError(f'size mismatch {len(g)} != {len(r)}')
    diff=[i for i,(a,b) in enumerate(zip(g,r)) if a!=b]
    used=[i for i,b in enumerate(g) if b!=255]
    union=[i for i,(a,b) in enumerate(zip(g,r)) if a!=255 or b!=255]
    regions=[]
    for i,(a,b) in enumerate(zip(g,r)):
        kind='MATCH' if a==b else 'DIFF'
        if not regions or regions[-1][2]!=kind: regions.append([i,i,kind])
        else: regions[-1][1]=i
    lines=[f'Golden SHA256: {hashlib.sha256(g).hexdigest()}',f'Rebuilt SHA256: {hashlib.sha256(r).hexdigest()}',f'FLASH size: {len(g)}',f'Used address range (golden non-FF): {hex(min(used)) if used else "none"} .. {hex(max(used)) if used else "none"}',f'Identical bytes: {len(g)-len(diff)}',f'Differing bytes: {len(diff)}',f'ALL FLASH identical percentage: {100*(len(g)-len(diff))/len(g):.6f}%']
    for name,addresses in [('PROGRAMMED / USED FLASH (golden non-FF)',used),('PROGRAMMED UNION (golden or rebuilt non-FF)',union),('USED EXTENT (includes internal erased holes)',range(min(used),max(used)+1) if used else [])]:
        addresses=list(addresses); n=sum(g[i]!=r[i] for i in addresses)
        lines.append(f'{name}: {len(addresses)} bytes, {n} differing, {100*(len(addresses)-n)/len(addresses) if addresses else 100:.6f}% identical')
    lines += [f'First mismatch: {hex(diff[0]) if diff else "none"}',f'Last mismatch: {hex(diff[-1]) if diff else "none"}',f'Number of mismatch regions: {sum(z[2]=="DIFF" for z in regions)}','START       END         LENGTH      TYPE']
    lines += [f'0x{a:06X}    0x{b:06X}    {b-a+1:<10}  {kind}' for a,b,kind in regions]
    return '\n'.join(lines)+'\n',bool(diff)

if __name__=='__main__':
    p=argparse.ArgumentParser(); p.add_argument('golden'); p.add_argument('rebuilt'); p.add_argument('--report'); p.add_argument('--report-only',action='store_true'); a=p.parse_args()
    output,different=report(pathlib.Path(a.golden).read_bytes(),pathlib.Path(a.rebuilt).read_bytes()); print(output,end='')
    if a.report: pathlib.Path(a.report).write_text(output)
    sys.exit(1 if different and not a.report_only else 0)
