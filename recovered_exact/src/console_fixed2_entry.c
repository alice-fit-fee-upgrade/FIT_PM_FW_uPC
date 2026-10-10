#include <stdint.h>

/* Shared exact tail restores the register saved here and returns to caller. */
void FUN_code_001397(void)
{
    register uint8_t mode asm("r20") = 2;
    asm volatile("sec\n\trjmp LAB_code_0013aa" : : "r" (mode) : "memory", "cc");
    __builtin_unreachable();
}
