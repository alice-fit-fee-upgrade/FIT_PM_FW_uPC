#include <stdint.h>

/* Legacy result is R21:R20, delimiter R16, validity in carry.
 * Original accepts up to four uppercase hex digits and wraps at 16 bits. */
void cli_get_hex(void)
{
    register uint8_t count asm("r18"), character asm("r16");
    register uint16_t result asm("r20");
    asm volatile("push r22\n\tpush r18\n\tclr r18\n\tclr r20\n\tclr r21"
                 : "=r" (count), "=r" (result) : : "memory", "cc");
next_digit:;
    asm volatile("rcall cli_get_next_char" : "=r" (character) : : "memory", "cc");
    register uint8_t digit asm("r22") = character;
    asm volatile("" : "+r" (digit), "+r" (character));
    asm goto("cpi r16, 0x30\n\tbrcs %l[done]\n\tcpi r16, 0x3a\n\tbrcs %l[decode]\n"
             "cpi r16, 0x41\n\tbrcs %l[done]\n\tcpi r16, 0x47\n\tbrcc %l[done]"
             : : "r" (character) : "cc" : done, decode);
    digit -= 7; asm volatile("" : "+r" (digit));
decode:
    digit -= '0'; asm volatile("" : "+r" (digit));
    result += result; asm volatile("" : "+r" (result));
    result += result; asm volatile("" : "+r" (result));
    result += result; asm volatile("" : "+r" (result));
    result += result; asm volatile("" : "+r" (result));
    result |= digit; asm volatile("" : "+r" (result));
    asm volatile("inc %0" : "+r" (count) : : "cc");
    asm goto("cpi %0, 4\n\tbrne %l[next_digit]" : : "r" (count) : "cc" : next_digit);
    asm volatile("rcall cli_get_next_char" : "=r" (character) : : "memory", "cc");
done:
    asm volatile("tst r18\n\tbreq 1f\n\tclc\n\trjmp 2f\n1:\n\tsec\n2:\n"
                 "pop r18\n\tpop r22\n\tret" : : "r" (count), "r" (result) : "memory", "cc");
    __builtin_unreachable();
}
