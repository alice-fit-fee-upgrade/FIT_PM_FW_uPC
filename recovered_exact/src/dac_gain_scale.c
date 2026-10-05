#include <stdint.h>

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_dac_final_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/dac_prepare.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * pm_dac_packet pm_dac_prepare_calibrated(uint16_t input, uint8_t channel,
 *                                         uint16_t calibration)
 * {
 *     pm_dac_packet p;
 *     p.value = (uint16_t)~(uint16_t)(pm_scale_unsigned(0x020cu, input) + calibration);
 *     p.input = calibration;
 *     p.product = (uint16_t)(2u * (input >> 8));
 *     p.control = (uint8_t)((uint8_t)(channel * 4u) | 3u);
 *     p.reserved = 0;
 *     return p;
 * }
 */

void pm_dac_gain_scale(void) asm("FUN_code_001068");
void pm_dac_gain_scale(void)
{
    register uint16_t requested asm("r20");
    asm volatile("" : "=r" (requested));
    register uint16_t coefficient asm("r18") = 20000;
    coefficient -= requested; asm volatile("" : "+r" (coefficient));
    requested = coefficient; asm volatile("" : "+r" (requested));
    coefficient = 0x0272;
    asm volatile("rcall FUN_code_0010a6" : "+r" (coefficient), "+r" (requested) : : "r0", "r1", "r16", "r17", "memory", "cc");
    register uint8_t channel asm("r22");
    asm volatile("" : "=r" (channel));
    channel += channel; asm volatile("" : "+r" (channel));
    channel += channel;
    asm volatile("rcall dac_send_value" : "+r" (channel) : : "memory", "cc");
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
uint32_t pm_logical_dac_gain_scale(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x20d0: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8402;
    }
    case 0x20d2: { // ldi r18, 0x20
        s->r[18] = 32;
        return 8404;
    }
    case 0x20d4: { // ldi r19, 0x4E
        s->r[19] = 78;
        return 8406;
    }
    case 0x20d6: { // sub r18, r20
        s->r[18] = pm_sub(s, s->r[18], s->r[20], 0, false);
        return 8408;
    }
    case 0x20d8: { // sbc r19, r21
        s->r[19] = pm_sub(s, s->r[19], s->r[21], pm_getflag(s, CARRY), true);
        return 8410;
    }
    case 0x20da: { // movw r20, r18
        uint16_t pair = pm_pointer(s, 18);
        pm_setpointer(s, 20, pair);
        return 8412;
    }
    case 0x20dc: { // ldi r18, 0x72
        s->r[18] = 114;
        return 8414;
    }
    case 0x20de: { // ldi r19, 0x02
        s->r[19] = 2;
        return 8416;
    }
    case 0x20e0: { // rcall .+106
        s->calls[s->call_depth++] = 8418;
        return 8524;
    }
    case 0x20e2: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8420;
    }
    case 0x20e4: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8422;
    }
    case 0x20e6: { // rcall .+450
        s->calls[s->call_depth++] = 8424;
        return 8874;
    }
    case 0x20e8: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 8426;
    }
    case 0x20ea: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
