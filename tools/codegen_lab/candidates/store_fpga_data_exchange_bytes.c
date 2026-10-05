#include <stdint.h>

void probe(void) {
register uint8_t *p asm("r28"); register uint8_t lo asm("r16"),hi asm("r17");
asm volatile("" : "=y" (p), "=r" (lo), "=r" (hi)); *p++=lo; *p++=hi; asm volatile("" : "+y" (p));
}
