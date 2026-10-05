#!/usr/bin/env python3
"""Try reviewed source substitutions; rollback unless full FLASH remains exact."""
import argparse,importlib.util,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];PROJECT=ROOT/'recovered_exact';MAN=PROJECT/'manifest.json'
def update_manifest(source):
 path=PROJECT/'tools/audit_c_provenance.py';spec=importlib.util.spec_from_file_location('audit',path);audit=importlib.util.module_from_spec(spec);spec.loader.exec_module(audit)
 assemblies=audit.compile_assembly();manifest=json.loads(MAN.read_text())
 for entry in manifest['accepted']:
  if not source.endswith('.h') and entry['source']!=source:continue
  rows=audit.instruction_provenance(assemblies[entry['source']])[entry['section']];address=entry['address'];ranges=[]
  for width,inline,_ in rows:
   if inline:
    if ranges and ranges[-1]['end_exclusive']==address:ranges[-1]['end_exclusive']+=width
    else:ranges.append({'start':address,'end_exclusive':address+width,'reason':'Exact retained instruction; codegen idiom pass'})
   address+=width
  assert address==entry['end_exclusive'],(entry['symbol'],address)
  entry['asm_helper_ranges']=ranges;entry['classification']='C_WITH_EXACT_ASM_HELPER' if ranges else 'C_BINARY_EXACT'
 MAN.write_text(json.dumps(manifest,indent=2)+'\n')
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--spec',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output=a.output.resolve();a.output.relative_to(ROOT);a.output.mkdir(parents=True,exist_ok=True);results=[]
 for candidate in json.loads(a.spec.read_text())['candidates']:
  ok=False
  path=PROJECT/candidate['source'];original=path.read_text();manifest=MAN.read_text();changed=original;name=candidate['name'];log=a.output/(name+'.log')
  try:
   for edit in candidate['edits']:
    count=changed.split('/* BEGIN COMPILED')[0].count(edit['old']);assert count==edit.get('expected_count',1),(name,count)
    changed=changed.replace(edit['old'],edit['new'],count)
   assert changed!=original
   path.write_text(changed)
   with log.open('w') as out:
    b=subprocess.run(['make','-C','recovered_exact','all'],cwd=ROOT,stdout=out,stderr=subprocess.STDOUT)
    ok=b.returncode==0
    if ok:
     update_manifest(candidate['source'])
     b=subprocess.run(['make','exact-check'],cwd=ROOT,stdout=out,stderr=subprocess.STDOUT);ok=b.returncode==0
   row={**candidate,'accepted':ok,'log':str(log.relative_to(ROOT)),'acceptance':'complete canonical FLASH equality and provenance audit','differing_bytes':0 if ok else None}
   if ok:row['inventory']=json.loads((PROJECT/'build/function_inventory.json').read_text())
  except Exception as error:
   ok=False;row={**candidate,'accepted':False,'reason':str(error)}
  finally:
   if not locals().get('ok',False):path.write_text(original);MAN.write_text(manifest)
  results.append(row);(a.output/'results.json').write_text(json.dumps({'results':results},indent=2)+'\n');print(name,'EXACT' if ok else 'RESTORED',flush=True)
 # Also leaves fresh build artifacts after the final rejected trial.
 with (a.output/'final_exact_check.log').open('w') as out:
  subprocess.run(['make','exact-check'],cwd=ROOT,stdout=out,stderr=subprocess.STDOUT,check=True)
if __name__=='__main__':main()
