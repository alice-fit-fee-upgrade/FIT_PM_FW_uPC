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
 * pm_dac_packet pm_dac_prepare_calibrated(uint16_t input, uint8_t channel,
 *                                         uint16_t calibration)
 * {
 *     pm_dac_packet p;
 *     p.value = (uint16_t)~(uint16_t)(pm_scale_unsigned(0x020cu, input) + calibration);
 *     p.input = calibration;
 *     p.product = (uint16_t)(2u * (input >> 8));
 *     p.control = (uint8_t)((uint8_t)(channel * 4u) | 3u);
 *     p.reserved = 0;
 *     return p;
 * }
 */

void pm_dac_gain_scale(void) asm("FUN_code_001068");
void pm_dac_gain_scale(void)
{
    register uint16_t requested asm("r20");
    asm volatile("" : "=r" (requested));
    register uint16_t coefficient asm("r18") = 20000;
    coefficient -= requested; asm volatile("" : "+r" (coefficient));
    requested = coefficient; asm volatile("" : "+r" (requested));
    coefficient = 0x0272;
    asm volatile("rcall FUN_code_0010a6" : "+r" (coefficient), "+r" (requested) : : "r0", "r1", "r16", "r17", "memory", "cc");
    register uint8_t channel asm("r22");
    asm volatile("" : "=r" (channel));
    channel += channel; asm volatile("" : "+r" (channel));
    channel += channel;
    asm volatile("rcall dac_send_value" : "+r" (channel) : : "memory", "cc");
}
