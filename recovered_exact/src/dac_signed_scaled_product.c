#include <stdint.h>

/* Original 0x2130: historical symbol fpga_is_ready is a signed scaler.
 * Keep MUL/MULSU, rounding and carry propagation exact; C expresses the
 * low-byte result copy and final high-byte sum. R1 is intentionally live.
 * Archived, binary-different C alternative below passed 796,432 direct
 * functional comparisons with its ASM bridge (docs/mixed_c_asm_checkpoint.md).
 * The bridge restores the private registers/flags; no timing, asynchronous
 * IRQ or hardware equivalence is established. Not compiled in this baseline.
 * source_recovery/src/scaling.c:
 * uint16_t pm_scale_signed(uint16_t coefficient, uint16_t input)
 * {
 *     int32_t value = input < 0x8000u ? (int32_t)input : (int32_t)input - 65536;
 *     int32_t product = value * coefficient;
 *     // Only bits 8..23 survive the 16-bit return. Unsigned shifting of * the modulo-2^32 product has exactly those bits for either sign; * no implementation-defined signed shift or runtime division is needed.
 *     uint32_t rounded_bits = (uint32_t)product + 128u;
 *     return (uint16_t)(rounded_bits >> 8);
 * }
 */
void fpga_is_ready(void)
{
    register uint8_t low asm("r16"), high asm("r17"), product_high asm("r1"), product_low asm("r0");
    asm volatile("clr r17\n\tmul r18, r20\n\tsbrc r0, 7\n\tinc r1"
                 : "=r" (high), "=r" (product_high) : : "r0", "cc");
    low = product_high;
    asm volatile("" : "+r" (low));
    register uint8_t cross_low asm("r0");
    asm volatile("mul r19, r20" : "=r" (cross_low) : : "r1", "cc");
    low += cross_low;
    asm volatile("" : "+r" (low));
    /* C high-byte value: high += product_high + carry;
     * ADC preserves the exact live SREG.C and dirty R1 contract. */
    asm volatile("adc %0, r1" : "+r" (high) : : "cc");
    asm volatile("mulsu r21, r18" : "=r" (cross_low) : : "r1", "cc");
    low += cross_low;
    asm volatile("" : "+r" (low));
    asm volatile("adc %0, r1" : "+r" (high) : : "cc");
    asm volatile("mulsu r21, r19" : "=r" (product_low) : : "r1", "cc");
    high += product_low;
    asm volatile("" : : "r" (low), "r" (high));
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
uint32_t pm_logical_dac_signed_scaled_product(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2130: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 8498;
    }
    case 0x2132: { // mul r18, r20
        uint16_t product = (s->r[18]) * (int)s->r[20];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8500;
    }
    case 0x2134: { // sbrc r0, 7
        return (!!(s->r[0] & (1u << 7)) == 0) ? 8504 : 8502;
    }
    case 0x2136: { // inc r1
        s->r[1]++;
        pm_nzv(s, s->r[1], s->r[1] == 128);
        return 8504;
    }
    case 0x2138: { // mov r16, r1
        s->r[16] = s->r[1];
        return 8506;
    }
    case 0x213a: { // mul r19, r20
        uint16_t product = (s->r[19]) * (int)s->r[20];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8508;
    }
    case 0x213c: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 8510;
    }
    case 0x213e: { // adc r17, r1
        s->r[17] = pm_add(s, s->r[17], s->r[1], pm_getflag(s, CARRY));
        return 8512;
    }
    case 0x2140: { // mulsu r21, r18
        uint16_t product = ((int8_t)s->r[21]) * (int)s->r[18];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8514;
    }
    case 0x2142: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 8516;
    }
    case 0x2144: { // adc r17, r1
        s->r[17] = pm_add(s, s->r[17], s->r[1], pm_getflag(s, CARRY));
        return 8518;
    }
    case 0x2146: { // mulsu r21, r19
        uint16_t product = ((int8_t)s->r[21]) * (int)s->r[19];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8520;
    }
    case 0x2148: { // add r17, r0
        s->r[17] = pm_add(s, s->r[17], s->r[0], 0);
        return 8522;
    }
    case 0x214a: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
