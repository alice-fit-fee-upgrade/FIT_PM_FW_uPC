/* Main-loop diagnostics. Config commands explicitly reset the FPGA and transfer
 * config-bus ownership. No DAC, PSU, EEPROM, flash-content or I2C writes. IRQ masking serializes against the recovered PLL ISR;
 * each transaction is finite, with at most 128 SPI polls per byte. Hardware
 * interrupt latency and electrical ownership still need validation. */
#include "hw_diag.h"
#include "bus_lock.h"

#define A 0x600u
#define B 0x620u
#define C 0x640u
#define E 0x680u
#define F 0x6a0u
#define PM_SPIC 0x8c0u
#define DIR 0u
#define DIRSET 1u
#define DIRCLR 2u
#define OUT 4u
#define SET 5u
#define CLR 6u
#define IN 8u
static uint8_t powered(void) { return (pm_read8(E+OUT)&4) && (pm_read8(E+IN)&10)==10; }
static void restore_bits(uint16_t port,uint8_t mask,uint8_t out)
{ pm_write8(port+CLR,mask & (uint8_t)~out);pm_write8(port+SET,mask & out); }
#ifdef __AVR__
/* Native R24 adapter to the timing-adapted legacy entry at 0x2608. */
extern uint8_t pm_diag_legacy_temp_byte(uint8_t tx);
#endif
static void temp_phase_delay(void)
{
#ifdef __AVR__
 /* 1 us at the recovered 32 MHz clock; a diagnostic timing experiment. */
 __builtin_avr_delay_cycles(32);
#endif
}
static uint8_t temp_byte(uint8_t tx,uint8_t slow)
{
#ifdef __AVR__
 if(!slow)return pm_diag_legacy_temp_byte(tx);
#else
 (void)slow; /* Host value/framing model; no electrical timing claim. */
#endif
 uint8_t rx=0;
 for(uint8_t n=0;n<8;++n) {
  pm_write8(A+((tx&128)?SET:CLR),8);temp_phase_delay();
  pm_write8(A+CLR,2);temp_phase_delay();
  /* DOUT is established after the falling edge. Capture before the rising
   * edge: after the final rising edge the sensor may already drive DOUT low. */
  rx=(uint8_t)((rx<<1)|((pm_read8(A+IN)>>2)&1));
  pm_write8(A+SET,2);temp_phase_delay();tx<<=1;
 }
 return rx;
}
/* Native TEMP retains its previously validated short IRQ exclusion. It cannot
 * overlap main-loop I2C; adding a second ownership wrapper here is unnecessary. */
static uint8_t temp_read(uint8_t reg,uint16_t *value,uint8_t slow)
{
 if(reg>7)return PM_DIAG_ARGUMENT;
 uint8_t irq=pm_irq_enter(),result=PM_DIAG_OK;
 uint8_t old=pm_read8(A+OUT);
 if(pm_read8(PM_BUS_OWNER_ADDR)&0x43u)result=PM_DIAG_BUSY;
 else if(!powered())result=PM_DIAG_POWER;
 else if((pm_read8(A+DIR)&0x1a)!=0x1a || !(old&16))result=PM_DIAG_BUSY;
 else {
  pm_write8(A+CLR,16);(void)temp_byte((uint8_t)(0x40|(reg<<3)),slow);
  uint16_t v=temp_byte(0,slow);if(reg==2 || reg==4 || reg==6 || reg==7)v=(uint16_t)((v<<8)|temp_byte(0,slow));
  pm_write8(A+SET,16);restore_bits(A,0x1a,old);*value=v;
 }
 pm_irq_leave(irq);return result;
}
uint8_t pm_diag_temp(uint8_t reg,uint16_t *value){return temp_read(reg,value,0);}
uint8_t pm_diag_temp_slow(uint8_t reg,uint16_t *value){return temp_read(reg,value,1);}
/* Explicit laboratory recovery, not a periodic operation. Owner excludes
 * timer temperature SPI throughout; IRQs remain enabled during the reset wait.
 * Each selected frame uses only a short IRQ exclusion. Values reproduce the
 * original startup; completion means sent, so callers must verify readback. */
