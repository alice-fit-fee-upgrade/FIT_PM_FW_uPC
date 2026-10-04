#include "legacy_spi.h"

void dac_send_value(void)
{
    register uint8_t header asm("r22"), scratch asm("r23"), select asm("r24");
    asm volatile("" : "=r" (header));
    asm volatile("cli" : : : "memory");
    scratch = 0xd5;
    asm volatile("sts %0, %1" : : "n" (_SFR_MEM_ADDR(SPIC_CTRL)), "r" (scratch) : "memory");
    /* Preserve original selection ordering and its flags. */
    asm volatile("mov %0, %2\n\tldi %1, 1\n\tandi %0, 0x30\n\tbreq 1f\n"
                 "\tlsl %1\n\tsubi %0, 0x10\n\tbreq 1f\n\tlsl %1\n1:"
                 : "=r" (scratch), "=r" (select) : "r" (header) : "cc");
    scratch = header;
    asm volatile("" : "+r" (scratch));
    asm volatile("swap %0" : "+r" (header));
    header &= 0xc0;
    asm volatile("" : "+r" (header));
    scratch += scratch;
    asm volatile("" : "+r" (scratch));
    scratch &= 6;
    asm volatile("" : "+r" (scratch));
    header |= scratch;
    asm volatile("" : "+r" (header));
    header |= 0x10;
    asm volatile("" : "+r" (header));
    PORTC_OUTCLR = select;
    /* Polling overwrites the first outgoing byte register, as in the original. */
    asm volatile("sts %1, %0\n\t1: lds %0, %2\n\tsbrs %0, 7\n\trjmp 1b"
        : "+r" (header) : "n" (_SFR_MEM_ADDR(SPIC_DATA)), "n" (_SFR_MEM_ADDR(SPIC_STATUS)) : "memory");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    header = 7;
    asm volatile("" : "+r" (header));
    PORTC_OUTSET = header;
    asm volatile("sei" : : : "memory");
}
