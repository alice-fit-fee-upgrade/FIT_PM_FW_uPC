#include <stdint.h>
register uint8_t *p asm("r26");
void probe(void) {
register uint8_t lo asm("r18"), hi asm("r19"); asm volatile("" : "=x" (p), "=r" (lo), "=r" (hi)); *p++=lo; *p++=hi; asm volatile("" : "+x" (p) : : "memory");
}
