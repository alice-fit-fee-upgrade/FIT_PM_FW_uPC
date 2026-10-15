#!/usr/bin/env python3
"""Bounded console regression. No firmware/config/EEPROM programming.
FPGATEST writes only the identified diagnostic scratch register and restores it.
Configuration reset/boot and legacy persistent/power commands are excluded.
"""
import argparse,fcntl,json,time
from pathlib import Path
PORT='/dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller-if00-port0'
COMMANDS=['@PING','@HELP','@HW','@PGTIME','RS','@TEMP','@TDC 0 83','@TDC 1 83','@TDC 2 83','@PLL','@CFG','@FPGA','@FPGATEST','@FLASHID','@FLASHREAD 000000 10',
          '@PING X','@TEMP X','@TDC 3 83','@TDC 0 GG','@PLL X','@FPGA X','@FPGATEST X','@CFG INVALID','@FLASHID X','@FLASHREAD 000000 11','@UNKNOWN',
          'RA','RC','RF','RT','RZ','RS','@PING']
def capture(s,command,limit=5):
 s.write((command+'\r').encode());s.flush();data=bytearray();start=last=time.monotonic()
 while time.monotonic()-start<limit:
  block=s.read(512)
  if block:data.extend(block);last=time.monotonic()
  elif data and time.monotonic()-last>.18:break
  if len(data)>20000:raise RuntimeError('unbounded response')
 return {'command':command,'reply':data.decode('ascii',errors='replace'),'duration_s':round(time.monotonic()-start,4)}
def main():
 p=argparse.ArgumentParser();p.add_argument('--out',required=True,type=Path);p.add_argument('--new',action='store_true');args=p.parse_args()
 assert not args.out.exists(),'do not overwrite evidence';args.out.parent.mkdir(parents=True,exist_ok=True)
 import serial
 records=[]
 report={'port':PORT,'commands':records,'excluded':['PA/PF programming streams','PC resets PLL defaults','WR EEPROM commit','ON/OF power transitions','CA/CP clear state','S* parameter/calibration changes','CFG HOLD/BOOT destroys FPGA SRAM; separate controlled restoration test'], 'scope':'read-only legacy commands, all preexisting diagnostic command names, bounded scratch test, invalid arguments; NOT all legacy state-changing paths'}
 with open('/home/codex-hil/.cache/pm12-210249A73E43.lock','a+') as lock:
  fcntl.flock(lock,fcntl.LOCK_EX|fcntl.LOCK_NB)
  s=serial.Serial(port=None,baudrate=115200,timeout=.03,write_timeout=1,rtscts=True,exclusive=True);s.dtr=False;s.rts=True;s.port=PORT;s.open()
  try:
   report['initial_drain']=s.read(4096).decode('ascii',errors='replace')
   for command in COMMANDS+(['@I2C','@INA 4','@INA 5','@INA 6','@POWER','@I2C','RS','@TEMP','@FPGATEST','@PING'] if args.new else []):
    row=capture(s,command);records.append(row);print(json.dumps(row),flush=True)
    if command=='@PING':assert row['reply']=='PONG\r\n',row
    if command=='RS':assert 'Board S/N: 1803' in row['reply'] and 'NORMAL' in row['reply'] and 'Board power ON, OK' in row['reply'],row
    if command=='@FPGATEST':assert row['reply']=='FPGA TEST PASS 32 RESTORED\r\n',row
   report['bounded_console_sequence']='PASS';report['cleanup']='UART closed,RTS/DTR deasserted,lease released'
  except Exception as e:report['error']=repr(e);raise
  finally:
   s.rts=False;s.dtr=False;s.close();args.out.write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
