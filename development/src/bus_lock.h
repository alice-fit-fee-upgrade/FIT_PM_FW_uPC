#ifndef PM_BUS_LOCK_H
#define PM_BUS_LOCK_H
#include "hw_diag.h"
/* Reset-cleared GPIORE/GPIORF are explicitly reserved by this development image.
 * No SRAM or CRT initialization. F: owner bits0..1, thermal pending bit7.
 * E: saturated ISR deferral count. ISR never waits; native code preserves I.
 */
#define PM_BUS_OWNER_ADDR 0x000fu
#define PM_BUS_DEFER_ADDR 0x000eu
#define PM_BUS_I2C 1u
#define PM_BUS_TEMP 2u
static inline uint8_t pm_bus_acquire(uint8_t owner)
{
 uint8_t irq=pm_irq_enter(),v=pm_read8(PM_BUS_OWNER_ADDR);
 uint8_t ok=!(v&0x43u) && (owner!=PM_BUS_I2C || !(v&0x80u));
 if(ok)pm_write8(PM_BUS_OWNER_ADDR,(uint8_t)((v&0xfcu)|owner));
 pm_irq_leave(irq);return ok;
}
static inline void pm_bus_release(void)
{
 uint8_t irq=pm_irq_enter();pm_write8(PM_BUS_OWNER_ADDR,pm_read8(PM_BUS_OWNER_ADDR)&0xfcu);pm_irq_leave(irq);
}
#endif
