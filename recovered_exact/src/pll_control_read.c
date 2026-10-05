#include "legacy_spi_c.h"

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_reads_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/device_reads.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint32_t pm_pll_read_c(void)
 * {
 *     pm_pll_write_unmasked(0x8e);
 *     PORTF_OUTCLR=0x10;
 *     uint8_t b0=pll_byte();
 *     uint8_t b1=pll_byte();
 *     uint8_t b2=pll_byte();
 *     uint8_t b3=pll_byte();
 *     PORTF_OUTSET=0x10;
 *     return (uint32_t)b0 | ((uint32_t)b1<<8) | ((uint32_t)b2<<16) | ((uint32_t)b3<<24);
 * }
 */

void pm_pll_control_read(void) asm("FUN_code_001267");
/* Select the original readback command, then return four bytes in R16..R19. */
void pm_pll_control_read(void)
{
    register uint8_t command asm("r16") = 0x8e;
    asm volatile("clr r17\n\tclr r18\n\tclr r19\n\trcall CDCE62005_send_control_settings"
                 : "+r" (command) : : "r17", "r18", "r19", "memory", "cc");
    PM_WRITE_R22(PORTF_OUTCLR, 0x10);
    register uint8_t dummy asm("r23");
    asm volatile("clr %0" : "=r" (dummy) : : "cc");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r23");
    PM_SPI_READ_REGISTER(SPIC, "r16");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r23");
    PM_SPI_READ_REGISTER(SPIC, "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r23");
    PM_SPI_READ_REGISTER(SPIC, "r18");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r23");
    PM_SPI_READ_REGISTER(SPIC, "r19");
    PM_WRITE_R22(PORTF_OUTSET, 0x10);
}
