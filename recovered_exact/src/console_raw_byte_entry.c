#include <stdint.h>

/* Shared exact tail restores the register saved here and returns to caller. */
void cli_get_next_byte(void)
{
    register uint8_t mode asm("r19") = 1;
    asm volatile("rjmp LAB_code_001420" : : "r" (mode) : "memory", "cc");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 48 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_raw_byte_entry(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2836: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 10296;
    }
    case 0x2838: { // ldi r19, 0x01
        s->r[19] = 1;
        return 10298;
    }
    case 0x283a: { // rjmp .+4
        return 10304;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
