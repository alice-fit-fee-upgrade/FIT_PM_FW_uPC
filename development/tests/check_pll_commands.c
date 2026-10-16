/* Stateful register-read model: tests command formatting AND physical-frame intent.
 * This is not an electrical/timing model of the real CDCE62005. */
#include "extension.h"
#include "hw_diag.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static const char *input;
static char output[1024];
static unsigned used,transactions,bytes,reads,selectors;
static uint8_t irq=1,ctrl=0xd5,out=0xf0,selected,read_phase,reg;
static uint32_t command;
static const uint32_t registers[9]={0x12345670,0x89abcde1,0xfedcba92,0x76543213,0xa5a5a5a4,0x5a5a5a55,0x1234abc6,0xcdef7897,0x80009d38};
uint8_t pm_console_getc(void){assert(*input);return (uint8_t)*input++;}
void pm_console_putc(uint8_t v){assert(used+1<sizeof output);output[used++]=(char)v;output[used]=0;}
uint8_t pm_irq_enter(void){uint8_t old=irq;irq=0;return old;}
void pm_irq_leave(uint8_t old){assert(irq==0);irq=old;}
void pm_i2c_delay(void){}
uint8_t pm_read8(uint16_t a)
{
 assert(!irq);
 switch(a){
 case 0x684:return 4;case 0x688:return 10;case 0x644:return 7;case 0x664:return 1;
 case 0x8c0:return ctrl;case 0x6a4:return out;
 case 0x8c2:return 128;
 case 0x8c3:assert(selected && bytes>=1 && bytes<=4);++reads;return read_phase?(uint8_t)(registers[reg]>>((bytes-1)*8)):0;
 default:assert(!"unexpected MMIO read");return 0;
 }
}
void pm_write8(uint16_t a,uint8_t v)
{
 if(a==15)return;
 assert(!irq);
 switch(a){
 case 0x8c0:ctrl=v;return;
 case 0x6a6:assert(v==16 && !selected && (out&16) && ctrl==0xf1);out&=(uint8_t)~16;selected=1;bytes=0;command=0;return;
 case 0x8c3:
  assert(selected && bytes<4);
  if(read_phase)assert(v==0);else command|=(uint32_t)v<<(bytes*8);
  ++bytes;return;
 case 0x6a5:
  assert(v==16);out|=16;
  if(selected){
   assert(bytes==4);
   if(!read_phase){
    assert((command&15)==14 && (command>>8)==0 && (command>>4)<=8);
    reg=(uint8_t)(command>>4);++selectors;
   }
   read_phase^=1;selected=0;++transactions;
  }
  return;
 default:assert(!"unexpected MMIO write");
 }
}
static void check(const char *request,const char *expected,unsigned frames,unsigned selects,unsigned expected_reads)
{
 input=request;used=transactions=selectors=reads=0;output[0]=0;
 pm_extension_dispatch();
 assert(!*input && strcmp(output,expected)==0);
 assert(transactions==frames && selectors==selects && reads==expected_reads);
 assert(!selected && !read_phase && irq==1 && ctrl==0xd5 && out==0xf0);
}
int main(void)
{
 check("PLL\r","CDCE62005 80009D38\r\n",2,1,8);
 check("PLL LEGACY\r","CDCE62005 80009D38\r\n",2,1,4);
 char expected[512];size_t pos=0;
 for(unsigned i=0;i<9;++i)pos+=(size_t)snprintf(expected+pos,sizeof expected-pos,"CDCE62005 REG=%02X %08lX\r\n",i,(unsigned long)registers[i]);
 check("PLL ALL\r",expected,18,9,72);
 check("PLL A\r","ERR ARGUMENTS\r\n",0,0,0);
 check("PLL AL\r","ERR ARGUMENTS\r\n",0,0,0);
 check("PLL ALL X\r","ERR ARGUMENTS\r\n",0,0,0);
 check("PLL LEGACY X\r","ERR ARGUMENTS\r\n",0,0,0);
 check("PLL PC\r","ERR ARGUMENTS\r\n",0,0,0);
 puts("PASS PLL console: legacy response, all9 register values/format, 18LE frames, LSB-first selector bytes, no configuration/EEPROM opcodes, rejected arguments and IRQ/controller restoration");
}
