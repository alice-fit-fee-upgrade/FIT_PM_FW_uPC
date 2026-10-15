#include "extension.h"
#include "hw_diag.h"
static const PM_FLASH char earg[]="ERR ARGUMENTS\r\n";
static const PM_FLASH char off[]="ERR POWER\r\n",busy[]="ERR BUSY\r\n",timeout[]="ERR TIMEOUT\r\n";
static const PM_FLASH char config_error[]="ERR CONFIG OWNERSHIP\r\n";
static void hex(uint32_t v,uint8_t digits) { while(digits){uint8_t n=(v>>(--digits*4))&15;pm_console_putc(n<10?'0'+n:'A'+n-10);} }
static void line(void){pm_console_putc('\r');pm_console_putc('\n');}
static uint8_t result(uint8_t s){if(s==PM_DIAG_OK)return 1;pm_console_write(s==PM_DIAG_POWER?off:s==PM_DIAG_BUSY?busy:s==PM_DIAG_TIMEOUT?timeout:s==PM_DIAG_CONFIG?config_error:earg);return 0;}
void pm_command_hw(const char *args)
{
 if(*args){pm_console_write(earg);return;}
 uint8_t saved=pm_irq_enter();uint8_t ports[6],state[12];
 for(uint8_t n=0;n<6;++n)ports[n]=pm_read8(0x608u+n*32u);
 for(uint8_t n=0;n<12;++n)state[n]=pm_read8(0x2157u+n);
 pm_irq_leave(saved);
 static const PM_FLASH char banner[]="HW V2 PORTS A..F=";pm_console_write(banner);
 for(uint8_t n=0;n<6;++n)hex(ports[n],2);
 static const PM_FLASH char label[]=" STATE 2157..2162=";pm_console_write(label);
 for(uint8_t n=0;n<12;++n)hex(state[n],2);
 line();
}
void pm_command_temp(const char *args)
{
 uint8_t count=4;
 if(*args){
  if(args[0]=='A' && args[1]=='L' && args[2]=='L' && !args[3])count=8;
  else{pm_console_write(earg);return;}
 }
 /* Read-only register extension; retain the original native byte timing. */
 static const PM_FLASH char label[]="ADT7311 REG=";
 for(uint8_t reg=0;reg<count;++reg){uint16_t v=0;if(!result(pm_diag_temp(reg,&v)))return;pm_console_write(label);hex(reg,2);pm_console_putc(' ');hex(v,(reg==2||reg==4||reg==6||reg==7)?4:2);line();}
}
void pm_command_tdc(const char *args)
{
 /* Chip decimal 0..2, followed by exactly two uppercase hex address digits. */
 if(args[0]<'0'||args[0]>'2'||args[1]!=' '||!args[2]||!args[3]||args[4]){pm_console_write(earg);return;}
 uint8_t address=0;
 for(uint8_t n=2;n<4;++n){uint8_t c=args[n],v;if(c>='0'&&c<='9')v=c-'0';else if(c>='A'&&c<='F')v=c-'A'+10;else{pm_console_write(earg);return;}address=(address<<4)|v;}
 uint16_t v=0;if(!result(pm_diag_tdc(args[0]-'0',address,&v)))return;
 static const PM_FLASH char label[]="THS788 ";pm_console_write(label);pm_console_putc(args[0]);pm_console_putc(' ');hex(address,2);pm_console_putc(' ');hex(v,4);line();
}
void pm_command_pll(const char *args)
{
 if(*args){pm_console_write(earg);return;}uint32_t v=0;if(!result(pm_diag_pll(&v)))return;
 static const PM_FLASH char label[]="CDCE62005 ";pm_console_write(label);hex(v,8);line();
}

void pm_command_pgtime(const char *args)
{
 if(*args){pm_console_write(earg);return;}
 uint8_t irq=pm_irq_enter();
 uint8_t state=pm_read8(0x0a2a),overflow=pm_read8(0x0a2b);
 uint16_t ticks=pm_read8(0x0a28);ticks|=(uint16_t)pm_read8(0x0a29)<<8;
 if(state==1){ticks=pm_read8(0x0a20);ticks|=(uint16_t)pm_read8(0x0a21)<<8;}
 pm_irq_leave(irq);
 static const PM_FLASH char label[]="PGTIME STATE=";pm_console_write(label);hex(state,2);
 static const PM_FLASH char t[]=" TICKS=";pm_console_write(t);hex(ticks,4);
 static const PM_FLASH char o[]=" OVF=";pm_console_write(o);hex(overflow,2);
 static const PM_FLASH char u[]=" US_NOMINAL=";pm_console_write(u);hex((uint32_t)ticks*32u,8);line();
}

