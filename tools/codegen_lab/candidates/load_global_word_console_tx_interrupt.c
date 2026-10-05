#include <stdint.h>
register const uint8_t *p asm("r30");
void probe(void) {
register uint16_t word asm("r16"); asm volatile("" : "=z" (p)); word=*(const uint16_t*)p; p+=2; asm volatile("" : "+z" (p) : "r" (word));
}
