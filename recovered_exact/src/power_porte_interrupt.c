#include "legacy_interrupt.h"
#include "legacy_r16.h"
#define SET_VALUE(constant) do { value=(constant); asm volatile("" : "+r" (value)); } while (0)

void PORTE_INT0_vect_isr(void)
{
    asm volatile("push r31\n\tin r31, 0x3f\n\tcli\n\tpush r16\n\tpush r17\n\tpush r18" : : : "memory");
    register uint8_t value asm("r16"), flags asm("r17"), changed asm("r18");
    asm volatile("clr %0" : "=r" (flags) : : "cc");
    value = PORTE_IN;
    asm volatile("bst %0, 1\n\tbld %1, 0\n\tbst %0, 3\n\tbld %1, 3\n\tbst %0, 2"
        : "+r" (value), "+r" (flags) : : "cc");
    value = pm_read_absolute(0x2157);
    changed = value; asm volatile("" : "+r" (changed));
    value &= 0xf6; asm volatile("" : "+r" (value));
    asm volatile("or %0, %1" : "+r" (value) : "r" (flags) : "cc");
    PM_RAM8(0x2157) = value;
    asm volatile("eor %0, %1" : "+r" (changed) : "r" (flags) : "cc");
    asm goto("sbrs %0, 0\n\trjmp %l[watch_retry]" : : "r" (changed) : : watch_retry);
    asm goto("sbrc %0, 0\n\trjmp %l[power_on]" : : "r" (flags) : : power_on);
    SET_VALUE(4);
    PORTE_OUTCLR = value;
    asm volatile("sbi 0, 1\n\trcall system_deinit" : "=r" (value) : : "memory", "cc");
    goto update_led;
power_on:
    flags = 0xd0; asm volatile("" : "+r" (flags));
    changed = 7; asm volatile("" : "+r" (changed));
store_retry:
    SET_VALUE(1);
    PM_RAM8(0x215b) = value;
    PM_RAM8(0x215c) = flags;
    PM_RAM8(0x215d) = changed;
    asm volatile("cbi 0, 1" : : : "memory");
    goto update_led;
watch_retry:
    asm goto("sbrs %0, 3\n\trjmp %l[finished]" : : "r" (changed) : : finished);
    asm goto("sbrc %0, 3\n\trjmp %l[finished]" : : "r" (flags) : : finished);
    value = pm_read_absolute(0x215b);
    asm goto("cpi %0, 2\n\tbrne %l[deinitialize]" : : "r" (value) : "cc" : deinitialize);
    value = pm_read_absolute(0x2442);
    asm goto("tst %0\n\tbreq %l[deinitialize]" : : "r" (value) : "cc" : deinitialize);
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
    asm volatile("sbi 0, 1" : : : "memory");
update_led:
    asm volatile("rcall set_status_and_vd8_led" : "=r" (value) : : "memory", "cc");
finished:
    asm volatile("pop r18\n\tpop r17\n\tpop r16\n\tout 0x3f, r31\n\tpop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}
