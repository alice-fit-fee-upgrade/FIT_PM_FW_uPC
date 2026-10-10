#include <avr/io.h>
#include "legacy_cpu.h"
#include "legacy_interrupt.h"
#include "legacy_r16_c.h"
#define SET_VALUE(constant) do { value=(constant); asm volatile("" : "+r" (value)); } while (0)

void alarms_clear(void)
{
    register uint8_t value asm("r16"), alarms asm("r17");
    register const uint8_t *message asm("r30");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000e30"
                 : "=r" (value) : : "memory", "cc");
    pm_cpu_disable_irq();
    value = pm_read_absolute(0x2157);
    alarms = GPIOR0; asm volatile("" : "+r" (alarms));
    alarms &= 3;
    if (alarms) goto fault_present;
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
    GPIOR0 &= (uint8_t)~(1u << 1);
    SET_VALUE(0x41); PORTA_OUTSET = value;
    pm_cpu_enable_irq();
    message = (const uint8_t *)0x2998;
send:
    asm volatile("rcall cli_send_msg" : "+z" (message) : : "memory", "cc");
}
asm(".pushsection .text.alarms_clear,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* REFERENCE_C_INFERRED: ordinary-C equivalents of the call fragments:
 *     if (legacy_console_getc() != '\r') goto legacy_command_error;
 *     legacy_console_send_flash(message);
 * The explicit C branches above describe alarm clearing/restart. Calls need
 * adapters for the original R16/Z/SREG contracts. The trailing RJMP is the
 * existing shared error continuation, not an additional alarm-clear action.
 * No independent replacement validation is claimed for this sketch.
 */
