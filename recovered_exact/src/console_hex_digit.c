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
 * uint16_t pm_hex_digit_c(uint8_t value,uint8_t incoming_sreg)
 * {
 *     uint8_t nibble=value&15u;
 *     uint8_t ascii=(uint8_t)(nibble+(nibble<10u?'0':('A'-10)));
 *     uint8_t flags=(incoming_sreg&0xc0u)|(nibble<10u?0x35u:1u);
 *     return (uint16_t)ascii|((uint16_t)flags<<8);
 * }
 */

void cli_send_digit_hex(void)
{
    register uint8_t digit asm("r16");
    asm volatile("" : "=r" (digit));
    digit = (digit & 0x0f) + '0';
    asm volatile("" : "+r" (digit));
    /* Step447 plain C `if (digit < 0x3a) goto send;` failed exact matching.
     * No standalone validation; original threshold flags/private ABI remain. */
    /* Exact unsigned threshold branch without a second scratch register. */
    asm goto("cpi %0, 0x3a\n\tbrlo %l[send]" : : "r" (digit) : "cc" : send);
    digit += 'A' - '0' - 10;
send:
    asm volatile("rcall cli_send_buf" : "+r" (digit) : : "memory", "cc");
}
