#include <avr/io.h>

void CDCE62005_control_rst(void)
{
    register const uint8_t *cursor asm("r30") = (const uint8_t *)0x2916;
    asm volatile("" : "+z" (cursor));
    register uint8_t remaining asm("r20") = 10;
next_setting:
    {
        register uint8_t b0 asm("r16"), b1 asm("r17"), b2 asm("r18"), b3 asm("r19");
        asm volatile("lpm %0, Z+\n\tlpm %1, Z+\n\tlpm %2, Z+\n\tlpm %3, Z+"
            : "=r" (b0), "=r" (b1), "=r" (b2), "=r" (b3), "+z" (cursor)
            : "r" (remaining) : "memory");
        /* Retain CALL rather than RCALL and the original interrupt window. */
        asm volatile("cli\n\tcall CDCE62005_send_control_settings\n\tsei"
            : "+r" (b0), "+r" (b1), "+r" (b2), "+r" (b3) : : "memory", "cc");
    }
    asm volatile("dec %0" : "+r" (remaining) : : "cc");
    asm goto("brne %l[next_setting]" : : "r" (remaining) : : next_setting);
    register uint8_t mask asm("r16") = 0x0a;
    asm volatile("" : "+r" (mask));
    PORTF_INTCTRL = mask;
    mask = 0x20;
    asm volatile("" : "+r" (mask));
    PORTB_OUTCLR = mask;
}
