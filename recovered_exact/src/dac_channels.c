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
 * pm_dac_packet pm_dac_prepare_signed(uint16_t input, uint8_t channel,
 *                                     uint8_t command)
 * {
 *     pm_dac_packet p;
 *     p.value = (uint16_t)(pm_scale_signed(0x4188u, input) ^ 0x8000u);
 *     p.input = input;
 *     int16_t high = (int16_t)(input >> 8);
 *     if (high >= 128) high -= 256;
 *     p.product = (uint16_t)(high * 65);
 *     p.control = (uint8_t)((uint8_t)(channel * 4u) | command);
 *     p.reserved = 0;
 *     return p;
 * }
 */

/* Fragment C equivalent: scaled_high ^= sign_mask;
 * Corrected trials357/358 changed region sizes. Step372 passed exact-check
 * with a read/write R18 barrier preventing constant-folding of the mask.
 * Functional evidence above applies to the historical complete alternative
 * with its bridge, not to this independently compiled fragment. */
#define DAC_CHANNEL(name, selector) \
void name(void) { \
    register uint16_t scale asm("r18") = 0x4188; \
    asm volatile("rcall fpga_is_ready" : "+r" (scale) : \
                 : "r0", "r1", "r16", "r17", "memory", "cc"); \
    register uint8_t sign_mask asm("r18") = 0x80; \
    register uint8_t scaled_high asm("r17"); \
    asm volatile("" : "=r" (scaled_high), "+r" (sign_mask)); \
    scaled_high ^= sign_mask; \
    asm volatile("" : "+r" (scaled_high)); \
    register uint8_t channel asm("r22"); \
    asm volatile("" : "=r" (channel)); \
    channel += channel; \
    asm volatile("" : "+r" (channel)); \
    channel += channel; \
    asm volatile("" : "+r" (channel)); \
    channel |= (selector); \
    asm volatile("rcall dac_send_value" : "+r" (channel) : \
                 : "r16", "r17", "r19", "r20", "r21", "memory", "cc"); \
}
/* Original helper's name is historical: fpga_is_ready performs multiplication. */
DAC_CHANNEL(dac_set_value_2, 2)
DAC_CHANNEL(dac_set_value, 1)

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 384 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_dac_channels(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x20ec: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8430;
    }
    case 0x20ee: { // ldi r18, 0x88
        s->r[18] = 136;
        return 8432;
    }
    case 0x20f0: { // ldi r19, 0x41
        s->r[19] = 65;
        return 8434;
    }
    case 0x20f2: { // rcall .+60
        s->calls[s->call_depth++] = 8436;
        return 8496;
    }
    case 0x20f4: { // ldi r18, 0x80
        s->r[18] = 128;
        return 8438;
    }
    case 0x20f6: { // eor r17, r18
        s->r[17] ^= s->r[18];
        pm_nzv(s, s->r[17], false);
        return 8440;
    }
    case 0x20f8: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8442;
    }
    case 0x20fa: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8444;
    }
    case 0x20fc: { // ori r22, 0x02
        s->r[22] |= 2;
        pm_nzv(s, s->r[22], false);
        return 8446;
    }
    case 0x20fe: { // rcall .+426
        s->calls[s->call_depth++] = 8448;
        return 8874;
    }
    case 0x2100: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 8450;
    }
    case 0x2102: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x2104: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8454;
    }
    case 0x2106: { // ldi r18, 0x88
        s->r[18] = 136;
        return 8456;
    }
    case 0x2108: { // ldi r19, 0x41
        s->r[19] = 65;
        return 8458;
    }
    case 0x210a: { // rcall .+36
        s->calls[s->call_depth++] = 8460;
        return 8496;
    }
    case 0x210c: { // ldi r18, 0x80
        s->r[18] = 128;
        return 8462;
    }
    case 0x210e: { // eor r17, r18
        s->r[17] ^= s->r[18];
        pm_nzv(s, s->r[17], false);
        return 8464;
    }
    case 0x2110: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8466;
    }
    case 0x2112: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8468;
    }
    case 0x2114: { // ori r22, 0x01
        s->r[22] |= 1;
        pm_nzv(s, s->r[22], false);
        return 8470;
    }
    case 0x2116: { // rcall .+402
        s->calls[s->call_depth++] = 8472;
        return 8874;
    }
    case 0x2118: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 8474;
    }
    case 0x211a: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
