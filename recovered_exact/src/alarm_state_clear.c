#include "legacy_cpu.h"
#include "legacy_interrupt.h"
#include "legacy_r16_c.h"
#define SET_VALUE(constant) do { value=(constant); asm volatile("" : "+r" (value)); } while (0)

void alarms_clear(void)
{
    register uint8_t value asm("r16"), alarms asm("r17");
    register const uint8_t *message asm("r30");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000e30\n\tcli"
                 : "=r" (value) : : "memory", "cc");
    value = pm_read_absolute(0x2157);
    alarms = GPIOR0;
    alarms &= 3; asm volatile("" : "+r" (alarms));
    asm goto("brne %l[fault_present]" : : "r" (alarms) : : fault_present);
    value = pm_read_absolute(0x2157);
    value &= 0x7f; asm volatile("" : "+r" (value));
    PM_RAM8(0x2157) = value;
    value = pm_read_absolute(0x2158);
    value &= 0xe1; asm volatile("" : "+r" (value));
    PM_RAM8(0x2158) = value;
    pm_cpu_enable_irq();
    SET_VALUE(0x40); PORTA_OUTSET = value;
    message = (const uint8_t *)0x2a08;
    asm volatile("" : "+z" (message));
    goto send;
fault_present:
    value &= 7; asm volatile("" : "+r" (value));
    if (value == 1) goto restart;
    pm_cpu_enable_irq();
    message = (const uint8_t *)0x299e;
    asm volatile("" : "+z" (message));
    goto send;
restart:
    value = pm_read_absolute(0x2157);
    value &= 0x1f; asm volatile("" : "+r" (value));
    PM_RAM8(0x2157) = value;
    PM_RAM8(0x2158) = pm_scratch_zero();
    SET_VALUE(1); PM_RAM8(0x215b) = value;
    SET_VALUE(4); PORTE_OUTCLR = value;
    SET_VALUE(0xd0); PM_RAM8(0x215c) = value;
    SET_VALUE(7); PM_RAM8(0x215d) = value;
    asm volatile("cbi 0, 1" : : : "memory");
    SET_VALUE(0x41); PORTA_OUTSET = value;
    pm_cpu_enable_irq();
    message = (const uint8_t *)0x2998;
send:
    asm volatile("rcall cli_send_msg" : "+z" (message) : : "memory", "cc");
}
asm(".pushsection .text.alarms_clear,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