void pm_command_fpga(const char *args)
{
 if(*args){pm_console_write(earg);return;}
 static const PM_FLASH char label[]="FPGA REG=";
 for(uint8_t a=0x40;a<=0x44;++a){uint16_t v=0;if(!result(pm_diag_fpga(0,a,&v)))return;pm_console_write(label);hex(a,2);pm_console_putc(' ');hex(v,4);line();}
}
void pm_command_fpgatest(const char *args)
{
 if(*args){pm_console_write(earg);return;}
 uint16_t v=0,original=0;
 if(!result(pm_diag_fpga(0,0x41,&v)))return;
 if(v!=0x504d)goto wrong_image;
 if(!result(pm_diag_fpga(0,0x42,&v)))return;
 if(v!=0x5350)goto wrong_image;
 if(!result(pm_diag_fpga(0,0x40,&original)))return;
 uint8_t failure=0;
 for(uint8_t n=0;n<32;++n){uint16_t pattern=(uint16_t)1<<(n&15);if(n>=16)pattern=(uint16_t)~pattern;v=pattern;
  uint8_t status=pm_diag_fpga(1,0x40,&v);if(!status)status=pm_diag_fpga(0,0x40,&v);
  if(status || v!=pattern){failure=1;break;}}
 v=original;if(pm_diag_fpga(1,0x40,&v) || pm_diag_fpga(0,0x40,&v) || v!=original)failure=1;
 {static const PM_FLASH char good[]="FPGA TEST PASS 32 RESTORED\r\n",bad[]="ERR FPGA TEST OR RESTORE\r\n";pm_console_write(failure?bad:good);}return;
wrong_image:
 {static const PM_FLASH char message[]="ERR FPGA IMAGE\r\n";pm_console_write(message);}
}
void pm_command_cfg(const char *args)
{
 if(!args[0]){uint8_t irq=pm_irq_enter(),in=pm_read8(0x668),out=pm_read8(0x664);pm_irq_leave(irq);
  static const PM_FLASH char message[]="CFG PORTD IN=";pm_console_write(message);hex(in,2);pm_console_putc(' ');hex(out,2);line();return;}
 if(args[0]=='H'&&args[1]=='O'&&args[2]=='L'&&args[3]=='D'&&!args[4]){if(result(pm_diag_config(1))){static const PM_FLASH char message[]="CFG HELD FPGA SRAM LOST\r\n";pm_console_write(message);}return;}
 if(args[0]=='B'&&args[1]=='O'&&args[2]=='O'&&args[3]=='T'&&!args[4]){if(result(pm_diag_config(0))){static const PM_FLASH char message[]="CFG BOOT REQUESTED CHECK INIT DONE\r\n";pm_console_write(message);}return;}
 pm_console_write(earg);
}
void pm_command_flashid(const char *args)
{
 if(*args){pm_console_write(earg);return;}uint8_t data[3];if(!result(pm_diag_flash(1,0,data,3)))return;
 static const PM_FLASH char message[]="FLASH JEDEC=";pm_console_write(message);for(uint8_t n=0;n<3;++n)hex(data[n],2);line();
}
void pm_command_flashread(const char *args)
{
 uint32_t address=0;uint8_t count=0,data[16];
 for(uint8_t n=0;n<9;++n){uint8_t c=args[n],v;
  if(n==6){if(c!=' ')goto invalid;continue;}
  if(c>='0'&&c<='9')v=c-'0';else if(c>='A'&&c<='F')v=c-'A'+10;else goto invalid;
  if(n<6)address=(address<<4)|v;else count=(count<<4)|v;
 }
 if(args[9] || !count || count>16)goto invalid;
 if(!result(pm_diag_flash(0,address,data,count)))return;
 {static const PM_FLASH char message[]="FLASH DATA=";pm_console_write(message);for(uint8_t n=0;n<count;++n)hex(data[n],2);line();}return;
invalid:pm_console_write(earg);
}
