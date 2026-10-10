#ifndef PM_LEGACY_R16_C_H
#define PM_LEGACY_R16_C_H
#include "legacy_r16.h"

/* Opt-in C constants and absolute reads for the private R16 entry profile.
 * CLR stays ASM because its flags differ from an LDI zero assignment.
 * Each user must independently pass whole-image exact-check. */
#undef pm_scratch_constant
#define pm_scratch_constant(constant) \
({ register uint8_t pm_value asm("r16") = (constant); \
   asm volatile("" : "+r" (pm_value)); \
   pm_value; })
#undef pm_read_absolute
#define pm_read_absolute(address) \
({ register uint8_t pm_value asm("r16") = *(volatile uint8_t *)(address); \
   asm volatile("" : "+r" (pm_value)); \
   pm_value; })
#endif
