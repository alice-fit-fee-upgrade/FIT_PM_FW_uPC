"""Opt-in two-entry arithmetic candidate, retaining every original body byte."""
import subprocess
SITES=[(0x2130,'.c_002130','pm_scaling_signed_entry'),(0x214c,'.c_00214c','pm_scaling_unsigned_entry')]
def apply(elf,golden,names,build):
 raw=build/'scaling-unpatched.bin'
 subprocess.run(['avr-objcopy','-O','binary',str(elf),str(raw)],check=True)
 data=raw.read_bytes()
 for i,(address,section,target) in enumerate(SITES):
  path=build/(section+'.bin')
  subprocess.run(['avr-objcopy','--dump-section',section+'='+str(path),str(elf)],check=True)
  body=path.read_bytes()
  assert body==golden[address:address+len(body)]
  assert body[:4]==bytes.fromhex('1127249f'), 'fallback prefix no longer matches'
  offset=names['pm_scaling_patch_table']+i*4
  path.write_bytes(data[offset:offset+4]+body[4:])
  subprocess.run(['avr-objcopy','--update-section',section+'='+str(path),str(elf)],check=True)
