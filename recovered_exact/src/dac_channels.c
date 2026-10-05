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
 * pm_dac_packet pm_dac_prepare_signed(uint16_t input, uint8_t channel,
 *                                     uint8_t command)
 * {
 *     pm_dac_packet p;
 *     p.value = (uint16_t)(pm_scale_signed(0x4188u, input) ^ 0x8000u);
 *     p.input = input;
 *     int16_t high = (int16_t)(input >> 8);
 *     if (high >= 128) high -= 256;
 *     p.product = (uint16_t)(high * 65);
 *     p.control = (uint8_t)((uint8_t)(channel * 4u) | command);
 *     p.reserved = 0;
 *     return p;
 * }
 */

/* Fragment C equivalent: scaled_high ^= sign_mask;
 * Corrected trials357/358 changed fixed region sizes and were restored.
 * Functional evidence above applies to the historical complete alternative
 * with its bridge, not to this independently compiled fragment. */
#define DAC_CHANNEL(name, selector) \
void name(void) { \
    register uint16_t scale asm("r18") = 0x4188; \
    asm volatile("rcall fpga_is_ready" : "+r" (scale) : \
                 : "r0", "r1", "r16", "r17", "memory", "cc"); \
    register uint8_t sign_mask asm("r18") = 0x80; \
    asm volatile("eor r17, %0" : : "r" (sign_mask) : "r17", "cc"); \
    register uint8_t channel asm("r22"); \
    asm volatile("" : "=r" (channel)); \
    channel += channel; \
    asm volatile("" : "+r" (channel)); \
    channel += channel; \
    asm volatile("" : "+r" (channel)); \
    channel |= (selector); \
    asm volatile("rcall dac_send_value" : "+r" (channel) : \
                 : "r16", "r17", "r19", "r20", "r21", "memory", "cc"); \
}
/* Original helper's name is historical: fpga_is_ready performs multiplication. */
DAC_CHANNEL(dac_set_value_2, 2)
DAC_CHANNEL(dac_set_value, 1)
