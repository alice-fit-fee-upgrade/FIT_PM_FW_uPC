/* Software I2C on PF7 SDA / PA1 SCL; PB6 RESET/alarm is input-only.
 * Main-loop only, interrupts enabled during transfer. The relocated temperature
 * ISR observes GPIORF ownership and defers; never spin in an ISR.
 */
#include "hw_diag.h"
#include "bus_lock.h"
#include "i2c_diag.h"
#ifdef __AVR__
#ifndef F_CPU
#define F_CPU 32000000UL
#endif
#include <util/delay.h>
void pm_i2c_delay(void) { _delay_us(4); }
#endif
#define A 0x600u
#define F 0x6a0u
#define DIRSET 1u
#define DIRCLR 2u
#define OUTSET 5u
#define OUTCLR 6u
#define IN 8u
typedef struct { uint16_t budget;uint8_t error; } bus;
static void sda(uint8_t high) { if(!high)pm_write8(F+OUTCLR,128);pm_write8(F+(high?DIRCLR:DIRSET),128);pm_i2c_delay(); }
static uint8_t clock(bus *b,uint8_t high)
{
 if(!high){pm_write8(A+OUTCLR,2);pm_write8(A+DIRSET,2);}
 else {
  pm_write8(A+DIRCLR,2);
  while(!(pm_read8(A+IN)&2)) {
   if(!b->budget){b->error=PM_I2C_TIMEOUT;return 0;}
   --b->budget;pm_i2c_delay();
  }
 }
 pm_i2c_delay();return 1;
}
static uint8_t start(bus *b)
{
 sda(1);if(!clock(b,1))return 0;
 if(!(pm_read8(F+IN)&128)){b->error=PM_I2C_STUCK;return 0;}
 sda(0);return clock(b,0);
}
static void stop(bus *b)
{
 clock(b,0);sda(0);
 if(clock(b,1))sda(1);
 else pm_write8(F+DIRCLR,128);
}
static uint8_t put(bus *b,uint8_t value)
{
 for(uint8_t bit=0;bit<8;++bit){sda(value&128);if(!clock(b,1))return 0;clock(b,0);value<<=1;}
 sda(1);if(!clock(b,1))return 0;
 uint8_t ack=!(pm_read8(F+IN)&128);clock(b,0);
 if(!ack)b->error=PM_I2C_NACK;
 return ack;
}
static uint8_t get(bus *b,uint8_t last,uint8_t *value)
{
 uint8_t v=0;sda(1);
 for(uint8_t bit=0;bit<8;++bit){if(!clock(b,1))return 0;v=(uint8_t)((v<<1)|!!(pm_read8(F+IN)&128));clock(b,0);}
 sda(last);if(!clock(b,1))return 0;clock(b,0);sda(1);*value=v;return 1;
}
static uint8_t mux(bus *b,uint8_t addr,uint8_t write,uint8_t *mask)
{
 uint8_t ok=start(b)&&put(b,(uint8_t)((addr<<1)|!write));
 if(ok)ok=write?put(b,*mask):get(b,1,mask);
 stop(b);return ok&&!b->error;
}
static uint8_t reg16(bus *b,uint8_t reg,uint16_t *word)
{
 uint8_t hi=0,lo=0;
 uint8_t ok=start(b)&&put(b,0x80)&&put(b,reg)&&start(b)&&put(b,0x81)&&get(b,0,&hi)&&get(b,1,&lo);
 stop(b);*word=((uint16_t)hi<<8)|lo;return ok&&!b->error;
}
int16_t pm_ina_code(uint16_t raw)
{
 uint16_t value=raw>>3;
 return (int16_t)(value&0x1000u?(int32_t)value-8192:(int32_t)value);
}
uint8_t pm_ina_read(uint8_t route,pm_ina_sample *sample)
{
 if(route<4 || route>6)return PM_I2C_ID;
 if(!pm_bus_acquire(PM_BUS_I2C))return PM_I2C_BUSY;
 bus b={2000,0};uint8_t saved=0,result=0,zero=0,selected=(uint8_t)(1u<<route),readback=0;
 /* Do not drive RESET or change its interrupt. Existing PSU rail currently
  * powers the monitors; loss is unavailable telemetry, not measured zero. */
 if(!(pm_read8(0x684)&4) || (pm_read8(0x688)&10)!=10){result=PM_I2C_POWER;goto release;}
 if(!(pm_read8(0x628)&64) || (pm_read8(0x620)&64)){result=PM_I2C_RESET;goto release;}
 if(!(pm_read8(A+4)&16)){result=PM_I2C_BUSY;goto release;}
 /* Retain SDA as released/no internal pull-down between transactions: old
  * temperature SCLK transitions must see a steady HIGH SDA, not false START. */
 pm_write8(F+DIRCLR,128);pm_write8(F+0x17,0);pm_write8(A+DIRCLR,2);
 pm_write8(A+OUTCLR,2);pm_write8(F+OUTCLR,128);
 if(!(pm_read8(A+IN)&2) || !(pm_read8(F+IN)&128)){result=PM_I2C_STUCK;goto cleanup;}
 if(!mux(&b,0x70,0,&sample->mask70))goto failed;
 saved|=1;
 if(!mux(&b,0x71,0,&sample->mask71))goto failed;
 saved|=2;
 if(!mux(&b,0x70,1,&zero)||!mux(&b,0x71,1,&zero)||!mux(&b,0x71,1,&selected)||!mux(&b,0x71,0,&readback))goto failed;
 if(readback!=selected){result=PM_I2C_RESTORE;goto restore;}
 {uint16_t id=0;if(!reg16(&b,0xfe,&id))goto failed;if(id!=0x5449){result=PM_I2C_ID;goto restore;}
  if(!reg16(&b,0xff,&id))goto failed;
  if(id!=0x3220){result=PM_I2C_ID;goto restore;}}
 if(!reg16(&b,0,&sample->configuration))goto failed;
 for(uint8_t r=1;r<=6;++r)if(!reg16(&b,r,&sample->raw[r-1]))goto failed;
 goto restore;
failed:result=b.error?b.error:PM_I2C_STUCK;
restore:
 /* Best effort both restores even after NACK; verify actual masks. */
 b.error=0;
 if(saved&1){if(!mux(&b,0x70,1,&sample->mask70)||!mux(&b,0x70,0,&readback)||readback!=sample->mask70)result=PM_I2C_RESTORE;}
 b.error=0;
 if(saved&2){if(!mux(&b,0x71,1,&sample->mask71)||!mux(&b,0x71,0,&readback)||readback!=sample->mask71)result=PM_I2C_RESTORE;}
cleanup:
 pm_write8(F+DIRCLR,128);pm_write8(A+DIRCLR,2);
 if(pm_read8(A+IN)&2){pm_write8(A+OUTSET,2);pm_write8(A+DIRSET,2);}
 else {
  /* Do not drive HIGH against a stuck/clock-stretching target. Block the
   * temperature clock too; report unavailable, leave PSU IRQ protection live. */
  uint8_t irq=pm_irq_enter();pm_write8(PM_BUS_OWNER_ADDR,pm_read8(PM_BUS_OWNER_ADDR)|0x40u);pm_irq_leave(irq);
  result=PM_I2C_STUCK;
 }
release:pm_bus_release();return result;
}
