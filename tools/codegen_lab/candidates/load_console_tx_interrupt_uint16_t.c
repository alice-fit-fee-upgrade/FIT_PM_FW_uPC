#include <stdint.h>

void probe(void) {
register uint16_t *p asm("r30"); register uint16_t word asm("r16");
asm volatile("" : "=z" (p)); word = *p++;
asm volatile("" : "+z" (p) : "r" (word));
}
