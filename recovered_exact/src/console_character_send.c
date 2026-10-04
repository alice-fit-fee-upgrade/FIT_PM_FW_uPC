#include <avr/io.h>

void cli_send_buf(void)
{
    asm volatile("push r31\n\tpush r30\n\tpush r20\n\tpush r19\n\tpush r18\n\tpush r17" : : : "memory");
    register uint8_t character asm("r16");
    asm volatile("" : "=r" (character));
    register uint8_t *cursor asm("r30") = (uint8_t *)0x2002;
    asm volatile("" : "+z" (cursor));
    register uint8_t read_index asm("r17"), write_index asm("r18"), ready asm("r19"), control asm("r20");
retry:
    asm volatile("cli\n\tld %0, Z+\n\tld %1, Z+"
        : "=r" (read_index), "=r" (write_index), "+z" (cursor) : : "memory");
    asm goto("cp %0, %1\n\tbreq %l[direct_send]" : : "r" (read_index), "r" (write_index) : "cc" : direct_send);
    asm volatile("inc %0" : "+r" (write_index) : : "cc");
    asm goto("cp %0, %1\n\tbrne %l[enqueue]" : : "r" (read_index), "r" (write_index) : "cc" : enqueue);
    asm volatile("sei" : : : "memory");
    cursor -= 2;
    asm volatile("" : "+z" (cursor));
    goto retry;
enqueue:
    cursor = (uint8_t *)0x2047;
    asm volatile("clr %1\n\tadd r30, %2\n\tadc r31, %1"
        : "+z" (cursor), "=r" (read_index) : "r" (write_index) : "cc");
    *cursor = character;
    cursor = (uint8_t *)0x2003;
    asm volatile("" : "+z" (cursor));
    *cursor = write_index;
    asm volatile("sei" : : : "memory");
    goto finished;
direct_send:
    asm volatile("inc %0" : "+r" (write_index) : : "cc");
    ready = *cursor;
    asm goto("tst %0\n\tbreq %l[enqueue]" : : "r" (ready) : "cc" : enqueue);
    control = USARTF0_STATUS;
    asm goto("sbrs %0, 5\n\trjmp %l[enqueue]" : : "r" (control) : : enqueue);
    USARTF0_DATA = character;
    control = USARTF0_CTRLA;
    control |= 2;
    asm volatile("" : "+r" (control));
    USARTF0_CTRLA = control;
    asm volatile("sei" : : : "memory");
finished:
    asm volatile("pop r17\n\tpop r18\n\tpop r19\n\tpop r20\n\tpop r30\n\tpop r31\n\tret" : : : "memory");
    __builtin_unreachable();
}
