/* Native outer frame: GCC emits the original entry PUSH under the local
 * call-saved register profile. -fno-ipa-pure-const prevents noreturn inference
 * from dropping that save before the exact shared RET/RETI tail. The tail
 * restores this register in ASM; full FLASH identity validates the pairing. */
#include <stdint.h>

/* Shared exact tail restores the register saved here and returns to caller. */
void FUN_code_00139f(void)
{
    register uint8_t mode asm("r20") = 3;
    asm volatile("sec\n\trjmp LAB_code_0013aa" : : "r" (mode) : "memory", "cc");
    __builtin_unreachable();
}
