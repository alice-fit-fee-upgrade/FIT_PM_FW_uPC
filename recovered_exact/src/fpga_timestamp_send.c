#include "legacy_spi.h"

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
    asm volatile("lpm %0, Z+\n\tlpm %1, Z+"
                 : "=r" (address), "=r" (offset), "+z" (cursor) : : "memory");
    PM_SPI_SEND_VALUE(SPIC, "r22", offset);
    PM_SPI_SEND_VALUE(SPIC, "r22", address);
    /* Original loop tests only ZL, sending precisely two words. */
    asm goto("cpi r30, 0x96\n\tbrne %l[next_word]" : : "z" (cursor) : "cc" : next_word);
    PM_WRITE_R22(PORTD_OUTSET, 1);
}
