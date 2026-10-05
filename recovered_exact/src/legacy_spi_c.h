#ifndef PM_LEGACY_SPI_C_H
#define PM_LEGACY_SPI_C_H
#include "legacy_spi.h"

/* Opt-in exact C SPI operations. Legacy register operands are captured without
 * emitting instructions; ordinary C performs MMIO stores, reads and polling.
 * Adopt per translation unit only after a complete zero-difference exact-check.
 * Source-specific archived C comments document any recorded functional tests;
 * those results are not a substitute for exact binary acceptance. */
#define PM_SPI_WAIT_AT(status_register, scratch) do { \
    register uint8_t status asm(scratch); \
    do { \
        status = (status_register); \
        asm volatile("" : "+r" (status)); \
    } while (!(status & 0x80u)); \
} while (0)
#undef PM_SPI_SEND_AT
#define PM_SPI_SEND_AT(data_register, status_register, scratch, value) do { \
    register uint8_t status asm(scratch); \
    (data_register) = (value); \
    do { \
        status = (status_register); \
        asm volatile("" : "+r" (status)); \
    } while (!(status & 0x80u)); \
    asm volatile("" : : "r" (status)); \
} while (0)
#undef PM_WRITE_R22
#define PM_WRITE_R22(peripheral, value) do { \
    register uint8_t mask asm("r22") = (value); \
    asm volatile("" : "+r" (mask)); \
    (peripheral) = mask; \
} while (0)
#undef PM_SPI_READ_REGISTER
#define PM_SPI_READ_REGISTER(peripheral, reg) do { \
    register uint8_t byte asm(reg) = peripheral##_DATA; \
    asm volatile("" : : "r" (byte)); \
} while (0)
#endif

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 6992 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_legacy_spi_c(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0202: { // lds r17, 0x0124
        uint16_t address = 292;
        s->r[17] = pm_read(s, address);
        return 518;
    }
    case 0x023e: { // rjmp .+536
        return 1112;
    }
    case 0x025e: { // rjmp .+2176
        return 2784;
    }
    case 0x029a: { // sts 0x0118, r28
        uint16_t address = 280;
        pm_write(s, address, s->r[28]);
        return 670;
    }
    case 0x029e: { // sts 0x0119, r29
        uint16_t address = 281;
        pm_write(s, address, s->r[29]);
        return 674;
    }
    case 0x02a4: { // sts 0x011A, r20
        uint16_t address = 282;
        pm_write(s, address, s->r[20]);
        return 680;
    }
    case 0x02b2: { // sts 0x0114, r16
        uint16_t address = 276;
        pm_write(s, address, s->r[16]);
        return 694;
    }
    case 0x02b8: { // sts 0x0115, r16
        uint16_t address = 277;
        pm_write(s, address, s->r[16]);
        return 700;
    }
    case 0x02be: { // sts 0x0110, r16
        uint16_t address = 272;
        pm_write(s, address, s->r[16]);
        return 706;
    }
    case 0x02cc: { // sts 0x0111, r16
        uint16_t address = 273;
        pm_write(s, address, s->r[16]);
        return 720;
    }
    case 0x02d4: { // sts 0x012C, r26
        uint16_t address = 300;
        pm_write(s, address, s->r[26]);
        return 728;
    }
    case 0x02d8: { // sts 0x012D, r27
        uint16_t address = 301;
        pm_write(s, address, s->r[27]);
        return 732;
    }
    case 0x02de: { // sts 0x012E, r20
        uint16_t address = 302;
        pm_write(s, address, s->r[20]);
        return 738;
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
    case 0x0304: { // lds r16, 0x0121
        uint16_t address = 289;
        s->r[16] = pm_read(s, address);
        return 776;
    }
    case 0x0328: { // sts 0x0118, r26
        uint16_t address = 280;
        pm_write(s, address, s->r[26]);
        return 812;
    }
    case 0x032c: { // sts 0x0119, r27
        uint16_t address = 281;
        pm_write(s, address, s->r[27]);
        return 816;
    }
    case 0x033a: { // sts 0x011A, r16
        uint16_t address = 282;
        pm_write(s, address, s->r[16]);
        return 830;
    }
    case 0x034a: { // sts 0x0116, r16
        uint16_t address = 278;
        pm_write(s, address, s->r[16]);
        return 846;
    }
    case 0x0398: { // lds r18, 0x2435
        uint16_t address = 9269;
        s->r[18] = pm_read(s, address);
        return 924;
    }
    case 0x03a2: { // sts 0x2435, r18
        uint16_t address = 9269;
        pm_write(s, address, s->r[18]);
        return 934;
    }
    case 0x03b8: { // sts 0x0125, r17
        uint16_t address = 293;
        pm_write(s, address, s->r[17]);
        return 956;
    }
    case 0x0416: { // rjmp .-170
        return 878;
    }
    case 0x041c: { // rjmp .-180
        return 874;
    }
    case 0x0474: { // lds r18, 0x0668
        uint16_t address = 1640;
        s->r[18] = pm_read(s, address);
        return 1144;
    }
    case 0x0478: { // sbrs r18, 4
        return (!!(s->r[18] & (1u << 4)) == 1) ? 1148 : 1146;
    }
    case 0x047a: { // rjmp .-12
        return 1136;
    }
    case 0x047c: { // sts 0x243C, r16
        uint16_t address = 9276;
        pm_write(s, address, s->r[16]);
        return 1152;
    }
    case 0x0480: { // sts 0x0111, r17
        uint16_t address = 273;
        pm_write(s, address, s->r[17]);
        return 1156;
    }
    case 0x0486: { // lds r16, 0x0111
        uint16_t address = 273;
        s->r[16] = pm_read(s, address);
        return 1162;
    }
    case 0x048a: { // sbrs r16, 4
        return (!!(s->r[16] & (1u << 4)) == 1) ? 1166 : 1164;
    }
    case 0x04d2: { // rjmp .+214
        return 1450;
    }
    case 0x04f6: { // rjmp .+316
        return 1588;
    }
    case 0x04fc: { // sts 0x068C, r16
        uint16_t address = 1676;
        pm_write(s, address, s->r[16]);
        return 1280;
    }
    case 0x0516: { // rjmp .+104
        return 1408;
    }
    case 0x051c: { // sbrs r16, 3
        return (!!(s->r[16] & (1u << 3)) == 1) ? 1312 : 1310;
    }
    case 0x051e: { // rjmp .+278
        return 1590;
    }
    case 0x0522: { // sts 0x0625, r16
        uint16_t address = 1573;
        pm_write(s, address, s->r[16]);
        return 1318;
    }
    case 0x052e: { // rjmp .+262
        return 1590;
    }
    case 0x0554: { // rjmp .+224
        return 1590;
    }
    case 0x05a8: { // rjmp .+140
        return 1590;
    }
    case 0x05b0: { // rjmp .+130
        return 1588;
    }
    case 0x05c6: { // sts 0x0645, r16
        uint16_t address = 1605;
        pm_write(s, address, s->r[16]);
        return 1482;
    }
    case 0x05d0: { // rjmp .+28
        return 1518;
    }
    case 0x05ec: { // rjmp .+72
        return 1590;
    }
    case 0x05f0: { // sts 0x062C, r16
        uint16_t address = 1580;
        pm_write(s, address, s->r[16]);
        return 1524;
    }
    case 0x0670: { // rjmp .+60
        return 1710;
    }
    case 0x0678: { // rjmp .+52
        return 1710;
    }
    case 0x0688: { // rjmp .+20
        return 1694;
    }
    case 0x06b2: { // rjmp .+124
        return 1840;
    }
    case 0x06d6: { // rjmp .-42
        return 1710;
    }
    case 0x0700: { // rjmp .-112
        return 1682;
    }
    case 0x0710: { // sts 0x066C, r16
        uint16_t address = 1644;
        pm_write(s, address, s->r[16]);
        return 1812;
    }
    case 0x0744: { // sts 0x0607, r16
        uint16_t address = 1543;
        pm_write(s, address, s->r[16]);
        return 1864;
    }
    case 0x075c: { // rjmp .+352
        return 2238;
    }
    case 0x07ec: { // rjmp .+208
        return 2238;
    }
    case 0x0800: { // rjmp .+188
        return 2238;
    }
    case 0x081c: { // rjmp .+152
        return 2230;
    }
    case 0x08bc: { // rjmp .-168
        return 2070;
    }
    case 0x08fc: { // lds r16, 0x222F
        uint16_t address = 8751;
        s->r[16] = pm_read(s, address);
        return 2304;
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
    case 0x09c2: { // lds r16, 0x2441
        uint16_t address = 9281;
        s->r[16] = pm_read(s, address);
        return 2502;
    }
    case 0x0a52: { // lds r16, 0x0668
        uint16_t address = 1640;
        s->r[16] = pm_read(s, address);
        return 2646;
    }
    case 0x0a70: { // rjmp .+34
        return 2708;
    }
    case 0x0aa6: { // lds r16, 0x2159
        uint16_t address = 8537;
        s->r[16] = pm_read(s, address);
        return 2730;
    }
    case 0x0af6: { // rjmp .+18
        return 2826;
    }
    case 0x0b14: { // lds r16, 0x2162
        uint16_t address = 8546;
        s->r[16] = pm_read(s, address);
        return 2840;
    }
    case 0x0b1a: { // sts 0x2162, r19
        uint16_t address = 8546;
        pm_write(s, address, s->r[19]);
        return 2846;
    }
    case 0x0b24: { // sbrs r19, 1
        return (!!(s->r[19] & (1u << 1)) == 1) ? 2856 : 2854;
    }
    case 0x0b26: { // rjmp .+8
        return 2864;
    }
    case 0x0b30: { // sbrs r19, 2
        return (!!(s->r[19] & (1u << 2)) == 1) ? 2868 : 2866;
    }
    case 0x0b32: { // rjmp .+2
        return 2870;
    }
    case 0x0b36: { // sbrs r19, 3
        return (!!(s->r[19] & (1u << 3)) == 1) ? 2874 : 2872;
    }
    case 0x0b46: { // sts 0x215A, r16
        uint16_t address = 8538;
        pm_write(s, address, s->r[16]);
        return 2890;
    }
    case 0x0b70: { // sts 0x0646, r16
        uint16_t address = 1606;
        pm_write(s, address, s->r[16]);
        return 2932;
    }
    case 0x0b76: { // sts 0x0626, r16
        uint16_t address = 1574;
        pm_write(s, address, s->r[16]);
        return 2938;
    }
    case 0x0b82: { // sts 0x2158, r16
        uint16_t address = 8536;
        pm_write(s, address, s->r[16]);
        return 2950;
    }
    case 0x0ba4: { // lds r16, 0x0688
        uint16_t address = 1672;
        s->r[16] = pm_read(s, address);
        return 2984;
    }
    case 0x0bc2: { // sbrs r18, 0
        return (!!(s->r[18] & (1u << 0)) == 1) ? 3014 : 3012;
    }
    case 0x0bc4: { // rjmp .+38
        return 3052;
    }
    case 0x0bc8: { // rjmp .+12
        return 3030;
    }
    case 0x0bd4: { // rjmp .+70
        return 3100;
    }
    case 0x0be0: { // sts 0x215C, r17
        uint16_t address = 8540;
        pm_write(s, address, s->r[17]);
        return 3044;
    }
    case 0x0be4: { // sts 0x215D, r18
        uint16_t address = 8541;
        pm_write(s, address, s->r[18]);
        return 3048;
    }
    case 0x0bea: { // rjmp .+48
        return 3100;
    }
    case 0x0bec: { // sbrs r18, 3
        return (!!(s->r[18] & (1u << 3)) == 1) ? 3056 : 3054;
    }
    case 0x0bee: { // rjmp .+46
        return 3102;
    }
    case 0x0bf2: { // rjmp .+42
        return 3102;
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
    case 0x0c06: { // sts 0x2442, r16
        uint16_t address = 9282;
        pm_write(s, address, s->r[16]);
        return 3082;
    }
    case 0x0c14: { // rjmp .-60
        return 3034;
    }
    case 0x0c3a: { // sbrs r17, 1
        return (!!(s->r[17] & (1u << 1)) == 1) ? 3134 : 3132;
    }
    case 0x0c3e: { // lds r18, 0x2006
        uint16_t address = 8198;
        s->r[18] = pm_read(s, address);
        return 3138;
    }
    case 0x0c44: { // sts 0x2006, r18
        uint16_t address = 8198;
        pm_write(s, address, s->r[18]);
        return 3144;
    }
    case 0x0c52: { // lds r16, 0x2158
        uint16_t address = 8536;
        s->r[16] = pm_read(s, address);
        return 3158;
    }
    case 0x0c80: { // rjmp .+10
        return 3212;
    }
    case 0x0c8e: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 3218;
    }
    case 0x0c98: { // sts 0x215C, r16
        uint16_t address = 8540;
        pm_write(s, address, s->r[16]);
        return 3228;
    }
    case 0x0c9c: { // sts 0x215D, r16
        uint16_t address = 8541;
        pm_write(s, address, s->r[16]);
        return 3232;
    }
    case 0x0ca0: { // sts 0x215B, r16
        uint16_t address = 8539;
        pm_write(s, address, s->r[16]);
        return 3236;
    }
    case 0x0ca4: { // sts 0x2159, r16
        uint16_t address = 8537;
        pm_write(s, address, s->r[16]);
        return 3240;
    }
    case 0x0ca8: { // sts 0x2441, r16
        uint16_t address = 9281;
        pm_write(s, address, s->r[16]);
        return 3244;
    }
    case 0x0cac: { // sts 0x2006, r16
        uint16_t address = 8198;
        pm_write(s, address, s->r[16]);
        return 3248;
    }
    case 0x0cb0: { // sts 0x2162, r16
        uint16_t address = 8546;
        pm_write(s, address, s->r[16]);
        return 3252;
    }
    case 0x0cb6: { // sts 0x0689, r16
        uint16_t address = 1673;
        pm_write(s, address, s->r[16]);
        return 3258;
    }
    case 0x0cbc: { // sts 0x06A9, r16
        uint16_t address = 1705;
        pm_write(s, address, s->r[16]);
        return 3264;
    }
    case 0x0cc2: { // sts 0x0629, r16
        uint16_t address = 1577;
        pm_write(s, address, s->r[16]);
        return 3270;
    }
    case 0x0cc6: { // sts 0x0669, r16
        uint16_t address = 1641;
        pm_write(s, address, s->r[16]);
        return 3274;
    }
    case 0x0ccc: { // sts 0x0622, r16
        uint16_t address = 1570;
        pm_write(s, address, s->r[16]);
        return 3280;
    }
    case 0x0cd2: { // sts 0x08C0, r16
        uint16_t address = 2240;
        pm_write(s, address, s->r[16]);
        return 3286;
    }
    case 0x0cd8: { // sts 0x0642, r16
        uint16_t address = 1602;
        pm_write(s, address, s->r[16]);
        return 3292;
    }
    case 0x0cde: { // sts 0x0662, r16
        uint16_t address = 1634;
        pm_write(s, address, s->r[16]);
        return 3298;
    }
    case 0x0cea: { // sts 0x06A6, r16
        uint16_t address = 1702;
        pm_write(s, address, s->r[16]);
        return 3310;
    }
    case 0x0cf0: { // sts 0x06A2, r16
        uint16_t address = 1698;
        pm_write(s, address, s->r[16]);
        return 3316;
    }
    case 0x0cf4: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 3320;
    }
    case 0x0cfa: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 3326;
    }
    case 0x0d00: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 3332;
    }
    case 0x0d06: { // sts 0x09C0, r16
        uint16_t address = 2496;
        pm_write(s, address, s->r[16]);
        return 3338;
    }
    case 0x0d0a: { // sts 0x0100, r16
        uint16_t address = 256;
        pm_write(s, address, s->r[16]);
        return 3342;
    }
    case 0x0d12: { // sts 0x0624, r16
        uint16_t address = 1572;
        pm_write(s, address, s->r[16]);
        return 3350;
    }
    case 0x0d18: { // sts 0x0621, r16
        uint16_t address = 1569;
        pm_write(s, address, s->r[16]);
        return 3356;
    }
    case 0x0d1e: { // sts 0x0644, r16
        uint16_t address = 1604;
        pm_write(s, address, s->r[16]);
        return 3362;
    }
    case 0x0d24: { // sts 0x0641, r16
        uint16_t address = 1601;
        pm_write(s, address, s->r[16]);
        return 3368;
    }
    case 0x0d30: { // sts 0x0661, r16
        uint16_t address = 1633;
        pm_write(s, address, s->r[16]);
        return 3380;
    }
    case 0x0d36: { // sts 0x06A5, r16
        uint16_t address = 1701;
        pm_write(s, address, s->r[16]);
        return 3386;
    }
    case 0x0d3c: { // sts 0x06A1, r16
        uint16_t address = 1697;
        pm_write(s, address, s->r[16]);
        return 3392;
    }
    case 0x0d4c: { // lds r16, 0x0100
        uint16_t address = 256;
        s->r[16] = pm_read(s, address);
        return 3408;
    }
    case 0x0d62: { // sts 0x0113, r16
        uint16_t address = 275;
        pm_write(s, address, s->r[16]);
        return 3430;
    }
    case 0x0d68: { // sts 0x011C, r16
        uint16_t address = 284;
        pm_write(s, address, s->r[16]);
        return 3436;
    }
    case 0x0d6e: { // sts 0x011D, r16
        uint16_t address = 285;
        pm_write(s, address, s->r[16]);
        return 3442;
    }
    case 0x0d74: { // sts 0x011E, r16
        uint16_t address = 286;
        pm_write(s, address, s->r[16]);
        return 3448;
    }
    case 0x0d7a: { // sts 0x0112, r16
        uint16_t address = 274;
        pm_write(s, address, s->r[16]);
        return 3454;
    }
    case 0x0d80: { // sts 0x0123, r16
        uint16_t address = 291;
        pm_write(s, address, s->r[16]);
        return 3460;
    }
    case 0x0d86: { // sts 0x012C, r16
        uint16_t address = 300;
        pm_write(s, address, s->r[16]);
        return 3466;
    }
    case 0x0d8c: { // sts 0x012D, r16
        uint16_t address = 301;
        pm_write(s, address, s->r[16]);
        return 3472;
    }
    case 0x0d92: { // sts 0x012E, r16
        uint16_t address = 302;
        pm_write(s, address, s->r[16]);
        return 3478;
    }
    case 0x0d98: { // sts 0x0128, r16
        uint16_t address = 296;
        pm_write(s, address, s->r[16]);
        return 3484;
    }
    case 0x0d9e: { // sts 0x0129, r16
        uint16_t address = 297;
        pm_write(s, address, s->r[16]);
        return 3490;
    }
    case 0x0da4: { // sts 0x012A, r16
        uint16_t address = 298;
        pm_write(s, address, s->r[16]);
        return 3496;
    }
    case 0x0da8: { // sts 0x0126, r16
        uint16_t address = 294;
        pm_write(s, address, s->r[16]);
        return 3500;
    }
    case 0x0dae: { // sts 0x0124, r16
        uint16_t address = 292;
        pm_write(s, address, s->r[16]);
        return 3506;
    }
    case 0x0db4: { // sts 0x0125, r16
        uint16_t address = 293;
        pm_write(s, address, s->r[16]);
        return 3512;
    }
    case 0x0dba: { // sts 0x0122, r16
        uint16_t address = 290;
        pm_write(s, address, s->r[16]);
        return 3518;
    }
    case 0x0dc0: { // sts 0x0120, r16
        uint16_t address = 288;
        pm_write(s, address, s->r[16]);
        return 3524;
    }
    case 0x0dc6: { // sts 0x0121, r16
        uint16_t address = 289;
        pm_write(s, address, s->r[16]);
        return 3530;
    }
    case 0x0e36: { // lds r16, 0x06A8
        uint16_t address = 1704;
        s->r[16] = pm_read(s, address);
        return 3642;
    }
    case 0x0e3c: { // rjmp .+54
        return 3700;
    }
    case 0x0e48: { // lds r18, 0x0BA1
        uint16_t address = 2977;
        s->r[18] = pm_read(s, address);
        return 3660;
    }
    case 0x0e4c: { // sbrs r18, 5
        return (!!(s->r[18] & (1u << 5)) == 1) ? 3664 : 3662;
    }
    case 0x0e68: { // lds r18, 0x0BA3
        uint16_t address = 2979;
        s->r[18] = pm_read(s, address);
        return 3692;
    }
    case 0x0e6e: { // sts 0x0BA3, r18
        uint16_t address = 2979;
        pm_write(s, address, s->r[18]);
        return 3698;
    }
    case 0x0eaa: { // lds r19, 0x0BA3
        uint16_t address = 2979;
        s->r[19] = pm_read(s, address);
        return 3758;
    }
    case 0x0eb0: { // sts 0x0BA3, r19
        uint16_t address = 2979;
        pm_write(s, address, s->r[19]);
        return 3764;
    }
    case 0x0eb4: { // rjmp .+24
        return 3790;
    }
    case 0x0ec4: { // sts 0x0BA0, r17
        uint16_t address = 2976;
        pm_write(s, address, s->r[17]);
        return 3784;
    }
    case 0x0f08: { // sts 0x06A5, r18
        uint16_t address = 1701;
        pm_write(s, address, s->r[18]);
        return 3852;
    }
    case 0x0f0c: { // lds r18, 0x2005
        uint16_t address = 8197;
        s->r[18] = pm_read(s, address);
        return 3856;
    }
    case 0x0f12: { // sts 0x2005, r18
        uint16_t address = 8197;
        pm_write(s, address, s->r[18]);
        return 3862;
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
    case 0x0f20: { // rjmp .+32
        return 3906;
    }
    case 0x0f38: { // lds r16, 0x2005
        uint16_t address = 8197;
        s->r[16] = pm_read(s, address);
        return 3900;
    }
    case 0x0f3e: { // sts 0x2005, r16
        uint16_t address = 8197;
        pm_write(s, address, s->r[16]);
        return 3906;
    }
    case 0x0f56: { // sts 0x0052, r16
        uint16_t address = 82;
        pm_write(s, address, s->r[16]);
        return 3930;
    }
    case 0x0f5c: { // sts 0x0050, r16
        uint16_t address = 80;
        pm_write(s, address, s->r[16]);
        return 3936;
    }
    case 0x0f60: { // lds r17, 0x0051
        uint16_t address = 81;
        s->r[17] = pm_read(s, address);
        return 3940;
    }
    case 0x0f64: { // sbrs r17, 3
        return (!!(s->r[17] & (1u << 3)) == 1) ? 3944 : 3942;
    }
    case 0x0f6a: { // sts 0x0055, r16
        uint16_t address = 85;
        pm_write(s, address, s->r[16]);
        return 3950;
    }
    case 0x0f78: { // sbrs r17, 4
        return (!!(s->r[17] & (1u << 4)) == 1) ? 3964 : 3962;
    }
    case 0x0f84: { // sts 0x0040, r16
        uint16_t address = 64;
        pm_write(s, address, s->r[16]);
        return 3976;
    }
    case 0x0f98: { // sts 0x0601, r16
        uint16_t address = 1537;
        pm_write(s, address, s->r[16]);
        return 3996;
    }
    case 0x0fa4: { // sts 0x0612, r16
        uint16_t address = 1554;
        pm_write(s, address, s->r[16]);
        return 4008;
    }
    case 0x0faa: { // sts 0x0611, r16
        uint16_t address = 1553;
        pm_write(s, address, s->r[16]);
        return 4014;
    }
    case 0x0fae: { // sts 0x0613, r16
        uint16_t address = 1555;
        pm_write(s, address, s->r[16]);
        return 4018;
    }
    case 0x0fb2: { // sts 0x0614, r16
        uint16_t address = 1556;
        pm_write(s, address, s->r[16]);
        return 4022;
    }
    case 0x0fb8: { // sts 0x0636, r16
        uint16_t address = 1590;
        pm_write(s, address, s->r[16]);
        return 4028;
    }
    case 0x0fbe: { // sts 0x0653, r16
        uint16_t address = 1619;
        pm_write(s, address, s->r[16]);
        return 4034;
    }
    case 0x0fc4: { // sts 0x0654, r16
        uint16_t address = 1620;
        pm_write(s, address, s->r[16]);
        return 4040;
    }
    case 0x0fca: { // sts 0x0671, r16
        uint16_t address = 1649;
        pm_write(s, address, s->r[16]);
        return 4046;
    }
    case 0x0fce: { // sts 0x0672, r16
        uint16_t address = 1650;
        pm_write(s, address, s->r[16]);
        return 4050;
    }
    case 0x0fe6: { // sts 0x0673, r16
        uint16_t address = 1651;
        pm_write(s, address, s->r[16]);
        return 4074;
    }
    case 0x0ff2: { // sts 0x0690, r16
        uint16_t address = 1680;
        pm_write(s, address, s->r[16]);
        return 4086;
    }
    case 0x0ff8: { // sts 0x0691, r16
        uint16_t address = 1681;
        pm_write(s, address, s->r[16]);
        return 4092;
    }
    case 0x0ffe: { // sts 0x0692, r16
        uint16_t address = 1682;
        pm_write(s, address, s->r[16]);
        return 4098;
    }
    case 0x1004: { // sts 0x0693, r16
        uint16_t address = 1683;
        pm_write(s, address, s->r[16]);
        return 4104;
    }
    case 0x100a: { // sts 0x068A, r16
        uint16_t address = 1674;
        pm_write(s, address, s->r[16]);
        return 4110;
    }
    case 0x1010: { // sts 0x068B, r16
        uint16_t address = 1675;
        pm_write(s, address, s->r[16]);
        return 4116;
    }
    case 0x101c: { // sts 0x06B1, r16
        uint16_t address = 1713;
        pm_write(s, address, s->r[16]);
        return 4128;
    }
    case 0x1022: { // sts 0x06B6, r16
        uint16_t address = 1718;
        pm_write(s, address, s->r[16]);
        return 4134;
    }
    case 0x1028: { // sts 0x06B7, r16
        uint16_t address = 1719;
        pm_write(s, address, s->r[16]);
        return 4140;
    }
    case 0x102e: { // sts 0x062A, r16
        uint16_t address = 1578;
        pm_write(s, address, s->r[16]);
        return 4146;
    }
    case 0x1034: { // sts 0x066A, r16
        uint16_t address = 1642;
        pm_write(s, address, s->r[16]);
        return 4152;
    }
    case 0x103a: { // sts 0x06AA, r16
        uint16_t address = 1706;
        pm_write(s, address, s->r[16]);
        return 4158;
    }
    case 0x1040: { // sts 0x06AB, r16
        uint16_t address = 1707;
        pm_write(s, address, s->r[16]);
        return 4164;
    }
    case 0x104c: { // sts 0x0BA7, r16
        uint16_t address = 2983;
        pm_write(s, address, s->r[16]);
        return 4176;
    }
    case 0x1052: { // sts 0x0BA6, r16
        uint16_t address = 2982;
        pm_write(s, address, s->r[16]);
        return 4182;
    }
    case 0x1058: { // sts 0x0BA5, r16
        uint16_t address = 2981;
        pm_write(s, address, s->r[16]);
        return 4188;
    }
    case 0x105e: { // sts 0x0BA4, r16
        uint16_t address = 2980;
        pm_write(s, address, s->r[16]);
        return 4194;
    }
    case 0x1064: { // sts 0x0BA3, r16
        uint16_t address = 2979;
        pm_write(s, address, s->r[16]);
        return 4200;
    }
    case 0x106a: { // sts 0x0800, r16
        uint16_t address = 2048;
        pm_write(s, address, s->r[16]);
        return 4206;
    }
    case 0x1070: { // sts 0x0826, r16
        uint16_t address = 2086;
        pm_write(s, address, s->r[16]);
        return 4212;
    }
    case 0x1076: { // sts 0x0827, r16
        uint16_t address = 2087;
        pm_write(s, address, s->r[16]);
        return 4218;
    }
    case 0x107c: { // sts 0x0801, r16
        uint16_t address = 2049;
        pm_write(s, address, s->r[16]);
        return 4224;
    }
    case 0x1082: { // sts 0x0804, r16
        uint16_t address = 2052;
        pm_write(s, address, s->r[16]);
        return 4230;
    }
    case 0x1088: { // sts 0x0806, r16
        uint16_t address = 2054;
        pm_write(s, address, s->r[16]);
        return 4236;
    }
    case 0x108e: { // sts 0x01CC, r16
        uint16_t address = 460;
        pm_write(s, address, s->r[16]);
        return 4242;
    }
    case 0x10ae: { // sbrs r16, 1
        return (!!(s->r[16] & (1u << 1)) == 1) ? 4274 : 4272;
    }
    case 0x10b2: { // sts 0x2004, r25
        uint16_t address = 8196;
        pm_write(s, address, s->r[25]);
        return 4278;
    }
    case 0x10e4: { // sts 0x215E, r16
        uint16_t address = 8542;
        pm_write(s, address, s->r[16]);
        return 4328;
    }
    case 0x10ea: { // sts 0x215F, r16
        uint16_t address = 8543;
        pm_write(s, address, s->r[16]);
        return 4334;
    }
    case 0x10fc: { // sts 0x00A2, r16
        uint16_t address = 162;
        pm_write(s, address, s->r[16]);
        return 4352;
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
    case 0x1190: { // rjmp .+324
        return 4822;
    }
    case 0x1194: { // lds r20, 0x0689
        uint16_t address = 1673;
        s->r[20] = pm_read(s, address);
        return 4504;
    }
    case 0x1198: { // sbrs r20, 3
        return (!!(s->r[20] & (1u << 3)) == 1) ? 4508 : 4506;
    }
    case 0x119a: { // rjmp .-48
        return 4460;
    }
    case 0x11a6: { // rjmp .+240
        return 4760;
    }
    case 0x11e8: { // rjmp .+236
        return 4822;
    }
    case 0x1210: { // rjmp .-166
        return 4460;
    }
    case 0x1228: { // rjmp .+172
        return 4822;
    }
    case 0x1250: { // rjmp .-230
        return 4460;
    }
    case 0x1268: { // rjmp .+108
        return 4822;
    }
    case 0x127e: { // rjmp .-276
        return 4460;
    }
    case 0x1296: { // rjmp .+62
        return 4822;
    }
    case 0x12ac: { // rjmp .-322
        return 4460;
    }
    case 0x12d4: { // rjmp .+0
        return 4822;
    }
    case 0x12e0: { // rjmp .-358
        return 4476;
    }
    case 0x12e6: { // rjmp .-364
        return 4476;
    }
    case 0x1300: { // rjmp .+2274
        return 7140;
    }
    case 0x1306: { // rjmp .+368
        return 5240;
    }
    case 0x1314: { // rjmp .+192
        return 5078;
    }
    case 0x131a: { // rjmp .+342
        return 5234;
    }
    case 0x1328: { // rjmp .+474
        return 5380;
    }
    case 0x132e: { // rjmp .+1806
        return 6718;
    }
    case 0x1334: { // rjmp .+624
        return 5542;
    }
    case 0x1342: { // rjmp .+1354
        return 6286;
    }
    case 0x1348: { // rjmp .+2328
        return 7266;
    }
    case 0x134e: { // rjmp .+1812
        return 6756;
    }
    case 0x1354: { // rjmp .+2460
        return 7410;
    }
    case 0x135a: { // rjmp .+1460
        return 6416;
    }
    case 0x1360: { // rjmp .+1372
        return 6334;
    }
    case 0x1362: { // rjmp .+100
        return 5064;
    }
    case 0x1378: { // rjmp .+2818
        return 7804;
    }
    case 0x137e: { // rjmp .+2912
        return 7904;
    }
    case 0x1384: { // rjmp .+2728
        return 7726;
    }
    case 0x138a: { // rjmp .+2602
        return 7606;
    }
    case 0x1390: { // rjmp .+2984
        return 7994;
    }
    case 0x1396: { // rjmp .+3062
        return 8078;
    }
    case 0x139c: { // rjmp .+3150
        return 8172;
    }
    case 0x13a2: { // rjmp .+2494
        return 7522;
    }
    case 0x13a8: { // rjmp .+2536
        return 7570;
    }
    case 0x13ae: { // rjmp .+296
        return 5336;
    }
    case 0x13b4: { // rjmp .+3224
        return 8270;
    }
    case 0x13c2: { // rjmp .+1514
        return 6574;
    }
    case 0x13e0: { // lds r16, 0x2234
        uint16_t address = 8756;
        s->r[16] = pm_read(s, address);
        return 5092;
    }
    case 0x13e8: { // rjmp .+3064
        return 8162;
    }
    case 0x13ea: { // sts 0x2234, r20
        uint16_t address = 8756;
        pm_write(s, address, s->r[20]);
        return 5102;
    }
    case 0x1470: { // rjmp .+2928
        return 8162;
    }
    case 0x1474: { // rjmp .-158
        return 5080;
    }
    case 0x1476: { // rjmp .-176
        return 5064;
    }
    case 0x1490: { // sts 0x222F, r16
        uint16_t address = 8751;
        pm_write(s, address, s->r[16]);
        return 5268;
    }
    case 0x14cc: { // sts 0x2230, r16
        uint16_t address = 8752;
        pm_write(s, address, s->r[16]);
        return 5328;
    }
    case 0x14d0: { // sts 0x2231, r17
        uint16_t address = 8753;
        pm_write(s, address, s->r[17]);
        return 5332;
    }
    case 0x14d4: { // rjmp .+2828
        return 8162;
    }
    case 0x14d6: { // rjmp .-272
        return 5064;
    }
    case 0x14f0: { // sts 0x2232, r20
        uint16_t address = 8754;
        pm_write(s, address, s->r[20]);
        return 5364;
    }
    case 0x14f4: { // sts 0x2233, r21
        uint16_t address = 8755;
        pm_write(s, address, s->r[21]);
        return 5368;
    }
    case 0x1502: { // rjmp .+2782
        return 8162;
    }
    case 0x153e: { // sts 0x003B, r22
        uint16_t address = 59;
        pm_write(s, address, s->r[22]);
        return 5442;
    }
    case 0x15a4: { // rjmp .-478
        return 5064;
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
    case 0x1638: { // sts 0x2435, r24
        uint16_t address = 9269;
        pm_write(s, address, s->r[24]);
        return 5692;
    }
    case 0x163c: { // sts 0x2436, r25
        uint16_t address = 9270;
        pm_write(s, address, s->r[25]);
        return 5696;
    }
    case 0x164c: { // rjmp .-84
        return 5626;
    }
    case 0x1662: { // rjmp .+2430
        return 8162;
    }
    case 0x1666: { // sts 0x0666, r16
        uint16_t address = 1638;
        pm_write(s, address, s->r[16]);
        return 5738;
    }
    case 0x166c: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5744;
    }
    case 0x1672: { // sts 0x0681, r16
        uint16_t address = 1665;
        pm_write(s, address, s->r[16]);
        return 5750;
    }
    case 0x1678: { // sts 0x0AC0, r16
        uint16_t address = 2752;
        pm_write(s, address, s->r[16]);
        return 5756;
    }
    case 0x168c: { // sts 0x0682, r16
        uint16_t address = 1666;
        pm_write(s, address, s->r[16]);
        return 5776;
    }
    case 0x169a: { // sts 0x0665, r16
        uint16_t address = 1637;
        pm_write(s, address, s->r[16]);
        return 5790;
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
    case 0x16e2: { // sts 0x2437, r24
        uint16_t address = 9271;
        pm_write(s, address, s->r[24]);
        return 5862;
    }
    case 0x16e6: { // sts 0x2438, r25
        uint16_t address = 9272;
        pm_write(s, address, s->r[25]);
        return 5866;
    }
    case 0x1720: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 5924;
    }
    case 0x1726: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 5930;
    }
    case 0x172a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 5934;
    }
    case 0x172e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 5938 : 5936;
    }
    case 0x1730: { // rjmp .-8
        return 5930;
    }
    case 0x1756: { // sts 0x0AC3, r24
        uint16_t address = 2755;
        pm_write(s, address, s->r[24]);
        return 5978;
    }
    case 0x1762: { // lds r24, 0x0AC3
        uint16_t address = 2755;
        s->r[24] = pm_read(s, address);
        return 5990;
    }
    case 0x176e: { // rjmp .-54
        return 5946;
    }
    case 0x1788: { // sts 0x0AC3, r30
        uint16_t address = 2755;
        pm_write(s, address, s->r[30]);
        return 6028;
    }
    case 0x1794: { // sts 0x0AC3, r29
        uint16_t address = 2755;
        pm_write(s, address, s->r[29]);
        return 6040;
    }
    case 0x17a0: { // sts 0x0AC3, r28
        uint16_t address = 2755;
        pm_write(s, address, s->r[28]);
        return 6052;
    }
    case 0x17b4: { // lds r22, 0x0AC2
        uint16_t address = 2754;
        s->r[22] = pm_read(s, address);
        return 6072;
    }
    case 0x17bc: { // sts 0x0AC3, r10
        uint16_t address = 2755;
        pm_write(s, address, s->r[10]);
        return 6080;
    }
    case 0x17c8: { // lds r22, 0x0AC3
        uint16_t address = 2755;
        s->r[22] = pm_read(s, address);
        return 6092;
    }
    case 0x17e2: { // rjmp .+162
        return 6278;
    }
    case 0x17f4: { // rjmp .+4
        return 6138;
    }
    case 0x180a: { // sts 0x0686, r17
        uint16_t address = 1670;
        pm_write(s, address, s->r[17]);
        return 6158;
    }
    case 0x181a: { // sts 0x0AC3, r22
        uint16_t address = 2755;
        pm_write(s, address, s->r[22]);
        return 6174;
    }
    case 0x1826: { // sts 0x0AC3, r21
        uint16_t address = 2755;
        pm_write(s, address, s->r[21]);
        return 6186;
    }
    case 0x1832: { // sts 0x0AC3, r20
        uint16_t address = 2755;
        pm_write(s, address, s->r[20]);
        return 6198;
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
    case 0x187e: { // sbrs r20, 7
        return (!!(s->r[20] & (1u << 7)) == 1) ? 6274 : 6272;
    }
    case 0x1882: { // lds r16, 0x0AC3
        uint16_t address = 2755;
        s->r[16] = pm_read(s, address);
        return 6278;
    }
    case 0x1888: { // sts 0x0685, r20
        uint16_t address = 1669;
        pm_write(s, address, s->r[20]);
        return 6284;
    }
    case 0x18bc: { // rjmp .+1828
        return 8162;
    }
    case 0x190c: { // rjmp .+1748
        return 8162;
    }
    case 0x190e: { // rjmp .-1352
        return 5064;
    }
    case 0x1968: { // sbrs r17, 0
        return (!!(s->r[17] & (1u << 0)) == 1) ? 6508 : 6506;
    }
    case 0x19aa: { // rjmp .+1590
        return 8162;
    }
    case 0x19ac: { // rjmp .-1510
        return 5064;
    }
    case 0x1a06: { // rjmp .-32
        return 6632;
    }
    case 0x1a0c: { // rjmp .+1492
        return 8162;
    }
    case 0x1a14: { // sts 0x01CA, r16
        uint16_t address = 458;
        pm_write(s, address, s->r[16]);
        return 6680;
    }
    case 0x1a18: { // sts 0x01C0, r28
        uint16_t address = 448;
        pm_write(s, address, s->r[28]);
        return 6684;
    }
    case 0x1a1c: { // sts 0x01C1, r29
        uint16_t address = 449;
        pm_write(s, address, s->r[29]);
        return 6688;
    }
    case 0x1a26: { // sts 0x0034, r17
        uint16_t address = 52;
        pm_write(s, address, s->r[17]);
        return 6698;
    }
    case 0x1a2a: { // sts 0x01CB, r16
        uint16_t address = 459;
        pm_write(s, address, s->r[16]);
        return 6702;
    }
    case 0x1a30: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 6708;
    }
    case 0x1a3c: { // rjmp .-1654
        return 5064;
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
    case 0x1b00: { // rjmp .+16
        return 6930;
    }
    case 0x1b08: { // sbrs r18, 6
        return (!!(s->r[18] & (1u << 6)) == 1) ? 6924 : 6922;
    }
    case 0x1b10: { // rjmp .-34
        return 6896;
    }
    case 0x1b1a: { // lds r19, 0x2162
        uint16_t address = 8546;
        s->r[19] = pm_read(s, address);
        return 6942;
    }
    case 0x1b4e: { // rjmp .+6
        return 6998;
    }
    case 0x1b54: { // rjmp .-102
        return 6896;
    }
    case 0x1b62: { // sbrs r18, 7
        return (!!(s->r[18] & (1u << 7)) == 1) ? 7014 : 7012;
    }
    case 0x1b64: { // rjmp .+44
        return 7058;
    }
    case 0x1b6c: { // lds r20, 0x2158
        uint16_t address = 8536;
        s->r[20] = pm_read(s, address);
        return 7024;
    }
    case 0x1b72: { // sbrs r20, 2
        return (!!(s->r[20] & (1u << 2)) == 1) ? 7030 : 7028;
    }
    case 0x1b9a: { // lds r20, 0x2441
        uint16_t address = 9281;
        s->r[20] = pm_read(s, address);
        return 7070;
    }
    case 0x1bd8: { // sbrs r20, 1
        return (!!(s->r[20] & (1u << 1)) == 1) ? 7132 : 7130;
    }
    case 0x1c16: { // rjmp .+68
        return 7260;
    }
    case 0x1c60: { // rjmp .-2202
        return 5064;
    }
    case 0x1cec: { // rjmp .-130
        return 7276;
    }
    case 0x1cf0: { // rjmp .-2346
        return 5064;
    }
    case 0x1d60: { // rjmp .-2458
        return 5064;
    }
    case 0x1d7e: { // sts 0x2230, r20
        uint16_t address = 8752;
        pm_write(s, address, s->r[20]);
        return 7554;
    }
    case 0x1d82: { // sts 0x2231, r21
        uint16_t address = 8753;
        pm_write(s, address, s->r[21]);
        return 7558;
    }
    case 0x1d90: { // rjmp .+592
        return 8162;
    }
    case 0x1da6: { // sts 0x222F, r20
        uint16_t address = 8751;
        pm_write(s, address, s->r[20]);
        return 7594;
    }
    case 0x1db4: { // rjmp .+556
        return 8162;
    }
    case 0x1e2a: { // rjmp .+438
        return 8162;
    }
    case 0x1e2c: { // rjmp .-2662
        return 5064;
    }
    case 0x1e78: { // rjmp .+360
        return 8162;
    }
    case 0x1e7a: { // rjmp .-2740
        return 5064;
    }
    case 0x1ede: { // rjmp .+258
        return 8162;
    }
    case 0x1f36: { // rjmp .+170
        return 8162;
    }
    case 0x1f38: { // rjmp .-2930
        return 5064;
    }
    case 0x1f8c: { // rjmp .+84
        return 8162;
    }
    case 0x1fea: { // rjmp .-3108
        return 5064;
    }
    case 0x204a: { // rjmp .-106
        return 8162;
    }
    case 0x204c: { // rjmp .-3206
        return 5064;
    }
    case 0x20a4: { // rjmp .-196
        return 8162;
    }
    case 0x219a: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8606;
    }
    case 0x21a2: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8614;
    }
    case 0x21ae: { // sts 0x0625, r20
        uint16_t address = 1573;
        pm_write(s, address, s->r[20]);
        return 8626;
    }
    case 0x21be: { // sts 0x0626, r20
        uint16_t address = 1574;
        pm_write(s, address, s->r[20]);
        return 8642;
    }
    case 0x21e4: { // sts 0x0625, r21
        uint16_t address = 1573;
        pm_write(s, address, s->r[21]);
        return 8680;
    }
    case 0x2216: { // rjmp .+134
        return 8862;
    }
    case 0x225e: { // sts 0x0622, r20
        uint16_t address = 1570;
        pm_write(s, address, s->r[20]);
        return 8802;
    }
    case 0x2274: { // lds r16, 0x0628
        uint16_t address = 1576;
        s->r[16] = pm_read(s, address);
        return 8824;
    }
    case 0x228a: { // sts 0x0621, r20
        uint16_t address = 1569;
        pm_write(s, address, s->r[20]);
        return 8846;
    }
    case 0x22b4: { // sts 0x08C0, r23
        uint16_t address = 2240;
        pm_write(s, address, s->r[23]);
        return 8888;
    }
    case 0x22d6: { // sts 0x0646, r24
        uint16_t address = 1606;
        pm_write(s, address, s->r[24]);
        return 8922;
    }
    case 0x22da: { // sts 0x08C3, r22
        uint16_t address = 2243;
        pm_write(s, address, s->r[22]);
        return 8926;
    }
    case 0x2300: { // sts 0x0645, r22
        uint16_t address = 1605;
        pm_write(s, address, s->r[22]);
        return 8964;
    }
    case 0x2326: { // sts 0x0666, r22
        uint16_t address = 1638;
        pm_write(s, address, s->r[22]);
        return 9002;
    }
    case 0x2336: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9018;
    }
    case 0x235c: { // sts 0x0665, r22
        uint16_t address = 1637;
        pm_write(s, address, s->r[22]);
        return 9056;
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
    case 0x248a: { // sts 0x08C0, r22
        uint16_t address = 2240;
        pm_write(s, address, s->r[22]);
        return 9358;
    }
    case 0x2490: { // sts 0x06A6, r22
        uint16_t address = 1702;
        pm_write(s, address, s->r[22]);
        return 9364;
    }
    case 0x2494: { // sts 0x08C3, r16
        uint16_t address = 2243;
        pm_write(s, address, s->r[16]);
        return 9368;
    }
    case 0x2498: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9372;
    }
    case 0x249c: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9376 : 9374;
    }
    case 0x24a0: { // sts 0x08C3, r17
        uint16_t address = 2243;
        pm_write(s, address, s->r[17]);
        return 9380;
    }
    case 0x24ac: { // sts 0x08C3, r18
        uint16_t address = 2243;
        pm_write(s, address, s->r[18]);
        return 9392;
    }
    case 0x24b8: { // sts 0x08C3, r19
        uint16_t address = 2243;
        pm_write(s, address, s->r[19]);
        return 9404;
    }
    case 0x24c6: { // sts 0x06A5, r22
        uint16_t address = 1701;
        pm_write(s, address, s->r[22]);
        return 9418;
    }
    case 0x24e4: { // sts 0x08C3, r23
        uint16_t address = 2243;
        pm_write(s, address, s->r[23]);
        return 9448;
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
    case 0x25a2: { // sts 0x0606, r18
        uint16_t address = 1542;
        pm_write(s, address, s->r[18]);
        return 9638;
    }
    case 0x25b0: { // sts 0x0605, r18
        uint16_t address = 1541;
        pm_write(s, address, s->r[18]);
        return 9652;
    }
    case 0x25c6: { // sts 0x0606, r20
        uint16_t address = 1542;
        pm_write(s, address, s->r[20]);
        return 9674;
    }
    case 0x260e: { // sts 0x0605, r17
        uint16_t address = 1541;
        pm_write(s, address, s->r[17]);
        return 9746;
    }
    case 0x2612: { // sbrs r16, 7
        return (!!(s->r[16] & (1u << 7)) == 1) ? 9752 : 9748;
    }
    case 0x2614: { // sts 0x0606, r17
        uint16_t address = 1542;
        pm_write(s, address, s->r[17]);
        return 9752;
    }
    case 0x2626: { // lds r17, 0x0608
        uint16_t address = 1544;
        s->r[17] = pm_read(s, address);
        return 9770;
    }
    case 0x2658: { // rjmp .-16
        return 9802;
    }
    case 0x2680: { // rjmp .+26
        return 9884;
    }
    case 0x2734: { // rjmp .+30
        return 10068;
    }
    case 0x273c: { // rjmp .+22
        return 10068;
    }
    case 0x2788: { // sbrs r14, 0
        return (!!(s->r[14] & (1u << 0)) == 1) ? 10124 : 10122;
    }
    case 0x2830: { // rjmp .-10
        return 10280;
    }
    case 0x285e: { // lds r20, 0x06A4
        uint16_t address = 1700;
        s->r[20] = pm_read(s, address);
        return 10338;
    }
    case 0x2862: { // sbrs r20, 0
        return (!!(s->r[20] & (1u << 0)) == 1) ? 10342 : 10340;
    }
    case 0x2864: { // rjmp .+14
        return 10356;
    }
    case 0x2870: { // sts 0x06A6, r20
        uint16_t address = 1702;
        pm_write(s, address, s->r[20]);
        return 10356;
    }
    case 0x2880: { // sts 0x2000, r17
        uint16_t address = 8192;
        pm_write(s, address, s->r[17]);
        return 10372;
    }
    case 0x2888: { // lds r17, 0x2005
        uint16_t address = 8197;
        s->r[17] = pm_read(s, address);
        return 10380;
    }
    case 0x288e: { // sts 0x2005, r17
        uint16_t address = 8197;
        pm_write(s, address, s->r[17]);
        return 10386;
    }
    case 0x2898: { // sbrs r19, 0
        return (!!(s->r[19] & (1u << 0)) == 1) ? 10396 : 10394;
    }
    case 0x28d0: { // rjmp .-22
        return 10428;
    }
    case 0x28f0: { // lds r20, 0x0BA1
        uint16_t address = 2977;
        s->r[20] = pm_read(s, address);
        return 10484;
    }
    case 0x28f4: { // sbrs r20, 5
        return (!!(s->r[20] & (1u << 5)) == 1) ? 10488 : 10486;
    }
    case 0x28f6: { // rjmp .-38
        return 10450;
    }
    case 0x28f8: { // sts 0x0BA0, r16
        uint16_t address = 2976;
        pm_write(s, address, s->r[16]);
        return 10492;
    }
    case 0x28fc: { // lds r20, 0x0BA3
        uint16_t address = 2979;
        s->r[20] = pm_read(s, address);
        return 10496;
    }
    case 0x2902: { // sts 0x0BA3, r20
        uint16_t address = 2979;
        pm_write(s, address, s->r[20]);
        return 10502;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
