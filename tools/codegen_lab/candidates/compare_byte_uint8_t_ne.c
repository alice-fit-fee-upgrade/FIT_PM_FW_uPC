#include <stdint.h>

void probe(void) {
register uint8_t value asm("r16"); asm volatile("" : "=r" (value)); if (value != 13) *(volatile uint8_t*)0x2157 = 1;
}
