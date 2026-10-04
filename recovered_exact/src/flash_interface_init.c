#include <avr/io.h>

/* Original entry 0x1664; legacy scratch/output register is R16.
 * Per-file compiler flags restrict temporaries to that register. */
void pm_flash_interface_init(void) asm("FUN_code_000b32");
void pm_flash_interface_init(void)
{
    PORTD_OUTCLR = 0x02;
    PORTE_OUTSET = 0x10;
    PORTE_DIRSET = 0xb0;
    SPIE_CTRL = 0x50;
}
