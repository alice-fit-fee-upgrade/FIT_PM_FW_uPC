#include <stdint.h>

void probe(void) {
register volatile uint8_t *p asm("r30"); register uint8_t lo asm("r16"), hi asm("r17");
asm volatile("" : "=z" (p)); lo = *p++; hi = *p++;
asm volatile("" : "+z" (p) : "r" (lo), "r" (hi));
}
