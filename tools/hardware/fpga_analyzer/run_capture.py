#!/usr/bin/env python3
"""Bounded, leased SRAM-only passive capture; optionally retain the validated analyzer; restore on failure."""
import argparse,csv,fcntl,hashlib,json,subprocess,sys,time
from pathlib import Path
HERE=Path(__file__).resolve().parent
sys.path.insert(0,'/home/codex-hil/fit-testing-publish/diagnostics/pm12')
from test_board import payload
sys.path.insert(0,str(HERE.parent))
from pm12_console_regression import PORT,capture
from pm12_power import power_good,wait_power_good,require_power_good
import serial
OLD=Path('/home/codex-hil/fit-bringup/evidence/pm12-fw-validation-2026-10-15')
DOCKER=['docker','-H','unix:///run/user/1001/docker-vivado.sock','exec','-e','LD_PRELOAD=/lib/x86_64-linux-gnu/libudev.so.1','9801db3654e2','timeout','130','/srv/codex-hil-data/toolchains/amd/Xilinx/2025.2/Vivado/bin/vivado','-mode','batch','-nolog','-nojournal','-notrace','-source']
def main():
 p=argparse.ArgumentParser();p.add_argument('--out',type=Path,required=True);p.add_argument('--leave-loaded',action='store_true',help='retain analyzer SRAM image only after capture and console validation pass');a=p.parse_args();leave_requested=a.leave_loaded;out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
 locks=[]
 for name in sorted(['atmel-ice-J41800043065','pm12-210249A73E43']):
  f=open('/home/codex-hil/.cache/'+name+'.lock','a+');fcntl.flock(f,fcntl.LOCK_EX|fcntl.LOCK_NB);locks.append(f)
 bit=HERE/'build/pm12_analyzer.bit';bits=payload(bit,out/'analyzer.hex')
 status={'fpga_sha256':hashlib.sha256(bit.read_bytes()).hexdigest(),'mcu_flash_writes':False,'persistent_writes':False,'input_only':['SDA','SCL','RESET_N'],'source_sha256':{n:hashlib.sha256((HERE/n).read_bytes()).hexdigest() for n in ['pm12_analyzer.v','pm12_mcu_spi.vhd','eth_jtag.v','pins.xdc','build.tcl']}}
 def run(tcl,vec,n,log):
  with log.open('w') as f:subprocess.run(DOCKER+[str(tcl),'-tclargs','210249A73E43','localhost:3121',str(vec),str(n)],stdout=f,stderr=subprocess.STDOUT,timeout=145,check=True)
 def console(commands, settled=True):
  u=serial.Serial(port=None,baudrate=115200,timeout=.03,write_timeout=1,rtscts=True,exclusive=True);u.dtr=False;u.rts=True;u.port=PORT;u.open()
  try:
   u.read(4096)
   if settled:
    status.setdefault('power_ready_checks',[]).append(wait_power_good(u,capture))
   else:
    row=capture(u,'@HW');status.setdefault('power_rechecks',[]).append(row)
    assert power_good(row['reply']),'PGOOD dropped before capture sensor access'
   rows=[]
   for c in commands:
    if c.startswith(('@TEMP','@INA','@POWER','@FPGATEST')):
     status.setdefault('sensor_power_rechecks',[]).append(require_power_good(u,capture))
    rows.append(capture(u,c))
   return rows
  finally:u.rts=False;u.dtr=False;u.close()
 base=(OLD/'load_passive_fpga.tcl').read_text()
 load=base.replace('!= 0x00000000','!= 0x504d5a02').replace('!= 0x504d5a02} {error "Loaded','!= 0x504d5a03} {error "Loaded')
 restore=base.replace('!= 0x00000000','!= 0x504d5a03')
 # Permit old image on rollback if loading was rejected before configuration.
 restore=restore.replace('if {[expr {("0x$baseline") & 0xffffffff}] != 0x504d5a03}', 'if {[expr {("0x$baseline") & 0xffffffff}] == 0x504d5a02} {puts "LOAD_PASS already restored"; pm12_close; exit}\nif {[expr {("0x$baseline") & 0xffffffff}] != 0x504d5a03 && [expr {("0x$baseline") & 0xffffffff}] != 0}')
 load=load.replace('if {[expr {("0x$baseline") & 0xffffffff}] != 0x504d5a02}', 'if {[expr {("0x$baseline") & 0xffffffff}] == 0x504d5a03} {puts "LOAD_PASS analyzer already present"; pm12_close; exit}\nif {[expr {("0x$baseline") & 0xffffffff}] != 0x504d5a02}')
 (out/'load.tcl').write_text(load);(out/'restore.tcl').write_text(restore)
 attempted=False;keep_verified=False
 try:
  status['before']=console(['@PING','@TEMP','@HW','RS'])
  assert 'REG=03 C3' in status['before'][1]['reply'],status['before']
  attempted=True;print('LOAD_ANALYZER',flush=True)
  run(out/'load.tcl',out/'analyzer.hex',bits,out/'load.log')
  assert 'LOAD_PASS' in (out/'load.log').read_text()
  console([]) # Stable physical PGOOD before arming the bounded capture window.
  with (out/'capture.log').open('w') as log:
   proc=subprocess.Popen(DOCKER+[str(HERE/'capture.tcl'),'-tclargs','210249A73E43','localhost:3121',str(out/'armed'),str(bits)],stdout=log,stderr=subprocess.STDOUT)
   try:
    deadline=time.monotonic()+30
    while not (out/'armed').exists():
     if proc.poll() is not None or time.monotonic()>deadline:raise RuntimeError('capture did not arm')
     time.sleep(.05)
    status['during']=console(['@TEMP','RS'],settled=False)
    proc.wait(timeout=90);assert proc.returncode==0
   finally:
    if proc.poll() is None:proc.terminate();proc.wait(timeout=5)
  text=(out/'capture.log').read_text();assert 'CAPTURE_PASS' in text
  import re
  rate=float(re.search(r'^CLOCK (.+)$',text,re.M)[1]);status['measured_sample_hz']=rate
  rows=[tuple(map(int,m)) for m in re.findall(r'^EVENT (\d+) (\d+) (\d+)$',text,re.M)]
  assert rows and all(b[1]>a[1] for a,b in zip(rows,rows[1:]))
  with (out/'events.csv').open('w') as f:
   w=csv.writer(f);w.writerow(['event','cycles','seconds','SDA','SCL','RESET_N'])
   for i,t,v in rows:w.writerow([i,t,t/rate,v&1,(v>>1)&1,(v>>2)&1])
  with (out/'capture.vcd').open('w') as f:
   f.write('$timescale 1ns $end\n$scope module pm12 $end\n$var wire 1 ! SDA $end\n$var wire 1 " SCL $end\n$var wire 1 # RESET_N $end\n$upscope $end\n$enddefinitions $end\n')
   for i,t,v in rows:
    f.write(f'#{round(t/rate*1e9)}\n')
    for n,s in enumerate('!"#'):f.write(f'{(v>>n)&1}{s}\n')
  widths={'HIGH':[],'LOW':[]};edges=[]
  for a,b in zip(rows,rows[1:]):
   if (a[2]^b[2])&2:edges.append(b[1])
  # Consecutive events may be SDA/RESET changes; compute widths only between
  # actual SCL transitions, preserving the state after the preceding edge.
  scl_edges=[(rows[0][1],(rows[0][2]>>1)&1)]
  for a,b in zip(rows,rows[1:]):
   if (a[2]^b[2])&2:scl_edges.append((b[1],(b[2]>>1)&1))
  for a,b in zip(scl_edges,scl_edges[1:]):
   widths['HIGH' if a[1] else 'LOW'].append((b[0]-a[0])/rate*1e9)
  import statistics
  status['scl']={'transitions':len(edges),'widths_ns':{k:{'min':min(v),'median':statistics.median(v),'max':max(v)} for k,v in widths.items() if v},'sda_changes':sum(bool((a[2]^b[2])&1) for a,b in zip(rows,rows[1:])),'reset_changes':sum(bool((a[2]^b[2])&4) for a,b in zip(rows,rows[1:]))}
  status['capture']='PASS';status['events']=len(rows)
  if leave_requested:
   status['retained_console']=console(['@PING','@TEMP','@HW','RS','@FPGATEST'])
   replies={r['command']:r['reply'] for r in status['retained_console']}
   assert replies['@PING']=='PONG\r\n' and 'REG=03 C3' in replies['@TEMP'] and ' NORMAL' in replies['RS'] and 'Board power ON, OK' in replies['RS'] and 'FPGA TEST PASS 32 RESTORED' in replies['@FPGATEST'],replies
   keep_verified=True;status['analyzer_left_loaded']=True
   status['capture_state']='stopped; trace retained in BRAM, ready for explicit rearm'
   print('ANALYZER_LEFT_LOADED',flush=True)
 except Exception as e:status['error']=repr(e);raise
 finally:
  try:
   if attempted and (not keep_verified or 'error' in status):
    print('RESTORE_PASSIVE',flush=True)
    run(out/'restore.tcl',OLD/'passive-fpga-payload.hex',int((OLD/'passive-fpga-payload-bits.txt').read_text()),out/'restore.log')
    assert 'LOAD_PASS' in (out/'restore.log').read_text();status['restored']=True;status['analyzer_left_loaded']=False
    status['after']=console(['@PING','@TEMP','@HW','RS','@FPGATEST'])
    assert 'REG=03 C3' in status['after'][1]['reply'] and 'Board power ON, OK' in status['after'][3]['reply'] and 'FPGA TEST PASS' in status['after'][4]['reply']
  except Exception as e:status['restore_error']=repr(e);raise
  finally:(out/'report.json').write_text(json.dumps(status,indent=2)+'\n')
 print(json.dumps(status,indent=2),flush=True)
if __name__=='__main__':main()
