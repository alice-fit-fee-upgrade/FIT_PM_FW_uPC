#include "legacy_spi_c.h"

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_flash_wait_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/flash_spi.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 * For flash_wait_ready only the SPI status transaction is described; the delay
 * and outer busy loop remained ASM in the historical tested implementation.
 *
 * uint16_t pm_flash_read_status_transaction(void)
 * {
 *     uint8_t status;
 *     PORTE_OUTCLR = 0x10;
 *     SPIE_DATA = 5;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     SPIE_DATA = 0;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     uint8_t data = SPIE_DATA;
 *     PORTE_OUTSET = 0x10;
 *     return (uint16_t)(((uint16_t)status << 8) | data);
 * }
 */

void pm_flash_wait_ready(void) asm("FUN_code_000b9d");
void pm_flash_wait_ready(void)
{
retry:
    {
        register uint16_t delay asm("r24") = 0x0640;
        asm volatile("1: sbiw %0, 1\n\tbrne 1b" : "+w" (delay) : : "cc");
    }
    register uint8_t command asm("r16") = 0x10;
    asm volatile("" : "+r" (command));
    PORTE_OUTCLR = command;
    command = 5;
    asm volatile("" : "+r" (command));
    SPIE_DATA = command;
    PM_SPI_WAIT_AT(SPIE_STATUS, "r19");
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r24");
    register uint8_t status asm("r24");
    status = SPIE_DATA;
    asm volatile("" : "+r" (status));
    command = 0x10;
    asm volatile("" : "+r" (command));
    PORTE_OUTSET = command;
    if (status & (1u << 0)) goto retry;
}
