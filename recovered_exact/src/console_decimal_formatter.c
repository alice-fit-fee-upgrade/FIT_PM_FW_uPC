#include "legacy_cpu.h"
#include <stdint.h>
/* Unvalidated C equivalents: *--cursor = digit; *--cursor = character;
 * character = *cursor++; steps441-443 changed exact encoding/layout.
 * Preserve original predecrement/postincrement pointer and private register ABI.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
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
    *cursor = count;
    asm volatile("" : : : "memory");
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
    asm volatile("pop r31\n\tpop r30\n\tpop r25\n\tpop r19\n\tpop r18\n\tpop r17\n\tpop r16\n\tpop r15\n\tpop r14\n\tpop r20" : : : "memory");
    return;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 1664 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_decimal_formatter(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x274e: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 10064;
    }
    case 0x2750: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 10066;
    }
    case 0x2752: { // clc
        pm_flag(s, CARRY, false);
        return 10068;
    }
    case 0x2754: { // push r14
        s->stack[s->depth++] = s->r[14];
        return 10070;
    }
    case 0x2756: { // push r15
        s->stack[s->depth++] = s->r[15];
        return 10072;
    }
    case 0x2758: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 10074;
    }
    case 0x275a: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 10076;
    }
    case 0x275c: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 10078;
    }
    case 0x275e: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 10080;
    }
    case 0x2760: { // push r25
        s->stack[s->depth++] = s->r[25];
        return 10082;
    }
    case 0x2762: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 10084;
    }
    case 0x2764: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 10086;
    }
    case 0x2766: { // eor r15, r15
        s->r[15] ^= s->r[15];
        pm_nzv(s, s->r[15], false);
        return 10088;
    }
    case 0x2768: { // brcc .+16
        return (pm_getflag(s, 0) == 0) ? 10106 : 10090;
    }
    case 0x276a: { // inc r15
        s->r[15]++;
        pm_nzv(s, s->r[15], s->r[15] == 128);
        return 10092;
    }
    case 0x276c: { // and r17, r17
        s->r[17] &= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 10094;
    }
    case 0x276e: { // brpl .+10
        return (pm_getflag(s, 2) == 0) ? 10106 : 10096;
    }
    case 0x2770: { // neg r15
        uint8_t old = s->r[15];
        s->r[15] = pm_sub(s, 0, old, 0, false);
        pm_flag(s, HALF, (s->r[15] | old) & 8);
        return 10098;
    }
    case 0x2772: { // neg r16
        uint8_t old = s->r[16];
        s->r[16] = pm_sub(s, 0, old, 0, false);
        pm_flag(s, HALF, (s->r[16] | old) & 8);
        return 10100;
    }
    case 0x2774: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 10104 : 10102;
    }
    case 0x2776: { // inc r17
        s->r[17]++;
        pm_nzv(s, s->r[17], s->r[17] == 128);
        return 10104;
    }
    case 0x2778: { // neg r17
        uint8_t old = s->r[17];
        s->r[17] = pm_sub(s, 0, old, 0, false);
        pm_flag(s, HALF, (s->r[17] | old) & 8);
        return 10106;
    }
    case 0x277a: { // ldi r30, 0x56
        s->r[30] = 86;
        return 10108;
    }
    case 0x277c: { // ldi r31, 0x21
        s->r[31] = 33;
        return 10110;
    }
    case 0x277e: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 10112;
    }
    case 0x2780: { // st Z, r19
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[19]);
        return 10114;
    }
    case 0x2782: { // ldi r18, 0x0D
        s->r[18] = 13;
        return 10116;
    }
    case 0x2784: { // eor r14, r14
        s->r[14] ^= s->r[14];
        pm_nzv(s, s->r[14], false);
        return 10118;
    }
    case 0x2786: { // subi r17, 0xA0
        s->r[17] = pm_sub(s, s->r[17], 160, 0, false);
        return 10120;
    }
    case 0x2788: { // sbrs r14, 0
        return (!!(s->r[14] & (1u << 0)) == 1) ? 10124 : 10122;
    }
    case 0x278a: { // brcs .+4
        return (pm_getflag(s, 0) == 1) ? 10128 : 10124;
    }
    case 0x278c: { // sec
        pm_flag(s, CARRY, true);
        return 10126;
    }
    case 0x278e: { // rjmp .+4
        return 10132;
    }
    case 0x2790: { // subi r17, 0x60
        s->r[17] = pm_sub(s, s->r[17], 96, 0, false);
        return 10130;
    }
    case 0x2792: { // clc
        pm_flag(s, CARRY, false);
        return 10132;
    }
    case 0x2794: { // adc r16, r16
        s->r[16] = pm_add(s, s->r[16], s->r[16], pm_getflag(s, CARRY));
        return 10134;
    }
    case 0x2796: { // adc r17, r17
        s->r[17] = pm_add(s, s->r[17], s->r[17], pm_getflag(s, CARRY));
        return 10136;
    }
    case 0x2798: { // adc r14, r14
        s->r[14] = pm_add(s, s->r[14], s->r[14], pm_getflag(s, CARRY));
        return 10138;
    }
    case 0x279a: { // dec r18
        s->r[18]--;
        pm_nzv(s, s->r[18], s->r[18] == 127);
        return 10140;
    }
    case 0x279c: { // brne .-24
        return (pm_getflag(s, 1) == 0) ? 10118 : 10142;
    }
    case 0x279e: { // mov r18, r17
        s->r[18] = s->r[17];
        return 10144;
    }
    case 0x27a0: { // ror r14
        bool carry = s->r[14] & 1;
        s->r[14] = (s->r[14] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[14], !!(s->r[14] & 128) ^ carry);
        return 10146;
    }
    case 0x27a2: { // ror r18
        bool carry = s->r[18] & 1;
        s->r[18] = (s->r[18] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[18], !!(s->r[18] & 128) ^ carry);
        return 10148;
    }
    case 0x27a4: { // andi r17, 0x1F
        s->r[17] &= 31;
        pm_nzv(s, s->r[17], false);
        return 10150;
    }
    case 0x27a6: { // swap r18
        s->r[18] = (s->r[18] >> 4) | (s->r[18] << 4);
        return 10152;
    }
    case 0x27a8: { // andi r18, 0x0F
        s->r[18] &= 15;
        pm_nzv(s, s->r[18], false);
        return 10154;
    }
    case 0x27aa: { // subi r18, 0xD0
        s->r[18] = pm_sub(s, s->r[18], 208, 0, false);
        return 10156;
    }
    case 0x27ac: { // st -Z, r18
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 10158;
    }
    case 0x27ae: { // inc r19
        s->r[19]++;
        pm_nzv(s, s->r[19], s->r[19] == 128);
        return 10160;
    }
    case 0x27b0: { // cp r20, r19
        pm_sub(s, s->r[20], s->r[19], 0, false);
        return 10162;
    }
    case 0x27b2: { // brne .+4
        return (pm_getflag(s, 1) == 0) ? 10168 : 10164;
    }
    case 0x27b4: { // ldi r18, 0x2E
        s->r[18] = 46;
        return 10166;
    }
    case 0x27b6: { // st -Z, r18
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 10168;
    }
    case 0x27b8: { // and r17, r17
        s->r[17] &= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 10170;
    }
    case 0x27ba: { // brne .-58
        return (pm_getflag(s, 1) == 0) ? 10114 : 10172;
    }
    case 0x27bc: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 10174;
    }
    case 0x27be: { // brne .-62
        return (pm_getflag(s, 1) == 0) ? 10114 : 10176;
    }
    case 0x27c0: { // cp r20, r19
        pm_sub(s, s->r[20], s->r[19], 0, false);
        return 10178;
    }
    case 0x27c2: { // brlt .+24
        return (pm_getflag(s, 4) == 1) ? 10204 : 10180;
    }
    case 0x27c4: { // ldi r18, 0x30
        s->r[18] = 48;
        return 10182;
    }
    case 0x27c6: { // st -Z, r18
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 10184;
    }
    case 0x27c8: { // breq .+18
        return (pm_getflag(s, 1) == 1) ? 10204 : 10186;
    }
    case 0x27ca: { // inc r19
        s->r[19]++;
        pm_nzv(s, s->r[19], s->r[19] == 128);
        return 10188;
    }
    case 0x27cc: { // cp r19, r20
        pm_sub(s, s->r[19], s->r[20], 0, false);
        return 10190;
    }
    case 0x27ce: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 10196 : 10192;
    }
    case 0x27d0: { // st -Z, r18
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 10194;
    }
    case 0x27d2: { // rjmp .-10
        return 10186;
    }
    case 0x27d4: { // ldi r18, 0x2E
        s->r[18] = 46;
        return 10198;
    }
    case 0x27d6: { // st -Z, r18
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 10200;
    }
    case 0x27d8: { // ldi r18, 0x30
        s->r[18] = 48;
        return 10202;
    }
    case 0x27da: { // st -Z, r18
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 10204;
    }
    case 0x27dc: { // ldi r17, 0x50
        s->r[17] = 80;
        return 10206;
    }
    case 0x27de: { // and r20, r20
        s->r[20] &= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 10208;
    }
    case 0x27e0: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 10212 : 10210;
    }
    case 0x27e2: { // dec r17
        s->r[17]--;
        pm_nzv(s, s->r[17], s->r[17] == 127);
        return 10212;
    }
    case 0x27e4: { // ldi r16, 0x20
        s->r[16] = 32;
        return 10214;
    }
    case 0x27e6: { // and r15, r15
        s->r[15] &= s->r[15];
        pm_nzv(s, s->r[15], false);
        return 10216;
    }
    case 0x27e8: { // breq .+10
        return (pm_getflag(s, 1) == 1) ? 10228 : 10218;
    }
    case 0x27ea: { // brpl .+2
        return (pm_getflag(s, 2) == 0) ? 10222 : 10220;
    }
    case 0x27ec: { // ldi r16, 0x2D
        s->r[16] = 45;
        return 10222;
    }
    case 0x27ee: { // st -Z, r16
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 10224;
    }
    case 0x27f0: { // ldi r16, 0x20
        s->r[16] = 32;
        return 10226;
    }
    case 0x27f2: { // dec r17
        s->r[17]--;
        pm_nzv(s, s->r[17], s->r[17] == 127);
        return 10228;
    }
    case 0x27f4: { // cp r30, r17
        pm_sub(s, s->r[30], s->r[17], 0, false);
        return 10230;
    }
    case 0x27f6: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 10236 : 10232;
    }
    case 0x27f8: { // st -Z, r16
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 10234;
    }
    case 0x27fa: { // rjmp .-8
        return 10228;
    }
    case 0x27fc: { // adiw r30, 0x01
        uint16_t old = pm_pointer(s, 30);
        uint16_t value = old + 1;
        pm_setpointer(s, 30, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 10238;
    }
    case 0x27fe: { // ld r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_read(s, address);
        return 10240;
    }
    case 0x2800: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 10242;
    }
    case 0x2802: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 10248 : 10244;
    }
    case 0x2804: { // rcall .+166
        s->calls[s->call_depth++] = 10246;
        return 10412;
    }
    case 0x2806: { // rjmp .-10
        return 10238;
    }
    case 0x2808: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 10250;
    }
    case 0x280a: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 10252;
    }
    case 0x280c: { // pop r25
        s->r[25] = s->stack[--s->depth];
        return 10254;
    }
    case 0x280e: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 10256;
    }
    case 0x2810: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 10258;
    }
    case 0x2812: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 10260;
    }
    case 0x2814: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 10262;
    }
    case 0x2816: { // pop r15
        s->r[15] = s->stack[--s->depth];
        return 10264;
    }
    case 0x2818: { // pop r14
        s->r[14] = s->stack[--s->depth];
        return 10266;
    }
    case 0x281a: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 10268;
    }
    case 0x281c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
