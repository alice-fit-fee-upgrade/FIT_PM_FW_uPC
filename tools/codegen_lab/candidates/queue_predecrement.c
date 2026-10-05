#include <stdint.h>
register volatile uint8_t *p asm("r30");
void probe(void) {
register uint8_t hi asm("r17"),lo asm("r16"); asm volatile("" : "=z" (p)); hi=*--p; lo=*--p; asm volatile("" : "+z" (p) : "r" (hi),"r" (lo));
}
