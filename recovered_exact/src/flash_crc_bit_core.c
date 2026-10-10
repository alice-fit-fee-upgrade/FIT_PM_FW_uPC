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