static uint8_t temp_begin_frame(uint8_t *irq)
{
 *irq=pm_irq_enter();
 if(!powered()){pm_irq_leave(*irq);return PM_DIAG_POWER;}
 if((pm_read8(A+DIR)&0x1a)!=0x1a || !(pm_read8(A+OUT)&16)){
  pm_irq_leave(*irq);return PM_DIAG_BUSY;
 }
 pm_write8(A+CLR,16);return PM_DIAG_OK;
}
static void temp_end_frame(uint8_t irq){pm_write8(A+SET,16);pm_irq_leave(irq);}
uint8_t pm_diag_temp_init(void)
{
 if(!pm_bus_acquire(PM_BUS_TEMP))return PM_DIAG_BUSY;
 uint8_t irq,result=temp_begin_frame(&irq);
 if(result)goto done;
 for(uint8_t n=0;n<4;++n)(void)temp_byte(0xff,1);
 temp_end_frame(irq);
#ifdef __AVR__
 __builtin_avr_delay_cycles(19200); /* >=600 us, IRQs enabled at command entry. */
#endif
 result=temp_begin_frame(&irq);if(result)goto done;
 (void)temp_byte(0x08,1);(void)temp_byte(0x50,1);temp_end_frame(irq);
 result=temp_begin_frame(&irq);if(result)goto done;
 (void)temp_byte(0x20,1);(void)temp_byte(0x23,1);(void)temp_byte(0x00,1);temp_end_frame(irq);
 result=temp_begin_frame(&irq);if(result)goto done;
 (void)temp_byte(0x30,1);(void)temp_byte(0x1e,1);(void)temp_byte(0x00,1);temp_end_frame(irq);
 done:pm_bus_release();return result;
}
static void clock_tdc(void) { pm_write8(B+SET,1);pm_write8(B+CLR,1); }
static void send_tdc_bit(uint8_t bit) { pm_write8(B+(bit?SET:CLR),2);clock_tdc(); }
uint8_t pm_diag_tdc(uint8_t chip,uint8_t reg,uint16_t *value)
{
 if(chip>2)return PM_DIAG_ARGUMENT;
 uint8_t irq=pm_irq_enter(),result=PM_DIAG_OK;
 uint8_t old=pm_read8(B+OUT),dir=pm_read8(B+DIR),select=(uint8_t)(4u<<chip);
 if(!powered() || !(old&32) || !(pm_read8(B+IN)&64))result=PM_DIAG_POWER;
 else if((old&0x1c)!=0x1c || (dir&0x1f)!=0x1f)result=PM_DIAG_BUSY;
 else {
  pm_write8(B+CLR,select);send_tdc_bit(1);send_tdc_bit(0);
  for(uint8_t n=0;n<8;++n){send_tdc_bit(reg&128);reg<<=1;}
  pm_write8(B+DIRCLR,2);clock_tdc();uint16_t v=0;
  for(uint8_t n=0;n<16;++n){pm_write8(B+SET,1);v=(uint16_t)((v<<1)|((pm_read8(B+IN)>>1)&1));pm_write8(B+CLR,1);}
  pm_write8(B+SET,select);pm_write8(B+DIRSET,2);clock_tdc();clock_tdc();
  restore_bits(B,0x1f,old);*value=v;
 }
 pm_irq_leave(irq);return result;
}
static uint8_t spi_at(uint16_t base,uint8_t tx,uint8_t *rx)
{
 pm_write8(base+3,tx);
 for(uint8_t n=0;n<128;++n)if(pm_read8(base+2)&128){*rx=pm_read8(base+3);return PM_DIAG_OK;}
 return PM_DIAG_TIMEOUT;
}
static uint8_t spi_byte(uint8_t tx,uint8_t *rx){return spi_at(PM_SPIC,tx,rx);}
uint8_t pm_diag_pll(uint32_t *value)
{
 uint8_t irq=pm_irq_enter(),result=PM_DIAG_OK,old=pm_read8(PM_SPIC),out=pm_read8(F+OUT),rx=0;
 if(!powered())result=PM_DIAG_POWER;
 else if(!(out&16) || (pm_read8(C+OUT)&7)!=7 || !(old&0x40) || !(pm_read8(0x664)&1))result=PM_DIAG_BUSY;
 else {
  /* Same 0x8e readback request as recovered FUN_code_001267. No PLL profile writes. */
  pm_write8(PM_SPIC,0xf1);pm_write8(F+CLR,16);
  for(uint8_t n=0;n<4 && result==PM_DIAG_OK;++n)result=spi_byte(n?0:0x8e,&rx);
  pm_write8(F+SET,16);
  uint32_t v=0;
  if(result==PM_DIAG_OK){pm_write8(F+CLR,16);for(uint8_t n=0;n<4 && result==PM_DIAG_OK;++n){result=spi_byte(0,&rx);v|=(uint32_t)rx<<(n*8);}}
  pm_write8(F+SET,16);pm_write8(PM_SPIC,old);
  if(result==PM_DIAG_OK)*value=v;
 }
 pm_irq_leave(irq);return result;
}

