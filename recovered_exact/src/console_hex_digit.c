#include <stdint.h>

void cli_send_digit_hex(void)
{
    register uint8_t digit asm("r16");
    asm volatile("" : "=r" (digit));
    digit = (digit & 0x0f) + '0';
    asm volatile("" : "+r" (digit));
    /* Exact unsigned threshold branch without a second scratch register. */
    asm goto("cpi %0, 0x3a\n\tbrlo %l[send]" : : "r" (digit) : "cc" : send);
    digit += 'A' - '0' - 10;
send:
    asm volatile("rcall cli_send_buf" : "+r" (digit) : : "memory", "cc");
}
