#include "legacy_cpu.h"
#include <avr/io.h>
#define RAM8(address) (*(volatile uint8_t *)(address))

void cli_get_next_char(void)
{
    /* The byte-mode entry jumps past the first two instructions with R19=1. */
    asm volatile("push r19\n\tclr r19\n\tpush r17\n\tpush r18\n\tpush r20\n\tpush r30\n\tpush r31" : : : "memory", "cc");
    register uint8_t *cursor asm("r30") = (uint8_t *)0x2000;
    asm volatile("" : "+z" (cursor));
    register uint8_t value asm("r16"), index asm("r17"), data asm("r18"), flow asm("r20"), mode asm("r19");
    asm volatile("" : "=r" (mode));
wait_character:
    pm_cpu_disable_irq();
    asm volatile("ld %0, Z\n\tldd %1, Z+1" : "=r" (index), "=r" (value) : "z" (cursor) : "memory");
    pm_cpu_enable_irq();
    if (value == index) goto wait_character;
    asm volatile("inc %0" : "+r" (index) : : "cc");
    index &= 0x3f;
    asm volatile("" : "+r" (index));
    flow = PORTF_OUT;
    asm goto("sbrs %0, 0\n\trjmp %l[fetch]" : : "r" (flow) : : fetch);
    data = index;
    asm volatile("" : "+r" (data));
    data += 20;
    asm volatile("" : "+r" (data));
    data &= 0x3f;
    asm volatile("" : "+r" (data));
    asm volatile("cpse %1, %2\n\tldi %0, 1" : "+r" (flow) : "r" (value), "r" (data) : "cc");
    PORTF_OUTCLR = flow;
fetch:
    asm volatile("clr %0" : "=r" (data) : : "cc");
    cursor = (uint8_t *)0x2007;
    asm volatile("add r30, %1\n\tadc r31, %2" : "+z" (cursor) : "r" (index), "r" (data) : "cc");
    data = *cursor;
    asm volatile("" : "+r" (data));
    RAM8(0x2000) = index;
    if (data != 13) goto case_fold;
    index = RAM8(0x2005);
    asm volatile("dec %0" : "+r" (index) : : "cc");
    RAM8(0x2005) = index;
case_fold:
    /* Retain signed BRLT, mode bit and original mask rather than libc folding. */
    asm volatile("cpi %0, 0x60\n\tbrlt 1f\n\tbreq 1f\n\tsbrs %1, 0\n\tandi %0, 0x5f\n1:"
                 : "+r" (data) : "r" (mode) : "cc");
    value = data;
    asm volatile("" : : "r" (value));
    asm volatile("pop r31\n\tpop r30\n\tpop r20\n\tpop r18\n\tpop r17\n\tpop r19\n\tret" : : : "memory");
    __builtin_unreachable();
}
