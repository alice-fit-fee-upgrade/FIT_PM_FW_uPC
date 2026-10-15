#!/usr/bin/env python3
"""Explicit PM12 application update with protected-memory verify and exact rollback.
Requires user-authorized programming, existing physical backup, PM12+ICE leases.
No chip erase, EEPROM/fuse/boot/configuration-flash writes.
"""
import argparse,fcntl,hashlib,json,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
MEM=['flash','boot','eeprom','fuse0','fuse1','fuse2','fuse4','fuse5','lock','usersig','prodsig','signature']
BASE=['avrdude','-c','atmelice_pdi','-p','x128a3u','-P','usb:J41800043065','-B','10','-u']
def main():
 p=argparse.ArgumentParser();p.add_argument('--evidence',required=True,type=Path);p.add_argument('--hex',required=True,type=Path);p.add_argument('--bin',required=True,type=Path);p.add_argument('--backup',required=True,type=Path);args=p.parse_args()
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
  run(BASE+['-U','application:w:'+str(args.hex)+':i']+reads(out),out/'program-and-read.log')
  report['protected_regions_unchanged']=verify(out,expected);report['programming']='PASS full FLASH/readback and protected regions'
  # Isolate sensor behavior BEFORE any I2C transaction or FPGA reload.
  import serial
  sys.path.insert(0,str(ROOT/'tools/hardware'));from pm12_console_regression import PORT,capture
  time.sleep(8)
  uart=serial.Serial(port=None,baudrate=115200,timeout=.03,write_timeout=1,rtscts=True,exclusive=True);uart.dtr=False;uart.rts=True;uart.port=PORT;uart.open()
  try:
   report['pre_i2c_sensor']=[]
   for command in ['@PING','@HW','RS','@I2C','@I2C TEST','@TEMP']:
    row=capture(uart,command);report['pre_i2c_sensor'].append(row);print('PRE_I2C '+json.dumps(row),flush=True)
   sensor=next(r['reply'] for r in report['pre_i2c_sensor'] if r['command']=='@TEMP')
   assert 'ADT7311 REG=03 C3\r\n' in sensor,'ADT7311 ID failed before I2C/FPGA reload'
  finally:uart.rts=False;uart.close()
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
   for command in ['@PING','@HELP','RS','@TEMP','@I2C','@INA 4','@INA 5','@INA 6','@I2C','RS','@TEMP','@FPGATEST']:
    row=capture(s,command);report['commands'].append(row);print(json.dumps(row),flush=True)
    assert row['reply'],row
    if command=='@PING':assert row['reply']=='PONG\r\n'
    if command=='@TEMP':assert 'ADT7311 REG=03 C3\r\n' in row['reply'],row
    if command.startswith('@INA'):assert 'CONFIG=7127' in row['reply'] and 'ERR ' not in row['reply'],row
    if command=='@FPGATEST':assert row['reply']=='FPGA TEST PASS 32 RESTORED\r\n',row
    if command=='RS':assert 'Board power ON, OK' in row['reply'] and ' NORMAL' in row['reply'],row
  finally:s.rts=False;s.dtr=False;s.close()
  report['basic_validation']='PASS';report['candidate_left_running']=True
 except Exception as e:
  report['error']=repr(e)
  if attempted:
   dest=out/'rollback';dest.mkdir()
   try:
    conf=Path('/home/codex-hil/fit-testing-publish/diagnostics/pm12/mcu/pdi-application-only.conf')
    run(['avrdude','-C','+'+str(conf),'-c','atmelice_pdi','-p','pm12_full_app','-P','usb:J41800043065','-B','10','-u','-U','app_full:w:'+str(backup/'application-full.bin')+':r']+reads(dest),dest/'rollback.log',180)
    verify(dest,old);report['rollback']='PASS exact pre-update application and protected memories'
    if (out/'restore-passive.tcl').exists():
     run(['docker','-H','unix:///run/user/1001/docker-vivado.sock','exec','9801db3654e2','timeout','130','/srv/codex-hil-data/toolchains/amd/Xilinx/2025.2/Vivado/bin/vivado','-mode','batch','-nolog','-nojournal','-notrace','-source',str(out/'restore-passive.tcl'),'-tclargs','210249A73E43','localhost:3121',str(prior/'passive-fpga-payload.hex'),(prior/'passive-fpga-payload-bits.txt').read_text().strip()],dest/'passive-restore.log',145)
     assert 'LOAD_PASS' in (dest/'passive-restore.log').read_text()
     report['rollback_fpga']='PASS passive SRAM restored'

   except Exception as cleanup:report['rollback_error']=repr(cleanup)
  raise
 finally:(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
