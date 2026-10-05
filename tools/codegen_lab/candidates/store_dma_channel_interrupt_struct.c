#include <stdint.h>
struct bytes { uint8_t lo,hi; };
void probe(void) {
register struct bytes *p asm("r26"); register uint8_t lo asm("r18"),hi asm("r19");
asm volatile("" : "=x" (p), "=r" (lo), "=r" (hi)); p->lo=lo; p->hi=hi; ++p; asm volatile("" : "+x" (p));
}
