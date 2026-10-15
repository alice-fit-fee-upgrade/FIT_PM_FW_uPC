"""Explicit four-byte instrumentation gates; no baseline source edits."""
import subprocess,re
SITES=[(0x106a,'pm_pg_init_hook'),(0x662,'pm_pg_start_hook'),(0xac,'pm_pg_irq_hook'),(0x68e,'pm_pg_timeout_hook'),(0x5e6,'pm_pg_thermal_hook'),(0xad4,'pm_pg_alarm_hook'),(0xbcc,'pm_pg_input_loss_hook'),(0xc0c,'pm_pg_retry_hook'),(0x1446,'pm_pg_manual_hook'),(0x1550,'pm_pg_restart_hook'),(0x1c3e,'pm_pg_reset_hook')]
def apply(elf,golden,names,build):
 sections=[]
 for line in subprocess.check_output(['avr-objdump','-h',str(elf)],text=True).splitlines():
  m=re.match(r'\s*\d+\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)',line)
  if m:sections.append((m[1],int(m[3],16),int(m[2],16)))
 raw=build/'pg-unpatched.bin';subprocess.run(['avr-objcopy','-O','binary',str(elf),str(raw)],check=True);data=raw.read_bytes()
 for i,(address,target) in enumerate(SITES):
  if address==0xac:assert golden[address+2:address+4]==b'\xff\xff'
  else:assert golden[address:address+4] in [bytes.fromhex('00930008'),bytes.fromhex('00938506'),bytes.fromhex('00938606')]
  name,start,size=next(x for x in sections if x[1]<=address<x[1]+x[2]);path=build/'pg-section.bin'
  subprocess.run(['avr-objcopy','--dump-section',name+'='+str(path),str(elf)],check=True)
  current=bytearray(path.read_bytes());assert current[address-start:address-start+4]==golden[address:address+4]
  offset=names['pm_pg_patch_table']+4*i;current[address-start:address-start+4]=data[offset:offset+4];path.write_bytes(current)
  subprocess.run(['avr-objcopy','--update-section',name+'='+str(path),str(elf)],check=True)
