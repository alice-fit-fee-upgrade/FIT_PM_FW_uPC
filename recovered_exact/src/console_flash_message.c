#include <stdint.h>

void cli_send_msg(void)
{
    register const uint8_t *cursor asm("r30");
    asm volatile("" : "=z" (cursor));
next_character:
    {
        register uint8_t byte asm("r16");
        asm volatile("lpm %0, Z+" : "=r" (byte), "+z" (cursor) : : "memory");
        asm goto("tst %0\n\tbreq %l[finished]" : : "r" (byte) : "cc" : finished);
        asm volatile("rcall cli_send_buf" : "+r" (byte) : : "memory", "cc");
    }
    goto next_character;
finished:
    /* Cursor remains advanced, including the consumed terminator. */
    asm volatile("" : : "z" (cursor));
}
