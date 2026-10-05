#include <stdint.h>
struct bytes { uint8_t lo,hi; };
void probe(void) {
register struct bytes *p asm("r28"); register uint8_t lo asm("r16"),hi asm("r17");
asm volatile("" : "=y" (p), "=r" (lo), "=r" (hi)); p->lo=lo; p->hi=hi; ++p; asm volatile("" : "+y" (p));
}
