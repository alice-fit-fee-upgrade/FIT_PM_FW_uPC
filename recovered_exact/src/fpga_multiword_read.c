#include "legacy_spi.h"

void pm_fpga_multiword_read(void) asm("FUN_code_0011e5");
/* Original eight-byte response is returned in R9:R8 ... R15:R14. */
void pm_fpga_multiword_read(void)
{
    PM_WRITE_R22(SPIC_CTRL, 0xd1);
    register uint8_t address asm("r18") = 0xbc;
    register uint8_t dummy asm("r21");
    asm volatile("clr %0" : "=r" (dummy) : "r" (address) : "cc");
    PM_WRITE_R22(PORTD_OUTCLR, 1);
    PM_SPI_SEND_VALUE(SPIC, "r22", address);
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r9");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r8");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r11");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r10");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r13");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r12");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r15");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r14");
    PM_WRITE_R22(PORTD_OUTSET, 1);
}
