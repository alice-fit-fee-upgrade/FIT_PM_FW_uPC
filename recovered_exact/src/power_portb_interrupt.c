#define PM_ISR_EXACT_SREG_READ
#include "legacy_interrupt_register_c.h"
/* O1 retry535 also failed the complete binary/layout gate. */
#include "legacy_r16.h"

/* Steps452/457: C absolute read of *(volatile uint8_t *)0x2157 changed
 * binary/layout. Unvalidated standalone; keep original R16 ISR helper.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
void PORTB_INT0_vect_isr(void)
{
    PM_ISR_ENTER_R16();
    uint8_t status = pm_read_absolute(0x2157);
    PM_RAM8(0x2157) = status | 0x60;
    PORTA_OUTCLR = 0x40;
    PORTE_OUTCLR = 4;
    PM_ISR_LEAVE_R16();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 224 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_power_portb_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0abc: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 2750;
    }
    case 0x0abe: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 2752;
    }
    case 0x0ac0: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 2754;
    }
    case 0x0ac2: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 2758;
    }
    case 0x0ac6: { // ori r16, 0x60
        s->r[16] |= 96;
        pm_nzv(s, s->r[16], false);
        return 2760;
    }
    case 0x0ac8: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 2764;
    }
    case 0x0acc: { // ldi r16, 0x40
        s->r[16] = 64;
        return 2766;
    }
    case 0x0ace: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 2770;
    }
    case 0x0ad2: { // ldi r16, 0x04
        s->r[16] = 4;
        return 2772;
    }
    case 0x0ad4: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 2776;
    }
    case 0x0ad8: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 2778;
    }
    case 0x0ada: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 2780;
    }
    case 0x0adc: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 2782;
    }
    case 0x0ade: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
