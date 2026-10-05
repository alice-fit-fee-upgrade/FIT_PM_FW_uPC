#include <stdint.h>
register const uint8_t *p asm("r28");
void probe(void) {
register uint8_t lo asm("r20"), hi asm("r21"); asm volatile("" : "=y" (p)); lo=*p++; hi=*p++; asm volatile("" : "+y" (p) : "r" (lo), "r" (hi));
}
