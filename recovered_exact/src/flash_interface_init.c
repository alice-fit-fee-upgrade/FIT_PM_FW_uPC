#include <avr/io.h>

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_flash_init_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/flash_spi.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * void pm_flash_spi_init(void)
 * {
 *     PORTD_OUTCLR = 2;
 *     PORTE_OUTSET = 0x10;
 *     PORTE_DIRSET = 0xb0;
 *     SPIE_CTRL = 0x50;
 * }
 */

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
