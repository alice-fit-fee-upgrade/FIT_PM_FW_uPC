#!/usr/bin/env python3
"""Compile C probes with an actual recovered TU profile; compare opcode bytes."""
import argparse,hashlib,json,re,shlex,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];PROJECT=ROOT/'recovered_exact'
def profiles():
 rows=subprocess.check_output(['make','-B','-n','all'],cwd=PROJECT,text=True).splitlines();out={}
 for row in rows:
  if row.startswith('avr-gcc ') and ' -c src/' in row:
   args=shlex.split(row);src=next(v for v in args if v.startswith('src/') and v.endswith('.c'));out[src]=args
 return out

def instructions(elf, binary=False, address=0):
 args=['avr-objdump', '-D' if binary else '-d']
 if binary: args += ['-b','binary','-m','avr','--adjust-vma='+str(address)]
 text=subprocess.check_output(args+[str(elf)],text=True)
 rows=[]
 for line in text.splitlines():
  m=re.match(r'^\s*([0-9a-f]+):\s+((?:[0-9a-f]{2}\s+)+)\s*(\S.*)$',line)
  if m:rows.append({'address':int(m[1],16),'bytes':''.join(m[2].split()),'instruction':m[3].strip()})
 return rows,text

def run(spec,output):
 commands=profiles();gold=(ROOT/'reference/flash_golden.bin').read_bytes();index=json.loads((ROOT/'docs/instruction_index.json').read_text());wanted_byaddr={r['address']:r for r in index};results=[];output.mkdir(parents=True,exist_ok=True)
 for candidate in spec['candidates']:
  name=candidate['name'];work=output/name;work.mkdir(exist_ok=True);source=(ROOT/candidate['source']).resolve();obj=work/'probe.o';elf=work/'probe.elf'
  args=commands[candidate['profile']][:];args[args.index('-c')+1]=str(source);args[args.index('-o')+1]=str(obj);args.insert(1,'-I'+str(PROJECT/'src'))
  address=int(candidate['address'],0);length=candidate['length'];desired=gold[address:address+length] if 'desired_bytes' not in candidate else bytes.fromhex(candidate['desired_bytes'])
  result={**candidate,'compile_command':args,'desired_bytes':desired.hex(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'profile_source':candidate['profile'],'comparison_scope':'entire probe text, excluding only final RET when requested; prologues/increment instructions remain included'}
  build=subprocess.run(args,cwd=PROJECT,text=True,capture_output=True);log=build.stdout+build.stderr
  if build.returncode:
   result.update(status='DIFFERENT',reason='COMPILE_ERROR',diagnostic=log);(work/'diagnostic.txt').write_text(log);results.append(result);print(name,'COMPILE_ERROR');continue
  script=work/'probe.ld';script.write_text('SECTIONS { .text '+hex(address)+' : { *(.text*) } /DISCARD/ : { *(.comment) *(.note*) } }\n')
  link=['avr-gcc','-mmcu=atxmega128a3u','-nostdlib','-Wl,-T,'+str(script),str(obj),'-o',str(elf)]
  linked=subprocess.run(link,cwd=PROJECT,text=True,capture_output=True);log+=linked.stdout+linked.stderr
  if linked.returncode:result.update(status='DIFFERENT',reason='LINK_ERROR',diagnostic=log);results.append(result);continue
  rows,dis=instructions(elf);(work/'probe.dis').write_text(dis)
  if candidate.get('strip_final_ret') and rows and rows[-1]['instruction'].split()[0]=='ret':rows=rows[:-1]
  rebuilt=bytes.fromhex(''.join(r['bytes'] for r in rows));wanted_bin=work/'desired.bin';wanted_bin.write_bytes(desired)
  desiredrows,_=instructions(wanted_bin, binary=True, address=address)
  mismatch=next((i for i in range(max(len(desired),len(rebuilt))) if i>=len(desired) or i>=len(rebuilt) or desired[i]!=rebuilt[i]),None)
  exact_bytes=sum(a==b for a,b in zip(desired,rebuilt));instruction_match=sum(i<len(rows) and d['bytes']==rows[i]['bytes'] for i,d in enumerate(desiredrows))
  first=None
  if mismatch is not None:
   d=next((r for r in desiredrows if r['address']-address<=mismatch<r['address']-address+len(bytes.fromhex(r['bytes']))),None);g=next((r for r in rows if r['address']-address<=mismatch<r['address']-address+len(bytes.fromhex(r['bytes']))),None)
   first={'byte_offset':mismatch,'desired_instruction':d,'generated_instruction':g}
  result.update(status='EXACT' if mismatch is None else 'DIFFERENT',generated_bytes=rebuilt.hex(),generated_instructions=rows,desired_instructions=desiredrows,first_mismatch=first,exact_byte_count=exact_bytes,exact_instruction_count=instruction_match,total_mismatches=max(len(desired),len(rebuilt))-exact_bytes,extra_generated_bytes=max(0,len(rebuilt)-len(desired)))
  results.append(result);print(name,result['status'],result['generated_bytes'])
 (output/'results.json').write_text(json.dumps({'toolchain':subprocess.check_output(['avr-gcc','--version'],text=True).splitlines()[0],'authoritative_acceptance':'root make exact-check, never a micro-probe alone','results':results},indent=2)+'\n')
 controls={r['name']:r['status'] for r in results}
 if 'positive_control_native_mmio' in controls:assert controls['positive_control_native_mmio']=='EXACT'
 if 'negative_control_mutated_opcode' in controls:assert controls['negative_control_mutated_opcode']=='DIFFERENT'
 return results
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--spec',type=Path,required=True);p.add_argument('--output',type=Path,default=PROJECT/'build/codegen_lab');a=p.parse_args();run(json.loads(a.spec.read_text()),a.output.resolve())
