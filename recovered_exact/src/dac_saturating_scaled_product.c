#include <stdint.h>

/* Exact scaled product uses MUL and live R1, including its original rounding.
 * C represents the overflow result; the arithmetic/flag helper is unchanged. */
void FUN_code_0010a6(void)
{
    register uint16_t result asm("r16");
    asm volatile("clr r17\n\tmul r18, r20\n\tsbrc r0, 7\n\tinc r1\n\tmov r16, r1\n"
                 "mul r19, r20\n\tadd r16, r0\n\tadc r17, r1\n"
                 "mul r18, r21\n\tadd r16, r0\n\tadc r17, r1\n\tmul r19, r21"
                 : "=r" (result) : : "r0", "r1", "cc");
    asm goto("tst r1\n\tbrne %l[overflow]\n\tadd r17, r0\n\tbrcs %l[overflow]" : : : "cc" : overflow);
    asm volatile("ret"); __builtin_unreachable();
overflow:
    result = UINT16_MAX;
    asm volatile("" : : "r" (result));
}
