#!/usr/bin/env python3
"""One-time forensic extraction of symbolic boot ASM; not called by firmware build."""
import argparse,pathlib,re,subprocess
p=argparse.ArgumentParser(); p.add_argument('binary'); p.add_argument('output'); args=p.parse_args()
out=subprocess.check_output(['avr-objdump','-D','-b','binary','-m','avr:106','--start-address=0x201a0','--stop-address=0x204e6',args.binary],text=True)
lines=['/* Recovered boot code; objdump addresses are bytes. No blob embedding. */','.section .boot,"ax",@progbits']
for line in out.splitlines():
 m=re.match(r'\s*([0-9a-f]+):\s*((?:[0-9a-f]{2} )+)\s*([^;]+)(?:;\s*(.*))?',line)
 if not m: continue
 a=int(m[1],16); ins=m[3].strip(); comment=m[4] or ''
 if ins.startswith('.word'):
  if bytes.fromhex(m[2])!=b'\xff\xff': raise ValueError('unknown non-erased instruction; explicit investigation required')
  continue
 if '.+' in ins or '.-' in ins:
  target=re.search(r'0x([0-9a-f]+)',comment)
  if not target: raise ValueError('missing relative target')
  ins=re.sub(r'\.[+-]\d+',f'boot_{int(target[1],16):06x}',ins)
 lines += [f'.org 0x{a-0x20000:x}, 0xff',f'.global boot_{a:06x}',f'boot_{a:06x}:','    '+ins]
pathlib.Path(args.output).write_text('\n'.join(lines)+'\n')
