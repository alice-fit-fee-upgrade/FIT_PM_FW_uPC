#include <stdint.h>

/* Original decimal parser: delimiter in R16, result in R21:R20, carry for
 * failure. MUL overflow and signed-negation decisions retain exact flags. */
void cli_get_integer(void)
{
    register uint8_t count asm("r18"), negative asm("r19");
    register uint16_t result asm("r20");
    asm volatile("push r22\n\tpush r17\n\tpush r18\n\tpush r19\n\tpush r1\n\tpush r0\n\tclr r18"
                 : "=r" (count) : : "memory", "cc");
    register uint8_t radix asm("r17") = 10;
    asm volatile("clr r19\n\tclr r20\n\tclr r21" : "=r" (negative), "=r" (result) : "r" (radix) : "cc");
next_digit:;
    register uint8_t character asm("r16");
    asm volatile("rcall cli_get_next_char" : "=r" (character) : : "memory", "cc");
    /* Rejected C selection/range alternatives (steps 236-238):
     * if (count != 0 || character != '-') goto validate;
     * if ((int8_t)character < '0' || (int8_t)character >= ':') goto done;
     * if (count == 1) goto error;
     * if (negative == 0) goto success;
     * if (count == 2) goto error;
     * These alternatives changed complete binary/layout matching. They have
     * no independent successful functional-test claim. Retain exact branches. */
    asm goto("tst r18\n\tbrne %l[validate]\n\tcpi r16, 0x2d\n\tbrne %l[validate]"
             : : "r" (count), "r" (character) : "cc" : validate);
    asm volatile("inc r19\n\tinc r18" : "+r" (negative), "+r" (count) : : "cc");
    goto next_digit;
validate:
    asm volatile("inc r18" : "+r" (count) : : "cc");
    asm goto("cpi r16, 0x30\n\tbrlt %l[done]\n\tcpi r16, 0x3a\n\tbrge %l[done]"
             : : "r" (character) : "cc" : done);
    register uint8_t digit asm("r22") = character;
    asm volatile("" : "+r" (digit));
    digit -= '0'; asm volatile("" : "+r" (digit));
    asm volatile("mul r20, r17" : "+r" (result) : "r" (radix) : "r0", "r1", "cc");
    {
        register uint8_t partial asm("r0");
        asm volatile("" : "=r" (partial));
        partial += digit;
        asm volatile("" : "+r" (partial));
    }
    asm volatile("clr r22\n\tadc r1, r22" : "+r" (digit) : : "r1", "cc");
    {
        register uint8_t old_high asm("r21");
        asm volatile("" : "=r" (old_high));
        digit = old_high;
        asm volatile("" : "+r" (digit));
    }
    /* R1 is a live nonzero product byte; keep this private copy in ASM. */
    asm volatile("mov r21, r1" : : : "cc");
    {
        register uint8_t partial asm("r0"), low asm("r20");
        asm volatile("" : "=r" (partial));
        low = partial;
        asm volatile("" : "+r" (low));
    }
    asm volatile("mul r17, r22" : : "r" (radix), "r" (digit) : "r0", "r1", "cc");
    asm volatile("" : "=r" (result), "=r" (digit));
    asm goto("tst r1\n\tbrne %l[error]" : : : "cc" : error);
    {
        register uint8_t high asm("r21"), partial asm("r0");
        asm volatile("" : "=r" (high), "=r" (partial));
        high += partial;
        asm volatile("" : "+r" (high));
    }
    asm goto("brcc %l[next_digit]\n\trjmp %l[error]" : : : : error, next_digit);
    __builtin_unreachable();
done:
    asm goto("cpi r18, 1\n\tbreq %l[error]\n\ttst r19\n\tbreq %l[success]\n\tcpi r18, 2\n\tbreq %l[error]"
             : : "r" (count), "r" (negative) : "cc" : error, success);
    asm volatile("clr r19" : "+r" (negative) : : "cc");
    {
        register uint8_t result_low asm("r20");
        asm volatile("" : "=r" (result_low));
        result_low = (uint8_t)-result_low;
        asm volatile("" : "+r" (result_low));
    }
    asm volatile("adc r21, r19" : : "r" (negative) : "cc");
    {
        register uint8_t result_high asm("r21");
        asm volatile("" : "=r" (result_high));
        result_high = (uint8_t)-result_high;
        asm volatile("" : "+r" (result_high));
    }
    asm goto("brpl %l[error]" : : : : error);
success:
    asm volatile("clc" : : : "cc");
    goto restore;
error:
    asm volatile("sec" : : : "cc");
restore:
    asm volatile("pop r0\n\tpop r1\n\tpop r19\n\tpop r18\n\tpop r17\n\tpop r22" : : : "memory");
    return;
}
