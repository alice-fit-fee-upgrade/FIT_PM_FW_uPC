#include <stdint.h>

/* Original decimal parser: delimiter in R16, result in R21:R20, carry for
 * failure. MUL overflow and signed-negation decisions retain exact flags. */
void cli_get_integer(void)
{
    register uint8_t count asm("r18"), negative asm("r19");
    register uint16_t result asm("r20");
    asm volatile("push r22\n\tpush r17\n\tpush r18\n\tpush r19\n\tpush r1\n\tpush r0\n\tclr r18"
                 : "=r" (count) : : "memory", "cc");
    register uint8_t radix asm("r17") = 10;
    asm volatile("clr r19\n\tclr r20\n\tclr r21" : "=r" (negative), "=r" (result) : "r" (radix) : "cc");
next_digit:;
    register uint8_t character asm("r16");
    asm volatile("rcall cli_get_next_char" : "=r" (character) : : "memory", "cc");
    /* Rejected C selection/range alternatives (steps 236-238):
     * if (count != 0 || character != '-') goto validate;
     * if ((int8_t)character < '0' || (int8_t)character >= ':') goto done;
     * if (count == 1) goto error;
     * if (negative == 0) goto success;
     * if (count == 2) goto error;
     * These alternatives changed complete binary/layout matching. They have
     * no independent successful functional-test claim. Retain exact branches.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("tst r18\n\tbrne %l[validate]\n\tcpi r16, 0x2d\n\tbrne %l[validate]"
             : : "r" (count), "r" (character) : "cc" : validate);
    asm volatile("inc r19\n\tinc r18" : "+r" (negative), "+r" (count) : : "cc");
    goto next_digit;
validate:
    asm volatile("inc r18" : "+r" (count) : : "cc");
    asm goto("cpi r16, 0x30\n\tbrlt %l[done]\n\tcpi r16, 0x3a\n\tbrge %l[done]"
             : : "r" (character) : "cc" : done);
    register uint8_t digit asm("r22") = character;
    asm volatile("" : "+r" (digit));
    digit -= '0'; asm volatile("" : "+r" (digit));
    asm volatile("mul r20, r17" : "+r" (result) : "r" (radix) : "r0", "r1", "cc");
    {
        register uint8_t partial asm("r0");
        asm volatile("" : "=r" (partial));
        partial += digit;
        asm volatile("" : "+r" (partial));
    }
    asm volatile("clr r22\n\tadc r1, r22" : "+r" (digit) : : "r1", "cc");
    {
        register uint8_t old_high asm("r21");
        asm volatile("" : "=r" (old_high));
        digit = old_high;
        asm volatile("" : "+r" (digit));
    }
    /* R1 is a live nonzero product byte; keep this private copy in ASM. */
    asm volatile("mov r21, r1" : : : "cc");
    {
        register uint8_t partial asm("r0"), low asm("r20");
        asm volatile("" : "=r" (partial));
        low = partial;
        asm volatile("" : "+r" (low));
    }
    asm volatile("mul r17, r22" : : "r" (radix), "r" (digit) : "r0", "r1", "cc");
    asm volatile("" : "=r" (result), "=r" (digit));
    asm goto("tst r1\n\tbrne %l[error]" : : : "cc" : error);
    {
        register uint8_t high asm("r21"), partial asm("r0");
        asm volatile("" : "=r" (high), "=r" (partial));
        high += partial;
        asm volatile("" : "+r" (high));
    }
    asm goto("brcc %l[next_digit]\n\trjmp %l[error]" : : : : error, next_digit);
    __builtin_unreachable();
done:
    asm goto("cpi r18, 1\n\tbreq %l[error]\n\ttst r19\n\tbreq %l[success]\n\tcpi r18, 2\n\tbreq %l[error]"
             : : "r" (count), "r" (negative) : "cc" : error, success);
    asm volatile("clr r19" : "+r" (negative) : : "cc");
    {
        register uint8_t result_low asm("r20");
        asm volatile("" : "=r" (result_low));
        result_low = (uint8_t)-result_low;
        asm volatile("" : "+r" (result_low));
    }
    asm volatile("adc r21, r19" : : "r" (negative) : "cc");
    {
        register uint8_t result_high asm("r21");
        asm volatile("" : "=r" (result_high));
        result_high = (uint8_t)-result_high;
        asm volatile("" : "+r" (result_high));
    }
    asm goto("brpl %l[error]" : : : : error);
success:
    asm volatile("clc" : : : "cc");
    goto restore;
error:
    asm volatile("sec" : : : "cc");
restore:
    asm volatile("pop r0\n\tpop r1\n\tpop r19\n\tpop r18\n\tpop r17\n\tpop r22" : : : "memory");
    return;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 960 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_integer_parser(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2634: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 9782;
    }
    case 0x2636: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 9784;
    }
    case 0x2638: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9786;
    }
    case 0x263a: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 9788;
    }
    case 0x263c: { // push r1
        s->stack[s->depth++] = s->r[1];
        return 9790;
    }
    case 0x263e: { // push r0
        s->stack[s->depth++] = s->r[0];
        return 9792;
    }
    case 0x2640: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 9794;
    }
    case 0x2642: { // ldi r17, 0x0A
        s->r[17] = 10;
        return 9796;
    }
    case 0x2644: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 9798;
    }
    case 0x2646: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 9800;
    }
    case 0x2648: { // eor r21, r21
        s->r[21] ^= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 9802;
    }
    case 0x264a: { // rcall .+496
        s->calls[s->call_depth++] = 9804;
        return 10300;
    }
    case 0x264c: { // and r18, r18
        s->r[18] &= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 9806;
    }
    case 0x264e: { // brne .+10
        return (pm_getflag(s, 1) == 0) ? 9818 : 9808;
    }
    case 0x2650: { // cpi r16, 0x2D
        pm_sub(s, s->r[16], 45, 0, false);
        return 9810;
    }
    case 0x2652: { // brne .+6
        return (pm_getflag(s, 1) == 0) ? 9818 : 9812;
    }
    case 0x2654: { // inc r19
        s->r[19]++;
        pm_nzv(s, s->r[19], s->r[19] == 128);
        return 9814;
    }
    case 0x2656: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 9816;
    }
    case 0x2658: { // rjmp .-16
        return 9802;
    }
    case 0x265a: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 9820;
    }
    case 0x265c: { // cpi r16, 0x30
        pm_sub(s, s->r[16], 48, 0, false);
        return 9822;
    }
    case 0x265e: { // brlt .+34
        return (pm_getflag(s, 4) == 1) ? 9858 : 9824;
    }
    case 0x2660: { // cpi r16, 0x3A
        pm_sub(s, s->r[16], 58, 0, false);
        return 9826;
    }
    case 0x2662: { // brge .+30
        return (pm_getflag(s, 4) == 0) ? 9858 : 9828;
    }
    case 0x2664: { // mov r22, r16
        s->r[22] = s->r[16];
        return 9830;
    }
    case 0x2666: { // subi r22, 0x30
        s->r[22] = pm_sub(s, s->r[22], 48, 0, false);
        return 9832;
    }
    case 0x2668: { // mul r20, r17
        uint16_t product = (s->r[20]) * (int)s->r[17];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 9834;
    }
    case 0x266a: { // add r0, r22
        s->r[0] = pm_add(s, s->r[0], s->r[22], 0);
        return 9836;
    }
    case 0x266c: { // eor r22, r22
        s->r[22] ^= s->r[22];
        pm_nzv(s, s->r[22], false);
        return 9838;
    }
    case 0x266e: { // adc r1, r22
        s->r[1] = pm_add(s, s->r[1], s->r[22], pm_getflag(s, CARRY));
        return 9840;
    }
    case 0x2670: { // mov r22, r21
        s->r[22] = s->r[21];
        return 9842;
    }
    case 0x2672: { // mov r21, r1
        s->r[21] = s->r[1];
        return 9844;
    }
    case 0x2674: { // mov r20, r0
        s->r[20] = s->r[0];
        return 9846;
    }
    case 0x2676: { // mul r17, r22
        uint16_t product = (s->r[17]) * (int)s->r[22];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 9848;
    }
    case 0x2678: { // and r1, r1
        s->r[1] &= s->r[1];
        pm_nzv(s, s->r[1], false);
        return 9850;
    }
    case 0x267a: { // brne .+32
        return (pm_getflag(s, 1) == 0) ? 9884 : 9852;
    }
    case 0x267c: { // add r21, r0
        s->r[21] = pm_add(s, s->r[21], s->r[0], 0);
        return 9854;
    }
    case 0x267e: { // brcc .-54
        return (pm_getflag(s, 0) == 0) ? 9802 : 9856;
    }
    case 0x2680: { // rjmp .+26
        return 9884;
    }
    case 0x2682: { // cpi r18, 0x01
        pm_sub(s, s->r[18], 1, 0, false);
        return 9860;
    }
    case 0x2684: { // breq .+22
        return (pm_getflag(s, 1) == 1) ? 9884 : 9862;
    }
    case 0x2686: { // and r19, r19
        s->r[19] &= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 9864;
    }
    case 0x2688: { // breq .+14
        return (pm_getflag(s, 1) == 1) ? 9880 : 9866;
    }
    case 0x268a: { // cpi r18, 0x02
        pm_sub(s, s->r[18], 2, 0, false);
        return 9868;
    }
    case 0x268c: { // breq .+14
        return (pm_getflag(s, 1) == 1) ? 9884 : 9870;
    }
    case 0x268e: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 9872;
    }
    case 0x2690: { // neg r20
        uint8_t old = s->r[20];
        s->r[20] = pm_sub(s, 0, old, 0, false);
        pm_flag(s, HALF, (s->r[20] | old) & 8);
        return 9874;
    }
    case 0x2692: { // adc r21, r19
        s->r[21] = pm_add(s, s->r[21], s->r[19], pm_getflag(s, CARRY));
        return 9876;
    }
    case 0x2694: { // neg r21
        uint8_t old = s->r[21];
        s->r[21] = pm_sub(s, 0, old, 0, false);
        pm_flag(s, HALF, (s->r[21] | old) & 8);
        return 9878;
    }
    case 0x2696: { // brpl .+4
        return (pm_getflag(s, 2) == 0) ? 9884 : 9880;
    }
    case 0x2698: { // clc
        pm_flag(s, CARRY, false);
        return 9882;
    }
    case 0x269a: { // rjmp .+2
        return 9886;
    }
    case 0x269c: { // sec
        pm_flag(s, CARRY, true);
        return 9886;
    }
    case 0x269e: { // pop r0
        s->r[0] = s->stack[--s->depth];
        return 9888;
    }
    case 0x26a0: { // pop r1
        s->r[1] = s->stack[--s->depth];
        return 9890;
    }
    case 0x26a2: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 9892;
    }
    case 0x26a4: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9894;
    }
    case 0x26a6: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 9896;
    }
    case 0x26a8: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 9898;
    }
    case 0x26aa: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
