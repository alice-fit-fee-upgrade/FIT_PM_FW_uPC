#include "legacy_spi_c.h"

/* Legacy input R18 is an address; output word is returned in R17:R16.
 * R21 starts with the caller's bits, as in the original (no invented clear). */
void fpga_msg_read_t1(void)
{
    PM_WRITE_R22(SPIC_CTRL, 0xd1);
    register uint8_t mask asm("r22") = 1;
    register uint8_t address asm("r18");
    register uint8_t address_low asm("r21");
    asm volatile("" : "=r" (address), "=r" (address_low));
    asm volatile("" : "+r" (address), "+r" (address_low), "+r" (mask));
    address >>= 1;
    asm volatile("ror %1" : "+r" (address), "+r" (address_low) : : "cc");
    address >>= 1;
    asm volatile("ror %1" : "+r" (address), "+r" (address_low) : : "cc");
    address |= 0x80;
    asm volatile("" : "+r" (address));
    asm volatile("" : "+r" (mask));
    PORTD_OUTCLR = mask;
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r18");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r21");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_READ_REGISTER(SPIC, "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    PM_SPI_READ_REGISTER(SPIC, "r16");
    PM_WRITE_R22(PORTD_OUTSET, 1);
}
