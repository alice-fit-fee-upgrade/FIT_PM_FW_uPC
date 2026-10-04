#include "legacy_spi.h"

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
