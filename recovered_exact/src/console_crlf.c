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

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 64 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_crlf(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x281e: { // ldi r30, 0x84
        s->r[30] = 132;
        return 10272;
    }
    case 0x2820: { // ldi r31, 0x29
        s->r[31] = 41;
        return 10274;
    }
    case 0x2822: { // rcall .+2
        s->calls[s->call_depth++] = 10276;
        return 10278;
    }
    case 0x2824: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
