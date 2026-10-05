#include <avr/io.h>

void PORTF_INT0_vect_isr(void)
{
    asm volatile("push r31\n\tin r31, 0x3f\n\tpush r31\n\tpush r30\n\tpush r29\n\tpush r18\n\tpush r17\n\tpush r16" : : : "memory");
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
    asm goto("sbrs %0, 5\n\trjmp %l[enable_tx]" : : "r" (control) : : enable_tx);
    asm volatile("clr %0\n\tinc %1" : "=r" (data), "+r" (read_index) : : "cc");
    cursor = (uint8_t *)0x2047;
    asm volatile("add r30, %1\n\tadc r31, %2" : "+z" (cursor) : "r" (read_index), "r" (data) : "cc");
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
    asm volatile("pop r16\n\tpop r17\n\tpop r18\n\tpop r29\n\tpop r30\n\tpop r31\n\tout 0x3f, r31\n\tpop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}
