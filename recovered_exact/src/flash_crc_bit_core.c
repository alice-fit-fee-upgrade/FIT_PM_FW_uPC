#include <stdint.h>

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
    asm volatile("add r18, r18\n\tadc r19, r19\n\tadc r16, r16\n\tadc r17, r17"
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
