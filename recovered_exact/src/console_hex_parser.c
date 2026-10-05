#include <stdint.h>

/* Legacy result is R21:R20, delimiter R16, validity in carry.
 * Original accepts up to four uppercase hex digits and wraps at 16 bits. */
void cli_get_hex(void)
{
    register uint8_t count asm("r18"), character asm("r16");
    register uint16_t result asm("r20");
    asm volatile("push r22\n\tpush r18\n\tclr r18\n\tclr r20\n\tclr r21"
                 : "=r" (count), "=r" (result) : : "memory", "cc");
next_digit:;
    asm volatile("rcall cli_get_next_char" : "=r" (character) : : "memory", "cc");
    register uint8_t digit asm("r22") = character;
    asm volatile("" : "+r" (digit), "+r" (character));
    /* Rejected C range alternative (step_235_rejected.log):
     * if (character < '0') goto done;
     * if (character < ':') goto decode;
     * if (character < 'A') goto done;
     * if (character >= 'G') goto done;
     * It changed complete binary/layout matching. No independent functional
     * test is claimed for this fragment; retain original branches below.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("cpi r16, 0x30\n\tbrcs %l[done]\n\tcpi r16, 0x3a\n\tbrcs %l[decode]\n"
             "cpi r16, 0x41\n\tbrcs %l[done]\n\tcpi r16, 0x47\n\tbrcc %l[done]"
             : : "r" (character) : "cc" : done, decode);
    digit -= 7; asm volatile("" : "+r" (digit));
decode:
    digit -= '0'; asm volatile("" : "+r" (digit));
    result += result; asm volatile("" : "+r" (result));
    result += result; asm volatile("" : "+r" (result));
    result += result; asm volatile("" : "+r" (result));
    result += result; asm volatile("" : "+r" (result));
    result |= digit; asm volatile("" : "+r" (result));
    /* C equivalent: ++count; trial436 changed the exact encoding/layout.
     * Unvalidated standalone; retain original INC and its flags.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("inc %0" : "+r" (count) : : "cc");
    if (count != 4) goto next_digit;
    asm volatile("rcall cli_get_next_char" : "=r" (character) : : "memory", "cc");
done:
    asm volatile("tst r18\n\tbreq 1f\n\tclc\n\trjmp 2f\n1:\n\tsec\n2:\n"
                 "pop r18\n\tpop r22" : : "r" (count), "r" (result) : "memory", "cc");
    return;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 608 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_hex_parser(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x26ac: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 9902;
    }
    case 0x26ae: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9904;
    }
    case 0x26b0: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 9906;
    }
    case 0x26b2: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 9908;
    }
    case 0x26b4: { // eor r21, r21
        s->r[21] ^= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 9910;
    }
    case 0x26b6: { // rcall .+388
        s->calls[s->call_depth++] = 9912;
        return 10300;
    }
    case 0x26b8: { // mov r22, r16
        s->r[22] = s->r[16];
        return 9914;
    }
    case 0x26ba: { // cpi r16, 0x30
        pm_sub(s, s->r[16], 48, 0, false);
        return 9916;
    }
    case 0x26bc: { // brcs .+42
        return (pm_getflag(s, 0) == 1) ? 9960 : 9918;
    }
    case 0x26be: { // cpi r16, 0x3A
        pm_sub(s, s->r[16], 58, 0, false);
        return 9920;
    }
    case 0x26c0: { // brcs .+10
        return (pm_getflag(s, 0) == 1) ? 9932 : 9922;
    }
    case 0x26c2: { // cpi r16, 0x41
        pm_sub(s, s->r[16], 65, 0, false);
        return 9924;
    }
    case 0x26c4: { // brcs .+34
        return (pm_getflag(s, 0) == 1) ? 9960 : 9926;
    }
    case 0x26c6: { // cpi r16, 0x47
        pm_sub(s, s->r[16], 71, 0, false);
        return 9928;
    }
    case 0x26c8: { // brcc .+30
        return (pm_getflag(s, 0) == 0) ? 9960 : 9930;
    }
    case 0x26ca: { // subi r22, 0x07
        s->r[22] = pm_sub(s, s->r[22], 7, 0, false);
        return 9932;
    }
    case 0x26cc: { // subi r22, 0x30
        s->r[22] = pm_sub(s, s->r[22], 48, 0, false);
        return 9934;
    }
    case 0x26ce: { // add r20, r20
        s->r[20] = pm_add(s, s->r[20], s->r[20], 0);
        return 9936;
    }
    case 0x26d0: { // adc r21, r21
        s->r[21] = pm_add(s, s->r[21], s->r[21], pm_getflag(s, CARRY));
        return 9938;
    }
    case 0x26d2: { // add r20, r20
        s->r[20] = pm_add(s, s->r[20], s->r[20], 0);
        return 9940;
    }
    case 0x26d4: { // adc r21, r21
        s->r[21] = pm_add(s, s->r[21], s->r[21], pm_getflag(s, CARRY));
        return 9942;
    }
    case 0x26d6: { // add r20, r20
        s->r[20] = pm_add(s, s->r[20], s->r[20], 0);
        return 9944;
    }
    case 0x26d8: { // adc r21, r21
        s->r[21] = pm_add(s, s->r[21], s->r[21], pm_getflag(s, CARRY));
        return 9946;
    }
    case 0x26da: { // add r20, r20
        s->r[20] = pm_add(s, s->r[20], s->r[20], 0);
        return 9948;
    }
    case 0x26dc: { // adc r21, r21
        s->r[21] = pm_add(s, s->r[21], s->r[21], pm_getflag(s, CARRY));
        return 9950;
    }
    case 0x26de: { // or r20, r22
        s->r[20] |= s->r[22];
        pm_nzv(s, s->r[20], false);
        return 9952;
    }
    case 0x26e0: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 9954;
    }
    case 0x26e2: { // cpi r18, 0x04
        pm_sub(s, s->r[18], 4, 0, false);
        return 9956;
    }
    case 0x26e4: { // brne .-48
        return (pm_getflag(s, 1) == 0) ? 9910 : 9958;
    }
    case 0x26e6: { // rcall .+340
        s->calls[s->call_depth++] = 9960;
        return 10300;
    }
    case 0x26e8: { // and r18, r18
        s->r[18] &= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 9962;
    }
    case 0x26ea: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 9968 : 9964;
    }
    case 0x26ec: { // clc
        pm_flag(s, CARRY, false);
        return 9966;
    }
    case 0x26ee: { // rjmp .+2
        return 9970;
    }
    case 0x26f0: { // sec
        pm_flag(s, CARRY, true);
        return 9970;
    }
    case 0x26f2: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9972;
    }
    case 0x26f4: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 9974;
    }
    case 0x26f6: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
