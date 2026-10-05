#include <stdint.h>

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_dac_final_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/dac_prepare.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * pm_dac_packet pm_dac_prepare_inverse(uint16_t input, uint8_t channel)
 * {
 *     pm_dac_packet p;
 *     p.input = (uint16_t)(20000u - input);
 *     p.value = pm_scale_unsigned(0x0272u, p.input);
 *     p.product = (uint16_t)(2u * (p.input >> 8));
 *     p.control = (uint8_t)(channel * 4u);
 *     p.reserved = 0;
 *     return p;
 * }
 */

void pm_dac_offset_scale(void) asm("FUN_code_001053");
void pm_dac_offset_scale(void)
{
    register uint16_t coefficient asm("r18") = 0x020c;
    register uint16_t value asm("r16");
    asm volatile("rcall FUN_code_0010a6" : "+r" (coefficient), "=r" (value) : : "r0", "r1", "memory", "cc");
    register const uint8_t *cursor asm("r30") = (const uint8_t *)0x2163;
    asm volatile("" : "+z" (cursor));
    register uint16_t correction asm("r20");
    asm volatile("clr r20" : "=r" (correction) : : "cc");
    register uint8_t channel asm("r22");
    asm volatile("" : "=r" (channel));
    channel += channel; asm volatile("" : "+r" (channel));
    {
        register uint8_t address_low asm("r30");
        asm volatile("" : "=r" (address_low) : "z" (cursor));
        address_low += channel; asm volatile("" : "+r" (address_low));
    }
    asm volatile("adc r31, r20" : "=z" (cursor) : "r" (correction) : "cc");
    channel += channel; asm volatile("" : "+r" (channel));
    channel |= 3; asm volatile("" : "+r" (channel));
    asm volatile("ld r20, Z+\n\tld r21, Z" : "=r" (correction), "+z" (cursor) : : "memory");
    value += correction; asm volatile("" : "+r" (value));
    value = ~value;
    asm volatile("rcall dac_send_value" : "+r" (value), "+r" (channel) : : "memory", "cc");
}
