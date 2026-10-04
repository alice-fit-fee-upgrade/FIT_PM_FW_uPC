#!/usr/bin/env python3
"""Minimal, auditable conversion of upstream analysis; never reads golden bytes."""
from pathlib import Path
import re
import argparse
root=Path(__file__).resolve().parents[1]
s=(root/'asm_analysis/main.S').read_text().splitlines(); result=[]; data=False
for line in s:
    if line.startswith(';\t.org (0x100d0'): break
    if line.startswith(';\t.byte'): continue
    if '.org (0x148b << 1)' in line: data=True
    if not line.lstrip().startswith(';') and re.match(r'\s*\.org\s',line):
        line+=', '+('0x00' if data else '0xff')
    line=line.replace('WUT PMxx','INR PM12')
    result.append(line)
s='\n'.join(result)+'\n'
s=s.replace('\tret\n\t;jmp         FUN_code_010184', '\tjmp         boot_020308\n\t; restored original JMP; upstream replacement was ret\n\t; FUN_code_010184')
s=s.replace('.section .text','.section .application,"ax",@progbits')
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--output', type=Path, default=root/'exact_asm/generated/application.S')
p=parser.parse_args().output; p.parent.mkdir(parents=True,exist_ok=True); p.write_text(s)
