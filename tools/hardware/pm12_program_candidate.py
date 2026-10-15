#!/usr/bin/env python3
"""Explicit PM12 application update with protected-memory verify and exact rollback.
Requires user-authorized programming, existing physical backup, PM12+ICE leases.
No chip erase, EEPROM/fuse/boot/configuration-flash writes.
"""
import argparse,fcntl,hashlib,json,re,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
MEM=['flash','boot','eeprom','fuse0','fuse1','fuse2','fuse4','fuse5','lock','usersig','prodsig','signature']
BASE=['avrdude','-c','atmelice_pdi','-p','x128a3u','-P','usb:J41800043065','-B','10','-u']
def main():
 p=argparse.ArgumentParser();p.add_argument('--evidence',required=True,type=Path);p.add_argument('--hex',required=True,type=Path);p.add_argument('--bin',required=True,type=Path);p.add_argument('--backup',required=True,type=Path);p.add_argument('--sensor-probe',action='store_true',help='capture ADT/flags repeatedly, then restore the pre-update application');p.add_argument('--full-app-write',action='store_true',help='use bounded 128 KiB application alias, including FF pages');p.add_argument('--temperature-registers',action='store_true',help='compare normal/slow readback of all ADT registers before and after I2C');p.add_argument('--expect-original-adt-settings',action='store_true',help='require CONFIG 50, TCRIT 2300, THYST 05, THIGH 1E00, TLOW 0500');p.add_argument('--recover-adt',action='store_true',help='explicitly send original ADT initialization after capturing initial readback');args=p.parse_args()
 assert not args.expect_original_adt_settings or args.temperature_registers
 out=args.evidence;out.mkdir(exist_ok=False);backup=args.backup
 expected=args.bin.read_bytes();old=(backup/'flash-full.bin').read_bytes()
 assert len(old)==len(expected)==0x22000 and old[0x20000:]==expected[0x20000:]
 import sys
 sys.path.insert(0,str(ROOT/'tools'));from memory_image import read_hex
 memory,_=read_hex(args.hex);assert all(a<0x20000 for a in memory)
 assert all(expected[a]==v for a,v in memory.items())
 report={'hex_sha256':hashlib.sha256(args.hex.read_bytes()).hexdigest(),'candidate_full_sha256':hashlib.sha256(expected).hexdigest(),'commands':[],'chip_erase':False}
 locks=[]
 for name in sorted(['pm12-210249A73E43','atmel-ice-J41800043065']):
  f=open('/home/codex-hil/.cache/'+name+'.lock','a+');fcntl.flock(f,fcntl.LOCK_EX|fcntl.LOCK_NB);locks.append(f)
 def run(cmd,path,timeout=120):
  with path.open('w') as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=timeout)
 def reads(dest):return sum((['-U',m+':r:'+str(dest/(m+'.bin'))+':r'] for m in MEM),[])
 def verify(dest,target):
  full=(dest/'flash.bin').read_bytes().ljust(0x22000,b'\xff');assert full==target,'canonical readback mismatch'
  protected={m:(dest/(m+'.bin')).read_bytes()==(backup/(m+'.bin')).read_bytes() for m in MEM[1:]}
  assert all(protected.values()),protected;return protected
 prior=Path('/home/codex-hil/fit-bringup/evidence/pm12-fw-validation-2026-10-15')
 text=(prior/'load_passive_fpga.tcl').read_text()
 text=text.replace('if {[expr {("0x$baseline") & 0xffffffff}] != 0x00000000}', 'if {[expr {("0x$baseline") & 0xffffffff}] == 0x504d5a02} {puts "LOAD_PASS existing passive image"; pm12_close; exit}\nif {[expr {("0x$baseline") & 0xffffffff}] != 0x00000000}')
 (out/'restore-passive.tcl').write_text(text)
 attempted=False
 try:
  run(BASE+['-n','-U','signature:r:'+str(out/'identity.bin')+':r'],out/'identify.log')
  assert (out/'identity.bin').read_bytes()==bytes.fromhex('1e9742')
  attempted=True
  if args.full_app_write:
   application=out/'candidate-application-full.bin';application.write_bytes(expected[:0x20000])
   conf=Path('/home/codex-hil/fit-testing-publish/diagnostics/pm12/mcu/pdi-application-only.conf')
   run(['avrdude','-C','+'+str(conf),'-c','atmelice_pdi','-p','pm12_full_app','-P','usb:J41800043065','-B','10','-u','-U','app_full:w:'+str(application)+':r']+reads(out),out/'program-and-read.log',180)
  else:
   run(BASE+['-U','application:w:'+str(args.hex)+':i']+reads(out),out/'program-and-read.log')
  report['protected_regions_unchanged']=verify(out,expected);report['programming']='PASS full FLASH/readback and protected regions'
  # Isolate sensor behavior BEFORE any I2C transaction or FPGA reload.
  import serial
  sys.path.insert(0,str(ROOT/'tools/hardware'));from pm12_console_regression import PORT,capture
  from pm12_power import wait_power_good,require_power_good

  uart=serial.Serial(port=None,baudrate=115200,timeout=.03,write_timeout=1,rtscts=True,exclusive=True);uart.dtr=False;uart.rts=True;uart.port=PORT;uart.open()
  try:
   report['pre_sensor_power_ready']=wait_power_good(uart,capture)
   report['pre_i2c_sensor']=[]
   if args.recover_adt:
    report['before_adt_recovery']=[]
    for c in ['@HW','@TEMP ALL','@TEMP SLOW']:
     if c.startswith('@TEMP'):report.setdefault('sensor_power_rechecks',[]).append(require_power_good(uart,capture))
     report['before_adt_recovery'].append(capture(uart,c))
    report['recovery_power_recheck']=require_power_good(uart,capture)
    report['adt_recovery']=capture(uart,'@TEMP INIT');print('ADT_RECOVERY '+json.dumps(report['adt_recovery']),flush=True)
    assert report['adt_recovery']['reply']=='ADT7311 INIT SENT VERIFY_READBACK\r\n'
    time.sleep(1)
   pre_commands=['@PING','@HW','RS','@TEMP','@HW','@TEMP','@HW','@TEMP','@HW','RS'] if args.sensor_probe else ['@PING','@HW','RS','@I2C','@I2C TEST','@TEMP']
   if args.temperature_registers:pre_commands+=['@TEMP ALL','@TEMP SLOW','@HW']
   for command in pre_commands:
    if command.startswith(('@TEMP','@INA','@POWER')):
     report.setdefault('sensor_power_rechecks',[]).append(require_power_good(uart,capture))
    row=capture(uart,command);report['pre_i2c_sensor'].append(row);print('PRE_I2C '+json.dumps(row),flush=True)
   sensors=[r['reply'] for r in report['pre_i2c_sensor'] if r['command']=='@TEMP']
   assert all('ADT7311 REG=03 C3\r\n' in sensor for sensor in sensors),'ADT7311 ID failed before I2C/FPGA reload'
   def settings(rows):
    samples=[]
    for command in ['@TEMP ALL','@TEMP SLOW']:
     reply=next(r['reply'] for r in rows if r['command']==command)
     regs={int(k,16):int(v,16) for k,v in re.findall(r'ADT7311 REG=([0-9A-F]{2}) ([0-9A-F]+)',reply)}
     assert set(regs)==set(range(8)) and regs[3]==0xc3,reply
     assert not(regs[2]&7) and 15<=(regs[2] if regs[2]<32768 else regs[2]-65536)/128<=45,reply
     samples.append({str(i):regs[i] for i in [1,3,4,5,6,7]})
    assert samples[0]==samples[1],('normal/slow ADT configuration mismatch',samples)
    return samples[0]
   if args.temperature_registers:
    report['adt_before_i2c']=settings(report['pre_i2c_sensor'])
    if args.expect_original_adt_settings:
     assert report['adt_before_i2c']=={'1':0x50,'3':0xc3,'4':0x2300,'5':5,'6':0x1e00,'7':0x0500},('ADT settings differ from original startup intent',report['adt_before_i2c'])
  finally:uart.rts=False;uart.close()
  if args.sensor_probe:
   report['sensor_probe']='PASS repeated ID capture only; not sensor configuration/feature acceptance; previous firmware restored below'
   return
  # Retain the known passive image if already present, otherwise reload its
  # identical SRAM payload after MCU reset. Full chain identity is in Tcl.
  prior=Path('/home/codex-hil/fit-bringup/evidence/pm12-fw-validation-2026-10-15')
  text=(prior/'load_passive_fpga.tcl').read_text()
  text=text.replace('if {[expr {("0x$baseline") & 0xffffffff}] != 0x00000000}', 'if {[expr {("0x$baseline") & 0xffffffff}] == 0x504d5a02} {puts "LOAD_PASS existing passive image"; pm12_close; exit}\nif {[expr {("0x$baseline") & 0xffffffff}] != 0x00000000}')
  tcl=out/'restore-passive.tcl';tcl.write_text(text)
  run(['docker','-H','unix:///run/user/1001/docker-vivado.sock','exec','9801db3654e2','timeout','130','/srv/codex-hil-data/toolchains/amd/Xilinx/2025.2/Vivado/bin/vivado','-mode','batch','-nolog','-nojournal','-notrace','-source',str(tcl),'-tclargs','210249A73E43','localhost:3121',str(prior/'passive-fpga-payload.hex'),(prior/'passive-fpga-payload-bits.txt').read_text().strip()],out/'passive-restore.log',145)
  assert 'LOAD_PASS' in (out/'passive-restore.log').read_text();report['fpga_passive_restore']='PASS'
  import serial
  sys.path.insert(0,str(ROOT/'tools/hardware'));from pm12_console_regression import PORT,capture
  time.sleep(1)
  s=serial.Serial(port=None,baudrate=115200,timeout=.03,write_timeout=1,rtscts=True,exclusive=True);s.dtr=False;s.rts=True;s.port=PORT;s.open()
  try:
   report['startup_drain']=s.read(4096).decode('ascii',errors='replace')
   report['post_fpga_power_ready']=wait_power_good(s,capture)
   commands=['@PING','@HELP','RS','@TEMP','@I2C','@INA 4','@INA 5','@INA 6','@I2C','RS','@TEMP','@FPGATEST']
   if args.temperature_registers:commands+=['@TEMP ALL','@TEMP SLOW','@HW']
   for command in commands:
    if command.startswith(('@TEMP','@INA','@POWER')):
     report.setdefault('sensor_power_rechecks',[]).append(require_power_good(s,capture))
    row=capture(s,command);report['commands'].append(row);print(json.dumps(row),flush=True)
    assert row['reply'],row
    if command=='@PING':assert row['reply']=='PONG\r\n'
    if command=='@TEMP':assert 'ADT7311 REG=03 C3\r\n' in row['reply'],row
    if command.startswith('@INA'):assert 'CONFIG=7127' in row['reply'] and 'ERR ' not in row['reply'],row
    if command=='@FPGATEST':assert row['reply']=='FPGA TEST PASS 32 RESTORED\r\n',row
    if command=='RS':assert 'Board power ON, OK' in row['reply'] and ' NORMAL' in row['reply'],row
  finally:s.rts=False;s.dtr=False;s.close()
  if args.temperature_registers:
   report['adt_after_i2c']=settings(report['commands'])
   assert report['adt_before_i2c']==report['adt_after_i2c'],'ADT configuration changed across FPGA restore/I2C reads'
  report['basic_validation']='PASS';report['candidate_left_running']=True
 except Exception as e:
  report['error']=repr(e)
  raise
 finally:
  if attempted and (args.sensor_probe or 'error' in report):
   dest=out/'rollback';dest.mkdir()
   try:
    conf=Path('/home/codex-hil/fit-testing-publish/diagnostics/pm12/mcu/pdi-application-only.conf')
    run(['avrdude','-C','+'+str(conf),'-c','atmelice_pdi','-p','pm12_full_app','-P','usb:J41800043065','-B','10','-u','-U','app_full:w:'+str(backup/'application-full.bin')+':r']+reads(dest),dest/'rollback.log',180)
    verify(dest,old);report['rollback']='PASS exact pre-update application and protected memories'
    if (out/'restore-passive.tcl').exists():
     run(['docker','-H','unix:///run/user/1001/docker-vivado.sock','exec','9801db3654e2','timeout','130','/srv/codex-hil-data/toolchains/amd/Xilinx/2025.2/Vivado/bin/vivado','-mode','batch','-nolog','-nojournal','-notrace','-source',str(out/'restore-passive.tcl'),'-tclargs','210249A73E43','localhost:3121',str(prior/'passive-fpga-payload.hex'),(prior/'passive-fpga-payload-bits.txt').read_text().strip()],dest/'passive-restore.log',145)
     assert 'LOAD_PASS' in (dest/'passive-restore.log').read_text()
     report['rollback_fpga']='PASS passive SRAM restored'
    # Verify the restored runtime as well as its memory image.
    import serial
    sys.path.insert(0,str(ROOT/'tools/hardware'));from pm12_console_regression import PORT,capture
    from pm12_power import wait_power_good,require_power_good
    time.sleep(1)
    restored=serial.Serial(port=None,baudrate=115200,timeout=.03,write_timeout=1,rtscts=True,exclusive=True);restored.dtr=False;restored.rts=True;restored.port=PORT;restored.open()
    try:
     restored.read(4096)
     report['rollback_power_ready']=wait_power_good(restored,capture)
     report['rollback_console']=[]
     for c in ['@PING','@TEMP','@HW','RS']:
      if c=='@TEMP':report['rollback_power_recheck']=require_power_good(restored,capture)
      report['rollback_console'].append(capture(restored,c))
     temp=next(r['reply'] for r in report['rollback_console'] if r['command']=='@TEMP')
     status=next(r['reply'] for r in report['rollback_console'] if r['command']=='RS')
     assert 'ADT7311 REG=03 C3\r\n' in temp and 'Board power ON, OK' in status and ' NORMAL' in status,report['rollback_console']
     report['rollback_runtime']='PASS ID C3, normal temperature, PSU ON'
    finally:restored.rts=False;restored.dtr=False;restored.close()

   except Exception as cleanup:report['rollback_error']=repr(cleanup)
  (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
  if 'rollback_error' in report:raise RuntimeError('rollback/restoration failed; see validation.json')
if __name__=='__main__':main()
