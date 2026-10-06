#!/usr/bin/env python3
"""Build development extensions from exact source objects; patch a six-byte gate."""
import json,shlex,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
DEV=ROOT/'development';BASE=ROOT/'recovered_exact';BUILD=DEV/'build'
def run(args,cwd=ROOT): subprocess.run([str(a) for a in args],cwd=cwd,check=True)
def symbols(elf):
    return {v[2]:int(v[0],16) for line in subprocess.check_output(['avr-nm','-n',str(elf)],text=True).splitlines() if len(v:=line.split())==3 and all(c in '0123456789abcdef' for c in v[0])}
def main():
    run(['make','-C',BASE,'all'])
    BUILD.mkdir(exist_ok=True)
    text=(BASE/'generated/sections.ld').read_text()
    assert text.count(' .boot 0x20000')==1
    extension=''' .extensions 0x4000 : {
 KEEP(*(.extension.entry)) *(.text*) *(.progmem*)
 } > flash
'''
    # Consume only new GNU code and explicit FLASH constants. Ordinary RAM
    # globals remain rejected: the legacy firmware has no CRT for these.
    text=text.replace(' .boot 0x20000',extension+' .boot 0x20000')
    text=text.replace('*(.text*) *(.data*) *(.rodata*) *(.bss*) *(COMMON)', '*(.data*) *(.rodata*) *(.bss*) *(COMMON) *(.eeprom*) *(.init*) *(.fini*)')
    script=BUILD/'development.ld'
    script.write_text('OUTPUT_FORMAT("elf32-avr")\nOUTPUT_ARCH(avr:106)\nENTRY(reset)\nMEMORY { flash (rx) : ORIGIN = 0, LENGTH = 0x22000 }\nSECTIONS {\nreset = 0;\n'+text+'}\nINCLUDE generated/assertions.ld\nASSERT(SIZEOF(.extensions) <= 0x4000, "extension exceeds reviewed FLASH window")\nASSERT(pm_extension_entry == 0x4000, "extension entry moved")\n')
    objects=[];frames=[]
    for source in sorted((DEV/'src').glob('*.c'))+sorted((DEV/'src').glob('*.S')):
        obj=BUILD/(source.stem+'.o');objects.append(obj)
        run(['avr-gcc','-mmcu=atxmega128a3u','-Os','-std=gnu99','-Wall','-Wextra','-Werror','-ffunction-sections','-fdata-sections','-fno-lto','-fstack-usage','-I'+str(DEV/'src'),'-c',source,'-o',obj])
        if source.suffix=='.c':
            for row in obj.with_suffix('.su').read_text().splitlines():
                function,size,kind=row.rsplit('\t',2)
                assert kind=='static' and int(size)<=128, ('unreviewed dynamic/large stack frame',function,size,kind)
                frames.append({'function':function,'bytes':int(size),'kind':kind})
    (BUILD/'stack_usage.json').write_text(json.dumps({'per_function_limit':128,'frames':frames,'scope':'per-frame guard; not a whole-program stack or recursion proof'},indent=2)+'\n')
    plan=subprocess.check_output(['make','-B','-n','build/reconstructed.elf'],cwd=BASE,text=True)
    link=next(shlex.split(line) for line in plan.splitlines() if line.startswith('avr-gcc ') and ' -nostdlib ' in line)
    original_objects=[BASE/arg for arg in link if arg.endswith('.o')]
    elf=BUILD/'development.elf'
    run(['avr-gcc','-mmcu=atxmega128a3u','-nostdlib','-Wl,-T,'+str(script)+',-Map,'+str(BUILD/'development.map'),*original_objects,*objects,'-lgcc','-o',elf],cwd=BASE)
    names=symbols(elf)
    hexfile=BUILD/'development.hex';binary=BUILD/'development.bin'
    run(['avr-objcopy','-O','ihex',elf,hexfile]);run([sys.executable,ROOT/'tools/memory_image.py',hexfile,binary])
    data=binary.read_bytes();gate=data[names['pm_patch_template']:names['pm_patch_template']+6]
    golden=(ROOT/'reference/flash_golden.bin').read_bytes()
    assert data[0x12ea:0x12f4]==golden[0x12ea:0x12f4], 'displaced prologue changed'
    section=BUILD/'dispatcher.bin'
    run(['avr-objcopy','--dump-section','.c_0012ea='+str(section),elf])
    original=section.read_bytes();assert len(original)==0x290
    section.write_bytes(gate+original[6:])
    run(['avr-objcopy','--update-section','.c_0012ea='+str(section),elf])
    run(['avr-objcopy','-O','ihex',elf,hexfile]);run([sys.executable,ROOT/'tools/memory_image.py',hexfile,binary])
    (BUILD/'development.dis').write_text(subprocess.check_output(['avr-objdump','-d',str(elf)],text=True))
    run([sys.executable,DEV/'tools/check_layout.py'])
    print('Development build: GNU ABI extensions at 0x4000; exact baseline remains separate.')
if __name__=='__main__':main()
