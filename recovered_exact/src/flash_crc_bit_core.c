#include <stdint.h>

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_crc_byte_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/crc_byte.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * pm_crc_packet pm_crc_byte_abi(uint32_t crc, uint32_t polynomial, uint8_t byte)
 * {
 *     uint8_t last_half = 0;
 *     uint8_t last_carry = (uint8_t)(byte >> 7);
 *     for (uint8_t bit = 0; bit < 8; ++bit) {
 *         last_half = (uint8_t)((uint8_t)(crc >> 24) & 8u);
 *         uint8_t feedback = (uint8_t)((crc >> 31) ^ (byte & 1u));
 *         crc <<= 1;
 *         if (feedback) crc ^= polynomial;
 *         byte >>= 1;
 *     }
 *     pm_crc_packet p;
 *     p.crc = crc;
 *     p.flags = (uint8_t)(2u | last_carry | (last_half << 2));
 *     p.reserved[0] = p.reserved[1] = p.reserved[2] = 0;
 *     return p;
 * }
 */

/* Original CRC state order is R17:R16:R19:R18. Bit input is consumed LSB
 * first. Carry-dependent decisions stay in ASM; polynomial XORs are C. */
void FUN_code_000bf2(void)
{
    register uint8_t state0 asm("r18"), state1 asm("r19"), state2 asm("r16"), state3 asm("r17");
    register uint8_t polynomial0 asm("r23"), polynomial1 asm("r24"), polynomial2 asm("r25"), polynomial3 asm("r26");
    asm volatile("" : "=r" (state0), "=r" (state1), "=r" (state2), "=r" (state3),
                 "=r" (polynomial0), "=r" (polynomial1), "=r" (polynomial2), "=r" (polynomial3));
    register uint8_t bits asm("r20") = 8;
    asm volatile("" : "+r" (bits));
next_bit:
    state0 += state0;
    asm volatile("" : "+r" (state0));
    /* C value equivalent of the remaining carry chain, using pre-shift bytes:
     * uint8_t carry = old_state0 >> 7;
     * uint8_t next = state1 >> 7;
     * state1 = (uint8_t)((state1 << 1) | carry); carry = next;
     * next = state2 >> 7;
     * state2 = (uint8_t)((state2 << 1) | carry); carry = next;
     * next = state3 >> 7;
     * state3 = (uint8_t)((state3 << 1) | carry); carry = next;
     * The following branch consumes the final CPU carry. This comment is
     * explanatory; the tested complete C alternative and its bridge scope
     * are documented above (mixed_crc_byte_checkpoint.md). */
    asm volatile("adc r19, r19\n\tadc r16, r16\n\tadc r17, r17"
                 : "+r" (state0), "+r" (state1), "+r" (state2), "+r" (state3) : : "cc");
    asm goto("brcc 1f\n\tlsr r22\n\tbrcs %l[finished_bit]\n\trjmp %l[xor_polynomial]\n1:\n"
             "lsr r22\n\tbrcc %l[finished_bit]" : : : "r22", "cc" : finished_bit, xor_polynomial);
xor_polynomial:
    state0 ^= polynomial0; asm volatile("" : "+r" (state0));
    state1 ^= polynomial1; asm volatile("" : "+r" (state1));
    state2 ^= polynomial2; asm volatile("" : "+r" (state2));
    state3 ^= polynomial3; asm volatile("" : "+r" (state3));
finished_bit:
    asm volatile("dec %0" : "+r" (bits) : : "cc");
    asm goto("brne %l[next_bit]" : : : : next_bit);
    asm volatile("" : : "r" (state0), "r" (state1), "r" (state2), "r" (state3));
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 288 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_crc_bit_core(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x17e4: { // ldi r20, 0x08
        s->r[20] = 8;
        return 6118;
    }
    case 0x17e6: { // add r18, r18
        s->r[18] = pm_add(s, s->r[18], s->r[18], 0);
        return 6120;
    }
    case 0x17e8: { // adc r19, r19
        s->r[19] = pm_add(s, s->r[19], s->r[19], pm_getflag(s, CARRY));
        return 6122;
    }
    case 0x17ea: { // adc r16, r16
        s->r[16] = pm_add(s, s->r[16], s->r[16], pm_getflag(s, CARRY));
        return 6124;
    }
    case 0x17ec: { // adc r17, r17
        s->r[17] = pm_add(s, s->r[17], s->r[17], pm_getflag(s, CARRY));
        return 6126;
    }
    case 0x17ee: { // brcc .+6
        return (pm_getflag(s, 0) == 0) ? 6134 : 6128;
    }
    case 0x17f0: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 6130;
    }
    case 0x17f2: { // brcs .+14
        return (pm_getflag(s, 0) == 1) ? 6146 : 6132;
    }
    case 0x17f4: { // rjmp .+4
        return 6138;
    }
    case 0x17f6: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 6136;
    }
    case 0x17f8: { // brcc .+8
        return (pm_getflag(s, 0) == 0) ? 6146 : 6138;
    }
    case 0x17fa: { // eor r18, r23
        s->r[18] ^= s->r[23];
        pm_nzv(s, s->r[18], false);
        return 6140;
    }
    case 0x17fc: { // eor r19, r24
        s->r[19] ^= s->r[24];
        pm_nzv(s, s->r[19], false);
        return 6142;
    }
    case 0x17fe: { // eor r16, r25
        s->r[16] ^= s->r[25];
        pm_nzv(s, s->r[16], false);
        return 6144;
    }
    case 0x1800: { // eor r17, r26
        s->r[17] ^= s->r[26];
        pm_nzv(s, s->r[17], false);
        return 6146;
    }
    case 0x1802: { // dec r20
        s->r[20]--;
        pm_nzv(s, s->r[20], s->r[20] == 127);
        return 6148;
    }
    case 0x1804: { // brne .-32
        return (pm_getflag(s, 1) == 0) ? 6118 : 6150;
    }
    case 0x1806: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
