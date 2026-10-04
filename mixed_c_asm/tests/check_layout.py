#!/usr/bin/env python3
import sys,pathlib,re,subprocess,json
ROOT=pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools'))
from compare_flash import report
build=ROOT/'mixed_c_asm/build'; golden=(ROOT/'reference/flash_golden.bin').read_bytes(); mixed=(build/'flash_mixed.bin').read_bytes()
sections={}
for line in subprocess.check_output(['avr-objdump','-h',str(build/'mixed.elf')],text=True).splitlines():
 m=re.match(r'\s*\d+\s+(\.\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)',line)
 if m: sections[m[1]]=(int(m[3],16),int(m[2],16))
assert sections['.application']==(0,0x2bdc)
assert sections['.boot']==(0x20000,0x4e6)
allowed=set(range(0x2130,0x2174)); allowed.update(range(0x20a6,0x2130)); allowed.update(range(0x17e4,0x1808)); allowed.update(range(0x1664,0x167e)); allowed.update(range(0x171e,0x173a)); allowed.update(range(0x1808,0x1840)); additions=[]
for name,base in [('.bridges',0x3000),('.text',0x4000)]:
 a,n=sections[name]; assert a==base and n>0
 assert all(b==255 for b in golden[a:a+n]),'new source overlaps golden programming'
 allowed.update(range(a,a+n)); additions.append({'section':name,'start':a,'size':n})
for name,(a,n) in sections.items():
 assert name in ('.application','.boot','.bridges','.text') or n==0,('unexpected section',name,a,n)
assert len(golden)==len(mixed)==0x22000
changes=[a for a,(g,r) in enumerate(zip(golden,mixed)) if g!=r]
assert changes and all(a in allowed for a in changes),'difference outside replacement/addition regions'
assert mixed[0x180c:0x1840]==b'\xff'*52
assert mixed[0x1722:0x173a]==b'\xff'*24
assert mixed[0x1668:0x167e]==b'\xff'*22
assert mixed[0x17e8:0x1808]==b'\xff'*32
assert mixed[0x2120:0x2130]==b'\xff'*16
assert mixed[0x2108:0x211c]==b'\xff'*20
assert mixed[0x20f0:0x2104]==b'\xff'*20
assert mixed[0x20aa:0x20d0]==b'\xff'*38
assert mixed[0x20d4:0x20ec]==b'\xff'*24
assert mixed[0x2134:0x214c]==b'\xff'*24
assert mixed[0x2150:0x2174]==b'\xff'*36

def symbols(elf):
 result={}
 for line in subprocess.check_output(['avr-nm','-n',str(elf)],text=True).splitlines():
  m=re.match(r'([0-9a-f]+)\s+[tT]\s+(\S+)',line)
  if m: result[m[2]]=int(m[1],16)
 return result
original=symbols(ROOT/'exact_asm/build/reconstructed.elf'); rebuilt=symbols(build/'mixed.elf')
for name,a in original.items(): assert rebuilt.get(name)==a,('original symbol moved/missing',name,a,rebuilt.get(name))
assert rebuilt['fpga_is_ready']==0x2130 and rebuilt['FUN_code_0010a6']==0x214c
assert 'pm_scale_unsigned' in rebuilt
assert 'pm_scale_signed_bridge' in rebuilt
text,_=report(golden,mixed); (build/'byte_comparison.txt').write_text(text)
result={'image_size':len(mixed),'changed_bytes':len(changes),'original_code_replaced':[{'start':0x2130,'size':28},{'start':0x214c,'size':40},{'start':0x20d0,'size':28},{'start':0x20a6,'size':42},{'start':0x20ec,'size':24},{'start':0x2104,'size':24},{'start':0x211c,'size':20},{'start':0x17e4,'size':36},{'start':0x1664,'size':26},{'start':0x171e,'size':28},{'start':0x1808,'size':56}],'additions':additions,'original_text_symbols_preserved':len(original),'boot_vectors_data_unchanged':True,'new_data_bss_bytes':0,'permitted_changes_only':True}
(build/'layout_result.json').write_text(json.dumps(result,indent=2)+'\n')
print('PASS mixed layout:',len(changes),'changed bytes;',len(original),'original text symbols unmoved; no new static SRAM')
