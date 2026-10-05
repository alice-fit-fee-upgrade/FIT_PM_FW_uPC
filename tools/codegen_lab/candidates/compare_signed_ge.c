#include <stdint.h>

void probe(void) {
register int16_t value asm("r20"); asm volatile("" : "=r" (value)); if (value >= 501) *(volatile uint8_t*)0x2157=1;
}
