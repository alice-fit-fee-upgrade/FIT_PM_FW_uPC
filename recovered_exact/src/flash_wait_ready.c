#include "legacy_spi.h"

void pm_flash_wait_ready(void) asm("FUN_code_000b9d");
void pm_flash_wait_ready(void)
{
retry:
    {
        register uint16_t delay asm("r24") = 0x0640;
        asm volatile("1: sbiw %0, 1\n\tbrne 1b" : "+w" (delay) : : "cc");
    }
    register uint8_t command asm("r16") = 0x10;
    asm volatile("" : "+r" (command));
    PORTE_OUTCLR = command;
    command = 5;
    asm volatile("" : "+r" (command));
    SPIE_DATA = command;
    asm volatile("1: lds r19, %0\n\tsbrs r19, 7\n\trjmp 1b"
                 : : "n" (_SFR_MEM_ADDR(SPIE_STATUS)) : "r19", "memory");
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r24");
    register uint8_t status asm("r24");
    asm volatile("lds %0, %1" : "=r" (status) : "n" (_SFR_MEM_ADDR(SPIE_DATA)) : "memory");
    command = 0x10;
    asm volatile("" : "+r" (command));
    PORTE_OUTSET = command;
    asm goto("sbrc %0, 0\n\trjmp %l[retry]" : : "r" (status) : : retry);
}
