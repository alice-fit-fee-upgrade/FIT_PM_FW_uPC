#include "legacy_spi.h"

/* R18 is the incoming address; never borrow it for peripheral constants. */

void fpga_msg_send_t2(void)
{
    PM_WRITE_R22(SPIC_CTRL, 0xd1);
    register uint8_t address asm("r18");
    register uint8_t address_low asm("r21");
    asm volatile("" : "=r" (address));
    /* Preserve the original address packing and its flag effects. */
    asm volatile("clr %1\n\tlsr %0\n\tror %1\n\tlsr %0\n\tror %1"
                 : "+r" (address), "=r" (address_low) : : "cc");
    PM_WRITE_R22(PORTD_OUTCLR, 1);
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r18");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r21");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    PM_WRITE_R22(PORTD_OUTSET, 1);
}
