#!/usr/bin/env python3
import hashlib,json,pathlib,subprocess
from memory_image import read_hex,canonical,FLASH_SIZE
ROOT=pathlib.Path(__file__).resolve().parents[1]
def sha(b): return hashlib.sha256(b).hexdigest()
def inspect(p,origin):
 b=p.read_bytes(); row=dict(path=str(p.relative_to(ROOT)),size=len(b),sha256=sha(b),format=subprocess.check_output(['file','-b',str(p)],text=True).strip(),origin=origin)
 if p.suffix.lower() in ('.hex','.eep'):
  m,t=read_hex(p); row.update(memory=('EEPROM' if 'eeprom' in p.name.lower() or p.suffix=='.eep' else 'user signature' if 'usersig' in p.name or 'PM_sign' in p.name else 'device signature' if p.name=='signature.hex' else 'fuses' if 'fuse' in p.name else 'lock bits' if 'lock' in p.name else 'FLASH' if p.name in ('flash.hex','application.hex','apptable.hex','boot.hex','1-PM.hex') else 'data-space dump; interpretation unverified'), records=t,address_range=[min(m),max(m)] if m else None,explicit_bytes=len(m),memory_payload_sha256=sha(bytes(m[k] for k in sorted(m))))
 return row
tracked=subprocess.check_output(['git','ls-tree','-r','--name-only','-z','152201f04d8aa6e04c36f70ec09e44567ef3bc0c'],cwd=ROOT).decode().split('\0')
rows=[inspect(ROOT/n,'upstream git 152201f04d8aa6e04c36f70ec09e44567ef3bc0c; acquisition not independently attested') for n in tracked if n and (ROOT/n).is_file()]
rows += [inspect(p,'user attachment; stated physical-device readout (PDF/screenshot documentary evidence)') for p in sorted((ROOT/'reference/originals').iterdir())]
(ROOT/'docs/forensic_inventory.json').write_text(json.dumps(rows,indent=2)+'\n')
comparisons=[]
for a,b,size in [('reference/originals/1-PM.hex','dump/flash.hex',FLASH_SIZE),('reference/originals/4-PM.eep','dump/eeprom.hex',2048),('reference/originals/3-PM_sign.hex','dump/usersig.hex',512),('reference/originals/3-PM_sign.hex','reference/originals/5-PM_sign.hex',512)]:
 x,y=ROOT/a,ROOT/b; xx=canonical(read_hex(x)[0],size); yy=canonical(read_hex(y)[0],size)
 comparisons.append(f'| {a} | {b} | {x.read_bytes()==y.read_bytes()} | {xx==yy} | {sum(u!=v for u,v in zip(xx,yy))} |')
for name,src,size in [('eeprom_golden.bin','4-PM.eep',2048),('usersig_golden.bin','3-PM_sign.hex',512)]:
 p=ROOT/'reference'/name
 if not p.exists(): p.write_bytes(canonical(read_hex(ROOT/'reference/originals'/src)[0],size))
lines=['# Forensic inventory','', 'Full machine-readable inventory: `forensic_inventory.json`. All upstream tracked files, including Ghidra project/database files, C, drivers, listings, makefiles and schematics, are inventoried below. Existing firmware/analysis files were not modified; root Makefile gains an isolated exact-check target.', '', 'Acquisition provenance is a user/repository assertion, not a verified chain of custody. Golden FLASH uses the supplied `1-PM.hex`, not an assumed-equivalent repo dump.', '', 'Target: ATxmega128A3U, PDI, user-reported signature 0x1E9742. Screenshot selects ATxmega128A3 (without U); this naming discrepancy is retained. Canonical FLASH is 0x22000 bytes: 128 KiB application plus 8 KiB boot; EEPROM 2 KiB; user signature 512 bytes. Device reference: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8386-8-and-16-bit-AVR-Microcontroller-ATxmega64A3U-128A3U-192A3U-256A3U_datasheet.pdf','', '| Supplied/reference | Comparison | Text identical | Canonical identical | Different bytes |','|---|---|---|---|---|']+comparisons
lines += ['', 'Repository flash HEX is shorter/sparse; supplied FLASH includes the full space. Both describe the same canonical FLASH only if the comparison above says True. Record checksums, EOF, record sizes, extended segment/linear addressing and conflicting overlaps are validated by `tools/memory_image.py`. Type 02 uses segment << 4; type 04 uses upper word << 16. Missing cells become FF. Addresses in this inventory are byte addresses.', '', '## Original ELF', '', '`file` reports ELF32 AVR EXEC, but there is no .text/application/boot section and no executable code. It is a device-memory programming container, 992 bytes, stripped, no symbols, entry 0, flags 0, four LOAD segments. `avr-readelf`, `avr-objdump`, `avr-nm` outputs are preserved alongside this document.', '', '| Section | Address | Length | Memory |','|---|---|---|---|','| .signature | 0x840000 | 3 | Device ID, stored 42 97 1E |','| .lock | 0x830000 | 1 | Lock bits |','| .fuse | 0x820000 | 6 | Fuses, includes unused index 3 |','| .user_signature | 0x850000 | 512 | User signature |','', '## Every input file','', '| Path | Bytes | SHA256 | Address range / record types | Memory | Format |','|---|---:|---|---|---|---|']
for r in rows:
 lines.append(f'| {r["path"]} | {r["size"]} | `{r["sha256"]}` | {r.get("address_range","not memory-addressed")}; {r.get("records", "")} | {r.get("memory", "ELF multi-memory container" if r["path"].endswith(".elf") else "not a memory image")} | {r["format"]} |')
lines += ['', '## Additional cross-checks', '']
g=canonical(read_hex(ROOT/'reference/originals/1-PM.hex')[0],FLASH_SIZE)
for name,base,size in [('application',0,0x20000),('boot',0x20000,0x2000)]:
    image=canonical(read_hex(ROOT/'dump'/f'{name}.hex')[0],size)
    lines.append(f'- Repo {name}.hex is region-relative (base 0x{base:X}); canonical region matches supplied FLASH: {image==g[base:base+size]}.')
for name in ('apptable','data'):
    lines.append(f'- Repo {name}.hex has EOF only, no payload; it does not establish that the corresponding hardware space is erased.')
lines += ['- Supplied and repo schematic PDFs differ at file level; both are separately hashed. No PDF semantic identity is assumed.', '- ELF user signature matches both supplied HEX files. Lock and defined fuses match repo dumps. Device signature has REVERSED BYTE ORDER: ELF 42 97 1E versus repo HEX 1E 97 42; both describe displayed ID 0x1E9742, but these payloads are not byte-identical.']
(ROOT/'docs/forensic_inventory.md').write_text('\n'.join(lines)+'\n')
