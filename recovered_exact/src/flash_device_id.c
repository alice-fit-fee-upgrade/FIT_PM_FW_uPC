#include "legacy_spi_c.h"

void pm_flash_device_id(void) asm("FUN_code_000c20");
/* JEDEC ID bytes return in original R18, R19, R16 order. */
void pm_flash_device_id(void)
{
    PORTE_OUTCLR = 0x10;
    SPIE_DATA = 0x9f;
    register uint8_t dummy asm("r20");
    asm volatile("clr %0" : "=r" (dummy) : : "cc");
    PM_SPI_WAIT_AT(SPIE_STATUS, "r19");
    PM_SPI_SEND_VALUE(SPIE, "r19", dummy);
    PM_SPI_READ_REGISTER(SPIE, "r18");
    PM_SPI_SEND_VALUE(SPIE, "r19", dummy);
    PM_SPI_READ_REGISTER(SPIE, "r19");
    /* Final polling deliberately reuses and overwrites the dummy register. */
    SPIE_DATA = dummy;
    PM_SPI_WAIT_AT(SPIE_STATUS, "r20");
    PM_SPI_READ_REGISTER(SPIE, "r16");
    register uint8_t mask asm("r20") = 0x10;
    asm volatile("" : "+r" (mask));
    PORTE_OUTSET = mask;
}
