#include "legacy_cpu.h"
#include "legacy_spi_c.h"

void dac_send_value(void)
{
    register uint8_t header asm("r22"), scratch asm("r23"), select asm("r24");
    asm volatile("" : "=r" (header));
    pm_cpu_disable_irq();
    scratch = 0xd5;
    asm volatile("" : "+r" (scratch));
    SPIC_CTRL = scratch;
    /* C value equivalent of selection (not an accepted replacement):
     * scratch = header; select = 1; scratch &= 0x30;
     * if (scratch != 0) {
     *     select <<= 1; scratch -= 0x10;
     *     if (scratch != 0) select <<= 1;
     * }
     * Split C copy/constant trials403/431 expanded the fixed region. Keep
     * exact ordering/flags; no standalone functional test is claimed. */
    scratch = header;
    asm volatile("" : "+r" (scratch), "+r" (header));
    select = 1; asm volatile("" : "+r" (select));
    scratch &= 0x30;
    if (!scratch) goto selected;
    select += select; asm volatile("" : "+r" (select));
    scratch -= 0x10;
    if (!scratch) goto selected;
    select += select; asm volatile("" : "+r" (select));
selected:
    scratch = header;
    asm volatile("" : "+r" (scratch));
    header = __builtin_avr_swap(header); asm volatile("" : "+r" (header));
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
    SPIC_DATA = header;
    PM_SPI_WAIT_AT(SPIC_STATUS, "r22");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    header = 7;
    asm volatile("" : "+r" (header));
    PORTC_OUTSET = header;
    pm_cpu_enable_irq();
}
