#include <avr/io.h>
#define RAM(a) (*(volatile uint8_t *)(a))
#include "legacy_r16.h"
void system_deinit(void)
{
    uint8_t zero = pm_scratch_zero();
    RAM(0x215c)=zero; RAM(0x215d)=zero; RAM(0x215b)=zero;
    RAM(0x2159)=zero; RAM(0x2441)=zero; RAM(0x2006)=zero; RAM(0x2162)=zero;
    PORTE_INTCTRL=1; PORTF_INTCTRL=2;
    zero=pm_scratch_constant(0);
    PORTB_INTCTRL=zero; PORTD_INTCTRL=zero;
    PORTB_DIRCLR=0xbf;
    SPIC_CTRL=pm_scratch_zero();
    PORTC_DIRCLR=0xff; PORTD_DIRCLR=0x41; PORTD_OUTCLR=4;
    PORTF_OUTCLR=0x20; PORTF_DIRCLR=pm_scratch_constant(0x20);
    RAM(0x2157)=pm_read_absolute(0x2157)&0xef;
    PORTA_OUTSET=0xa0;
    zero=pm_scratch_zero();
    SPID_CTRL=zero; DMA_CTRL=zero;
}
