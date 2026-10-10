#ifndef PM_LEGACY_R16_H
#define PM_LEGACY_R16_H
#include <stdint.h>

/* Tiny exact fragments for the original non-GNU scratch-register ABI.
 * CLR updates flags; LDI leaves them untouched. They are not interchangeable.
 * These helpers are private to files compiled with the r16_only profile. */
static inline uint8_t pm_scratch_zero(void)
{
    register uint8_t value asm("r16");
    asm volatile ("clr %0" : "=r" (value) : : "cc");
    return value;
}

/* GNU statement expressions avoid introducing a GNU R24 argument ABI. */
#define pm_scratch_constant(constant) \
({ register uint8_t pm_value asm("r16"); \
   asm volatile ("ldi %0, %1" : "=r" (pm_value) : "M" (constant)); \
   pm_value; })

#define pm_read_absolute(address) \
({ register uint8_t pm_value asm("r16"); \
   asm volatile ("lds %0, %1" : "=r" (pm_value) : "n" (address) : "memory"); \
   pm_value; })
#endif
