#include <avr/io.h>
/* Exact polling fragment; reserve legacy R16 across the helper so the
 * following C write reloads its historical scratch value. */
static inline void wait_spie_complete(void)
{
    asm volatile (
        "1: lds r19, %[status]\n\t"
        "sbrs r19, 7\n\t"
        "rjmp 1b"
        : : [status] "n" (_SFR_MEM_ADDR(SPIE_STATUS)) : "r16", "r19", "memory");
}
void pm_flash_write_enable(void) asm("FUN_code_000b8f");
void pm_flash_write_enable(void)
{
    PORTE_OUTCLR = 0x10;
    SPIE_DATA = 0x06;
    wait_spie_complete();
    PORTE_OUTSET = 0x10;
}
