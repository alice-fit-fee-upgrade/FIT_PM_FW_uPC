#include <stdint.h>

void probe(void) {
register int16_t *p asm("r30"); register int16_t word asm("r16");
asm volatile("" : "=z" (p)); word = *p++;
asm volatile("" : "+z" (p) : "r" (word));
}
