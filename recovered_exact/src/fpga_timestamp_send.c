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
 * void pm_fpga_stamp_c(void)
 * {
 *     SPIC_CTRL=0xd1;
 *     PORTD_OUTCLR=1;
 *     fpga_byte(0x3d); fpga_byte(0x40);
 *     uint16_t address=0x2b92;
 *     for (uint8_t words=0;words!=2;++words) {
 *         uint8_t low=pgm_read_byte(address++);
 *         uint8_t high=pgm_read_byte(address++);
 *         fpga_byte(high); fpga_byte(low);
 *     }
 *     PORTD_OUTSET=1;
 * }
 */

void fpga_send_mcu_ts(void)
{
    register const uint8_t *cursor asm("r30") = (const uint8_t *)0x2b92;
    /* Keep the original pointer setup before configuring SPI. */
    asm volatile("" : "+z" (cursor));
    PM_WRITE_R22(SPIC_CTRL, 0xd1);
    register uint8_t address asm("r18") = 0x3d;
    register uint8_t offset asm("r21") = 0x40;
    asm volatile("" : "+r" (address), "+r" (offset));
    PM_WRITE_R22(PORTD_OUTCLR, 1);
    PM_SPI_SEND_VALUE(SPIC, "r22", address);
    PM_SPI_SEND_VALUE(SPIC, "r22", offset);
next_word:
    /* C FLASH read equivalent with const __flash uint8_t *cursor:
     * address = *cursor++; offset = *cursor++;
     * Trials 239-249 could not reproduce the private pointer/register layout;
     * keep exact LPM Z+ instructions. This is explanatory, with no new
     * independent functional-test claim. Existing historical test scope,
     * when available, is documented above. */
    asm volatile("lpm %0, Z+\n\tlpm %1, Z+"
                 : "=r" (address), "=r" (offset), "+z" (cursor) : : "memory");
    PM_SPI_SEND_VALUE(SPIC, "r22", offset);
    PM_SPI_SEND_VALUE(SPIC, "r22", address);
    /* Original loop tests only ZL, sending precisely two words. */
    asm goto("cpi r30, 0x96\n\tbrne %l[next_word]" : : "z" (cursor) : "cc" : next_word);
    PM_WRITE_R22(PORTD_OUTSET, 1);
}
