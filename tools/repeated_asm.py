#!/usr/bin/env python3
"""Rank contiguous retained ASM opcode patterns; windows must not be summed."""
import argparse,json
from collections import defaultdict
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def report():
 manifest=json.loads((ROOT/'recovered_exact/manifest.json').read_text())
 index=json.loads((ROOT/'docs/instruction_index.json').read_text())
 owners={a:f['source'] for f in manifest['accepted'] for z in f.get('asm_helper_ranges',[]) for a in range(z['start'],z['end_exclusive'])}
 patterns=defaultdict(list)
 for width in (2,3,4):
  for i in range(len(index)-width+1):
   rows=index[i:i+width];addresses=[r['address'] for r in rows]
   if any(a not in owners for a in addresses):continue
   if len({owners[a] for a in addresses})!=1:continue
   if any(rows[j]['address']+len(bytes.fromhex(rows[j]['bytes']))!=rows[j+1]['address'] for j in range(width-1)):continue
   if any(a not in owners for row in rows for a in range(row['address'],row['address']+len(bytes.fromhex(row['bytes'])))):continue
   key=tuple(r['instruction'].split()[0] for r in rows)
   patterns[key].append({'address':addresses[0],'bytes':sum(len(bytes.fromhex(r['bytes'])) for r in rows),'source':owners[addresses[0]],'instructions':[r['instruction'] for r in rows]})
 result=[]
 for k,v in patterns.items():
  if len(v)<3:continue
  covered={a for row in v for a in range(row['address'],row['address']+row['bytes'])}
  result.append({'pattern':' / '.join(k),'instructions_per_window':len(k),'occurrences':len(v),'unique_bytes':len(covered),'source_files':len({row['source'] for row in v}),'examples':v})
 result.sort(key=lambda row:(-row['unique_bytes'],row['pattern']))
 return {'scope':'retained inline ASM in accepted C regions; opcode shapes, not proof of interchangeable semantics','warning':'windows overlap; unique_bytes must not be summed across patterns; inspect operands and live flags','patterns':result}
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path);args=parser.parse_args();text=json.dumps(report(),indent=2)+'\n'
 if args.output:args.output.write_text(text)
 else:print(text,end='')
