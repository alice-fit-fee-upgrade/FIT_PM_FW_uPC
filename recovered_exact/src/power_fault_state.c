#include <avr/io.h>
#include "legacy_r16.h"
#define RAM(a) (*(volatile uint8_t *)(a))

void pm_power_fault_state(void) asm("FUN_code_0005b4");
void pm_power_fault_state(void)
{
    PORTE_INTCTRL = 1;
    PORTC_OUTCLR = 8;
    PORTB_OUTCLR = 0xa0;
    PORTA_OUTSET = 0xc0;
    uint8_t zero = pm_scratch_zero();
    RAM(0x2158) = zero;
    RAM(0x2159) = zero;
    RAM(0x2157) = pm_read_absolute(0x2157) & 0x7f;
}
