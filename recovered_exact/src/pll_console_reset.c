#include "legacy_cpu.h"
#include <stdint.h>

void cdce62005_rst(void)
{
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000d1e" : : : "r16", "memory", "cc");
    register const uint8_t *cursor asm("r30") = (const uint8_t *)0x293e;
    asm volatile("" : "+z" (cursor));
    register uint8_t count asm("r20") = 9;
    asm volatile("" : "+r" (count));
    pm_cpu_disable_irq();
next_setting:
    {
        register uint8_t b0 asm("r16"), b1 asm("r17"), b2 asm("r18"), b3 asm("r19");
        asm volatile("lpm %0, Z+\n\tlpm %1, Z+\n\tlpm %2, Z+\n\tlpm %3, Z+\n\trcall CDCE62005_send_control_settings"
            : "=r" (b0), "=r" (b1), "=r" (b2), "=r" (b3), "+z" (cursor) : : "memory", "cc");
    }
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_setting]" : : "r" (count) : : next_setting);
    pm_cpu_enable_irq();
    cursor = (const uint8_t *)0x2998;
    asm volatile("rcall cli_send_msg" : "+z" (cursor) : : "memory", "cc");
}
