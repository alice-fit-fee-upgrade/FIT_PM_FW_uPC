#include <stdint.h>

void probe(void) {
register int16_t *p asm("r28"); register int16_t word asm("r16");
asm volatile("" : "=y" (p)); word = *p++;
asm volatile("" : "+y" (p) : "r" (word));
}