uint8_t pm_diag_fpga(uint8_t write,uint8_t address,uint16_t *word)
{
 uint8_t irq=pm_irq_enter(),result=PM_DIAG_OK;
 uint8_t old=pm_read8(PM_SPIC),rx=0,high=0;
 if(!powered() || !(pm_read8(0x668)&8) || !(pm_read8(0x664)&2))result=PM_DIAG_POWER;
 else if(!(pm_read8(0x664)&1) || !(pm_read8(F+OUT)&16) || (pm_read8(C+OUT)&7)!=7)result=PM_DIAG_BUSY;
 else {
  pm_write8(PM_SPIC,0xd1);pm_write8(0x666,1);
  result=spi_byte((address>>2)|(write?0:0x80),&rx);
  if(!result)result=spi_byte((uint8_t)(address<<6),&rx);
  if(!result)result=spi_byte(write?(*word>>8):0,&high);
  if(!result)result=spi_byte(write?(uint8_t)*word:0,&rx);
  pm_write8(0x665,1);pm_write8(PM_SPIC,old);
  if(!result && !write)*word=((uint16_t)high<<8)|rx;
 }
 pm_irq_leave(irq);return result;
}
uint8_t pm_diag_config(uint8_t hold)
{
 uint8_t irq=pm_irq_enter(),result=PM_DIAG_OK;
 if(!powered())result=PM_DIAG_POWER;
 else if(!(pm_read8(0x660)&2))result=PM_DIAG_CONFIG;
 else if(hold){
  /* Original flash interface: PROGRAM_B LOW, CS HIGH, MCU SPIE owns the bus. */
  pm_write8(0xac0,0);pm_write8(0x666,2);pm_write8(E+SET,16);
  pm_write8(E+DIRSET,0xb0);pm_write8(0xac0,0x50);
 }else{
  /* Release config bus before PROGRAM_B. Original IRQ/state machine handles DONE. */
  pm_write8(E+SET,16);pm_write8(0xac0,0);pm_write8(E+DIRCLR,0xb0);pm_write8(0x665,2);
 }
 pm_irq_leave(irq);return result;
}
uint8_t pm_diag_flash(uint8_t id,uint32_t address,uint8_t *data,uint8_t count)
{
 if(!count || count>16 || address>0xffffffUL || address+count>0x1000000UL)return PM_DIAG_ARGUMENT;
 uint8_t irq=pm_irq_enter(),result=PM_DIAG_OK,rx=0;
 if(!powered())result=PM_DIAG_POWER;
 else if((pm_read8(0x664)&2) || (pm_read8(0x668)&8) || (pm_read8(E+DIR)&0xb0)!=0xb0 || !(pm_read8(0xac0)&0x40) || !(pm_read8(E+OUT)&16))result=PM_DIAG_CONFIG;
 else {
  pm_write8(E+CLR,16);result=spi_at(0xac0,id?0x9f:0x03,&rx);
  if(!id)for(uint8_t n=3;n && !result;--n)result=spi_at(0xac0,(uint8_t)(address>>((n-1)*8)),&rx);
  for(uint8_t n=0;n<count && !result;++n)result=spi_at(0xac0,0,data+n);
  pm_write8(E+SET,16);
 }
 pm_irq_leave(irq);return result;
}
