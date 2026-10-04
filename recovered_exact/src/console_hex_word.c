#include <stdint.h>
#define SWAP_NIBBLES(value) asm volatile("swap %0" : "+r" (value))
#define SEND_DIGIT(value) asm volatile("rcall cli_send_digit_hex" : "+r" (value) : : "memory", "cc")

/* Historical symbol says 32bit; the original emits four nibbles of R17:R16. */
void cli_send_32bit_hex(void)
{
    register uint8_t digit asm("r16");
    register uint8_t high asm("r17");
    asm volatile("" : "=r" (digit), "=r" (high));
    register uint8_t low asm("r18") = digit;
    SWAP_NIBBLES(high);
    digit = high;
    SEND_DIGIT(digit);
    SWAP_NIBBLES(high);
    digit = high;
    SEND_DIGIT(digit);
    SWAP_NIBBLES(low);
    digit = low;
    SEND_DIGIT(digit);
    SWAP_NIBBLES(low);
    digit = low;
    SEND_DIGIT(digit);
}
