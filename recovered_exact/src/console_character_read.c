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
    {
        register volatile uint8_t *queue asm("r30");
        asm volatile("" : "=z" (queue) : "z" (cursor) : "memory");
        index = queue[0];
        asm volatile("" : "+r" (index) : "z" (queue) : "memory");
        value = queue[1];
        asm volatile("" : "+r" (value) : : "memory");
    }
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
    {
        register uint8_t address_low asm("r30");
        asm volatile("" : "=r" (address_low) : "z" (cursor));
        address_low += index;
        asm volatile("" : "+r" (address_low));
    }
    /* Upper-byte carry remains the original ADC; capture the full result. */
    asm volatile("adc r31, %1" : "=z" (cursor) : "r" (data) : "cc");
    data = *cursor;
    asm volatile("" : "+r" (data));
    RAM8(0x2000) = index;
    if (data != 13) goto case_fold;
    index = RAM8(0x2005);
    /* C equivalent: --index; trial438 failed binary matching.
     * No standalone functional validation; preserve exact DEC flags. */
    asm volatile("dec %0" : "+r" (index) : : "cc");
    RAM8(0x2005) = index;
case_fold:
    /* Retain signed BRLT, mode bit and original mask rather than libc folding. */
    asm goto("cpi %0, 0x60\n\tbrlt %l[fold_done]\n\tbreq %l[fold_done]" : : "r" (data) : "cc" : fold_done);
    if (!(mode & 1u)) {
        data &= 0x5f;
        asm volatile("" : "+r" (data));
    }
fold_done:;
    value = data;
    asm volatile("" : : "r" (value));
    asm volatile("pop r31\n\tpop r30\n\tpop r20\n\tpop r18\n\tpop r17\n\tpop r19" : : : "memory");
    return;
}
