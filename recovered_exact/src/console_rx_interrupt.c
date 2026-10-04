#include <avr/io.h>
#define RAM8(address) (*(volatile uint8_t *)(address))

void USARTF0_RXC_vect_isr(void)
{
    asm volatile("push r31\n\tin r31, 0x3f\n\tpush r31\n\tpush r30\n\tpush r19\n\tpush r18\n\tpush r17\n\tpush r16" : : : "memory");
    register uint8_t *cursor asm("r30") = (uint8_t *)0x2000;
    register uint8_t read_index asm("r16"), write_index asm("r17"), data asm("r18"), status asm("r19");
    asm volatile("ld %0, Z+\n\tld %1, Z" : "=r" (read_index), "=r" (write_index), "+z" (cursor) : : "memory");
    asm volatile("inc %0" : "+r" (write_index) : : "cc");
    write_index &= 0x3f;
    asm volatile("" : "+r" (write_index));
    data = write_index;
    asm volatile("" : "+r" (data));
    data += 20;
    asm volatile("" : "+r" (data));
    data &= 0x3f;
    asm volatile("" : "+r" (data));
    asm goto("cp %0, %1\n\tbrne %l[receive]" : : "r" (data), "r" (read_index) : "cc" : receive);
    data = 1;
    asm volatile("" : "+r" (data));
    PORTF_OUTSET = data;
    data = RAM8(0x2005);
    data |= 0x80;
    asm volatile("" : "+r" (data));
    RAM8(0x2005) = data;
receive:
    status = USARTF0_STATUS;
    asm volatile("" : "+r" (status));
    data = USARTF0_DATA;
    asm volatile("" : "+r" (data));
    asm goto("sbrc %0, 4\n\trjmp %l[finished]" : : "r" (status) : : finished);
    asm goto("cp %0, %1\n\tbreq %l[finished]" : : "r" (read_index), "r" (write_index) : "cc" : finished);
    *cursor = write_index;
    cursor = (uint8_t *)0x2007;
    asm volatile("clr %1\n\tadd r30, %2\n\tadc r31, %1"
        : "+z" (cursor), "=r" (read_index) : "r" (write_index) : "cc");
    asm volatile("" : "+r" (data));
    *cursor = data;
    asm goto("cpi %0, 13\n\tbrne %l[finished]" : : "r" (data) : "cc" : finished);
    read_index = RAM8(0x2005);
    asm volatile("inc %0" : "+r" (read_index) : : "cc");
    RAM8(0x2005) = read_index;
finished:
    asm volatile("pop r16\n\tpop r17\n\tpop r18\n\tpop r19\n\tpop r30\n\tpop r31\n\tout 0x3f, r31\n\tpop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}
