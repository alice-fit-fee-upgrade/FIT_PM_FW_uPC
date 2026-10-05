#ifndef PM_LEGACY_FLAG_OPS_H
#define PM_LEGACY_FLAG_OPS_H

/* Exact in-place private-register operations. These are instruction macros,
 * not GNU C ABI calls. The original raw register mutation stays opaque to GCC:
 * callers must capture the register before C reads it, as in the existing ISR.
 * No return value, stack frame or extra branch is introduced.
 *
 * REFERENCE_C_INFERRED, value-only equivalents:
 *   clear:     value = 0;
 *   increment: value = (uint8_t)(value + 1);
 *   decrement: value = (uint8_t)(value - 1);
 *
 * The exact SREG effects matter even when these expressions have the same value:
 * EOR-self sets Z=1 and N=V=S=0; C/H/T/I are unchanged.
 * INC/DEC set Z/N/V/S (overflow at 0x7f->0x80 / 0x80->0x7f), preserving C/H/T/I.
 * Ordinary C tried in the ISR generated different opcode bytes (LDI/SUBI).
 * The separately compiled transition model below verifies the actual operations;
 * it is not a claim of whole-path validation of the value-only expressions. */
#define PM_CLEAR_LEGACY_BYTE(reg) \
    asm volatile("eor " reg "," reg : : : "memory", "cc")
#define PM_INCREMENT_LEGACY_BYTE(reg) \
    asm volatile("inc " reg : : : "memory", "cc")
#define PM_DECREMENT_LEGACY_BYTE(reg) \
    asm volatile("dec " reg : : : "memory", "cc")
#endif

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 656 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_legacy_flag_ops(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0392: { // eor r31, r31
        s->r[31] ^= s->r[31];
        pm_nzv(s, s->r[31], false);
        return 916;
    }
    case 0x03a0: { // eor r18, r19
        s->r[18] ^= s->r[19];
        pm_nzv(s, s->r[18], false);
        return 930;
    }
    case 0x09a6: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 2472;
    }
    case 0x0b1e: { // eor r16, r19
        s->r[16] ^= s->r[19];
        pm_nzv(s, s->r[16], false);
        return 2848;
    }
    case 0x0bc0: { // eor r18, r17
        s->r[18] ^= s->r[17];
        pm_nzv(s, s->r[18], false);
        return 3010;
    }
    case 0x0c04: { // dec r16
        s->r[16]--;
        pm_nzv(s, s->r[16], s->r[16] == 127);
        return 3078;
    }
    case 0x0c96: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3224;
    }
    case 0x0dce: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 3536;
    }
    case 0x0dd0: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 3538;
    }
    case 0x0e00: { // dec r20
        s->r[20]--;
        pm_nzv(s, s->r[20], s->r[20] == 127);
        return 3586;
    }
    case 0x0e34: { // eor r29, r29
        s->r[29] ^= s->r[29];
        pm_nzv(s, s->r[29], false);
        return 3638;
    }
    case 0x0e3e: { // inc r29
        s->r[29]++;
        pm_nzv(s, s->r[29], s->r[29] == 128);
        return 3648;
    }
    case 0x0eb6: { // inc r16
        s->r[16]++;
        pm_nzv(s, s->r[16], s->r[16] == 128);
        return 3768;
    }
    case 0x0ef8: { // inc r17
        s->r[17]++;
        pm_nzv(s, s->r[17], s->r[17] == 128);
        return 3834;
    }
    case 0x1092: { // eor r25, r25
        s->r[25] ^= s->r[25];
        pm_nzv(s, s->r[25], false);
        return 4244;
    }
    case 0x10b0: { // inc r25
        s->r[25]++;
        pm_nzv(s, s->r[25], s->r[25] == 128);
        return 4274;
    }
    case 0x177e: { // eor r10, r10
        s->r[10] ^= s->r[10];
        pm_nzv(s, s->r[10], false);
        return 6016;
    }
    case 0x17fa: { // eor r18, r23
        s->r[18] ^= s->r[23];
        pm_nzv(s, s->r[18], false);
        return 6140;
    }
    case 0x17fc: { // eor r19, r24
        s->r[19] ^= s->r[24];
        pm_nzv(s, s->r[19], false);
        return 6142;
    }
    case 0x17fe: { // eor r16, r25
        s->r[16] ^= s->r[25];
        pm_nzv(s, s->r[16], false);
        return 6144;
    }
    case 0x1800: { // eor r17, r26
        s->r[17] ^= s->r[26];
        pm_nzv(s, s->r[17], false);
        return 6146;
    }
    case 0x184c: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 6222;
    }
    case 0x198c: { // eor r24, r24
        s->r[24] ^= s->r[24];
        pm_nzv(s, s->r[24], false);
        return 6542;
    }
    case 0x19a2: { // inc r23
        s->r[23]++;
        pm_nzv(s, s->r[23], s->r[23] == 128);
        return 6564;
    }
    case 0x19ee: { // eor r17, r16
        s->r[17] ^= s->r[16];
        pm_nzv(s, s->r[17], false);
        return 6640;
    }
    case 0x1d3c: { // inc r20
        s->r[20]++;
        pm_nzv(s, s->r[20], s->r[20] == 128);
        return 7486;
    }
    case 0x1f80: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 8066;
    }
    case 0x20f6: { // eor r17, r18
        s->r[17] ^= s->r[18];
        pm_nzv(s, s->r[17], false);
        return 8440;
    }
    case 0x2152: { // inc r1
        s->r[1]++;
        pm_nzv(s, s->r[1], s->r[1] == 128);
        return 8532;
    }
    case 0x2190: { // dec r19
        s->r[19]--;
        pm_nzv(s, s->r[19], s->r[19] == 127);
        return 8594;
    }
    case 0x21d6: { // dec r21
        s->r[21]--;
        pm_nzv(s, s->r[21], s->r[21] == 127);
        return 8664;
    }
    case 0x231a: { // eor r21, r21
        s->r[21] ^= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8988;
    }
    case 0x24d4: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 9430;
    }
    case 0x24e2: { // eor r23, r23
        s->r[23] ^= s->r[23];
        pm_nzv(s, s->r[23], false);
        return 9444;
    }
    case 0x262e: { // dec r18
        s->r[18]--;
        pm_nzv(s, s->r[18], s->r[18] == 127);
        return 9776;
    }
    case 0x2654: { // inc r19
        s->r[19]++;
        pm_nzv(s, s->r[19], s->r[19] == 128);
        return 9814;
    }
    case 0x266c: { // eor r22, r22
        s->r[22] ^= s->r[22];
        pm_nzv(s, s->r[22], false);
        return 9838;
    }
    case 0x2766: { // eor r15, r15
        s->r[15] ^= s->r[15];
        pm_nzv(s, s->r[15], false);
        return 10088;
    }
    case 0x276a: { // inc r15
        s->r[15]++;
        pm_nzv(s, s->r[15], s->r[15] == 128);
        return 10092;
    }
    case 0x2784: { // eor r14, r14
        s->r[14] ^= s->r[14];
        pm_nzv(s, s->r[14], false);
        return 10118;
    }
    case 0x288c: { // dec r17
        s->r[17]--;
        pm_nzv(s, s->r[17], s->r[17] == 127);
        return 10382;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
