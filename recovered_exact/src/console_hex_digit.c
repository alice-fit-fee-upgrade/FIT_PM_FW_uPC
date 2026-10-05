#include "legacy_console_call_c.h"
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
     * No standalone validation; original threshold flags/private ABI remain.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    /* Exact unsigned threshold branch without a second scratch register. */
    asm goto("cpi %0, 0x3a\n\tbrlo %l[send]" : : "r" (digit) : "cc" : send);
    digit += 'A' - '0' - 10;
send:
    digit = pm_console_send_character(digit);
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 112 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_hex_digit(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2720: { // andi r16, 0x0F
        s->r[16] &= 15;
        pm_nzv(s, s->r[16], false);
        return 10018;
    }
    case 0x2722: { // subi r16, 0xD0
        s->r[16] = pm_sub(s, s->r[16], 208, 0, false);
        return 10020;
    }
    case 0x2724: { // cpi r16, 0x3A
        pm_sub(s, s->r[16], 58, 0, false);
        return 10022;
    }
    case 0x2726: { // brcs .+2
        return (pm_getflag(s, 0) == 1) ? 10026 : 10024;
    }
    case 0x2728: { // subi r16, 0xF9
        s->r[16] = pm_sub(s, s->r[16], 249, 0, false);
        return 10026;
    }
    case 0x272a: { // rcall .+384
        s->calls[s->call_depth++] = 10028;
        return 10412;
    }
    case 0x272c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
