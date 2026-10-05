#include <stdint.h>

/* Shared exact tail restores the register saved here and returns to caller. */
void FUN_code_001397(void)
{
    asm volatile("push r20" : : : "memory");
    register uint8_t mode asm("r20") = 2;
    asm volatile("sec\n\trjmp LAB_code_0013aa" : : "r" (mode) : "memory", "cc");
    __builtin_unreachable();
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
uint32_t pm_logical_console_fixed2_entry(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x272e: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 10032;
    }
    case 0x2730: { // ldi r20, 0x02
        s->r[20] = 2;
        return 10034;
    }
    case 0x2732: { // sec
        pm_flag(s, CARRY, true);
        return 10036;
    }
    case 0x2734: { // rjmp .+30
        return 10068;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
