#include <stdint.h>
void probe(void) { register uint8_t value asm("r16") = 13; asm volatile("" : "+r" (value)); *(volatile uint8_t*)0x2157 = value; }
