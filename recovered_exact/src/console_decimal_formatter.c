#include "legacy_cpu.h"
#include <stdint.h>
#define STORE_DIGIT() asm volatile("st -Z, r18" : "+z" (cursor) : "r" (digit) : "memory")
#define STORE_CHARACTER() asm volatile("st -Z, r16" : "+z" (cursor) : "r" (character) : "memory")

/* Shared signed/fixed-point entries jump to the original frame at +6.
 * The historical divide-by-ten bit loop remains an exact small ASM core. */
void cli_send_uint16(void)
{
    asm volatile("push r20\n\tclr r20\n\tclc" : : : "memory", "cc");
    asm volatile("push r14\n\tpush r15\n\tpush r16\n\tpush r17\n\tpush r18\n\tpush r19\n\tpush r25\n\tpush r30\n\tpush r31" : : : "memory");
    asm goto("clr r15\n\tbrcc %l[frame_ready]\n\tinc r15\n\ttst r17\n\tbrpl %l[frame_ready]" : : : "cc" : frame_ready);
    {
        register uint8_t magnitude asm("r15");
        asm volatile("" : "=r" (magnitude));
        magnitude = (uint8_t)-magnitude;
        asm volatile("" : "+r" (magnitude));
    }
    {
        register uint8_t magnitude asm("r16");
        asm volatile("" : "=r" (magnitude));
        magnitude = (uint8_t)-magnitude;
        asm volatile("" : "+r" (magnitude));
    }
    asm goto("brcc %l[negate_high]" : : : : negate_high);
    asm volatile("inc r17" : : : "cc");
negate_high:
    {
        register uint8_t magnitude asm("r17");
        asm volatile("" : "=r" (magnitude));
        magnitude = (uint8_t)-magnitude;
        asm volatile("" : "+r" (magnitude));
    }
frame_ready:;
    register uint8_t *cursor asm("r30") = (uint8_t *)0x2156;
    asm volatile("" : "+z" (cursor));
    register uint8_t count asm("r19");
    asm volatile("clr %0" : "=r" (count) : : "cc");
    asm volatile("st Z, r19" : : "z" (cursor), "r" (count) : "memory");
next_digit:;
    register uint8_t digit asm("r18") = 13;
    asm volatile("clr r14" : : "r" (digit) : "cc");
    asm volatile("1: subi r17, 0xa0\n\tsbrs r14, 0\n\tbrcs 2f\n\tsec\n\trjmp 3f\n"
                 "2: subi r17, 0x60\n\tclc\n3:\n\tadc r16, r16\n\tadc r17, r17\n\tadc r14, r14\n\tdec r18\n\tbrne 1b"
                 : "+r" (digit) : : "memory", "cc");
    register uint8_t quotient_high asm("r17");
    asm volatile("" : "=r" (quotient_high));
    digit = quotient_high;
    asm volatile("ror r14\n\tror r18" : "+r" (digit) : : "cc");
    quotient_high &= 0x1f;
    asm volatile("" : "+r" (quotient_high));
    digit = __builtin_avr_swap(digit); asm volatile("" : "+r" (digit));
    digit &= 0x0f; asm volatile("" : "+r" (digit));
    digit += '0'; asm volatile("" : "+r" (digit));
    STORE_DIGIT();
    asm volatile("inc r19" : "+r" (count) : : "cc");
    asm goto("cp r20, r19\n\tbrne %l[test_quotient]" : : "r" (count) : "cc" : test_quotient);
    digit = '.'; STORE_DIGIT();
test_quotient:
    asm goto("tst r17\n\tbrne %l[next_digit]\n\ttst r16\n\tbrne %l[next_digit]" : : : "cc" : next_digit);
    asm goto("cp r20, r19\n\tbrlt %l[format_sign]" : : "r" (count) : "cc" : format_sign);
    digit = '0'; STORE_DIGIT();
    asm goto("breq %l[format_sign]" : : : : format_sign);
pad_fraction:
    asm volatile("inc r19" : "+r" (count) : : "cc");
    asm goto("cp r19, r20\n\tbreq %l[decimal_point]" : : "r" (count) : "cc" : decimal_point);
    STORE_DIGIT();
    goto pad_fraction;
decimal_point:
    digit = '.'; STORE_DIGIT();
    digit = '0'; STORE_DIGIT();
format_sign:
    quotient_high = 0x50; asm volatile("" : "+r" (quotient_high));
    asm goto("tst r20\n\tbreq %l[sign_character]" : : : "cc" : sign_character);
    asm volatile("dec r17" : "+r" (quotient_high) : : "cc");
sign_character:;
    register uint8_t character asm("r16") = ' ';
    asm volatile("" : "+r" (character));
    asm goto("tst r15\n\tbreq %l[pad_width]\n\tbrpl %l[store_sign]" : : : "cc" : pad_width, store_sign);
    character = '-'; asm volatile("" : "+r" (character));
store_sign:
    STORE_CHARACTER();
    character = ' '; asm volatile("" : "+r" (character));
    asm volatile("dec r17" : "+r" (quotient_high) : : "cc");
pad_width:
    asm goto("cp r30, r17\n\tbreq %l[begin_output]" : : "z" (cursor), "r" (quotient_high) : "cc" : begin_output);
    STORE_CHARACTER();
    goto pad_width;
begin_output:
    cursor += 1; asm volatile("" : "+z" (cursor));
next_character:
    asm volatile("ld r16, Z+" : "=r" (character), "+z" (cursor) : : "memory");
    asm goto("tst r16\n\tbreq %l[restore]" : : "r" (character) : "cc" : restore);
    asm volatile("rcall cli_send_buf" : "+r" (character) : : "memory", "cc");
    goto next_character;
restore:
    asm volatile("pop r31\n\tpop r30\n\tpop r25\n\tpop r19\n\tpop r18\n\tpop r17\n\tpop r16\n\tpop r15\n\tpop r14\n\tpop r20\n\tret" : : : "memory");
    __builtin_unreachable();
}
