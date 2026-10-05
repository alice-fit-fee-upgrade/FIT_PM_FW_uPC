#include "legacy_cpu.h"
#include <avr/io.h>
#include "legacy_interrupt.h"
#include "legacy_r16_c.h"
#define SET_VALUE(constant) do { value=(constant); asm volatile("" : "+r" (value)); } while (0)

void PORTE_INT0_vect_isr(void)
{
    asm volatile("push r31" : : : "memory");
    {
        register uint8_t saved_status asm("r31") = SREG;
        asm volatile("" : "+r" (saved_status) : : "memory");
    }
    pm_cpu_disable_irq();
    asm volatile("push r16\n\tpush r17\n\tpush r18" : : : "memory");
    register uint8_t value asm("r16"), flags asm("r17"), changed asm("r18");
    asm volatile("clr %0" : "=r" (flags) : : "cc");
    value = PORTE_IN;
    asm volatile("bst %0, 1\n\tbld %1, 0\n\tbst %0, 3\n\tbld %1, 3\n\tbst %0, 2"
        : "+r" (value), "+r" (flags) : : "cc");
    value = pm_read_absolute(0x2157);
    changed = value; asm volatile("" : "+r" (changed));
    value &= 0xf6; asm volatile("" : "+r" (value));
    value |= flags; asm volatile("" : "+r" (value));
    PM_RAM8(0x2157) = value;
    changed ^= flags; asm volatile("" : "+r" (changed));
    if (!(changed & (1u << 0))) goto watch_retry;
    if (flags & (1u << 0)) goto power_on;
    SET_VALUE(4);
    PORTE_OUTCLR = value;
    GPIOR0 |= (1u << 1);
    asm volatile("rcall system_deinit" : "=r" (value) : : "memory", "cc");
    goto update_led;
power_on:
    flags = 0xd0; asm volatile("" : "+r" (flags));
    changed = 7; asm volatile("" : "+r" (changed));
store_retry:
    SET_VALUE(1);
    PM_RAM8(0x215b) = value;
    PM_RAM8(0x215c) = flags;
    PM_RAM8(0x215d) = changed;
    GPIOR0 &= (uint8_t)~(1u << 1);
    goto update_led;
watch_retry:
    if (!(changed & (1u << 3))) goto finished;
    if (flags & (1u << 3)) goto finished;
    value = pm_read_absolute(0x215b);
    if (value != 2) goto deinitialize;
    value = pm_read_absolute(0x2442);
    if (value == 0) goto deinitialize;
    asm volatile("dec %0" : "+r" (value) : : "cc");
    PM_RAM8(0x2442) = value;
    SET_VALUE(4);
    PORTE_OUTCLR = value;
    flags = 0x88; asm volatile("" : "+r" (flags));
    changed = 0x13; asm volatile("" : "+r" (changed));
    goto store_retry;
deinitialize:
    asm volatile("rcall system_deinit" : "=r" (value) : : "memory", "cc");
    asm goto("brtc %l[finished]" : : : : finished);
    GPIOR0 |= (1u << 1);
update_led:
    asm volatile("rcall set_status_and_vd8_led" : "=r" (value) : : "memory", "cc");
finished:
    asm volatile("pop r18\n\tpop r17\n\tpop r16" : : : "memory");
    {
        register uint8_t saved_status asm("r31");
        asm volatile("" : "=r" (saved_status) : : "memory");
        SREG = saved_status;
    }
    /* C operation: restore_private_frame_and_return_from_interrupt();
     * POP/RETI retain the original interrupt frame and return contract. */
    asm volatile("pop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}
