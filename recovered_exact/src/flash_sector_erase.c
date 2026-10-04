#include <avr/io.h>

void pm_flash_sector_erase(void) asm("FUN_code_000b88");
void pm_flash_sector_erase(void)
{
    /* Original callees use a register ABI and two-byte relative calls. */
    asm volatile ("rcall FUN_code_000b8f" : : : "r16", "r19", "memory", "cc");
    register uint8_t command asm("r16") = 0xd8;
    asm volatile ("rcall FUN_code_000c04" : "+r" (command)
        : : "r17", "r19", "memory", "cc");
    PORTE_OUTSET = 0x10;
}
