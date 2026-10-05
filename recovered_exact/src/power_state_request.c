#include <avr/io.h>
/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_gpio_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/fpga_state.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint16_t pm_fpga_set_state(uint8_t state, uint8_t incoming_sreg)
 * {
 *     REG8(0x215b) = state;
 *     REG8(0x0626) = 0x80;
 *     uint8_t previous = REG8(0x2159);
 *     uint8_t flags;
 *     if (previous == 0) {
 *         flags = (incoming_sreg & 0xe1u) | 2u;
 *     } else {
 *         uint8_t result = (uint8_t)(previous - 5u);
 *         uint8_t n = result >> 7;
 *         uint8_t v = ((previous ^ 5u) & (previous ^ result)) >> 7;
 *         flags = (incoming_sreg & 0xc0u) | (previous < 5u)
 *               | ((result == 0) << 1) | (n << 2) | (v << 3)
 *               | ((n ^ v) << 4) | ((previous % 16u < 5u) << 5);
 *     }
 *     if (previous == 0 || previous == 5) {
 *         previous = 4;
 *         REG8(0x2159) = previous;
 *     }
 *     return (uint16_t)previous | ((uint16_t)flags << 8);
 * }
 */

#define RAM8(address) (*(volatile uint8_t *)(address))

void pm_power_state_request(void) asm("FUN_code_00054e");
void pm_power_state_request(void)
{
    register uint8_t value asm("r16");
    asm volatile("" : "=r" (value));
    RAM8(0x215b) = value;
    PORTB_OUTCLR = 0x80;
    value = RAM8(0x2159);
    /* Retain the two early branches and the no-change return exactly. */
    asm goto("tst %0\n\tbreq %l[changed]\n\tcpi %0, 5\n\tbreq %l[changed]\n\tret"
        : : "r" (value) : "cc" : changed);
changed:
    RAM8(0x2159) = 4;
}
