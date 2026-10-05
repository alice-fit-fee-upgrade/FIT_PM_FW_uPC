#include "legacy_spi_c.h"
#include <avr/io.h>
/* Archived C alternative for the exact implementation below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_flash_enable_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/flash_spi.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint8_t pm_flash_write_enable(void)
 * {
 *     PORTE_OUTCLR = 0x10;
 *     SPIE_DATA = 6;
 *     uint8_t status;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     PORTE_OUTSET = 0x10;
 *     return status;
 * }
 */

/* Exact polling fragment; reserve legacy R16 across the helper so the
 * following C write reloads its historical scratch value. */
static inline void wait_spie_complete(void)
{
    PM_SPI_WAIT_AT(SPIE_STATUS, "r19");
    /* The original helper also invalidates the R16 constant, forcing reload. */
    asm volatile("" : : : "r16", "memory");
}
void pm_flash_write_enable(void) asm("FUN_code_000b8f");
void pm_flash_write_enable(void)
{
    PORTE_OUTCLR = 0x10;
    SPIE_DATA = 0x06;
    wait_spie_complete();
    PORTE_OUTSET = 0x10;
}
