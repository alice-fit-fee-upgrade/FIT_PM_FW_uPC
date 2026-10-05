#include "legacy_cpu.h"
#include "legacy_interrupt.h"
#include "legacy_r16_c.h"

void PORTD_INT0_vect_isr(void)
{
    PM_ISR_ENTER_R16();
    register uint8_t value asm("r16") = PORTD_IN;
    asm volatile("bst %0, 3\n\tcli" : : "r" (value) : "memory", "cc");
    value = pm_read_absolute(0x2157);
    asm volatile("bld %0, 4" : "+r" (value) : : "cc");
    PM_RAM8(0x2157) = value;
    pm_cpu_enable_irq();
    asm goto("brtc %l[cleared]" : : : : cleared);
    pm_cpu_disable_irq();
    value = 5;
    asm volatile("rcall FUN_code_00054e\n\tsei" : "+r" (value) : : "memory", "cc");
    goto leave;
cleared:
    PORTE_INTCTRL = 1;
    PM_RAM8(0x2158) = pm_scratch_zero();
    value = pm_read_absolute(0x2157);
    PM_RAM8(0x2157) = value & 0x7f;
    PORTA_OUTSET = 0xc0;
    PM_RAM8(0x2441) = 1;
leave:
    PM_ISR_LEAVE_R16();
}
