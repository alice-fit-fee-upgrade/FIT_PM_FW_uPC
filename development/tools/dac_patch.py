"""Two opt-in DAC entry gates; original executable bodies remain untouched."""
import subprocess
SITES=[(0x22aa,'.c_0022aa','pm_dev_dac_send_entry',0x230e,'6f937f93'),(0x9de,'.c_0009de','pm_dev_dac_apply_entry',0xa4c,'ff93ef93')]
def apply(elf,golden,names,build):
 raw=build/'dac-unpatched.bin'
 subprocess.run(['avr-objcopy','-O','binary',str(elf),str(raw)],check=True)
 data=raw.read_bytes()
 for i,(address,section,target,end,prefix) in enumerate(SITES):
  path=build/(section+'.bin')
  subprocess.run(['avr-objcopy','--dump-section',section+'='+str(path),str(elf)],check=True)
  body=path.read_bytes();assert body==golden[address:end] and body[:4]==bytes.fromhex(prefix)
  offset=names['pm_dac_patch_table']+4*i
  path.write_bytes(data[offset:offset+4]+body[4:])
  subprocess.run(['avr-objcopy','--update-section',section+'='+str(path),str(elf)],check=True)
