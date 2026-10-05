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
#define SEND_DIGIT(value) ((value) = pm_console_send_hex_digit(value))

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

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 320 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_hex_word(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x26f8: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 9978;
    }
    case 0x26fa: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 9980;
    }
    case 0x26fc: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9982;
    }
    case 0x26fe: { // mov r18, r16
        s->r[18] = s->r[16];
        return 9984;
    }
    case 0x2700: { // swap r17
        s->r[17] = (s->r[17] >> 4) | (s->r[17] << 4);
        return 9986;
    }
    case 0x2702: { // mov r16, r17
        s->r[16] = s->r[17];
        return 9988;
    }
    case 0x2704: { // rcall .+26
        s->calls[s->call_depth++] = 9990;
        return 10016;
    }
    case 0x2706: { // swap r17
        s->r[17] = (s->r[17] >> 4) | (s->r[17] << 4);
        return 9992;
    }
    case 0x2708: { // mov r16, r17
        s->r[16] = s->r[17];
        return 9994;
    }
    case 0x270a: { // rcall .+20
        s->calls[s->call_depth++] = 9996;
        return 10016;
    }
    case 0x270c: { // swap r18
        s->r[18] = (s->r[18] >> 4) | (s->r[18] << 4);
        return 9998;
    }
    case 0x270e: { // mov r16, r18
        s->r[16] = s->r[18];
        return 10000;
    }
    case 0x2710: { // rcall .+14
        s->calls[s->call_depth++] = 10002;
        return 10016;
    }
    case 0x2712: { // swap r18
        s->r[18] = (s->r[18] >> 4) | (s->r[18] << 4);
        return 10004;
    }
    case 0x2714: { // mov r16, r18
        s->r[16] = s->r[18];
        return 10006;
    }
    case 0x2716: { // rcall .+8
        s->calls[s->call_depth++] = 10008;
        return 10016;
    }
    case 0x2718: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 10010;
    }
    case 0x271a: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 10012;
    }
    case 0x271c: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 10014;
    }
    case 0x271e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
