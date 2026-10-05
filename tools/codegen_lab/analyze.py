#!/usr/bin/env python3
"""Operand-aware inventory of retained idioms, with conservative CFG liveness."""
import bisect,hashlib,json,re,subprocess
from collections import Counter,defaultdict
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
PATTERNS={'LD_LD':('ld','ld'),'ST_ST':('st','st'),'CPI_BRNE':('cpi','brne'),'CPI_CPC_BRGE':('cpi','cpc','brge'),'RCALL_BRCS':('rcall','brcs')}
ALL={f'r{i}' for i in range(32)}|set('C Z N V S H T I'.split())
PTR={'X':{'r26','r27'},'Y':{'r28','r29'},'Z':{'r30','r31'}}
FLAG_WRITE={'cpi':'C Z N V S H','cp':'C Z N V S H','cpc':'C Z N V S H','add':'C Z N V S H','adc':'C Z N V S H','sub':'C Z N V S H','subi':'C Z N V S H','sbc':'C Z N V S H','sbci':'C Z N V S H','and':'Z N V S','andi':'Z N V S','or':'Z N V S','ori':'Z N V S','eor':'Z N V S','inc':'Z N V S','dec':'Z N V S','tst':'Z N V S','lsl':'C Z N V S H','rol':'C Z N V S H','lsr':'C Z N V S','asr':'C Z N V S','ror':'C Z N V S','adiw':'C Z N V S','sbiw':'C Z N V S','mul':'Z C','mulsu':'Z C','neg':'C Z N V S H','com':'C Z N V S','clr':'Z N V S'}
BR_FLAGS={'brne':{'Z'},'breq':{'Z'},'brcs':{'C'},'brcc':{'C'},'brlo':{'C'},'brsh':{'C'},'brge':{'S'},'brlt':{'S'},'brmi':{'N'},'brpl':{'N'},'brtc':{'T'},'brts':{'T'}}
def decoded(row):
 text=row['instruction'].split(';')[0].strip();op,*rest=text.split(None,1);args=[x.strip() for x in rest[0].split(',')] if rest else []
 return op,args

def target(row):
 m=re.search(r';\s*0x([0-9a-f]+)',row['instruction']);return int(m[1],16) if m else None

def effects(row):
 op,args=decoded(row);regs=set(re.findall(r'\br\d+\b',' '.join(args)));u=set();d=set();confidence='modeled'
 for p,rs in PTR.items():
  if any(re.search(r'(?<![A-Za-z])'+p+r'(?:\+|$)|-'+p,a) for a in args):
   u|=rs
   if any(p+'+' in a or '-'+p in a for a in args):d|=rs
 if op in ('call','rcall','icall','eicall','ijmp','eijmp'):
  # No guessed GNU ABI: all state is a conservative use and no kill at calls.
  u|=ALL;confidence='unknown private callee; conservatively all state live'
 elif op in ('ret','reti'):
  u|=ALL;confidence='unknown private caller; conservatively all state live'
 elif op in ('ld','ldd','lds','lpm','elpm','in','ldi','pop'):
  if args and re.fullmatch(r'r\d+',args[0]):d.add(args[0])
  if op=='in' and len(args)>1 and int(args[1],0)==0x3f:u|=set('C Z N V S H T I'.split())
 elif op in ('st','std','sts','out','push'):u|=regs
 elif op in ('mov','movw'):
  d.add(args[0]);u.add(args[1])
  if op=='movw':d.add('r'+str(int(args[0][1:])+1));u.add('r'+str(int(args[1][1:])+1))
 elif op in ('cp','cpc','cpi','tst','sbrc','sbrs','bst'):u|=regs
 elif op in ('mul','muls','mulsu'):u|=regs;d|={'r0','r1'}
 elif regs:
  u|=regs
  if args and re.fullmatch(r'r\d+',args[0]):d.add(args[0])
  if op in ('adiw','sbiw'):d.add('r'+str(int(args[0][1:])+1));u.add('r'+str(int(args[0][1:])+1))
 if op=='eor' and len(args)==2 and args[0]==args[1]:u.discard(args[0])
 d|=set(FLAG_WRITE.get(op,'').split());u|=BR_FLAGS.get(op,set())
 if op in ('adc','sbc','sbci','ror','rol','cpc'):u.add('C')
 if op in ('sbc','sbci','cpc'):u.add('Z')
 if op=='bst':d.add('T')
 if op=='bld':u.add('T')
 if op=='sec':d.add('C')
 if op=='clc':d.add('C')
 if op in ('cli','sei'):d.add('I')
 if op in ('brbs','brbc') and args:u.add('C Z N V S H T I'.split()[int(args[0],0)])
 if op=='out' and args and int(args[0],0)==0x3f:d|=set('C Z N V S H T I'.split())
 return u,d,confidence

