#include <stdint.h>

void probe(void) {
register int16_t *p asm("r26"); register int16_t word asm("r18");
asm volatile("" : "=x" (p), "=r" (word)); *p++=word; asm volatile("" : "+x" (p));
}
