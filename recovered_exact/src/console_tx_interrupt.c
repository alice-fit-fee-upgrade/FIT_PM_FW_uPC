/* Original cursor-read sequence:
 * read_index = *cursor++; write_index = *cursor++; ready = *cursor;
 * Steps365–370 failed allocation or changed fixed layout/bytes, including
 * one local Z-allocation retry. Step390 moved the final read to exact C;
 * the two postincrement loads retain ASM. No behavioral test is claimed. */
#include <avr/io.h>

void USARTF0_DRE_vect_isr(void)
{
    asm volatile("push r31" : : : "memory");
    {
        register uint8_t saved_status asm("r31") = SREG;
        asm volatile("" : "+r" (saved_status) : : "memory");
    }
    asm volatile("push r31\n\tpush r30\n\tpush r19\n\tpush r18\n\tpush r17\n\tpush r16" : : : "memory");
    register uint8_t *cursor asm("r30") = (uint8_t *)0x2002;
    register uint8_t read_index asm("r16"), write_index asm("r17"), ready asm("r18"), control asm("r19");
    asm volatile("ld %0, Z+\n\tld %1, Z+"
        : "=r" (read_index), "=r" (write_index), "+z" (cursor) : : "memory");
    ready = *cursor;
    asm volatile("" : "+r" (ready));
    asm goto("tst %0\n\tbreq %l[disable]" : : "r" (ready) : "cc" : disable);
    asm goto("cp %0, %1\n\tbrne %l[send]" : : "r" (read_index), "r" (write_index) : "cc" : send);
disable:
    control = USARTF0_CTRLA;
    control &= 0xfc;
    asm volatile("" : "+r" (control));
    USARTF0_CTRLA = control;
    goto finished;
send:
    asm volatile("inc %0\n\tclr %1" : "+r" (read_index), "=r" (write_index) : : "cc");
    cursor = (uint8_t *)0x2047;
    {
        register uint8_t address_low asm("r30");
        asm volatile("" : "=r" (address_low) : "z" (cursor));
        address_low += read_index;
        asm volatile("" : "+r" (address_low));
    }
    /* Upper-byte carry remains the original ADC; capture the full result. */
    asm volatile("adc r31, %1" : "=z" (cursor) : "r" (write_index) : "cc");
    write_index = *cursor;
    asm volatile("" : "+r" (write_index));
    USARTF0_DATA = write_index;
    cursor = (uint8_t *)0x2002;
    asm volatile("" : "+z" (cursor));
    *cursor = read_index;
finished:
    asm volatile("pop r16\n\tpop r17\n\tpop r18\n\tpop r19\n\tpop r30\n\tpop r31" : : : "memory");
    {
        register uint8_t saved_status asm("r31");
        asm volatile("" : "=r" (saved_status) : : "memory");
        SREG = saved_status;
    }
    /* Restore the private frame; ordinary C returns cannot express RETI. */
    asm volatile("pop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}
