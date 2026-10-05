#include <avr/io.h>

void pm_flash_sector_erase(void) asm("FUN_code_000b88");
void pm_flash_sector_erase(void)
{
    /* Original callees use a register ABI and two-byte relative calls. */
    asm volatile ("rcall FUN_code_000b8f" : : : "r16", "r19", "memory", "cc");
    register uint8_t command asm("r16") = 0xd8;
    asm volatile ("rcall FUN_code_000c04" : "+r" (command)
        : : "r17", "r19", "memory", "cc");
    PORTE_OUTSET = 0x10;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 96 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_sector_erase(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1710: { // rcall .+12
        s->calls[s->call_depth++] = 5906;
        return 5918;
    }
    case 0x1712: { // ldi r16, 0xD8
        s->r[16] = 216;
        return 5908;
    }
    case 0x1714: { // rcall .+242
        s->calls[s->call_depth++] = 5910;
        return 6152;
    }
    case 0x1716: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5912;
    }
    case 0x1718: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5916;
    }
    case 0x171c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
