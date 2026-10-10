/* Native outer frame: GCC emits the original entry PUSH under the local
 * call-saved register profile. -fno-ipa-pure-const prevents noreturn inference
 * from dropping that save before the exact shared RET/RETI tail. The tail
 * restores this register in ASM; full FLASH identity validates the pairing. */
/* C equivalent of the retained cursor-read helper:
 * ready++; data = *--cursor; read_index = *--cursor;
 * Steps365–370 failed allocation or changed fixed layout/bytes, including
 * one local Z-allocation retry. No standalone functional test is claimed.
 * Validated flag-aware transition model: tests/logical_models/console_cts_interrupt.c,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
#include <avr/io.h>

void PORTF_INT0_vect_isr(void)
{
    {
        register uint8_t saved_status asm("r31") = SREG;
        asm volatile("" : "+r" (saved_status) : : "memory");
    }
    asm volatile("push r31\n\tpush r30\n\tpush r29\n\tpush r18\n\tpush r17\n\tpush r16" : : : "memory");
    register uint8_t *cursor asm("r30") = (uint8_t *)0x2004;
    asm volatile("" : "+z" (cursor));
    register uint8_t ready asm("r29");
    asm volatile("clr %0" : "=r" (ready) : : "cc");
    register uint8_t read_index asm("r16") = PORTF_IN;
    register uint8_t data asm("r17"), control asm("r18");
    asm goto("sbrc %0, 1\n\trjmp %l[store_ready]" : : "r" (read_index) : : store_ready);
    asm volatile("inc %0\n\tld %1, -Z\n\tld %2, -Z"
        : "+r" (ready), "=r" (data), "=r" (read_index), "+z" (cursor) : : "memory", "cc");
    if (read_index == data) goto advance_ready;
    control = USARTF0_STATUS;
    if (!(control & (1u << 5))) goto enable_tx;
    asm volatile("clr %0\n\tinc %1" : "=r" (data), "+r" (read_index) : : "cc");
    cursor = (uint8_t *)0x2047;
    {
        register uint8_t address_low asm("r30");
        asm volatile("" : "=r" (address_low) : "z" (cursor));
        address_low += read_index;
        asm volatile("" : "+r" (address_low));
    }
    /* Upper-byte carry remains the original ADC; capture the full result. */
    asm volatile("adc r31, %1" : "=z" (cursor) : "r" (data) : "cc");
    data = *cursor;
    asm volatile("" : "+r" (data));
    USARTF0_DATA = data;
    cursor = (uint8_t *)0x2002;
    asm volatile("" : "+z" (cursor));
    *cursor = read_index;
enable_tx:
    control = USARTF0_CTRLA;
    control |= 2;
    asm volatile("" : "+r" (control));
    USARTF0_CTRLA = control;
advance_ready:
    cursor += 2;
    asm volatile("" : "+z" (cursor));
store_ready:
    *cursor = ready;
    asm volatile("pop r16\n\tpop r17\n\tpop r18\n\tpop r29\n\tpop r30\n\tpop r31" : : : "memory");
    {
        register uint8_t saved_status asm("r31");
        asm volatile("" : "=r" (saved_status) : : "memory");
        SREG = saved_status;
    }
    /* Restore the private frame; ordinary C returns cannot express RETI. */
    asm volatile("pop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}
