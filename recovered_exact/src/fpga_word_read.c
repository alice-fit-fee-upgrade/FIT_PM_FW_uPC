#include "legacy_spi.h"

/* Legacy input R18 is an address; output word is returned in R17:R16.
 * R21 starts with the caller's bits, as in the original (no invented clear). */
void fpga_msg_read_t1(void)
{
    PM_WRITE_R22(SPIC_CTRL, 0xd1);
    register uint8_t mask asm("r22") = 1;
    register uint8_t address asm("r18");
    register uint8_t address_low asm("r21");
    asm volatile("" : "=r" (address), "=r" (address_low));
    asm volatile("lsr %0\n\tror %1\n\tlsr %0\n\tror %1\n\tori %0, 0x80"
                 : "+r" (address), "+r" (address_low) : "r" (mask) : "cc");
    asm volatile("sts %0, %1" : : "n" (_SFR_MEM_ADDR(PORTD_OUTCLR)), "r" (mask) : "memory");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r18");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r21");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_READ_REGISTER(SPIC, "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    PM_SPI_READ_REGISTER(SPIC, "r16");
    PM_WRITE_R22(PORTD_OUTSET, 1);
}
