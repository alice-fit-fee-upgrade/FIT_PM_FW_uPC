/* Ordinary GNU C implementation of the legacy register8 read transaction.
 * Blocking completion polling intentionally preserves the original contract.
 * The diagnostic API has separate bounded/error-returning transactions.
 * No IRQ policy, profile write, EEPROM copy or reset is introduced here. */
#include "hw_diag.h"
static void send_byte(uint8_t byte)
{
 pm_write8(0x08c3,byte);
 while(!(pm_read8(0x08c2)&0x80)){}
}
static uint8_t read_byte(void)
{
 send_byte(0);
 return pm_read8(0x08c3);
}
uint32_t pm_pll_read_status_c(void)
{
 pm_write8(0x08c0,0xf1);
 pm_write8(0x06a6,0x10);
 send_byte(0x8e);send_byte(0);send_byte(0);send_byte(0);
 pm_write8(0x06a5,0x10);
 pm_write8(0x06a6,0x10);
 uint8_t b0=read_byte(),b1=read_byte(),b2=read_byte(),b3=read_byte();
 pm_write8(0x06a5,0x10);
 return (uint32_t)b0|((uint32_t)b1<<8)|((uint32_t)b2<<16)|((uint32_t)b3<<24);
}
