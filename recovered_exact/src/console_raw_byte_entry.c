#include <stdint.h>

/* Shared exact tail restores the register saved here and returns to caller. */
void cli_get_next_byte(void)
{
    asm volatile("push r19" : : : "memory");
    register uint8_t mode asm("r19") = 1;
    asm volatile("rjmp LAB_code_001420" : : "r" (mode) : "memory", "cc");
    __builtin_unreachable();
}
