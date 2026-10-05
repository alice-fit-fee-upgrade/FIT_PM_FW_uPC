#define PM_ISR_EXACT_SREG_READ
#include "legacy_interrupt_register_c.h"
#include "legacy_r16.h"

void PORTB_INT0_vect_isr(void)
{
    PM_ISR_ENTER_R16();
    uint8_t status = pm_read_absolute(0x2157);
    PM_RAM8(0x2157) = status | 0x60;
    PORTA_OUTCLR = 0x40;
    PORTE_OUTCLR = 4;
    PM_ISR_LEAVE_R16();
}
