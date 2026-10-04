#include "legacy_spi.h"

void pm_flash_device_id(void) asm("FUN_code_000c20");
/* JEDEC ID bytes return in original R18, R19, R16 order. */
void pm_flash_device_id(void)
{
    PORTE_OUTCLR = 0x10;
    SPIE_DATA = 0x9f;
    register uint8_t dummy asm("r20");
    asm volatile("clr %0\n\t1: lds r19, %1\n\tsbrs r19, 7\n\trjmp 1b"
                 : "=r" (dummy) : "n" (_SFR_MEM_ADDR(SPIE_STATUS)) : "r19", "memory", "cc");
    PM_SPI_SEND_VALUE(SPIE, "r19", dummy);
    PM_SPI_READ_REGISTER(SPIE, "r18");
    PM_SPI_SEND_VALUE(SPIE, "r19", dummy);
    PM_SPI_READ_REGISTER(SPIE, "r19");
    /* Final polling deliberately reuses and overwrites the dummy register. */
    asm volatile("sts %1, %0\n\t1: lds %0, %2\n\tsbrs %0, 7\n\trjmp 1b"
                 : "+r" (dummy) : "n" (_SFR_MEM_ADDR(SPIE_DATA)),
                   "n" (_SFR_MEM_ADDR(SPIE_STATUS)) : "memory");
    PM_SPI_READ_REGISTER(SPIE, "r16");
    register uint8_t mask asm("r20") = 0x10;
    asm volatile("sts %0, %1" : : "n" (_SFR_MEM_ADDR(PORTE_OUTSET)), "r" (mask) : "memory");
}
