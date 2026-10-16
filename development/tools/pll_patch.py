"""Single reviewed development entry JMP; retain the original body and symbols."""
import subprocess
ADDRESS=0x24ce
SECTION='.c_0024ce'
TARGET='pm_pll_read_legacy_adapter'
def apply(elf,golden,names,build):
 raw=build/'pll-unpatched.bin'
 subprocess.run(['avr-objcopy','-O','binary',str(elf),str(raw)],check=True)
 data=raw.read_bytes()
 path=build/'pll-section.bin'
 subprocess.run(['avr-objcopy','--dump-section',SECTION+'='+str(path),str(elf)],check=True)
 body=path.read_bytes()
 assert body==golden[ADDRESS:ADDRESS+len(body)]
 assert body[:4]==bytes.fromhex('6f937f93'),'unexpected original read prologue'
 offset=names['pm_pll_patch_table']
 path.write_bytes(data[offset:offset+4]+body[4:])
 subprocess.run(['avr-objcopy','--update-section',SECTION+'='+str(path),str(elf)],check=True)
