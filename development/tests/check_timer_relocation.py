#!/usr/bin/env python3
"""Check every relocated instruction/edge, without treating timing as equivalent."""
import sys,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'development/tools'))
from build import BUILD,BASE,symbols
from relocate_timer import decode,START,END,INVERSE,SKIPS
from power_patch import SITES
hooks=dict(SITES)
old=decode(BASE/'build/reconstructed.elf');new=decode(BUILD/'development.elf')
names=symbols(BUILD/'development.elf')
rows=json.loads((BUILD/'timer_relocation.json').read_text())
mapping={r['original']:names[r['symbol']] for r in rows}
def relocated(a):return mapping.get(a,a)
counts={'unchanged_instruction_encodings':0,'control_flow_edges':0}
for row in rows:
    a=row['original'];b=names[f'pm_timer_instruction_{a:04x}']
    op,args,n,target,raw=old[a];actual=new[b]
    following=mapping.get(a+n)
    if a in hooks:
        assert actual[0]=='jmp' and actual[3]==names[hooks[a]]
        counts['control_flow_edges']+=1
    elif op in ('rjmp','jmp','rcall','call'):
        assert actual[0]==('call' if op in ('call','rcall') else 'jmp')
        assert actual[3]==relocated(target),(a,actual,target)
        assert following==b+4
        counts['control_flow_edges']+=1
    elif op in INVERSE:
        assert actual[0]==INVERSE[op] and actual[3]==b+6
        assert new[b+2][0]=='jmp' and new[b+2][3]==relocated(target)
        assert following==b+6
        counts['control_flow_edges']+=2
    elif op in SKIPS:
        assert actual[0]==SKIPS[op] and actual[1]==args
        assert new[b+2][0]=='jmp' and new[b+2][3]==relocated(a+n+old[a+n][2])
        assert following==b+6
        counts['control_flow_edges']+=2
    else:
        assert actual[4]==raw,('instruction changed',hex(a),actual,old[a])
        if following is not None:assert following==b+n
        counts['unchanged_instruction_encodings']+=1
assert new[mapping[END-2]][0]=='reti'
report={'status':'PASS','original_start':START,'original_end_exclusive':END,
        'relocated_start':names['pm_timer_relocated'],'instruction_sites':len(rows),**counts,
        'scope':'All ordinary opcode bytes and transformed control-flow edges checked; no timing or hardware proof.'}
(BUILD/'timer_relocation_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS timer relocation:',report)
