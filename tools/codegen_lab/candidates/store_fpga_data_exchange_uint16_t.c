#include <stdint.h>

void probe(void) {
register uint16_t *p asm("r28"); register uint16_t word asm("r16");
asm volatile("" : "=y" (p), "=r" (word)); *p++=word; asm volatile("" : "+y" (p));
}
