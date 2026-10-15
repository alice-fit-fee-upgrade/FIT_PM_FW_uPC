#include "hw_diag.h"
#include "extension.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint8_t mem[0x2500],irq,wire[32],n,fail,rx,selected;
static uint16_t registers[256];
static unsigned writes,polls,scratch_writes;
static char output[512];static unsigned used;static const char *input;
uint8_t pm_console_getc(void){assert(*input);return *input++;}
void pm_console_putc(uint8_t c){assert(used+1<sizeof output);output[used++]=c;output[used]=0;}
uint8_t pm_irq_enter(void){uint8_t s=irq;irq=0;return s;}
void pm_irq_leave(uint8_t s){assert(!irq);irq=s;}
uint8_t pm_read8(uint16_t a){assert(!irq);if(a==0x8c2||a==0xac2){++polls;return fail&&n>=fail?0:128;}if(a==0x8c3||a==0xac3)return rx;return mem[a];}
void pm_write8(uint16_t a,uint8_t v){
 assert(!irq);++writes;
 if(a==0x8c0||a==0xac0){mem[a]=v;return;}
 if(a==0x8c3||a==0xac3){assert(selected && n<sizeof wire);wire[n++]=v;rx=0;
  if(a==0x8c3 && n>=3){uint8_t addr=((wire[0]&0x7f)<<2)|(wire[1]>>6);assert(addr>=0x40&&addr<=0x44);
   if(wire[0]&128)rx=n==3?registers[addr]>>8:registers[addr];
   else if(n==4 && !(fail&&n>=fail)){assert(addr==0x40);registers[addr]=((uint16_t)wire[2]<<8)|v;++scratch_writes;}
  }else if(a==0xac3){assert(wire[0]==0x9f||wire[0]==3);rx=0xa0+n;}
  return;}
 uint16_t port=a&0xffe0;uint8_t off=a-port;
 assert((port==0x660 && !(v&~3)) || (port==0x680 && !(v&~0xb0)));
 assert(off==1||off==2||off==5||off==6);
 uint16_t target=port+(off<4?0:4);
 if(off==1||off==5)mem[target]|=v;else mem[target]&=~v;
 if((a==0x666 && (v&1))||(a==0x686 && (v&16))){assert(!selected);selected=1;n=0;}
 if((a==0x665 && (v&1))||(a==0x685 && (v&16)))selected=0;
}
static void init(uint8_t s){memset(mem,0,sizeof mem);irq=s;fail=n=selected=0;writes=polls=scratch_writes=0;
 mem[0x684]=4|16;mem[0x688]=10;mem[0x660]=3;mem[0x664]=3;mem[0x668]=12;mem[0x644]=7;mem[0x6a4]=16;mem[0x8c0]=0xf1;
 registers[0x40]=0xbeef;registers[0x41]=0x504d;registers[0x42]=0x5350;
}
static void command(const char *in,const char *out){input=in;used=0;output[0]=0;pm_extension_dispatch();assert(!*input);assert(!strcmp(output,out));}
void pm_i2c_delay(void) {}
int main(void){for(uint8_t s=0;s<2;++s){uint16_t word;uint8_t data[16];
 init(s);word=0;assert(!pm_diag_fpga(0,0x40,&word)&&word==0xbeef&&wire[0]==0x90&&wire[1]==0&&n==4&&mem[0x8c0]==0xf1&&mem[0x664]==3&&irq==s);
 init(s);word=0x1234;assert(!pm_diag_fpga(1,0x40,&word)&&registers[0x40]==0x1234&&wire[0]==0x10&&wire[2]==0x12&&wire[3]==0x34);
 for(uint8_t f=1;f<=4;++f){init(s);fail=f;word=0xcafe;assert(pm_diag_fpga(0,0x40,&word)==PM_DIAG_TIMEOUT&&word==0xcafe&&irq==s&&mem[0x664]==3&&mem[0x8c0]==0xf1&&polls==127u+f);}
 init(s);mem[0x644]=6;assert(pm_diag_fpga(0,0x40,&word)==PM_DIAG_BUSY&&!writes);
 init(s);assert(pm_diag_flash(1,0,data,3)==PM_DIAG_CONFIG&&!writes);
 init(s);assert(!pm_diag_config(1)&&mem[0x664]==1&&mem[0x684]==20&&mem[0x680]==0xb0&&mem[0xac0]==0x50&&irq==s);
 mem[0x668]=0;assert(!pm_diag_flash(1,0,data,3)&&wire[0]==0x9f&&n==4&&data[0]==0xa2&&data[2]==0xa4&&mem[0x684]==20);
 assert(!pm_diag_flash(0,0x123456,data,16)&&n==20&&wire[0]==3&&wire[1]==0x12&&wire[2]==0x34&&wire[3]==0x56);
 for(uint8_t f=1;f<=20;++f){fail=f;assert(pm_diag_flash(0,0x123456,data,16)==PM_DIAG_TIMEOUT&&!selected&&mem[0x684]==20&&irq==s);}
 fail=0;assert(!pm_diag_config(0)&&mem[0x664]==3&&mem[0x680]==0&&mem[0xac0]==0&&mem[0x684]==20&&irq==s);
 init(s);mem[0x688]=0;assert(pm_diag_config(1)==PM_DIAG_POWER&&!writes);
 init(s);mem[0x660]=1;assert(pm_diag_config(1)==PM_DIAG_CONFIG&&!writes);
 init(s);command("FPGATEST\r","FPGA TEST PASS 32 RESTORED\r\n");assert(registers[0x40]==0xbeef&&scratch_writes==33&&irq==s);
 init(s);registers[0x41]=0;command("FPGATEST\r","ERR FPGA IMAGE\r\n");assert(!scratch_writes);
 }
 puts("PASS FPGA headers, walking bits/restoration/image guard; flash read-only opcodes, ownership, reset/release, all byte timeouts and PSU-preserving GPIO whitelist");
}
