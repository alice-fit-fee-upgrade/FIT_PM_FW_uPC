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
 * uint32_t pm_adt8_c(uint8_t command,uint8_t data)
 * {
 *     PORTA_OUTCLR=0x10;
 *     (void)pm_adt_byte_call(command);
 *     uint32_t packet=pm_adt_byte_call(data);
 *     PORTA_OUTSET=0x10;
 *     return packet; // Original 8-bit entry discards RX and preserves all GPRs.
 * }
 */

#define chip_select(address) \
 do { register uint8_t mask asm("r18")=0x10; \
 asm volatile ("" : "+r" (mask)); \
 *(volatile uint8_t *)(address) = mask; } while (0)
void adt7311_8bit_rw(void)
{
    register uint8_t command asm("r16");
    asm volatile ("" : "=r" (command));
    chip_select(_SFR_MEM_ADDR(PORTA_OUTCLR));
    register uint8_t input_data asm("r17");
    asm volatile ("" : "=r" (input_data));
    register uint8_t data asm("r19");
    data = input_data;
    asm volatile ("" : "+r" (data));
    asm volatile ("rcall adt7311_byte_rw" : "+r" (command)
        : : "r17", "r18", "memory", "cc");
    command=data;
    asm volatile ("rcall adt7311_byte_rw" : "+r" (command)
        : : "r17", "r18", "memory", "cc");
    chip_select(_SFR_MEM_ADDR(PORTA_OUTSET));
}
