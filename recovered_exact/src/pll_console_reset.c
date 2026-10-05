#include "legacy_cpu.h"
#include <stdint.h>

void cdce62005_rst(void)
{
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000d1e" : : : "r16", "memory", "cc");
    register const uint8_t *cursor asm("r30") = (const uint8_t *)0x293e;
    asm volatile("" : "+z" (cursor));
    register uint8_t count asm("r20") = 9;
    asm volatile("" : "+r" (count));
    pm_cpu_disable_irq();
next_setting:
    {
        register uint8_t b0 asm("r16"), b1 asm("r17"), b2 asm("r18"), b3 asm("r19");
        asm volatile("lpm %0, Z+\n\tlpm %1, Z+\n\tlpm %2, Z+\n\tlpm %3, Z+\n\trcall CDCE62005_send_control_settings"
            : "=r" (b0), "=r" (b1), "=r" (b2), "=r" (b3), "+z" (cursor) : : "memory", "cc");
    }
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_setting]" : : "r" (count) : : next_setting);
    pm_cpu_enable_irq();
    cursor = (const uint8_t *)0x2998;
    asm volatile("rcall cli_send_msg" : "+z" (cursor) : : "memory", "cc");
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 304 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_pll_console_reset(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1a3e: { // rcall .+3580
        s->calls[s->call_depth++] = 6720;
        return 10300;
    }
    case 0x1a40: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6722;
    }
    case 0x1a42: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 6716 : 6724;
    }
    case 0x1a44: { // ldi r30, 0x3E
        s->r[30] = 62;
        return 6726;
    }
    case 0x1a46: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6728;
    }
    case 0x1a48: { // ldi r20, 0x09
        s->r[20] = 9;
        return 6730;
    }
    case 0x1a4a: { // cli
        pm_irq(s, false);
        return 6732;
    }
    case 0x1a4c: { // lpm r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_golden_flash[address];
        return 6734;
    }
    case 0x1a4e: { // lpm r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_golden_flash[address];
        return 6736;
    }
    case 0x1a50: { // lpm r18, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[18] = pm_golden_flash[address];
        return 6738;
    }
    case 0x1a52: { // lpm r19, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[19] = pm_golden_flash[address];
        return 6740;
    }
    case 0x1a54: { // rcall .+2608
        s->calls[s->call_depth++] = 6742;
        return 9350;
    }
    case 0x1a56: { // dec r20
        s->r[20]--;
        pm_nzv(s, s->r[20], s->r[20] == 127);
        return 6744;
    }
    case 0x1a58: { // brne .-14
        return (pm_getflag(s, 1) == 0) ? 6732 : 6746;
    }
    case 0x1a5a: { // sei
        pm_irq(s, true);
        return 6748;
    }
    case 0x1a5c: { // ldi r30, 0x98
        s->r[30] = 152;
        return 6750;
    }
    case 0x1a5e: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6752;
    }
    case 0x1a60: { // rcall .+3524
        s->calls[s->call_depth++] = 6754;
        return 10278;
    }
    case 0x1a62: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
