#include <stdint.h>

void cli_send_msg(void)
{
    register const uint8_t *cursor asm("r30");
    asm volatile("" : "=z" (cursor));
next_character:
    {
        register uint8_t byte asm("r16");
        /* C FLASH read equivalent with const __flash uint8_t *cursor:
         * byte = *cursor++;
         * Trials 239-249 could not reproduce the private pointer/register layout;
         * keep exact LPM Z+ instructions. This is explanatory, with no new
         * independent functional-test claim. Existing historical test scope,
         * when available, is documented above. */
        asm volatile("lpm %0, Z+" : "=r" (byte), "+z" (cursor) : : "memory");
        /* Unvalidated C equivalent: if (!byte) goto finished; step473
         * changed layout. Keep the original terminator flags/branch.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
        asm goto("tst %0\n\tbreq %l[finished]" : : "r" (byte) : "cc" : finished);
        asm volatile("rcall cli_send_buf" : "+r" (byte) : : "memory", "cc");
    }
    goto next_character;
finished:
    /* Cursor remains advanced, including the consumed terminator. */
    asm volatile("" : : "z" (cursor));
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 128 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_flash_message(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2826: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 10280;
    }
    case 0x2828: { // lpm r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_golden_flash[address];
        return 10282;
    }
    case 0x282a: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 10284;
    }
    case 0x282c: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 10290 : 10286;
    }
    case 0x282e: { // rcall .+124
        s->calls[s->call_depth++] = 10288;
        return 10412;
    }
    case 0x2830: { // rjmp .-10
        return 10280;
    }
    case 0x2832: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 10292;
    }
    case 0x2834: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
