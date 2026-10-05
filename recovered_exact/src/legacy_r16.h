#ifndef PM_LEGACY_R16_H
#define PM_LEGACY_R16_H
#include <stdint.h>

/* Tiny exact fragments for the original non-GNU scratch-register ABI.
 * CLR updates flags; LDI leaves them untouched. They are not interchangeable.
 * These helpers are private to files compiled with the r16_only profile. */
static inline uint8_t pm_scratch_zero(void)
{
    register uint8_t value asm("r16");
    asm volatile ("clr %0" : "=r" (value) : : "cc");
    return value;
}

/* GNU statement expressions avoid introducing a GNU R24 argument ABI. */
#define pm_scratch_constant(constant) \
({ register uint8_t pm_value asm("r16"); \
   asm volatile ("ldi %0, %1" : "=r" (pm_value) : "M" (constant)); \
   pm_value; })

#define pm_read_absolute(address) \
({ register uint8_t pm_value asm("r16"); \
   asm volatile ("lds %0, %1" : "=r" (pm_value) : "n" (address) : "memory"); \
   pm_value; })
#endif

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 5648 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_legacy_r16(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0202: { // lds r17, 0x0124
        uint16_t address = 292;
        s->r[17] = pm_read(s, address);
        return 518;
    }
    case 0x0206: { // ldi r16, 0x11
        s->r[16] = 17;
        return 520;
    }
    case 0x0210: { // ldi r28, 0x39
        s->r[28] = 57;
        return 530;
    }
    case 0x02bc: { // ldi r16, 0x94
        s->r[16] = 148;
        return 702;
    }
    case 0x02d0: { // ldi r26, 0x39
        s->r[26] = 57;
        return 722;
    }
    case 0x02f6: { // lds r16, 0x09C2
        uint16_t address = 2498;
        s->r[16] = pm_read(s, address);
        return 762;
    }
    case 0x02fa: { // lds r16, 0x09C3
        uint16_t address = 2499;
        s->r[16] = pm_read(s, address);
        return 766;
    }
    case 0x02fe: { // ldi r16, 0x84
        s->r[16] = 132;
        return 768;
    }
    case 0x0304: { // lds r16, 0x0121
        uint16_t address = 289;
        s->r[16] = pm_read(s, address);
        return 776;
    }
    case 0x0356: { // ldi r16, 0xB4
        s->r[16] = 180;
        return 856;
    }
    case 0x038a: { // ldi r31, 0x01
        s->r[31] = 1;
        return 908;
    }
    case 0x0392: { // eor r31, r31
        s->r[31] ^= s->r[31];
        pm_nzv(s, s->r[31], false);
        return 916;
    }
    case 0x0398: { // lds r18, 0x2435
        uint16_t address = 9269;
        s->r[18] = pm_read(s, address);
        return 924;
    }
    case 0x03a0: { // eor r18, r19
        s->r[18] ^= s->r[19];
        pm_nzv(s, s->r[18], false);
        return 930;
    }
    case 0x0426: { // ldi r26, 0x3D
        s->r[26] = 61;
        return 1064;
    }
    case 0x0474: { // lds r18, 0x0668
        uint16_t address = 1640;
        s->r[18] = pm_read(s, address);
        return 1144;
    }
    case 0x0486: { // lds r16, 0x0111
        uint16_t address = 273;
        s->r[16] = pm_read(s, address);
        return 1162;
    }
    case 0x04c2: { // ldi r30, 0x59
        s->r[30] = 89;
        return 1220;
    }
    case 0x052a: { // ldi r16, 0xFA
        s->r[16] = 250;
        return 1324;
    }
    case 0x0556: { // ldi r16, 0x82
        s->r[16] = 130;
        return 1368;
    }
    case 0x058c: { // ldi r17, 0x03
        s->r[17] = 3;
        return 1422;
    }
    case 0x0590: { // ldi r19, 0x07
        s->r[19] = 7;
        return 1426;
    }
    case 0x05fe: { // ldi r18, 0xA6
        s->r[18] = 166;
        return 1536;
    }
    case 0x0600: { // ldi r19, 0x03
        s->r[19] = 3;
        return 1538;
    }
    case 0x0608: { // ldi r16, 0x81
        s->r[16] = 129;
        return 1546;
    }
    case 0x0668: { // ldi r24, 0xE8
        s->r[24] = 232;
        return 1642;
    }
    case 0x066a: { // ldi r25, 0x03
        s->r[25] = 3;
        return 1644;
    }
    case 0x06d2: { // ldi r24, 0x88
        s->r[24] = 136;
        return 1748;
    }
    case 0x06d4: { // ldi r25, 0x13
        s->r[25] = 19;
        return 1750;
    }
    case 0x078e: { // ldi r17, 0x06
        s->r[17] = 6;
        return 1936;
    }
    case 0x0796: { // ldi r17, 0x04
        s->r[17] = 4;
        return 1944;
    }
    case 0x07d0: { // ldi r18, 0x14
        s->r[18] = 20;
        return 2002;
    }
    case 0x0802: { // ldi r18, 0x7D
        s->r[18] = 125;
        return 2052;
    }
    case 0x0850: { // ldi r16, 0x1E
        s->r[16] = 30;
        return 2130;
    }
    case 0x0860: { // ldi r16, 0x96
        s->r[16] = 150;
        return 2146;
    }
    case 0x0896: { // ldi r20, 0x0C
        s->r[20] = 12;
        return 2200;
    }
    case 0x0898: { // ldi r21, 0xFE
        s->r[21] = 254;
        return 2202;
    }
    case 0x089c: { // ldi r20, 0xF4
        s->r[20] = 244;
        return 2206;
    }
    case 0x089e: { // ldi r21, 0x01
        s->r[21] = 1;
        return 2208;
    }
    case 0x08ee: { // ldi r18, 0x7C
        s->r[18] = 124;
        return 2288;
    }
    case 0x08fc: { // lds r16, 0x222F
        uint16_t address = 8751;
        s->r[16] = pm_read(s, address);
        return 2304;
    }
    case 0x090c: { // ldi r28, 0xB7
        s->r[28] = 183;
        return 2318;
    }
    case 0x0922: { // ldi r18, 0x24
        s->r[18] = 36;
        return 2340;
    }
    case 0x0924: { // ldi r28, 0x87
        s->r[28] = 135;
        return 2342;
    }
    case 0x093c: { // lds r16, 0x2230
        uint16_t address = 8752;
        s->r[16] = pm_read(s, address);
        return 2368;
    }
    case 0x0940: { // lds r17, 0x2231
        uint16_t address = 8753;
        s->r[17] = pm_read(s, address);
        return 2372;
    }
    case 0x094c: { // lds r16, 0x2160
        uint16_t address = 8544;
        s->r[16] = pm_read(s, address);
        return 2384;
    }
    case 0x0950: { // lds r17, 0x2161
        uint16_t address = 8545;
        s->r[17] = pm_read(s, address);
        return 2388;
    }
    case 0x095e: { // lds r16, 0x2232
        uint16_t address = 8754;
        s->r[16] = pm_read(s, address);
        return 2402;
    }
    case 0x0962: { // lds r17, 0x2233
        uint16_t address = 8755;
        s->r[17] = pm_read(s, address);
        return 2406;
    }
    case 0x0966: { // ldi r18, 0xBD
        s->r[18] = 189;
        return 2408;
    }
    case 0x0974: { // ldi r17, 0x0F
        s->r[17] = 15;
        return 2422;
    }
    case 0x0996: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 2456;
    }
    case 0x0998: { // ldi r29, 0x21
        s->r[29] = 33;
        return 2458;
    }
    case 0x09ac: { // ldi r28, 0x63
        s->r[28] = 99;
        return 2478;
    }
    case 0x09c2: { // lds r16, 0x2441
        uint16_t address = 9281;
        s->r[16] = pm_read(s, address);
        return 2502;
    }
    case 0x09c8: { // ldi r18, 0xBE
        s->r[18] = 190;
        return 2506;
    }
    case 0x0a52: { // lds r16, 0x0668
        uint16_t address = 1640;
        s->r[16] = pm_read(s, address);
        return 2646;
    }
    case 0x0aa0: { // ldi r16, 0x80
        s->r[16] = 128;
        return 2722;
    }
    case 0x0aa6: { // lds r16, 0x2159
        uint16_t address = 8537;
        s->r[16] = pm_read(s, address);
        return 2730;
    }
    case 0x0af8: { // ldi r16, 0xF5
        s->r[16] = 245;
        return 2810;
    }
    case 0x0afc: { // ldi r18, 0x04
        s->r[18] = 4;
        return 2814;
    }
    case 0x0afe: { // ldi r19, 0x40
        s->r[19] = 64;
        return 2816;
    }
    case 0x0b14: { // lds r16, 0x2162
        uint16_t address = 8546;
        s->r[16] = pm_read(s, address);
        return 2840;
    }
    case 0x0b1e: { // eor r16, r19
        s->r[16] ^= s->r[19];
        pm_nzv(s, s->r[16], false);
        return 2848;
    }
    case 0x0b6e: { // ldi r16, 0x08
        s->r[16] = 8;
        return 2928;
    }
    case 0x0b7a: { // ldi r16, 0xC0
        s->r[16] = 192;
        return 2940;
    }
    case 0x0ba4: { // lds r16, 0x0688
        uint16_t address = 1672;
        s->r[16] = pm_read(s, address);
        return 2984;
    }
    case 0x0bc0: { // eor r18, r17
        s->r[18] ^= s->r[17];
        pm_nzv(s, s->r[18], false);
        return 3010;
    }
    case 0x0bd6: { // ldi r17, 0xD0
        s->r[17] = 208;
        return 3032;
    }
    case 0x0bd8: { // ldi r18, 0x07
        s->r[18] = 7;
        return 3034;
    }
    case 0x0bf4: { // lds r16, 0x215B
        uint16_t address = 8539;
        s->r[16] = pm_read(s, address);
        return 3064;
    }
    case 0x0bfc: { // lds r16, 0x2442
        uint16_t address = 9282;
        s->r[16] = pm_read(s, address);
        return 3072;
    }
    case 0x0c10: { // ldi r17, 0x88
        s->r[17] = 136;
        return 3090;
    }
    case 0x0c12: { // ldi r18, 0x13
        s->r[18] = 19;
        return 3092;
    }
    case 0x0c34: { // ldi r18, 0x7F
        s->r[18] = 127;
        return 3126;
    }
    case 0x0c3e: { // lds r18, 0x2006
        uint16_t address = 8198;
        s->r[18] = pm_read(s, address);
        return 3138;
    }
    case 0x0c52: { // lds r16, 0x2158
        uint16_t address = 8536;
        s->r[16] = pm_read(s, address);
        return 3158;
    }
    case 0x0c96: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3224;
    }
    case 0x0cb4: { // ldi r16, 0x01
        s->r[16] = 1;
        return 3254;
    }
    case 0x0cc0: { // ldi r16, 0x00
        s->r[16] = 0;
        return 3266;
    }
    case 0x0cca: { // ldi r16, 0xBF
        s->r[16] = 191;
        return 3276;
    }
    case 0x0cd6: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 3288;
    }
    case 0x0cdc: { // ldi r16, 0x41
        s->r[16] = 65;
        return 3294;
    }
    case 0x0ce2: { // ldi r16, 0x04
        s->r[16] = 4;
        return 3300;
    }
    case 0x0ce8: { // ldi r16, 0x20
        s->r[16] = 32;
        return 3306;
    }
    case 0x0cf4: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 3320;
    }
    case 0x0cfe: { // ldi r16, 0xA0
        s->r[16] = 160;
        return 3328;
    }
    case 0x0d10: { // ldi r16, 0xBC
        s->r[16] = 188;
        return 3346;
    }
    case 0x0d1c: { // ldi r16, 0x07
        s->r[16] = 7;
        return 3358;
    }
    case 0x0d3a: { // ldi r16, 0x30
        s->r[16] = 48;
        return 3388;
    }
    case 0x0d40: { // ldi r16, 0xD1
        s->r[16] = 209;
        return 3394;
    }
    case 0x0d46: { // ldi r16, 0x40
        s->r[16] = 64;
        return 3400;
    }
    case 0x0d4c: { // lds r16, 0x0100
        uint16_t address = 256;
        s->r[16] = pm_read(s, address);
        return 3408;
    }
    case 0x0d54: { // ldi r16, 0x44
        s->r[16] = 68;
        return 3414;
    }
    case 0x0d5a: { // ldi r16, 0x83
        s->r[16] = 131;
        return 3420;
    }
    case 0x0d60: { // ldi r16, 0x6A
        s->r[16] = 106;
        return 3426;
    }
    case 0x0d66: { // ldi r16, 0xC3
        s->r[16] = 195;
        return 3432;
    }
    case 0x0d6c: { // ldi r16, 0x09
        s->r[16] = 9;
        return 3438;
    }
    case 0x0d84: { // ldi r16, 0x39
        s->r[16] = 57;
        return 3462;
    }
    case 0x0d8a: { // ldi r16, 0x24
        s->r[16] = 36;
        return 3468;
    }
    case 0x0db8: { // ldi r16, 0x05
        s->r[16] = 5;
        return 3514;
    }
    case 0x0dbe: { // ldi r16, 0xA4
        s->r[16] = 164;
        return 3520;
    }
    case 0x0dcc: { // ldi r17, 0x10
        s->r[17] = 16;
        return 3534;
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
    case 0x0dea: { // ldi r30, 0x16
        s->r[30] = 22;
        return 3564;
    }
    case 0x0dee: { // ldi r20, 0x0A
        s->r[20] = 10;
        return 3568;
    }
    case 0x0e04: { // ldi r16, 0x0A
        s->r[16] = 10;
        return 3590;
    }
    case 0x0e30: { // ldi r30, 0x04
        s->r[30] = 4;
        return 3634;
    }
    case 0x0e34: { // eor r29, r29
        s->r[29] ^= s->r[29];
        pm_nzv(s, s->r[29], false);
        return 3638;
    }
    case 0x0e36: { // lds r16, 0x06A8
        uint16_t address = 1704;
        s->r[16] = pm_read(s, address);
        return 3642;
    }
    case 0x0e48: { // lds r18, 0x0BA1
        uint16_t address = 2977;
        s->r[18] = pm_read(s, address);
        return 3660;
    }
    case 0x0e68: { // lds r18, 0x0BA3
        uint16_t address = 2979;
        s->r[18] = pm_read(s, address);
        return 3692;
    }
    case 0x0e98: { // ldi r30, 0x02
        s->r[30] = 2;
        return 3738;
    }
    case 0x0e9a: { // ldi r31, 0x20
        s->r[31] = 32;
        return 3740;
    }
    case 0x0eaa: { // lds r19, 0x0BA3
        uint16_t address = 2979;
        s->r[19] = pm_read(s, address);
        return 3758;
    }
    case 0x0eba: { // ldi r30, 0x47
        s->r[30] = 71;
        return 3772;
    }
    case 0x0ef0: { // ldi r30, 0x00
        s->r[30] = 0;
        return 3826;
    }
    case 0x0f06: { // ldi r18, 0x01
        s->r[18] = 1;
        return 3848;
    }
    case 0x0f0c: { // lds r18, 0x2005
        uint16_t address = 8197;
        s->r[18] = pm_read(s, address);
        return 3856;
    }
    case 0x0f16: { // lds r19, 0x0BA1
        uint16_t address = 2977;
        s->r[19] = pm_read(s, address);
        return 3866;
    }
    case 0x0f1a: { // lds r18, 0x0BA0
        uint16_t address = 2976;
        s->r[18] = pm_read(s, address);
        return 3870;
    }
    case 0x0f28: { // ldi r30, 0x07
        s->r[30] = 7;
        return 3882;
    }
    case 0x0f38: { // lds r16, 0x2005
        uint16_t address = 8197;
        s->r[16] = pm_read(s, address);
        return 3900;
    }
    case 0x0f54: { // ldi r16, 0xCB
        s->r[16] = 203;
        return 3926;
    }
    case 0x0f60: { // lds r17, 0x0051
        uint16_t address = 81;
        s->r[17] = pm_read(s, address);
        return 3940;
    }
    case 0x0f68: { // ldi r16, 0xC2
        s->r[16] = 194;
        return 3946;
    }
    case 0x0f6e: { // ldi r16, 0x19
        s->r[16] = 25;
        return 3952;
    }
    case 0x0f88: { // ldi r16, 0x18
        s->r[16] = 24;
        return 3978;
    }
    case 0x0f8e: { // ldi r25, 0xFF
        s->r[25] = 255;
        return 3984;
    }
    case 0x0f92: { // ldi r25, 0x3F
        s->r[25] = 63;
        return 3988;
    }
    case 0x0f96: { // ldi r16, 0xFB
        s->r[16] = 251;
        return 3992;
    }
    case 0x0f9c: { // ldi r16, 0xF3
        s->r[16] = 243;
        return 3998;
    }
    case 0x0fc8: { // ldi r16, 0x28
        s->r[16] = 40;
        return 4042;
    }
    case 0x106e: { // ldi r16, 0x7D
        s->r[16] = 125;
        return 4208;
    }
    case 0x1092: { // eor r25, r25
        s->r[25] ^= s->r[25];
        pm_nzv(s, s->r[25], false);
        return 4244;
    }
    case 0x1098: { // ldi r28, 0x43
        s->r[28] = 67;
        return 4250;
    }
    case 0x109a: { // ldi r29, 0x24
        s->r[29] = 36;
        return 4252;
    }
    case 0x10e2: { // ldi r16, 0xE8
        s->r[16] = 232;
        return 4324;
    }
    case 0x1106: { // ldi r17, 0x50
        s->r[17] = 80;
        return 4360;
    }
    case 0x110e: { // ldi r17, 0x00
        s->r[17] = 0;
        return 4368;
    }
    case 0x1110: { // ldi r18, 0x23
        s->r[18] = 35;
        return 4370;
    }
    case 0x111a: { // ldi r18, 0x1E
        s->r[18] = 30;
        return 4380;
    }
    case 0x1124: { // ldi r28, 0x00
        s->r[28] = 0;
        return 4390;
    }
    case 0x1126: { // ldi r29, 0x10
        s->r[29] = 16;
        return 4392;
    }
    case 0x1128: { // ldi r17, 0x22
        s->r[17] = 34;
        return 4394;
    }
    case 0x1138: { // ldi r30, 0x62
        s->r[30] = 98;
        return 4410;
    }
    case 0x1146: { // lds r16, 0x2006
        uint16_t address = 8198;
        s->r[16] = pm_read(s, address);
        return 4426;
    }
    case 0x1164: { // lds r16, 0x0624
        uint16_t address = 1572;
        s->r[16] = pm_read(s, address);
        return 4456;
    }
    case 0x1194: { // lds r20, 0x0689
        uint16_t address = 1673;
        s->r[20] = pm_read(s, address);
        return 4504;
    }
    case 0x11be: { // ldi r16, 0x2C
        s->r[16] = 44;
        return 4544;
    }
    case 0x11c0: { // ldi r17, 0x01
        s->r[17] = 1;
        return 4546;
    }
    case 0x11c6: { // ldi r17, 0x75
        s->r[17] = 117;
        return 4552;
    }
    case 0x1200: { // ldi r17, 0xFE
        s->r[17] = 254;
        return 4610;
    }
    case 0x1204: { // ldi r16, 0xF4
        s->r[16] = 244;
        return 4614;
    }
    case 0x1274: { // ldi r17, 0x4E
        s->r[17] = 78;
        return 4726;
    }
    case 0x12be: { // ldi r30, 0xCF
        s->r[30] = 207;
        return 4800;
    }
    case 0x13cc: { // ldi r30, 0x88
        s->r[30] = 136;
        return 5070;
    }
    case 0x13e0: { // lds r16, 0x2234
        uint16_t address = 8756;
        s->r[16] = pm_read(s, address);
        return 5092;
    }
    case 0x1414: { // ldi r28, 0xD1
        s->r[28] = 209;
        return 5142;
    }
    case 0x1554: { // ldi r16, 0x43
        s->r[16] = 67;
        return 5462;
    }
    case 0x1560: { // ldi r16, 0x26
        s->r[16] = 38;
        return 5474;
    }
    case 0x1582: { // ldi r30, 0x76
        s->r[30] = 118;
        return 5508;
    }
    case 0x15b6: { // ldi r27, 0x24
        s->r[27] = 36;
        return 5560;
    }
    case 0x161a: { // lds r24, 0x2435
        uint16_t address = 9269;
        s->r[24] = pm_read(s, address);
        return 5662;
    }
    case 0x161e: { // lds r25, 0x2436
        uint16_t address = 9270;
        s->r[25] = pm_read(s, address);
        return 5666;
    }
    case 0x1664: { // ldi r16, 0x02
        s->r[16] = 2;
        return 5734;
    }
    case 0x166a: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5740;
    }
    case 0x1670: { // ldi r16, 0xB0
        s->r[16] = 176;
        return 5746;
    }
    case 0x1676: { // ldi r16, 0x50
        s->r[16] = 80;
        return 5752;
    }
    case 0x169e: { // ldi r16, 0x88
        s->r[16] = 136;
        return 5792;
    }
    case 0x16a4: { // ldi r16, 0x13
        s->r[16] = 19;
        return 5798;
    }
    case 0x16b8: { // lds r24, 0x2437
        uint16_t address = 9271;
        s->r[24] = pm_read(s, address);
        return 5820;
    }
    case 0x16bc: { // lds r25, 0x2438
        uint16_t address = 9272;
        s->r[25] = pm_read(s, address);
        return 5824;
    }
    case 0x16c0: { // ldi r26, 0x35
        s->r[26] = 53;
        return 5826;
    }
    case 0x16c2: { // ldi r27, 0x22
        s->r[27] = 34;
        return 5828;
    }
    case 0x1712: { // ldi r16, 0xD8
        s->r[16] = 216;
        return 5908;
    }
    case 0x1724: { // ldi r16, 0x06
        s->r[16] = 6;
        return 5926;
    }
    case 0x172a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 5934;
    }
    case 0x173a: { // ldi r24, 0x40
        s->r[24] = 64;
        return 5948;
    }
    case 0x173c: { // ldi r25, 0x06
        s->r[25] = 6;
        return 5950;
    }
    case 0x1762: { // lds r24, 0x0AC3
        uint16_t address = 2755;
        s->r[24] = pm_read(s, address);
        return 5990;
    }
    case 0x1778: { // ldi r16, 0x03
        s->r[16] = 3;
        return 6010;
    }
    case 0x177e: { // eor r10, r10
        s->r[10] ^= s->r[10];
        pm_nzv(s, s->r[10], false);
        return 6016;
    }
    case 0x17a8: { // ldi r18, 0xFF
        s->r[18] = 255;
        return 6058;
    }
    case 0x17aa: { // ldi r19, 0xFF
        s->r[19] = 255;
        return 6060;
    }
    case 0x17ac: { // ldi r23, 0xB7
        s->r[23] = 183;
        return 6062;
    }
    case 0x17ae: { // ldi r24, 0x1D
        s->r[24] = 29;
        return 6064;
    }
    case 0x17b0: { // ldi r25, 0xC1
        s->r[25] = 193;
        return 6066;
    }
    case 0x17b2: { // ldi r26, 0x04
        s->r[26] = 4;
        return 6068;
    }
    case 0x17b4: { // lds r22, 0x0AC2
        uint16_t address = 2754;
        s->r[22] = pm_read(s, address);
        return 6072;
    }
    case 0x17c8: { // lds r22, 0x0AC3
        uint16_t address = 2755;
        s->r[22] = pm_read(s, address);
        return 6092;
    }
    case 0x17e4: { // ldi r20, 0x08
        s->r[20] = 8;
        return 6118;
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
    case 0x1846: { // ldi r16, 0x9F
        s->r[16] = 159;
        return 6216;
    }
    case 0x184c: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 6222;
    }
    case 0x1862: { // lds r18, 0x0AC3
        uint16_t address = 2755;
        s->r[18] = pm_read(s, address);
        return 6246;
    }
    case 0x1872: { // lds r19, 0x0AC3
        uint16_t address = 2755;
        s->r[19] = pm_read(s, address);
        return 6262;
    }
    case 0x187a: { // lds r20, 0x0AC2
        uint16_t address = 2754;
        s->r[20] = pm_read(s, address);
        return 6270;
    }
    case 0x1882: { // lds r16, 0x0AC3
        uint16_t address = 2755;
        s->r[16] = pm_read(s, address);
        return 6278;
    }
    case 0x189a: { // ldi r18, 0x64
        s->r[18] = 100;
        return 6300;
    }
    case 0x18cc: { // ldi r18, 0x0D
        s->r[18] = 13;
        return 6350;
    }
    case 0x18ea: { // ldi r18, 0x4C
        s->r[18] = 76;
        return 6380;
    }
    case 0x191c: { // ldi r18, 0x3E
        s->r[18] = 62;
        return 6430;
    }
    case 0x1940: { // ldi r18, 0x3F
        s->r[18] = 63;
        return 6466;
    }
    case 0x1958: { // ldi r18, 0x40
        s->r[18] = 64;
        return 6490;
    }
    case 0x198c: { // eor r24, r24
        s->r[24] ^= s->r[24];
        pm_nzv(s, s->r[24], false);
        return 6542;
    }
    case 0x19bc: { // ldi r16, 0x36
        s->r[16] = 54;
        return 6590;
    }
    case 0x19d2: { // ldi r30, 0x63
        s->r[30] = 99;
        return 6612;
    }
    case 0x19d4: { // ldi r31, 0x21
        s->r[31] = 33;
        return 6614;
    }
    case 0x19d6: { // ldi r28, 0xFF
        s->r[28] = 255;
        return 6616;
    }
    case 0x19d8: { // ldi r29, 0x0F
        s->r[29] = 15;
        return 6618;
    }
    case 0x19da: { // ldi r20, 0x22
        s->r[20] = 34;
        return 6620;
    }
    case 0x19ee: { // eor r17, r16
        s->r[17] ^= s->r[16];
        pm_nzv(s, s->r[17], false);
        return 6640;
    }
    case 0x19f4: { // ldi r19, 0x01
        s->r[19] = 1;
        return 6646;
    }
    case 0x1a12: { // ldi r16, 0x35
        s->r[16] = 53;
        return 6676;
    }
    case 0x1a22: { // ldi r17, 0xD8
        s->r[17] = 216;
        return 6692;
    }
    case 0x1a30: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 6708;
    }
    case 0x1a44: { // ldi r30, 0x3E
        s->r[30] = 62;
        return 6726;
    }
    case 0x1a48: { // ldi r20, 0x09
        s->r[20] = 9;
        return 6730;
    }
    case 0x1a6a: { // ldi r30, 0x6A
        s->r[30] = 106;
        return 6764;
    }
    case 0x1a7a: { // ldi r30, 0x7E
        s->r[30] = 126;
        return 6780;
    }
    case 0x1a92: { // ldi r30, 0xA6
        s->r[30] = 166;
        return 6804;
    }
    case 0x1a9a: { // lds r18, 0x2157
        uint16_t address = 8535;
        s->r[18] = pm_read(s, address);
        return 6814;
    }
    case 0x1aa6: { // lds r20, 0x0688
        uint16_t address = 1672;
        s->r[20] = pm_read(s, address);
        return 6826;
    }
    case 0x1abc: { // ldi r30, 0xC0
        s->r[30] = 192;
        return 6846;
    }
    case 0x1ac4: { // ldi r30, 0xCE
        s->r[30] = 206;
        return 6854;
    }
    case 0x1acc: { // ldi r30, 0xE2
        s->r[30] = 226;
        return 6862;
    }
    case 0x1ad4: { // ldi r30, 0xDC
        s->r[30] = 220;
        return 6870;
    }
    case 0x1adc: { // ldi r30, 0xD6
        s->r[30] = 214;
        return 6878;
    }
    case 0x1ae2: { // ldi r30, 0xEC
        s->r[30] = 236;
        return 6884;
    }
    case 0x1af4: { // ldi r30, 0xFC
        s->r[30] = 252;
        return 6902;
    }
    case 0x1b0c: { // ldi r30, 0xF2
        s->r[30] = 242;
        return 6926;
    }
    case 0x1b14: { // ldi r30, 0x28
        s->r[30] = 40;
        return 6934;
    }
    case 0x1b1a: { // lds r19, 0x2162
        uint16_t address = 8546;
        s->r[19] = pm_read(s, address);
        return 6942;
    }
    case 0x1b1e: { // ldi r30, 0x36
        s->r[30] = 54;
        return 6944;
    }
    case 0x1b2e: { // ldi r30, 0x48
        s->r[30] = 72;
        return 6960;
    }
    case 0x1b34: { // ldi r30, 0x52
        s->r[30] = 82;
        return 6966;
    }
    case 0x1b3a: { // ldi r30, 0x5A
        s->r[30] = 90;
        return 6972;
    }
    case 0x1b42: { // ldi r30, 0x5E
        s->r[30] = 94;
        return 6980;
    }
    case 0x1b58: { // ldi r30, 0x12
        s->r[30] = 18;
        return 7002;
    }
    case 0x1b66: { // ldi r30, 0x20
        s->r[30] = 32;
        return 7016;
    }
    case 0x1b6c: { // lds r20, 0x2158
        uint16_t address = 8536;
        s->r[20] = pm_read(s, address);
        return 7024;
    }
    case 0x1b82: { // ldi r30, 0x2E
        s->r[30] = 46;
        return 7044;
    }
    case 0x1b94: { // ldi r30, 0x96
        s->r[30] = 150;
        return 7062;
    }
    case 0x1b9a: { // lds r20, 0x2441
        uint16_t address = 9281;
        s->r[20] = pm_read(s, address);
        return 7070;
    }
    case 0x1ba0: { // ldi r30, 0xA8
        s->r[30] = 168;
        return 7074;
    }
    case 0x1bbc: { // ldi r30, 0x44
        s->r[30] = 68;
        return 7102;
    }
    case 0x1bd4: { // ldi r30, 0x4E
        s->r[30] = 78;
        return 7126;
    }
    case 0x1bdc: { // ldi r30, 0x5C
        s->r[30] = 92;
        return 7134;
    }
    case 0x1c12: { // ldi r30, 0x08
        s->r[30] = 8;
        return 7188;
    }
    case 0x1c14: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 7190;
    }
    case 0x1c20: { // ldi r30, 0x9E
        s->r[30] = 158;
        return 7202;
    }
    case 0x1c42: { // ldi r16, 0xD0
        s->r[16] = 208;
        return 7236;
    }
    case 0x1c58: { // ldi r30, 0x98
        s->r[30] = 152;
        return 7258;
    }
    case 0x1c6c: { // ldi r30, 0xC6
        s->r[30] = 198;
        return 7278;
    }
    case 0x1c92: { // ldi r24, 0x20
        s->r[24] = 32;
        return 7316;
    }
    case 0x1cac: { // ldi r17, 0xFF
        s->r[17] = 255;
        return 7342;
    }
    case 0x1cfe: { // ldi r30, 0x68
        s->r[30] = 104;
        return 7424;
    }
    case 0x1d70: { // ldi r26, 0x10
        s->r[26] = 16;
        return 7538;
    }
    case 0x1daa: { // ldi r18, 0x00
        s->r[18] = 0;
        return 7596;
    }
    case 0x1dda: { // ldi r26, 0x0C
        s->r[26] = 12;
        return 7644;
    }
    case 0x1de8: { // ldi r26, 0x05
        s->r[26] = 5;
        return 7658;
    }
    case 0x1e14: { // ldi r18, 0x25
        s->r[18] = 37;
        return 7702;
    }
    case 0x1e4c: { // ldi r24, 0x10
        s->r[24] = 16;
        return 7758;
    }
    case 0x1e9a: { // ldi r24, 0x0F
        s->r[24] = 15;
        return 7836;
    }
    case 0x1eaa: { // ldi r18, 0xB0
        s->r[18] = 176;
        return 7852;
    }
    case 0x1efa: { // ldi r24, 0x00
        s->r[24] = 0;
        return 7932;
    }
    case 0x1f02: { // ldi r24, 0xFF
        s->r[24] = 255;
        return 7940;
    }
    case 0x1f10: { // ldi r28, 0x7B
        s->r[28] = 123;
        return 7954;
    }
    case 0x1f20: { // ldi r22, 0x20
        s->r[22] = 32;
        return 7970;
    }
    case 0x1f24: { // ldi r16, 0x0C
        s->r[16] = 12;
        return 7974;
    }
    case 0x1f58: { // ldi r24, 0x4E
        s->r[24] = 78;
        return 8026;
    }
    case 0x1f6e: { // ldi r18, 0x83
        s->r[18] = 131;
        return 8048;
    }
    case 0x1f80: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 8066;
    }
    case 0x1fa8: { // ldi r24, 0x75
        s->r[24] = 117;
        return 8106;
    }
    case 0x2026: { // ldi r18, 0x82
        s->r[18] = 130;
        return 8232;
    }
    case 0x2070: { // ldi r24, 0xFE
        s->r[24] = 254;
        return 8306;
    }
    case 0x2086: { // ldi r18, 0x81
        s->r[18] = 129;
        return 8328;
    }
    case 0x20a8: { // ldi r18, 0x0C
        s->r[18] = 12;
        return 8362;
    }
    case 0x20aa: { // ldi r19, 0x02
        s->r[19] = 2;
        return 8364;
    }
    case 0x20d2: { // ldi r18, 0x20
        s->r[18] = 32;
        return 8404;
    }
    case 0x20d4: { // ldi r19, 0x4E
        s->r[19] = 78;
        return 8406;
    }
    case 0x20dc: { // ldi r18, 0x72
        s->r[18] = 114;
        return 8414;
    }
    case 0x20ee: { // ldi r18, 0x88
        s->r[18] = 136;
        return 8432;
    }
    case 0x20f0: { // ldi r19, 0x41
        s->r[19] = 65;
        return 8434;
    }
    case 0x20f4: { // ldi r18, 0x80
        s->r[18] = 128;
        return 8438;
    }
    case 0x20f6: { // eor r17, r18
        s->r[17] ^= s->r[18];
        pm_nzv(s, s->r[17], false);
        return 8440;
    }
    case 0x2126: { // ldi r30, 0xB4
        s->r[30] = 180;
        return 8488;
    }
    case 0x218a: { // ldi r22, 0x1C
        s->r[22] = 28;
        return 8588;
    }
    case 0x218e: { // ldi r22, 0x04
        s->r[22] = 4;
        return 8592;
    }
    case 0x21a0: { // ldi r20, 0x02
        s->r[20] = 2;
        return 8610;
    }
    case 0x21ba: { // ldi r21, 0x18
        s->r[21] = 24;
        return 8636;
    }
    case 0x21e2: { // ldi r21, 0x1C
        s->r[21] = 28;
        return 8676;
    }
    case 0x2242: { // ldi r21, 0x08
        s->r[21] = 8;
        return 8772;
    }
    case 0x226a: { // ldi r21, 0x10
        s->r[21] = 16;
        return 8812;
    }
    case 0x2274: { // lds r16, 0x0628
        uint16_t address = 1576;
        s->r[16] = pm_read(s, address);
        return 8824;
    }
    case 0x22b2: { // ldi r23, 0xD5
        s->r[23] = 213;
        return 8884;
    }
    case 0x22ba: { // ldi r24, 0x01
        s->r[24] = 1;
        return 8892;
    }
    case 0x22fe: { // ldi r22, 0x07
        s->r[22] = 7;
        return 8960;
    }
    case 0x2314: { // ldi r22, 0xD1
        s->r[22] = 209;
        return 8982;
    }
    case 0x231a: { // eor r21, r21
        s->r[21] ^= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8988;
    }
    case 0x2324: { // ldi r22, 0x01
        s->r[22] = 1;
        return 8998;
    }
    case 0x23a8: { // lds r17, 0x08C3
        uint16_t address = 2243;
        s->r[17] = pm_read(s, address);
        return 9132;
    }
    case 0x23b8: { // lds r16, 0x08C3
        uint16_t address = 2243;
        s->r[16] = pm_read(s, address);
        return 9148;
    }
    case 0x23d6: { // ldi r18, 0xBC
        s->r[18] = 188;
        return 9176;
    }
    case 0x2404: { // lds r9, 0x08C3
        uint16_t address = 2243;
        s->r[9] = pm_read(s, address);
        return 9224;
    }
    case 0x2414: { // lds r8, 0x08C3
        uint16_t address = 2243;
        s->r[8] = pm_read(s, address);
        return 9240;
    }
    case 0x2424: { // lds r11, 0x08C3
        uint16_t address = 2243;
        s->r[11] = pm_read(s, address);
        return 9256;
    }
    case 0x2434: { // lds r10, 0x08C3
        uint16_t address = 2243;
        s->r[10] = pm_read(s, address);
        return 9272;
    }
    case 0x2444: { // lds r13, 0x08C3
        uint16_t address = 2243;
        s->r[13] = pm_read(s, address);
        return 9288;
    }
    case 0x2454: { // lds r12, 0x08C3
        uint16_t address = 2243;
        s->r[12] = pm_read(s, address);
        return 9304;
    }
    case 0x2464: { // lds r15, 0x08C3
        uint16_t address = 2243;
        s->r[15] = pm_read(s, address);
        return 9320;
    }
    case 0x2474: { // lds r14, 0x08C3
        uint16_t address = 2243;
        s->r[14] = pm_read(s, address);
        return 9336;
    }
    case 0x2488: { // ldi r22, 0xF1
        s->r[22] = 241;
        return 9354;
    }
    case 0x248e: { // ldi r22, 0x10
        s->r[22] = 16;
        return 9360;
    }
    case 0x2498: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9372;
    }
    case 0x24d2: { // ldi r16, 0x8E
        s->r[16] = 142;
        return 9428;
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
    case 0x2510: { // lds r18, 0x08C3
        uint16_t address = 2243;
        s->r[18] = pm_read(s, address);
        return 9492;
    }
    case 0x2520: { // lds r19, 0x08C3
        uint16_t address = 2243;
        s->r[19] = pm_read(s, address);
        return 9508;
    }
    case 0x253a: { // ldi r30, 0x92
        s->r[30] = 146;
        return 9532;
    }
    case 0x253c: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 9534;
    }
    case 0x2544: { // ldi r18, 0x3D
        s->r[18] = 61;
        return 9542;
    }
    case 0x2546: { // ldi r21, 0x40
        s->r[21] = 64;
        return 9544;
    }
    case 0x25a0: { // ldi r18, 0x10
        s->r[18] = 16;
        return 9634;
    }
    case 0x25c4: { // ldi r20, 0x10
        s->r[20] = 16;
        return 9670;
    }
    case 0x2608: { // ldi r18, 0x08
        s->r[18] = 8;
        return 9738;
    }
    case 0x260a: { // ldi r17, 0x08
        s->r[17] = 8;
        return 9740;
    }
    case 0x2618: { // ldi r17, 0x02
        s->r[17] = 2;
        return 9754;
    }
    case 0x2626: { // lds r17, 0x0608
        uint16_t address = 1544;
        s->r[17] = pm_read(s, address);
        return 9770;
    }
    case 0x2642: { // ldi r17, 0x0A
        s->r[17] = 10;
        return 9796;
    }
    case 0x266c: { // eor r22, r22
        s->r[22] ^= s->r[22];
        pm_nzv(s, s->r[22], false);
        return 9838;
    }
    case 0x2740: { // ldi r20, 0x03
        s->r[20] = 3;
        return 10050;
    }
    case 0x2766: { // eor r15, r15
        s->r[15] ^= s->r[15];
        pm_nzv(s, s->r[15], false);
        return 10088;
    }
    case 0x277a: { // ldi r30, 0x56
        s->r[30] = 86;
        return 10108;
    }
    case 0x2784: { // eor r14, r14
        s->r[14] ^= s->r[14];
        pm_nzv(s, s->r[14], false);
        return 10118;
    }
    case 0x27b4: { // ldi r18, 0x2E
        s->r[18] = 46;
        return 10166;
    }
    case 0x27c4: { // ldi r18, 0x30
        s->r[18] = 48;
        return 10182;
    }
    case 0x27ec: { // ldi r16, 0x2D
        s->r[16] = 45;
        return 10222;
    }
    case 0x281e: { // ldi r30, 0x84
        s->r[30] = 132;
        return 10272;
    }
    case 0x2820: { // ldi r31, 0x29
        s->r[31] = 41;
        return 10274;
    }
    case 0x285e: { // lds r20, 0x06A4
        uint16_t address = 1700;
        s->r[20] = pm_read(s, address);
        return 10338;
    }
    case 0x286e: { // ldi r20, 0x01
        s->r[20] = 1;
        return 10352;
    }
    case 0x2888: { // lds r17, 0x2005
        uint16_t address = 8197;
        s->r[17] = pm_read(s, address);
        return 10380;
    }
    case 0x28de: { // ldi r30, 0x03
        s->r[30] = 3;
        return 10464;
    }
    case 0x28f0: { // lds r20, 0x0BA1
        uint16_t address = 2977;
        s->r[20] = pm_read(s, address);
        return 10484;
    }
    case 0x28fc: { // lds r20, 0x0BA3
        uint16_t address = 2979;
        s->r[20] = pm_read(s, address);
        return 10496;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
