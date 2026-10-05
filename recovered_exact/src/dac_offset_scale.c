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
 * pm_dac_packet pm_dac_prepare_inverse(uint16_t input, uint8_t channel)
 * {
 *     pm_dac_packet p;
 *     p.input = (uint16_t)(20000u - input);
 *     p.value = pm_scale_unsigned(0x0272u, p.input);
 *     p.product = (uint16_t)(2u * (p.input >> 8));
 *     p.control = (uint8_t)(channel * 4u);
 *     p.reserved = 0;
 *     return p;
 * }
 */

void pm_dac_offset_scale(void) asm("FUN_code_001053");
void pm_dac_offset_scale(void)
{
    register uint16_t coefficient asm("r18") = 0x020c;
    register uint16_t value asm("r16");
    asm volatile("rcall FUN_code_0010a6" : "+r" (coefficient), "=r" (value) : : "r0", "r1", "memory", "cc");
    register const uint8_t *cursor asm("r30") = (const uint8_t *)0x2163;
    asm volatile("" : "+z" (cursor));
    register uint16_t correction asm("r20");
    asm volatile("clr r20" : "=r" (correction) : : "cc");
    register uint8_t channel asm("r22");
    asm volatile("" : "=r" (channel));
    channel += channel; asm volatile("" : "+r" (channel));
    {
        register uint8_t address_low asm("r30");
        asm volatile("" : "=r" (address_low) : "z" (cursor));
        address_low += channel; asm volatile("" : "+r" (address_low));
    }
    asm volatile("adc r31, r20" : "=z" (cursor) : "r" (correction) : "cc");
    channel += channel; asm volatile("" : "+r" (channel));
    channel |= 3; asm volatile("" : "+r" (channel));
    asm volatile("ld r20, Z+" : "=r" (correction), "+z" (cursor) : : "memory");
    { register uint8_t high asm("r21") = *cursor;
      asm volatile("" : "+r" (high) : : "memory"); }
    asm volatile("" : "=r" (correction));
    value += correction; asm volatile("" : "+r" (value));
    value = ~value;
    asm volatile("rcall dac_send_value" : "+r" (value), "+r" (channel) : : "memory", "cc");
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 336 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_dac_offset_scale(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x20a6: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8360;
    }
    case 0x20a8: { // ldi r18, 0x0C
        s->r[18] = 12;
        return 8362;
    }
    case 0x20aa: { // ldi r19, 0x02
        s->r[19] = 2;
        return 8364;
    }
    case 0x20ac: { // rcall .+158
        s->calls[s->call_depth++] = 8366;
        return 8524;
    }
    case 0x20ae: { // ldi r30, 0x63
        s->r[30] = 99;
        return 8368;
    }
    case 0x20b0: { // ldi r31, 0x21
        s->r[31] = 33;
        return 8370;
    }
    case 0x20b2: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 8372;
    }
    case 0x20b4: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8374;
    }
    case 0x20b6: { // add r30, r22
        s->r[30] = pm_add(s, s->r[30], s->r[22], 0);
        return 8376;
    }
    case 0x20b8: { // adc r31, r20
        s->r[31] = pm_add(s, s->r[31], s->r[20], pm_getflag(s, CARRY));
        return 8378;
    }
    case 0x20ba: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8380;
    }
    case 0x20bc: { // ori r22, 0x03
        s->r[22] |= 3;
        pm_nzv(s, s->r[22], false);
        return 8382;
    }
    case 0x20be: { // ld r20, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[20] = pm_read(s, address);
        return 8384;
    }
    case 0x20c0: { // ld r21, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[21] = pm_read(s, address);
        return 8386;
    }
    case 0x20c2: { // add r16, r20
        s->r[16] = pm_add(s, s->r[16], s->r[20], 0);
        return 8388;
    }
    case 0x20c4: { // adc r17, r21
        s->r[17] = pm_add(s, s->r[17], s->r[21], pm_getflag(s, CARRY));
        return 8390;
    }
    case 0x20c6: { // com r16
        s->r[16] = ~s->r[16];
        pm_nzv(s, s->r[16], false);
        pm_flag(s, CARRY, true);
        return 8392;
    }
    case 0x20c8: { // com r17
        s->r[17] = ~s->r[17];
        pm_nzv(s, s->r[17], false);
        pm_flag(s, CARRY, true);
        return 8394;
    }
    case 0x20ca: { // rcall .+478
        s->calls[s->call_depth++] = 8396;
        return 8874;
    }
    case 0x20cc: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 8398;
    }
    case 0x20ce: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
