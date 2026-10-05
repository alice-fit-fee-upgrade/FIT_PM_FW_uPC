#include "legacy_cpu.h"
#include <avr/io.h>
#include "legacy_r16.h"
#define RAM(a) (*(volatile uint8_t *)(a))
void pm_flash_interface_deinit(void) asm("FUN_code_000b3f");
void pm_flash_interface_deinit(void)
{
    pm_cpu_disable_irq();
    uint8_t zero=pm_scratch_zero();
    PORTD_INTCTRL=zero; SPIE_CTRL=zero;
    PORTE_DIRCLR=0xb0;
    asm volatile ("cbi 0, 1" : : : "memory");
    PORTA_OUTSET=1; PORTD_OUTSET=2;
    RAM(0x215c)=0x88; RAM(0x215d)=0x13;
    register uint8_t state asm("r16")=4;
    asm volatile ("rcall FUN_code_00054e" : "+r" (state) : : "memory", "cc");
    pm_cpu_enable_irq();
}
