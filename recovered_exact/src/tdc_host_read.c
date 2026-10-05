#include <avr/io.h>

void ths788_read(void)
{
    register uint8_t command asm("r16"), lo asm("r17"), hi asm("r18");
    register uint8_t device asm("r19"), data_mask asm("r20");
    register uint8_t count asm("r21"), clock asm("r22");
    asm volatile("" : "=r" (command), "=r" (lo), "=r" (hi), "=r" (device));
    if (device < 3) goto selected;
    goto finished;
selected:
    clock = 4;
    asm volatile("1: dec %0\n\tbrmi 2f\n\tlsl %1\n\trjmp 1b\n2:"
                 : "+r" (device), "+r" (clock) : : "cc");
    PORTB_OUTCLR = clock;
    clock = 1;
    asm volatile("" : "+r" (clock));
    data_mask = 2;
    asm volatile("" : "+r" (data_mask));
    PORTB_OUTSET = data_mask;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    PORTB_OUTCLR = data_mask;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count = 8;
next_command_bit:
    asm volatile("sbrs %0, 7\n\tsts %2, %1\n\tsbrc %0, 7\n\tsts %3, %1\n\tadc %0, %0"
        : "+r" (command) : "r" (data_mask), "n" (_SFR_MEM_ADDR(PORTB_OUTCLR)),
          "n" (_SFR_MEM_ADDR(PORTB_OUTSET)), "r" (count) : "memory", "cc");
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    /* C counter value: --count; if (count != 0) goto loop_start;
     * DEC/BRNE also preserves C for the serial shift. Plain C decrements
     * need not preserve that flag; this is an explanatory value equivalent. */
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_command_bit]" : : "r" (count) : : next_command_bit);
    PORTB_DIRCLR = data_mask;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count = 16;
next_response_bit:
    asm volatile("" : : "r" (count));
    PORTB_OUTSET = clock;
    lo += lo;
    asm volatile("" : "+r" (lo));
    /* C value equivalent of ADC after the low-byte shift:
     * uint8_t carry = old_lo >> 7;
     * hi = (uint8_t)((hi << 1) + carry);
     * Here carry is the live SREG.C from lo += lo. A trial that captured
     * it in R0 and expressed the high byte in C failed the fixed-region
     * size assertion (step_218_rejected.log). Keep ADC and its flags exact.
     * This fragment has no separate successful functional-test claim. */
    asm volatile("adc %0, %0" : "+r" (hi) : : "cc");
    command = PORTB_IN;
    asm volatile("sbrc %1, 1\n\tori %0, 1" : "+r" (lo) : "r" (command) : "cc");
    PORTB_OUTCLR = clock;
    /* C counter value: --count; if (count != 0) goto loop_start;
     * DEC/BRNE also preserves C for the serial shift. Plain C decrements
     * need not preserve that flag; this is an explanatory value equivalent. */
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_response_bit]" : : "r" (count) : : next_response_bit);
    count = 0x1c;
    asm volatile("" : "+r" (count));
    PORTB_OUTSET = count;
    PORTB_DIRSET = data_mask;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
finished:
    asm volatile("" : : "r" (lo), "r" (hi));
}
