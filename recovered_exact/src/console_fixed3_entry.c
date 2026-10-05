/* Native outer frame: GCC emits the original entry PUSH under the local
 * call-saved register profile. -fno-ipa-pure-const prevents noreturn inference
 * from dropping that save before the exact shared RET/RETI tail. The tail
 * restores this register in ASM; full FLASH identity validates the pairing. */
#include <stdint.h>

/* Shared exact tail restores the register saved here and returns to caller. */
void FUN_code_00139f(void)
{
    register uint8_t mode asm("r20") = 3;
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
uint32_t pm_logical_console_fixed3_entry(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x273e: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 10048;
    }
    case 0x2740: { // ldi r20, 0x03
        s->r[20] = 3;
        return 10050;
    }
    case 0x2742: { // sec
        pm_flag(s, CARRY, true);
        return 10052;
    }
    case 0x2744: { // rjmp .+14
        return 10068;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
