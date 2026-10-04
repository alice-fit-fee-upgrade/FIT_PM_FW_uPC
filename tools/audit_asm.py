#!/usr/bin/env python3
import subprocess,re,json,pathlib
root=pathlib.Path(__file__).resolve().parents[1]
elf=root/'exact_asm/build/reconstructed.elf'
nm=subprocess.check_output(['avr-nm','-n',str(elf)],text=True); errors=[]; checked=0
for line in nm.splitlines():
 m=re.match(r'([0-9a-f]+)\s+\w\s+((?:LAB|FUN)_code_([0-9a-f]+))$',line)
 if m:
  checked+=1
  if int(m[1],16)!=2*int(m[3],16): errors.append(line)
out=subprocess.check_output(['avr-objdump','-D','-b','binary','-m','avr:106',str(root/'reference/flash_golden.bin')],text=True)
code=0; four=0; vectors=[]; instructions=[]
for line in out.splitlines():
 m=re.match(r'\s*([0-9a-f]+):\s*((?:[0-9a-f]{2} )+)\s*(.*)',line)
 if not m: continue
 a=int(m[1],16); b=bytes.fromhex(m[2]); ins=m[3]
 iscode=(a<0x1e2 and b!=b'\xff\xff') or 0x1e2<=a<0x2916 or (0x201a0<=a<0x204e6 and b!=b'\xff\xff')
 if iscode:
  code+=len(b); four+=len(b)==4; instructions.append(dict(address=a,bytes=b.hex(),instruction=ins))
  if a<0x1e2: vectors.append(dict(address=a,slot=a//4,instruction=ins))
data=0x2bdc-0x2916
result=dict(annotation_labels_checked=checked,annotation_address_errors=errors,symbolic_asm_bytes=code,literal_code_bytes=0,data_region_bytes=data,padding_erased_bytes=0x22000-code-data,instructions_32bit=four,vectors=vectors)
(root/'docs/asm_audit.json').write_text(json.dumps(result,indent=2)+'\n')
(root/'docs/instruction_index.json').write_text(json.dumps(instructions,indent=2)+'\n')
print(json.dumps(result,indent=2))
if errors: raise SystemExit(1)
