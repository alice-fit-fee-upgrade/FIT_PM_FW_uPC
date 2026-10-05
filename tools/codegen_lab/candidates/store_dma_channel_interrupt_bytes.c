#include <stdint.h>

void probe(void) {
register uint8_t *p asm("r26"); register uint8_t lo asm("r18"),hi asm("r19");
asm volatile("" : "=x" (p), "=r" (lo), "=r" (hi)); *p++=lo; *p++=hi; asm volatile("" : "+x" (p));
}
