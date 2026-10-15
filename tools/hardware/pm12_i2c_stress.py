#!/usr/bin/env python3
"""Bounded PM12 read-only sensor/UART interleaving; no programming or power changes.
A POWER burst is at most 35 RX bytes. BUSY is retried after thermal service;
other errors fail. This is laboratory coverage, not forced-fault validation.
"""
import argparse,fcntl,json,re,time
from pathlib import Path
from pm12_console_regression import PORT,capture

def main():
 p=argparse.ArgumentParser();p.add_argument('--out',type=Path,required=True)
 p.add_argument('--cycles',type=int,default=12);p.add_argument('--burst',type=int,default=5)
 a=p.parse_args();assert 1<=a.cycles<=20 and 1<=a.burst<=5 and not a.out.exists()
 import serial
 report={'scope':'read-only POWER bursts interleaved with ADT/console/ownership reads; no injected physical faults','records':[],'busy_replies':0,'successful_monitor_reads':0};started=time.monotonic()
 def record(s,c):
  assert time.monotonic()-started<120,'overall deadline'
  row=capture(s,c,limit=6);report['records'].append(row);assert row['reply'],row
  return row['reply']
 with open('/home/codex-hil/.cache/pm12-210249A73E43.lock','a+') as lease:
  fcntl.flock(lease,fcntl.LOCK_EX|fcntl.LOCK_NB)
  s=serial.Serial(port=None,baudrate=115200,timeout=.03,write_timeout=1,rtscts=True,exclusive=True);s.dtr=False;s.rts=True;s.port=PORT;s.open()
  try:
   s.read(4096)
   for n in range(a.cycles):
    reply=record(s,'\r'.join(['@POWER']*a.burst));errors=[v for v in reply.splitlines() if v.startswith('ERR ')]
    assert all(v=='ERR I2C BUSY' for v in errors),errors
    report['busy_replies']+=len(errors)
    for line in reply.splitlines():
     if ' ADDR=' in line:
      assert 'CONFIG=7127 MASKS_RESTORED=00/00' in line,line
      report['successful_monitor_reads']+=1
    if errors:time.sleep(1.2)
    reply=record(s,'@TEMP');assert 'ADT7311 REG=03 C3\r\n' in reply,reply
    word=int(re.search(r'REG=02 ([0-9A-F]{4})',reply)[1],16)
    assert not(word&7) and 15<=(word if word<32768 else word-65536)/128<=45,reply
    reply=record(s,'RS');assert ' NORMAL' in reply and 'Board power ON, OK' in reply and 'PLL LOCKED' in reply,reply
    reply=record(s,'@I2C');m=re.search(r'LINES=([0-9A-F]{2}) OWNER=([0-9A-F]{2}) DEFER=([0-9A-F]{2})',reply);assert m,reply
    lines,owner,defer=(int(v,16) for v in m.groups());assert lines==7 and not(owner&0x43),reply
    print(json.dumps({'cycle':n+1,'deferred':defer,'owner':owner,'monitor_reads':report['successful_monitor_reads'],'busy':report['busy_replies']}),flush=True)
   assert report['successful_monitor_reads']>=a.cycles*3
   report['status']='PASS';report['last_defer_counter']=defer
   report['hardware_contention_observed']=defer>0
  except Exception as e:report['status']='FAIL';report['error']=repr(e);raise
  finally:
   s.rts=False;s.dtr=False;s.close();report['duration_s']=round(time.monotonic()-started,3)
   a.out.write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
