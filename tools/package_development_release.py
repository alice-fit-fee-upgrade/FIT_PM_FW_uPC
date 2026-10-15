#!/usr/bin/env python3
"""Build a reviewable candidate + exact rollback bundle; never flash hardware."""
import argparse,hashlib,json,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from memory_image import read_hex

def run(args,log):
    with log.open('w') as out:
        subprocess.run(args,cwd=ROOT,stdout=out,stderr=subprocess.STDOUT,check=True)

def application_hex(source,dest):
    memory,_=read_hex(source)
    lines=[]
    # Application only. No bootloader, EEPROM, signatures, fuses or lock bits.
    for base in range(0,0x20000,0x10000):
        upper=base>>16
        record=bytes([2,0,0,4,upper>>8,upper&255])
        lines.append(':'+ (record+bytes([-sum(record)&255])).hex().upper())
        for a in range(base,base+0x10000,16):
            values=[memory.get(i) for i in range(a,a+16)]
            if not any(v is not None for v in values):continue
            payload=bytes(255 if v is None else v for v in values)
            record=bytes([16,(a>>8)&255,a&255,0])+payload
            lines.append(':'+(record+bytes([-sum(record)&255])).hex().upper())
    dest.write_text('\n'.join(lines)+ '\n:00000001FF\n')
    decoded,_=read_hex(dest)
    assert all(0<=a<0x20000 for a in decoded)
    assert all(decoded[a]==v for a,v in memory.items() if a<0x20000)

def main():
    parser=argparse.ArgumentParser();parser.add_argument('tag');args=parser.parse_args()
    assert '/' not in args.tag and args.tag not in ('.','..')
    assert not subprocess.check_output(['git','status','--porcelain'],cwd=ROOT,text=True).strip(),'commit changes before packaging'
    out=ROOT/'development/build/releases'/args.tag
    assert not out.exists(), 'never overwrite a release bundle'
    out.mkdir(parents=True)
    run(['make','exact-check'],out/'exact-check.log')
    run(['make','development-check'],out/'development-check.log')
    commit=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip()
    copies={
      'development/build/development.elf':'candidate.elf',
      'development/build/development.bin':'candidate-full-flash-audit.bin',
      'recovered_exact/build/flash_rebuilt.bin':'rollback-full-flash-audit.bin',
      'development/build/layout_report.json':'layout_report.json',
      'development/build/timer_relocation_validation.json':'timer_relocation_validation.json',
      'development/build/stack_usage.json':'stack_usage.json',
    }
    for src,dst in copies.items():shutil.copyfile(ROOT/src,out/dst)
    application_hex(ROOT/'development/build/development.hex',out/'candidate-application.hex')
    # Use the original dump representation, not a presumed filename for build HEX.
    golden=ROOT/'reference/flash_golden.bin'
    rollback=ROOT/'recovered_exact/build/reconstructed.elf'
    temp=out/'rollback-full.hex'
    subprocess.run(['avr-objcopy','-O','ihex',str(rollback),str(temp)],check=True)
    application_hex(temp,out/'rollback-original-application.hex');temp.unlink()
    assert (out/'rollback-full-flash-audit.bin').read_bytes()==golden.read_bytes()
    for name,full in [('candidate-application.hex','candidate-full-flash-audit.bin'),('rollback-original-application.hex','rollback-full-flash-audit.bin')]:
        memory,_=read_hex(out/name);image=bytearray(b'\xff'*0x20000)
        for a,v in memory.items():image[a]=v
        assert image==(out/full).read_bytes()[:0x20000]
    (out/'README.txt').write_text(f'''PM12 {args.tag}\nSource commit: {commit}\n\nSOFTWARE-CHECKED PRERELEASE. Physical PM12/tester validation: NOT_RUN.\nThis step relocates the timer ISR; it is NOT a new ordinary-C ISR.\nThe candidate includes the @PING/@HELP CLI gate and relocated original timer logic.\nBus lock, INA telemetry and FPGA-independent diagnostics are NOT implemented.\nWidened control transfers change timing; compare both versions with the tester.\n\nProgramming artifacts (application only, addresses below 0x20000):\n  candidate-application.hex\n  rollback-original-application.hex (exact original application)\nUse the established ATxmega128A3U PDI procedure, verify identity and read back.\nDo not chip-erase or alter bootloader/EEPROM/fuses/signatures/lock bits.\nConfirm that the chosen programmer procedure preserves those regions.\nNo programming or device validation is performed by this packaging tool.\n\nThe *full-flash-audit.bin files are canonical comparison artifacts, including\nthe unchanged bootloader; they are NOT the default programming files.\nSoftware PASS does not imply hardware acceptance.\n''')
    manifest={'tag':args.tag,'source_commit':commit,'stage':'timer relocation prerequisite; not C migration',
      'software_checks':'PASS','board_validation':'NOT_RUN','tester_validation':'NOT_RUN',
      'exact_baseline_differing_bytes':0,'golden_sha256':hashlib.sha256(golden.read_bytes()).hexdigest(),
      'artifacts':{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(out.iterdir()) if p.is_file()}}
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (out/'SHA256SUMS').write_text(''.join(f'{hashlib.sha256(p.read_bytes()).hexdigest()}  {p.name}\n' for p in sorted(out.iterdir()) if p.is_file()))
    print(out)
if __name__=='__main__':main()
