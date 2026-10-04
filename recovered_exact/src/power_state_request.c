#include <avr/io.h>
#define RAM8(address) (*(volatile uint8_t *)(address))

void pm_power_state_request(void) asm("FUN_code_00054e");
void pm_power_state_request(void)
{
    register uint8_t value asm("r16");
    asm volatile("" : "=r" (value));
    RAM8(0x215b) = value;
    PORTB_OUTCLR = 0x80;
    value = RAM8(0x2159);
    /* Retain the two early branches and the no-change return exactly. */
    asm goto("tst %0\n\tbreq %l[changed]\n\tcpi %0, 5\n\tbreq %l[changed]\n\tret"
        : : "r" (value) : "cc" : changed);
changed:
    RAM8(0x2159) = 4;
}
