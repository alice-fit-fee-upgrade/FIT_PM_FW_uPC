#include <stdint.h>

void probe(void) {
register uint16_t *p asm("r26"); register uint16_t word asm("r18");
asm volatile("" : "=x" (p), "=r" (word)); *p++=word; asm volatile("" : "+x" (p));
}
