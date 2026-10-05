#include <avr/io.h>
/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_gpio_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/fpga_state.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint16_t pm_fpga_set_state(uint8_t state, uint8_t incoming_sreg)
 * {
 *     REG8(0x215b) = state;
 *     REG8(0x0626) = 0x80;
 *     uint8_t previous = REG8(0x2159);
 *     uint8_t flags;
 *     if (previous == 0) {
 *         flags = (incoming_sreg & 0xe1u) | 2u;
 *     } else {
 *         uint8_t result = (uint8_t)(previous - 5u);
 *         uint8_t n = result >> 7;
 *         uint8_t v = ((previous ^ 5u) & (previous ^ result)) >> 7;
 *         flags = (incoming_sreg & 0xc0u) | (previous < 5u)
 *               | ((result == 0) << 1) | (n << 2) | (v << 3)
 *               | ((n ^ v) << 4) | ((previous % 16u < 5u) << 5);
 *     }
 *     if (previous == 0 || previous == 5) {
 *         previous = 4;
 *         REG8(0x2159) = previous;
 *     }
 *     return (uint16_t)previous | ((uint16_t)flags << 8);
 * }
 */

#define RAM8(address) (*(volatile uint8_t *)(address))

void pm_power_state_request(void) asm("FUN_code_00054e");
void pm_power_state_request(void)
{
    register uint8_t value asm("r16");
    asm volatile("" : "=r" (value));
    RAM8(0x215b) = value;
    PORTB_OUTCLR = 0x80;
    value = RAM8(0x2159);
    /* Retain the two early branches and the no-change return exactly. */
    asm goto("tst %0\n\tbreq %l[changed]\n\tcpi %0, 5\n\tbreq %l[changed]"
        : : "r" (value) : "cc" : changed);
    return;
changed:
    RAM8(0x2159) = 4;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 192 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_power_state_request(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0a9c: { // sts 0x215B, r16
        uint16_t address = 8539;
        pm_write(s, address, s->r[16]);
        return 2720;
    }
    case 0x0aa0: { // ldi r16, 0x80
        s->r[16] = 128;
        return 2722;
    }
    case 0x0aa2: { // sts 0x0626, r16
        uint16_t address = 1574;
        pm_write(s, address, s->r[16]);
        return 2726;
    }
    case 0x0aa6: { // lds r16, 0x2159
        uint16_t address = 8537;
        s->r[16] = pm_read(s, address);
        return 2730;
    }
    case 0x0aaa: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 2732;
    }
    case 0x0aac: { // breq .+6
        return (pm_getflag(s, 1) == 1) ? 2740 : 2734;
    }
    case 0x0aae: { // cpi r16, 0x05
        pm_sub(s, s->r[16], 5, 0, false);
        return 2736;
    }
    case 0x0ab0: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 2740 : 2738;
    }
    case 0x0ab2: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x0ab4: { // ldi r16, 0x04
        s->r[16] = 4;
        return 2742;
    }
    case 0x0ab6: { // sts 0x2159, r16
        uint16_t address = 8537;
        pm_write(s, address, s->r[16]);
        return 2746;
    }
    case 0x0aba: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
