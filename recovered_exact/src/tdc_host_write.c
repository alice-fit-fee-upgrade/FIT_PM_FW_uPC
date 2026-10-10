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
    count = device;
    asm volatile("" : "+r" (count), "+r" (device));
    device &= 3;
    asm volatile("" : "+r" (device));
    /* C value selection: clock = device == 3 ? 0x1c : (4u << device);
     * The exact loop additionally leaves device at its decremented value and
     * preserves live flags; this is an explanatory value description. */
    if (device < 3) goto single_strobe;
    clock = 0x1c; asm volatile("" : "+r" (clock));
    goto clock_selected;
single_strobe:
    clock = 4; asm volatile("" : "+r" (clock));
shift_clock:
    asm volatile("dec %0" : "+r" (device) : : "cc");
    asm goto("brmi %l[clock_selected]" : : "r" (device) : : clock_selected);
    clock += clock; asm volatile("" : "+r" (clock));
    goto shift_clock;
clock_selected:
    clock |= 2;
    asm volatile("" : "+r" (clock));
    PORTB_OUTCLR = clock;
    clock = 1;
    asm volatile("" : "+r" (clock));
    data_mask = 2;
    asm volatile("" : "+r" (data_mask));
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count &= 4;
    asm volatile("" : "+r" (count));
    /* C branch value equivalent: if (count == 0) goto command_clock;
     * BREQ consumes flags from the exact ANDI emitted for the C mask. */
    /* Input-only boundary retry542 also changed layout. */
    /* Step446 `if (!count) goto command_clock;` changed the binary/layout.
     * Unvalidated standalone; consume original ANDI flags directly.
 * Validated flag-aware transition model: tests/logical_models/tdc_host_write.c,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("breq %l[command_clock]" : : "r" (count) : : command_clock);
    PORTB_OUTSET = data_mask;
command_clock:
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count = 24;
next_bit:
    if (!(b0 & 0x80u)) PORTB_OUTCLR = data_mask;
    asm volatile("" : "+r" (b0) : "r" (data_mask), "r" (count) : "memory");
    if (b0 & 0x80u) PORTB_OUTSET = data_mask;
    /* C values: shift the b1/b2/b0 serial packet with input carry.
     * Exact ADC instructions also propagate SREG.C between the bytes. */
    asm volatile("adc %1, %1\n\tadc %2, %2\n\tadc %0, %0"
        : "+r" (b0), "+r" (b1), "+r" (b2) : : "cc");
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
