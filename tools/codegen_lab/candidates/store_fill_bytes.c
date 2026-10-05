#include <stdint.h>

void probe(void) {
register uint8_t *p asm("r26"); register uint8_t v asm("r16"); asm volatile("" : "=x" (p), "=r" (v)); *p++=v; *p++=v; *p++=v; asm volatile("" : "+x" (p));
}