def main():
 idx=json.loads((ROOT/'docs/instruction_index.json').read_text());manifest=json.loads((ROOT/'recovered_exact/manifest.json').read_text());inv=json.loads((ROOT/'recovered_exact/build/function_inventory.json').read_text())
 owners={a:f for f in manifest['accepted'] for z in f.get('asm_helper_ranges',[]) for a in range(z['start'],z['end_exclusive'])};byaddr={r['address']:i for i,r in enumerate(idx)}
 effects_all=[effects(r) for r in idx];succ=[]
 for i,r in enumerate(idx):
  op,args=decoded(r);nxt=r['address']+len(bytes.fromhex(r['bytes']));t=target(r);s=set()
  if op in ('rjmp','jmp'):
   if t in byaddr:s.add(byaddr[t])
  elif op not in ('ret','reti','ijmp','eijmp'):
   if nxt in byaddr:s.add(byaddr[nxt])
   if op.startswith('br') and t in byaddr:s.add(byaddr[t])
   if op in ('sbrc','sbrs','cpse','sbic','sbis') and nxt in byaddr:
    after=nxt+len(bytes.fromhex(idx[byaddr[nxt]]['bytes']))
    if after in byaddr:s.add(byaddr[after])
  succ.append(s)
 lin=[set() for _ in idx];lout=[set() for _ in idx]
 changed=True
 while changed:
  changed=False
  for i in range(len(idx)-1,-1,-1):
   o=set().union(*(lin[j] for j in succ[i]));u,d,_=effects_all[i];v=u|(o-d)
   if v!=lin[i] or o!=lout[i]:lin[i]=v;lout[i]=o;changed=True
 records=[]
 for kind,pattern in PATTERNS.items():
  for i in range(len(idx)-len(pattern)+1):
   rows=idx[i:i+len(pattern)]
   if tuple(decoded(r)[0] for r in rows)!=pattern:continue
   addresses=[r['address'] for r in rows]
   if any(a not in owners for a in addresses):continue
   if len({owners[a]['symbol'] for a in addresses})!=1:continue
   if any(rows[j]['address']+len(bytes.fromhex(rows[j]['bytes']))!=rows[j+1]['address'] for j in range(len(rows)-1)):continue
   f=owners[addresses[0]];operands=[decoded(r)[1] for r in rows];registers=sorted(set(re.findall(r'\br\d+\b',' '.join(r['instruction'] for r in rows))))
   pointer=next((p for p in PTR if any(re.search(r'\b'+p+r'\b',r['instruction'].split(';')[0]) for r in rows)),None);bt=target(rows[-1]);callee=target(rows[0]) if kind=='RCALL_BRCS' else None
   if kind in ('LD_LD','ST_ST'):
    data=[a[0] if kind=='LD_LD' else a[1] for a in operands];nums=[int(r[1:]) for r in data]
    if any('-'+str(pointer) in ' '.join(a) for a in operands):cl='predecrement_queue_bytes'
    elif nums[0]%2==0 and nums[1]==nums[0]+1:cl='adjacent_register_word_'+('load' if kind=='LD_LD' else 'store')
    elif nums[0]==nums[1]:cl='repeated_byte_fill'
    else:cl='distinct_byte_stream'
    if f['source'] in ('src/console_cts_interrupt.c','src/console_tx_interrupt.c','src/console_character_send.c'):
     cl='queue_cursor_fields_predecrement' if cl.startswith('predecrement') else 'queue_cursor_fields_postincrement'
    elif f['source']=='src/dma_channel_interrupt.c' and cl!='repeated_byte_fill':
     cl='dma_descriptor_or_limit_fields'
    elif cl.startswith('adjacent_register_word'):
     cl='configuration_word_'+('load' if kind=='LD_LD' else 'store')
    direction='read' if kind=='LD_LD' else 'write';mem={'direction':direction,'pointer':pointer,'address':'runtime pointer; see context/setup, not assumed constant','order':'instruction order','volatile_reason':'no blanket volatile assumption; queue/device stream ownership must be inspected'}
    semantic=f'{cl}: {operands}; pointer updates exactly as encoded; no SREG writes'
    if kind=='LD_LD':ref='lo = *p++; hi = *p++;' if cl.startswith('adjacent') else ('second = *--p; first = *--p;' if cl.startswith('predecrement') else 'first = *p++; second = *p++;')
    else:ref='*p++ = first; *p++ = second;'
   else:
    mem={'direction':'none directly; callee effects unknown' if callee else 'none','pointer':None};samefunc=bt is not None and f['address']<=bt<f['end_exclusive'] if 'end_exclusive' in f else None
    if kind=='RCALL_BRCS':cl='carry_parser_status' if callee==0x2634 else 'carry_device_or_stream_status';semantic=f'call 0x{callee:x}; branch to 0x{bt:x} if returned C=1';ref='if (legacy_result.error) goto error;'
    elif kind=='CPI_CPC_BRGE':cl='signed_word_upper_bound';semantic=f'signed 16-bit compare, CPC consumes C and Z from CPI; BRGE tests S=N xor V; target 0x{bt:x}';ref='if ((int16_t)value >= signed_limit) goto error;'
    else:
     compared=operands[0][0];imm=int(operands[0][1],0)
     if compared=='r16' and imm in (13,32,44):cl='delimiter_validation'
     elif compared=='r16' and f['source'].endswith('console_dispatch.c'):cl='command_dispatch'
     elif bt is not None and bt<addresses[0]:cl='backward_loop_or_retry'
     else:cl='state_or_sentinel_validation'
     semantic=f'branch when {compared} != {imm}; byte equality is signedness-independent; target 0x{bt:x}';ref=f'if (value != {imm}) goto target;'
   start=i;end=i+len(rows)-1;reads=set().union(*(effects_all[j][0] for j in range(start,end+1)));writes=set().union(*(effects_all[j][1] for j in range(start,end+1)))
   def view(rs):return [{**r,'address_hex':hex(r['address'])} for r in rs]
   records.append({'idiom':kind,'address':addresses[0],'address_hex':hex(addresses[0]),'function':f['symbol'],'source':f['source'],'before':view(idx[max(0,i-4):i]),'sequence':view(rows),'after':view(idx[i+len(rows):i+len(rows)+4]),'registers':registers,'pointer_registers':sorted(PTR.get(pointer,set())),'memory':mem,'memory_setup_source_evidence':[line.strip() for line in (ROOT/'recovered_exact'/f['source']).read_text().split('/* BEGIN COMPILED')[0].splitlines() if re.search(r'(?:settings|cursor|pointer|address)\s*(?:asm\([^)]*\))?\s*=.*0x[0-9a-f]+',line)][:12],'branch_target':bt,'branch_target_scope':'within_entry' if bt is not None and f['address']<=bt<f['end_exclusive'] else 'outside_entry' if bt is not None else None,'callee':callee,'callee_sreg_writes_unknown':callee is not None,'sreg_reads':sorted(reads&set('C Z N V S H T I'.split())),'sreg_writes':sorted(writes&set('C Z N V S H T I'.split())),'live_before_conservative':sorted(lin[start]),'live_after_conservative':sorted(lout[end]),'liveness_scope':'CFG over verified instructions; unknown calls/returns use all state; not a precise private ABI proof','semantic_class':cl,'inferred_semantics':semantic,'reference_c':ref,'reference_status':'REFERENCE_C_INFERRED','confidence':'operand-level inference; dynamic address and full call contract require manual evidence'})
 result={'baseline':inv,'site_measure':'contiguous nonempty ASM helper ranges; zero-byte barriers excluded','asm_sites':sum(len(f.get('asm_helper_ranges',[])) for f in manifest['accepted']),'occurrences':records,'classes':{k:dict(Counter(r['semantic_class'] for r in records if r['idiom']==k)) for k in PATTERNS},'overlap_warning':'neighboring windows overlap; unique bytes are union, never sum occurrences * width'}
 (ROOT/'docs/asm_idioms.json').write_text(json.dumps(result,indent=2)+'\n')
 out='# Operand-aware retained ASM idioms\n\nGenerated by tools/codegen_lab/analyze.py. Full machine-readable fields are in\n[asm_idioms.json](asm_idioms.json). All addresses are FLASH byte addresses.\n\nLiveness is conservative CFG dataflow: unknown private calls and returns keep\nall registers/flags live. It is not an exact private ABI proof. Dynamic memory\naddresses are explicitly unknown unless the source/context establishes them.\nReference snippets are REFERENCE_C_INFERRED; existing transition models are\nseparate evidence and do not certify these value-only snippets.\n\n'
 for kind in PATTERNS:
  rr=[r for r in records if r['idiom']==kind];covered={a for r in rr for z in r['sequence'] for a in range(z['address'],z['address']+len(bytes.fromhex(z['bytes'])))}
  out+=f'## {kind}: {len(rr)} occurrences / {len(covered)} unique bytes\n\nClasses: `{result["classes"][kind]}`.\n\n'
  for r in rr:
   out+=f'### {r["address_hex"]} — {r["function"]}\n\nSource: `{r["source"]}`; class: `{r["semantic_class"]}`.\n\n```text\n'
   for label in ('before','sequence','after'):
    out+=label+':\n'
    for z in r[label]:out+=f'  {z["address_hex"]}: {z["bytes"]}  {z["instruction"]}\n'
   out+='```\n\n'+r['inferred_semantics']+'\n\n'
   out+=f'Registers: {r["registers"]}; pointer: {r["pointer_registers"]}; memory: {r["memory"]}.\n\n'
   out+=f'SREG reads/writes: {r["sreg_reads"]} / {r["sreg_writes"]}; branch: {r["branch_target"]} ({r["branch_target_scope"]}); callee: {r["callee"]}.\n\n'
   out+=f'Conservative live before: {r["live_before_conservative"]}; after: {r["live_after_conservative"]}.\n\n'
   out+='REFERENCE_C_INFERRED: `'+r['reference_c']+'`\n\n'
 (ROOT/'docs/asm_idioms.md').write_text(out)
 print(json.dumps(result['classes']))
if __name__=='__main__':main()
