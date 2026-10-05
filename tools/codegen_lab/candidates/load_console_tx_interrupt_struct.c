#include <stdint.h>
struct bytes { uint8_t lo,hi; };
void probe(void) {
register struct bytes *p asm("r30"); register uint8_t lo asm("r16"), hi asm("r17");
asm volatile("" : "=z" (p)); lo=p->lo; hi=p->hi; ++p;
asm volatile("" : "+z" (p) : "r" (lo), "r" (hi));
}
