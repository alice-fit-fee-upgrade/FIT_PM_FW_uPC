#include <stdint.h>
/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_reads_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/console_output.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint8_t pm_hex16_c(uint16_t value,uint8_t incoming_sreg)
 * {
 *     uint8_t high=(uint8_t)(value>>8),low=(uint8_t)value;
 *     uint8_t flags=emit_digit(high>>4,incoming_sreg);
 *     flags=emit_digit(high,flags);
 *     flags=emit_digit(low>>4,flags);
 *     return emit_digit(low,flags);
 * }
 */

#define SWAP_NIBBLES(value) do { \
 (value) = __builtin_avr_swap(value); \
 asm volatile("" : "+r" (value)); \
} while (0)
#define SEND_DIGIT(value) asm volatile("rcall cli_send_digit_hex" : "+r" (value) : : "memory", "cc")

/* Historical symbol says 32bit; the original emits four nibbles of R17:R16. */
void cli_send_32bit_hex(void)
{
    register uint8_t digit asm("r16");
    register uint8_t high asm("r17");
    asm volatile("" : "=r" (digit), "=r" (high));
    register uint8_t low asm("r18") = digit;
    SWAP_NIBBLES(high);
    digit = high;
    SEND_DIGIT(digit);
    SWAP_NIBBLES(high);
    digit = high;
    SEND_DIGIT(digit);
    SWAP_NIBBLES(low);
    digit = low;
    SEND_DIGIT(digit);
    SWAP_NIBBLES(low);
    digit = low;
    SEND_DIGIT(digit);
}
