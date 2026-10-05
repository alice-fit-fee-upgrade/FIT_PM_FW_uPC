#include <stdint.h>
register const uint8_t *p asm("r28");
void probe(void) {
register uint16_t word asm("r20"); asm volatile("" : "=y" (p)); word=*(const uint16_t*)p; p+=2; asm volatile("" : "+y" (p) : "r" (word));
}
