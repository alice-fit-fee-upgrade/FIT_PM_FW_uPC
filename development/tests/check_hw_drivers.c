#include "hw_diag.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint8_t mem[0x2500],irq,timeout,spi_count,bit_count,tx[12],tx_count;
static unsigned polls,writes,steps;
static uint8_t pll_tx[8],pll_frames;
static uint32_t temp_stream;
static uint8_t temp_tx[8],temp_tx_bits;
uint8_t pm_irq_enter(void){uint8_t s=irq;irq=0;return s;}
void pm_irq_leave(uint8_t s){assert(irq==0);irq=s;}
uint8_t pm_read8(uint16_t a)
{
 assert(!irq);++steps;
 if(a==0x8c2){++polls;return (timeout && spi_count>=timeout)?0:128;}
 if(a==0x8c3)return spi_count>4?(uint8_t)(0x11*(spi_count-4)):0;
 if(a==0x608){uint8_t bit=(temp_stream>>(31-(bit_count++&31)))&1;return (mem[a]&~4)|(bit<<2);}
 if(a==0x628 && !(mem[0x620]&2) && (mem[0x624]&1)){
  uint8_t bit=(0xa55au>>(15-bit_count++))&1;return 64|(bit<<1);
 }
 return mem[a];
}
void pm_write8(uint16_t a,uint8_t v)
{
 assert(!irq);++steps;
 if(a==15){mem[a]=v;return;}
 ++writes;
 if(a==0x605 && (v&2) && !(mem[0x604]&2) && !(mem[0x604]&16) && temp_tx_bits<64){
  uint8_t byte=temp_tx_bits/8;temp_tx[byte]=(uint8_t)((temp_tx[byte]<<1)|!!(mem[0x604]&8));++temp_tx_bits;
 }
 if(a==0x8c0){mem[a]=v;return;}
 if(a==0x8c3){assert(spi_count<8);pll_tx[spi_count]=v;++spi_count;mem[a]=v;return;}
 if(a==0x6a6 && (v&16)){assert(mem[0x8c0]==0xf1);++pll_frames;}
 uint16_t port=a&0xffe0;uint8_t off=a-port;
 assert((port==0x600 && !(v&~0x1a)) || (port==0x620 && !(v&~0x1f)) || (port==0x6a0 && !(v&~0x10)));
 assert(off==1||off==2||off==5||off==6);
 if(port==0x620 && off==5 && (v&1) && (mem[port]&2) && (mem[port+4]&0x1c)!=0x1c && tx_count<12)tx[tx_count++]=(mem[port+4]>>1)&1;
 uint16_t target=port+(off<4?0:4);
 if(off==1||off==5)mem[target]|=v;else mem[target]&=(uint8_t)~v;
}
static void init(uint8_t s)
{
 memset(pll_tx,0,sizeof pll_tx);pll_frames=0;
 memset(mem,0,sizeof mem);memset(temp_tx,0,sizeof temp_tx);temp_tx_bits=0;irq=s;timeout=spi_count=bit_count=tx_count=0;polls=writes=steps=0;
 mem[0x684]=4;mem[0x688]=10;mem[0x600]=0x1a;mem[0x604]=0xfa;
 mem[0x620]=0xbf;mem[0x624]=0xbc;mem[0x628]=64;
 mem[0x664]=3;mem[0x644]=7;mem[0x6a4]=0xf0;mem[0x8c0]=0xd5;temp_stream=0x00c30000;
}
int main(void)
{
 for(uint8_t s=0;s<2;++s){
  uint16_t v=0xbeef;uint32_t p=0;
  init(s);assert(pm_diag_temp(3,&v)==PM_DIAG_OK && v==0xc3 && irq==s && mem[0x604]==0xfa);
  for(uint8_t reg=0;reg<8;++reg){
   init(s);temp_stream=0x00a55a00;
   assert(pm_diag_temp(reg,&v)==PM_DIAG_OK);
   assert(v==((reg==2||reg==4||reg==6||reg==7)?0xa55a:0xa5));
   assert(bit_count==((reg==2||reg==4||reg==6||reg==7)?24:16));
   assert(irq==s && mem[0x604]==0xfa);
  }
  init(s);assert(pm_diag_temp_restore()==PM_DIAG_OK && irq==s && mem[0x604]==0xfa);
  {const uint8_t expected[]={8,0x50,0x20,0x23,0,0x30,0x1e,0};assert(temp_tx_bits==64 && !memcmp(expected,temp_tx,8));}
  init(s);mem[0x688]=0;assert(pm_diag_temp_restore()==PM_DIAG_POWER && writes==0 && irq==s);
  init(s);mem[0x604]&=~16;assert(pm_diag_temp_restore()==PM_DIAG_BUSY && writes==0 && irq==s);
  init(s);assert(pm_diag_tdc(2,0x83,&v)==PM_DIAG_OK && v==0xa55a && irq==s && mem[0x624]==0xbc && mem[0x620]==0xbf);
  assert(tx_count==10 && tx[0]==1 && tx[1]==0);
  for(unsigned n=0;n<8;++n)assert(tx[n+2]==((0x83>>(7-n))&1));
  init(s);assert(pm_diag_pll(&p)==PM_DIAG_OK && p==0x44332211 && irq==s && mem[0x8c0]==0xd5 && mem[0x6a4]==0xf0);
  for(uint8_t fail=1;fail<=8;++fail){init(s);timeout=fail;p=0xcafebabe;assert(pm_diag_pll(&p)==PM_DIAG_TIMEOUT && polls==127u+fail && spi_count==fail && p==0xcafebabe && irq==s && mem[0x8c0]==0xd5 && mem[0x6a4]==0xf0);}
  for(uint8_t reg=0;reg<9;++reg){
   init(s);p=0;assert(pm_diag_pll_register(reg,&p)==PM_DIAG_OK && p==0x44332211);
   assert(spi_count==8 && pll_frames==2 && pll_tx[0]==(uint8_t)((reg<<4)|14));
   for(uint8_t i=1;i<8;++i)assert(pll_tx[i]==0);
   assert(irq==s && mem[0x8c0]==0xd5 && mem[0x6a4]==0xf0);
   for(uint8_t fail=1;fail<=8;++fail){
    init(s);timeout=fail;p=0xcafebabe;
    assert(pm_diag_pll_register(reg,&p)==PM_DIAG_TIMEOUT && polls==127u+fail && spi_count==fail);
    assert(p==0xcafebabe && irq==s && mem[0x8c0]==0xd5 && mem[0x6a4]==0xf0);
    assert(pll_tx[0]==(uint8_t)((reg<<4)|14));
   }
  }
  init(s);p=0xcafebabe;
  assert(pm_diag_pll_register(9,&p)==PM_DIAG_ARGUMENT && p==0xcafebabe && steps==0 && irq==s);
  assert(pm_diag_pll_register(255,&p)==PM_DIAG_ARGUMENT && p==0xcafebabe && steps==0 && irq==s);

  init(s);mem[0x688]=0;assert(pm_diag_temp(3,&v)==PM_DIAG_POWER && writes==0 && irq==s);
  assert(pm_diag_tdc(0,0x83,&v)==PM_DIAG_POWER && writes==0 && irq==s);
  assert(pm_diag_pll(&p)==PM_DIAG_POWER && writes==0 && irq==s);
  init(s);mem[0x604]&=~16;assert(pm_diag_temp(3,&v)==PM_DIAG_BUSY && writes==0 && irq==s);
  init(s);mem[0x624]&=~4;assert(pm_diag_tdc(1,0x83,&v)==PM_DIAG_BUSY && writes==0 && irq==s);
  init(s);mem[0x664]=2;assert(pm_diag_pll(&p)==PM_DIAG_BUSY && writes==0 && irq==s);
  init(s);mem[0x644]=6;assert(pm_diag_pll(&p)==PM_DIAG_BUSY && writes==0 && irq==s);
  init(s);assert(pm_diag_temp(8,&v)==PM_DIAG_ARGUMENT && writes==0 && irq==s);
  assert(pm_diag_tdc(3,0x83,&v)==PM_DIAG_ARGUMENT && writes==0 && irq==s);
 }
 puts("PASS device serial framing, readbacks, IRQ restoration, bounded timeout, GPIO restoration, power/busy guards and write whitelist");
}
