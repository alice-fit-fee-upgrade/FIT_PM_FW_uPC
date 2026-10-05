#include <stdint.h>

void probe(void) {
register uint16_t *p asm("r28"); register uint16_t word asm("r20");
asm volatile("" : "=y" (p)); word = *p++;
asm volatile("" : "+y" (p) : "r" (word));
}
