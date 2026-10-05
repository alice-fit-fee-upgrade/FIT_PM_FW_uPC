#include <stdint.h>
enum delimiter { END_LINE=13, SEPARATOR=32 };
void probe(void) {
register enum delimiter value asm("r16"); asm volatile("" : "=r" (value)); if (value != END_LINE) *(volatile uint8_t*)0x2157=1;
}
