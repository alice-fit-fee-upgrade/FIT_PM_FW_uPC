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
 * uint32_t pm_crlf_c(void)
 * {
 *     return pm_flash_message_call(0x2984u);
 * }
 */

static inline void send_flash_message(uint16_t address)
{
    register uint16_t z asm("r30") = address;
    /* Historical relative-call encoding; callee preserves other GPRs. */
    asm volatile ("rcall cli_send_msg" : "+z" (z) : : "memory", "cc");
}
void cli_send_crlf(void)
{
    send_flash_message(0x2984);
}
