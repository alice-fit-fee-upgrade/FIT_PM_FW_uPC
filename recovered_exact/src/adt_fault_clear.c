#include <avr/io.h>

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_reads_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/device_reads.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint32_t pm_adt_faults_c(void)
 * {
 *     PORTA_OUTCLR=0x10;
 *     (void)pm_adt_byte_call(0xff);
 *     (void)pm_adt_byte_call(0xff);
 *     (void)pm_adt_byte_call(0xff);
 *     uint32_t packet=pm_adt_byte_call(0xff);
 *     PORTA_OUTSET=0x10;
 *     return packet;
 * }
 */

/* No GNU argument call: byte transfer uses the original R16/17/18 contract. */
#define PM_ADT_SEND_BYTE(byte) \
 do { register uint8_t value asm("r16")=(byte); \
 asm volatile ("rcall adt7311_byte_rw" : "+r" (value) \
     : : "r17", "r18", "memory", "cc"); } while (0)

void adt7311_faults_clr(void)
{
    PORTA_OUTCLR=0x10;
    PM_ADT_SEND_BYTE(0xff);
    PM_ADT_SEND_BYTE(0xff);
    PM_ADT_SEND_BYTE(0xff);
    PM_ADT_SEND_BYTE(0xff);
    PORTA_OUTSET=0x10;
}
