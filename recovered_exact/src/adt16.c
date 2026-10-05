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
 * uint32_t pm_adt16_c(uint8_t command,uint16_t data)
 * {
 *     PORTA_OUTCLR=0x10;
 *     (void)pm_adt_byte_call(command);
 *     uint8_t high=(uint8_t)pm_adt_byte_call((uint8_t)(data>>8));
 *     uint32_t last=pm_adt_byte_call((uint8_t)data);
 *     PORTA_OUTSET=0x10;
 *     return (last&0x00ff0000UL) | ((uint32_t)high<<8) | (uint8_t)last;
 * }
 */

#define CS_REGISTER(address, reg) \
 do { register uint8_t mask asm(reg)=0x10; \
 asm volatile ("" : "+r" (mask)); \
 *(volatile uint8_t *)(address) = mask; } while (0)
#define TRANSFER(value) \
 asm volatile ("rcall adt7311_byte_rw" : "+r" (value) \
     : : "r17", "r18", "memory", "cc")
void adt7311_16bit_rw(void)
{
    register uint8_t command asm("r16");
    asm volatile ("" : "=r" (command));
    CS_REGISTER(_SFR_MEM_ADDR(PORTA_OUTCLR), "r20");
    register uint8_t tx_low asm("r17");
    register uint8_t tx_high asm("r18");
    asm volatile ("" : "=r" (tx_low), "=r" (tx_high));
    register uint8_t low asm("r20");
    register uint8_t high asm("r21");
    low = tx_low;
    asm volatile ("" : "+r" (low));
    high = tx_high;
    asm volatile ("" : "+r" (high));
    TRANSFER(command);
    command=high;
    TRANSFER(command);
    high=command;
    command=low;
    TRANSFER(command);
    low=command;
    CS_REGISTER(_SFR_MEM_ADDR(PORTA_OUTSET), "r18");
    asm volatile ("" : : "r" (low), "r" (high));
}
