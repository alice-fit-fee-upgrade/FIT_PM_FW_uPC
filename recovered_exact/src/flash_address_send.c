#include "legacy_spi_c.h"

/* Archived C alternative for the exact implementation below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_flash_address_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/flash_spi.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint8_t pm_flash_send_address(uint8_t command, uint8_t high,
 *                               uint8_t middle, uint8_t low)
 * {
 *     uint8_t status;
 *     PORTE_OUTCLR = 0x10;
 *     SPIE_DATA = command;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     SPIE_DATA = high;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     SPIE_DATA = middle;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     SPIE_DATA = low;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     return status;
 * }
 */

void pm_flash_address_send(void) asm("FUN_code_000c04");
void pm_flash_address_send(void)
{
    PORTE_OUTCLR = 0x10;
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r16"); /* Command. */
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r22"); /* Address high byte. */
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r21");
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r20"); /* Address low byte. */
}
