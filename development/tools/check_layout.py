#!/usr/bin/env python3
"""Reject every unplanned development difference, symbol move or RAM allocation."""
import hashlib,json,subprocess,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).parent))
from build import ROOT,BUILD,BASE,symbols

def main():
    golden=(ROOT/'reference/flash_golden.bin').read_bytes();dev=(BUILD/'development.bin').read_bytes()
    assert len(dev)==len(golden)==0x22000
    names=symbols(BUILD/'development.elf');original=symbols(BASE/'build/reconstructed.elf')
    for name,addr in original.items():
        if name in names: assert names[name]==addr, ('legacy symbol moved',name)
        else: raise AssertionError(('legacy symbol missing',name))
    sections=subprocess.check_output(['avr-objdump','-h',str(BUILD/'development.elf')],text=True)
    import re
    match=re.search(r'^\s*\d+\s+\.extensions\s+([0-9a-f]+)\s+([0-9a-f]+)',sections,re.M);assert match
    size,start=map(lambda s:int(s,16),match.groups());assert start==0x4000 and 0<size<=0x4000
    assert golden[start:start+size]==b'\xff'*size,'extension window is not erased'
    assert dev[start:start+10]==golden[0x12ea:0x12f4], 'original prologue/first-byte read not replayed'
    expected=dev[names['pm_patch_template']:names['pm_patch_template']+6]
    assert dev[0x12ea:0x12f0]==expected and expected!=golden[0x12ea:0x12f0]
    first=int.from_bytes(expected[:2],'little');second=int.from_bytes(expected[2:4],'little')
    assert first & 0xfe0e == 0x940c and expected[4:] == b'\0\0', 'gate must be JMP + NOP'
    destination=2*(((first & 0x1f0)<<13)|((first & 1)<<16)|second)
    assert destination==names['pm_extension_entry'], 'gate does not reach extension entry'
    assert dev[0x49e:0x4a2]==golden[0x49e:0x4a2], 'original ISR entry moved'
    allowed=set(range(0x12ea,0x12f0))|set(range(start,start+size))
    for i,(address,target) in enumerate(((0x77e,'pm_temperature_claim'),(0x788,'pm_temperature_release'))):
        offset=names['pm_temperature_patch_table']+4*i
        gate=dev[offset:offset+4]
        assert dev[address:address+4]==gate
        word=int.from_bytes(gate[:2],'little')
        assert word & 0xfe0e==0x940c
        assert 2*(((word & 0x1f0)<<13)|((word & 1)<<16)|int.from_bytes(gate[2:],'little'))==names[target]
        allowed.update(range(address,address+4))
    from power_patch import SITES
    for i,(address,target) in enumerate(SITES):
        gate=dev[names['pm_pg_patch_table']+4*i:names['pm_pg_patch_table']+4*i+4]
        assert dev[address:address+4]==gate
        word=int.from_bytes(gate[:2],'little')
        assert word & 0xfe0e == 0x940c
        assert 2*(((word & 0x1f0)<<13)|((word & 1)<<16)|int.from_bytes(gate[2:],'little'))==names[target]
        allowed.update(range(address,address+4))
    from pll_patch import ADDRESS as pll_address,TARGET as pll_target
    gate=dev[names['pm_pll_patch_table']:names['pm_pll_patch_table']+4]
    assert dev[pll_address:pll_address+4]==gate
    word=int.from_bytes(gate[:2],'little')
    assert word & 0xfe0e==0x940c
    assert 2*(((word & 0x1f0)<<13)|((word & 1)<<16)|int.from_bytes(gate[2:],'little'))==names[pll_target]
    assert dev[pll_address+4:0x2530]==golden[pll_address+4:0x2530], 'original PLL body overwritten'
    allowed.update(range(pll_address,pll_address+4))
    differing={i for i,(a,b) in enumerate(zip(golden,dev)) if a!=b}
    assert not differing-allowed, ('unexpected FLASH differences',sorted(differing-allowed)[:16])
    assert dev[0x20000:]==golden[0x20000:], 'bootloader modified'
    report={'acceptance':'planned development differences only; NOT binary-exact PM.hex',
            'golden_sha256':hashlib.sha256(golden).hexdigest(),'development_sha256':hashlib.sha256(dev).hexdigest(),
            'differing_bytes':len(differing),'gate_start':0x12ea,'gate_length':6,'timer_mode':'legacy body in place; two four-byte temperature gates','temperature_guard_sites':[0x77e,0x788],'extension_start':start,
            'extension_size':size,'extension_limit':0x8000,'legacy_symbols_preserved':len(original),
            'bootloader_unchanged':True,'pll_read_gate':{'address':pll_address,'length':4,'target':pll_target,'original_body_retained':True},'retained_diagnostic_pg_hooks':[hex(a) for a,_ in SITES],'bus_lock':'GPIORE/GPIORF explicitly reserved, reset-cleared and startup initialized','ram_globals':'rejected by linker; no unverified SRAM reservation'}
    (BUILD/'layout_report.json').write_text(json.dumps(report,indent=2)+'\n')
    print('PASS development layout:',len(differing),'planned differing bytes; extension',size,'bytes')
if __name__=='__main__':main()
