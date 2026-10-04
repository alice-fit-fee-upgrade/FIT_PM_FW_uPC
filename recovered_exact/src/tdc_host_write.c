#include <avr/io.h>

/* Legacy R19 selects a TDC/broadcast and the additional command bit.
 * The payload uses R16/R18/R17 in the original carry-dependent order. */
void ths788_write(void)
{
    register uint8_t b0 asm("r16"), b1 asm("r17"), b2 asm("r18");
    register uint8_t device asm("r19"), data_mask asm("r20");
    register uint8_t count asm("r21"), clock asm("r22");
    asm volatile("" : "=r" (b0), "=r" (b1), "=r" (b2), "=r" (device));
    /* Exact selection loop: DEC/BRMI retains its original carry behavior. */
    asm volatile("mov %1, %0\n\tandi %0, 3\n\tcpi %0, 3\n\tbrlo 1f\n\tldi %2, 0x1c\n\trjmp 3f\n"
                 "1: ldi %2, 4\n2: dec %0\n\tbrmi 3f\n\tlsl %2\n\trjmp 2b\n3:"
                 : "+r" (device), "=r" (count), "=r" (clock) : : "cc");
    clock |= 2;
    asm volatile("" : "+r" (clock));
    PORTB_OUTCLR = clock;
    clock = 1;
    asm volatile("" : "+r" (clock));
    data_mask = 2;
    asm volatile("" : "+r" (data_mask));
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    asm goto("andi %0, 4\n\tbreq %l[command_clock]" : : "r" (count) : "cc" : command_clock);
    PORTB_OUTSET = data_mask;
command_clock:
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count = 24;
next_bit:
    asm volatile("sbrs %0, 7\n\tsts %4, %3\n\tsbrc %0, 7\n\tsts %5, %3\n"
                 "\tadc %1, %1\n\tadc %2, %2\n\tadc %0, %0"
        : "+r" (b0), "+r" (b1), "+r" (b2) : "r" (data_mask),
          "n" (_SFR_MEM_ADDR(PORTB_OUTCLR)), "n" (_SFR_MEM_ADDR(PORTB_OUTSET)), "r" (count) : "memory", "cc");
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_bit]" : : "r" (count) : : next_bit);
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count = 0x1c;
    asm volatile("" : "+r" (count));
    PORTB_OUTSET = count;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
}
