#include <avr/io.h>

/* Eight clock edges, with original bit tests and sampling timing preserved. */
void adt7311_byte_rw(void)
{
    register uint8_t count asm("r18") = 8;
    register uint8_t data asm("r16");
    asm volatile("" : "=r" (data));
next_bit:
    {
        register uint8_t data_mask asm("r17") = 8;
        asm volatile("sbrc %0, 7\n\tsts %2, %1\n\tsbrs %0, 7\n\tsts %3, %1"
            : : "r" (data), "r" (data_mask), "n" (_SFR_MEM_ADDR(PORTA_OUTSET)),
                "n" (_SFR_MEM_ADDR(PORTA_OUTCLR)) : "memory");
    }
    PORTA_OUTCLR = 2;
    asm volatile("lsl %0\n\tnop" : "+r" (data) : : "cc");
    PORTA_OUTSET = 2;
    register uint8_t sample asm("r17") = PORTA_IN;
    asm volatile("bst %1, 2\n\tbld %0, 0" : "+r" (data) : "r" (sample) : "cc");
    /* DEC, unlike SUBI, preserves the original carry flag. */
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_bit]" : : "r" (count) : : next_bit);
}
