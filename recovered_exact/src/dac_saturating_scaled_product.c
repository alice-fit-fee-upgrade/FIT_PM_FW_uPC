#include <stdint.h>

/* Archived C alternative for the exact multiply/round/saturate ASM below.
 * pm_scale_unsigned_abi was integrated with a private-ABI ASM bridge in the
 * historical binary-different mixed build. Recorded functional validation:
 * docs/mixed_unsigned_checkpoint.md and docs/mixed_recovery_wave_checkpoint.md
 * (796,432 direct comparisons). The bridge restores registers, R1:R0 and SREG;
 * this returned value/flags packet alone is not the original CPU ABI.
 * Dependencies: source_recovery/src/scaling.c and pm_recovered.h.
 * No timing, asynchronous IRQ or hardware equivalence is established.
 * This alternative is a comment, not an accepted binary-exact implementation.
 *
 * uint32_t pm_scale_unsigned_abi(uint16_t coefficient, uint16_t input,
 *                              uint8_t entry_sreg)
 * {
 *     uint16_t low = (uint16_t)((coefficient & 255u) * (input & 255u));
 *     uint16_t middle = (uint16_t)((low >> 8) + ((low & 128u) != 0));
 *     middle = (uint16_t)(middle + (coefficient >> 8) * (input & 255u));
 *     uint16_t cross = (uint16_t)((coefficient & 255u) * (input >> 8));
 *     uint8_t carry = (uint16_t)((middle & 255u) + (cross & 255u)) > 255u;
 *     uint8_t half = ((middle >> 8) & 15u) + ((cross >> 8) & 15u) + carry > 15u;
 *     middle = (uint16_t)(middle + cross);
 *     uint16_t top = (uint16_t)((coefficient >> 8) * (input >> 8));
 *     uint8_t flags;
 *     uint16_t value;
 *     if (top >> 8) {
 *         uint8_t negative = (uint8_t)(top >> 15);
 *         flags = (uint8_t)(negative * 0x15u + half * 0x20u);
 *         value = 65535u;
 *     } else {
 *         uint8_t a = (uint8_t)(middle >> 8), b = (uint8_t)top;
 *         uint16_t sum = (uint16_t)a + b;
 *         uint8_t result = (uint8_t)sum;
 *         uint8_t negative = result >> 7;
 *         uint8_t overflow = (uint8_t)((~(a ^ b) & (a ^ result)) >> 7);
 *         flags = (uint8_t)((sum > 255u) | ((result == 0) << 1) |
 *                 (negative << 2) | (overflow << 3) |
 *                 ((negative ^ overflow) << 4) |
 *                 ((((a & 15u) + (b & 15u)) > 15u) << 5));
 *         value = sum > 255u ? 65535u : (uint16_t)((middle & 255u) | (sum << 8));
 *     }
 *     return (uint32_t)value | ((uint32_t)(flags | (entry_sreg & 0xc0u)) << 16);
 * }
 */

/* Exact scaled product uses MUL and live R1, including its original rounding.
 * C represents overflow and low partial-product sums; MUL and upper carry
 * helpers retain the exact private register/flag contract. */
void FUN_code_0010a6(void)
{
    register uint16_t result asm("r16");
    register uint8_t low asm("r16"), high asm("r17"), product_high asm("r1");
    asm volatile("clr r17\n\tmul r18, r20\n\tsbrc r0, 7\n\tinc r1"
                 : "=r" (high), "=r" (product_high) : : "r0", "cc");
    low = product_high;
    asm volatile("" : "+r" (low));
    /* C value equivalent: add each partial product into low/high with carry.
     * The historically tested complete C alternative and bridge are above;
     * this exact helper also preserves dirty R1 and live arithmetic flags. */
    register uint8_t cross_low asm("r0");
    asm volatile("mul r19, r20" : "=r" (cross_low) : : "r1", "cc");
    low += cross_low;
    asm volatile("" : "+r" (low));
    /* C high-byte value: high += product_high + carry;
     * ADC preserves the exact live SREG.C and dirty R1 contract. */
    asm volatile("adc %0, r1" : "+r" (high) : : "cc");
    asm volatile("mul r18, r21" : "=r" (cross_low) : : "r1", "cc");
    low += cross_low;
    asm volatile("" : "+r" (low));
    asm volatile("adc %0, r1" : "+r" (high) : : "cc");
    asm volatile("mul r19, r21" : "=r" (cross_low) : : "r1", "cc");
    /* C value for the final ADD: high += cross_low;
     * Trials412/414/415 changed region size, even with local tail merging
     * disabled. No new standalone functional-test claim; the complete
     * historical alternative with its bridge is documented above.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("tst r1\n\tbrne %l[overflow]\n\tadd r17, r0\n\tbrcs %l[overflow]" : : : "cc" : overflow);
    return;
overflow:
    result = UINT16_MAX;
    asm volatile("" : : "r" (result));
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 320 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_dac_saturating_scaled_product(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x214c: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 8526;
    }
    case 0x214e: { // mul r18, r20
        uint16_t product = (s->r[18]) * (int)s->r[20];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8528;
    }
    case 0x2150: { // sbrc r0, 7
        return (!!(s->r[0] & (1u << 7)) == 0) ? 8532 : 8530;
    }
    case 0x2152: { // inc r1
        s->r[1]++;
        pm_nzv(s, s->r[1], s->r[1] == 128);
        return 8532;
    }
    case 0x2154: { // mov r16, r1
        s->r[16] = s->r[1];
        return 8534;
    }
    case 0x2156: { // mul r19, r20
        uint16_t product = (s->r[19]) * (int)s->r[20];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8536;
    }
    case 0x2158: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 8538;
    }
    case 0x215a: { // adc r17, r1
        s->r[17] = pm_add(s, s->r[17], s->r[1], pm_getflag(s, CARRY));
        return 8540;
    }
    case 0x215c: { // mul r18, r21
        uint16_t product = (s->r[18]) * (int)s->r[21];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8542;
    }
    case 0x215e: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 8544;
    }
    case 0x2160: { // adc r17, r1
        s->r[17] = pm_add(s, s->r[17], s->r[1], pm_getflag(s, CARRY));
        return 8546;
    }
    case 0x2162: { // mul r19, r21
        uint16_t product = (s->r[19]) * (int)s->r[21];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8548;
    }
    case 0x2164: { // and r1, r1
        s->r[1] &= s->r[1];
        pm_nzv(s, s->r[1], false);
        return 8550;
    }
    case 0x2166: { // brne .+6
        return (pm_getflag(s, 1) == 0) ? 8558 : 8552;
    }
    case 0x2168: { // add r17, r0
        s->r[17] = pm_add(s, s->r[17], s->r[0], 0);
        return 8554;
    }
    case 0x216a: { // brcs .+2
        return (pm_getflag(s, 0) == 1) ? 8558 : 8556;
    }
    case 0x216c: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x216e: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 8560;
    }
    case 0x2170: { // ldi r17, 0xFF
        s->r[17] = 255;
        return 8562;
    }
    case 0x2172: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
