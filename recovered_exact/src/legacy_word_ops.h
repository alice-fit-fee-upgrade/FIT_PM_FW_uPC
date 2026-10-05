#ifndef PM_LEGACY_WORD_OPS_H
#define PM_LEGACY_WORD_OPS_H
#include <stdint.h>

/* Exact private-ABI operations, expanded at the call site. Not ordinary GNU
 * calls. Signed limits use the original preloaded high byte and an immediate
 * low byte; the caller must preload high == ((uint16_t)limit >> 8).
 * error_label is an assembler symbol string for an original shared error tail.
 * Logical C: if ((int16_t)value >= limit) goto error;
 *           if ((int16_t)value < limit) goto error;
 * SREG is modified exactly by CPI/CPC; no registers or memory are written. */
#define PM_REJECT_I16_GE(value, high, limit, error_label) \
    asm volatile("cpi %A0, %2\n\tcpc %B0, %1\n\tbrge " error_label \
        : : "r" (value), "r" (high), "n" ((uint16_t)(limit) & 0xffu) : "cc")
#define PM_REJECT_I16_LT(value, high, limit, error_label) \
    asm volatile("cpi %A0, %2\n\tcpc %B0, %1\n\tbrlt " error_label \
        : : "r" (value), "r" (high), "n" ((uint16_t)(limit) & 0xffu) : "cc")

/* Original settings store: Y addresses the low byte, value is R21:R20.
 * Writes little-endian bytes; advances Y by one, leaving it at the high byte.
 * Logical C: *cursor++ = (uint8_t)value; *cursor = (uint8_t)(value >> 8);
 * No interrupt masking is added; the caller retains its original IRQ window.
 * ST Y+ remains exact ASM; the high-byte store is ordinary C. */
#define PM_STORE_WORD_LE_Y(cursor, value) do { \
    asm volatile("st Y+, %A1" : "+y" (cursor) : "r" (value) : "memory"); \
    register uint8_t pm_store_high asm("r21"); \
    asm volatile("" : "=r" (pm_store_high) : "r" (value)); \
    *(cursor) = pm_store_high; \
    asm volatile("" : : : "memory"); \
} while (0)
#endif

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 2368 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_legacy_word_ops(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0220: { // cpi r16, 0xAA
        pm_sub(s, s->r[16], 170, 0, false);
        return 546;
    }
    case 0x022c: { // cpi r17, 0x07
        pm_sub(s, s->r[17], 7, 0, false);
        return 558;
    }
    case 0x0232: { // cpi r28, 0x41
        pm_sub(s, s->r[28], 65, 0, false);
        return 564;
    }
    case 0x02a8: { // st Y+, r18
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[18]);
        return 682;
    }
    case 0x02aa: { // st Y+, r19
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[19]);
        return 684;
    }
    case 0x0386: { // cpc r17, r18
        pm_sub(s, s->r[17], s->r[18], pm_getflag(s, CARRY), true);
        return 904;
    }
    case 0x042a: { // st X+, r18
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[18]);
        return 1068;
    }
    case 0x042c: { // st X+, r19
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[19]);
        return 1070;
    }
    case 0x0430: { // st X+, r17
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[17]);
        return 1074;
    }
    case 0x04ec: { // cpi r16, 0x0F
        pm_sub(s, s->r[16], 15, 0, false);
        return 1262;
    }
    case 0x0540: { // cpi r16, 0xC4
        pm_sub(s, s->r[16], 196, 0, false);
        return 1346;
    }
    case 0x0564: { // cpi r17, 0x01
        pm_sub(s, s->r[17], 1, 0, false);
        return 1382;
    }
    case 0x0624: { // cpi r16, 0x8C
        pm_sub(s, s->r[16], 140, 0, false);
        return 1574;
    }
    case 0x06b4: { // cpi r16, 0x03
        pm_sub(s, s->r[16], 3, 0, false);
        return 1718;
    }
    case 0x06d8: { // cpi r16, 0x04
        pm_sub(s, s->r[16], 4, 0, false);
        return 1754;
    }
    case 0x0736: { // cpi r24, 0xF4
        pm_sub(s, s->r[24], 244, 0, false);
        return 1848;
    }
    case 0x0738: { // cpc r25, r16
        pm_sub(s, s->r[25], s->r[16], pm_getflag(s, CARRY), true);
        return 1850;
    }
    case 0x07ba: { // cpi r17, 0x06
        pm_sub(s, s->r[17], 6, 0, false);
        return 1980;
    }
    case 0x07f0: { // cpc r17, r19
        pm_sub(s, s->r[17], s->r[19], pm_getflag(s, CARRY), true);
        return 2034;
    }
    case 0x083a: { // cpc r17, r21
        pm_sub(s, s->r[17], s->r[21], pm_getflag(s, CARRY), true);
        return 2108;
    }
    case 0x0848: { // cpi r16, 0x97
        pm_sub(s, s->r[16], 151, 0, false);
        return 2122;
    }
    case 0x084c: { // cpi r20, 0x1E
        pm_sub(s, s->r[20], 30, 0, false);
        return 2126;
    }
    case 0x0890: { // cpi r21, 0x0C
        pm_sub(s, s->r[21], 12, 0, false);
        return 2194;
    }
    case 0x0894: { // brge .+10
        return (pm_getflag(s, 4) == 0) ? 2208 : 2198;
    }
    case 0x091e: { // cpi r18, 0x0C
        pm_sub(s, s->r[18], 12, 0, false);
        return 2336;
    }
    case 0x0936: { // cpi r18, 0x3C
        pm_sub(s, s->r[18], 60, 0, false);
        return 2360;
    }
    case 0x09a8: { // cpi r18, 0xB0
        pm_sub(s, s->r[18], 176, 0, false);
        return 2474;
    }
    case 0x09be: { // cpi r18, 0xBC
        pm_sub(s, s->r[18], 188, 0, false);
        return 2496;
    }
    case 0x0aae: { // cpi r16, 0x05
        pm_sub(s, s->r[16], 5, 0, false);
        return 2736;
    }
    case 0x0bf8: { // cpi r16, 0x02
        pm_sub(s, s->r[16], 2, 0, false);
        return 3066;
    }
    case 0x0e74: { // st Z, r29
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[29]);
        return 3702;
    }
    case 0x0ecc: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 3790;
    }
    case 0x0f26: { // st Z, r17
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[17]);
        return 3880;
    }
    case 0x0f32: { // st Z, r18
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 3892;
    }
    case 0x0f34: { // cpi r18, 0x0D
        pm_sub(s, s->r[18], 13, 0, false);
        return 3894;
    }
    case 0x109c: { // st Z+, r25
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        pm_write(s, address, s->r[25]);
        return 4254;
    }
    case 0x10a0: { // cpc r31, r29
        pm_sub(s, s->r[31], s->r[29], pm_getflag(s, CARRY), true);
        return 4258;
    }
    case 0x112c: { // st Z+, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        pm_write(s, address, s->r[16]);
        return 4398;
    }
    case 0x1130: { // cpc r31, r17
        pm_sub(s, s->r[31], s->r[17], pm_getflag(s, CARRY), true);
        return 4402;
    }
    case 0x11b0: { // cpi r16, 0x31
        pm_sub(s, s->r[16], 49, 0, false);
        return 4530;
    }
    case 0x11b2: { // cpc r17, r24
        pm_sub(s, s->r[17], s->r[24], pm_getflag(s, CARRY), true);
        return 4532;
    }
    case 0x11d8: { // st Y+, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[16]);
        return 4570;
    }
    case 0x11da: { // st Y+, r17
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[17]);
        return 4572;
    }
    case 0x11ea: { // cpi r19, 0x01
        pm_sub(s, s->r[19], 1, 0, false);
        return 4588;
    }
    case 0x11f0: { // cpi r16, 0xF5
        pm_sub(s, s->r[16], 245, 0, false);
        return 4594;
    }
    case 0x11f4: { // brge .+14
        return (pm_getflag(s, 4) == 0) ? 4612 : 4598;
    }
    case 0x11f8: { // cpi r16, 0x0C
        pm_sub(s, s->r[16], 12, 0, false);
        return 4602;
    }
    case 0x11fc: { // brge .+26
        return (pm_getflag(s, 4) == 0) ? 4632 : 4606;
    }
    case 0x122a: { // cpi r19, 0x02
        pm_sub(s, s->r[19], 2, 0, false);
        return 4652;
    }
    case 0x126c: { // cpi r16, 0x21
        pm_sub(s, s->r[16], 33, 0, false);
        return 4718;
    }
    case 0x129a: { // cpi r16, 0xA1
        pm_sub(s, s->r[16], 161, 0, false);
        return 4764;
    }
    case 0x12f4: { // cpi r16, 0x43
        pm_sub(s, s->r[16], 67, 0, false);
        return 4854;
    }
    case 0x1302: { // cpi r16, 0x50
        pm_sub(s, s->r[16], 80, 0, false);
        return 4868;
    }
    case 0x1308: { // cpi r16, 0x4F
        pm_sub(s, s->r[16], 79, 0, false);
        return 4874;
    }
    case 0x1310: { // cpi r16, 0x4E
        pm_sub(s, s->r[16], 78, 0, false);
        return 4882;
    }
    case 0x1316: { // cpi r16, 0x46
        pm_sub(s, s->r[16], 70, 0, false);
        return 4888;
    }
    case 0x1336: { // cpi r16, 0x52
        pm_sub(s, s->r[16], 82, 0, false);
        return 4920;
    }
    case 0x134a: { // cpi r16, 0x53
        pm_sub(s, s->r[16], 83, 0, false);
        return 4940;
    }
    case 0x1356: { // cpi r16, 0x54
        pm_sub(s, s->r[16], 84, 0, false);
        return 4952;
    }
    case 0x135c: { // cpi r16, 0x5A
        pm_sub(s, s->r[16], 90, 0, false);
        return 4958;
    }
    case 0x1374: { // cpi r16, 0x4C
        pm_sub(s, s->r[16], 76, 0, false);
        return 4982;
    }
    case 0x138c: { // cpi r16, 0x44
        pm_sub(s, s->r[16], 68, 0, false);
        return 5006;
    }
    case 0x13aa: { // cpi r16, 0x56
        pm_sub(s, s->r[16], 86, 0, false);
        return 5036;
    }
    case 0x13b6: { // cpi r16, 0x57
        pm_sub(s, s->r[16], 87, 0, false);
        return 5048;
    }
    case 0x1530: { // cpi r22, 0x02
        pm_sub(s, s->r[22], 2, 0, false);
        return 5426;
    }
    case 0x1532: { // brge .-94
        return (pm_getflag(s, 4) == 0) ? 5334 : 5428;
    }
    case 0x1536: { // cpc r4, r31
        pm_sub(s, s->r[4], s->r[31], pm_getflag(s, CARRY), true);
        return 5432;
    }
    case 0x1538: { // cpc r5, r22
        pm_sub(s, s->r[5], s->r[22], pm_getflag(s, CARRY), true);
        return 5434;
    }
    case 0x1590: { // cpi r30, 0x7E
        pm_sub(s, s->r[30], 126, 0, false);
        return 5522;
    }
    case 0x15b8: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 5562;
    }
    case 0x15be: { // st X, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_write(s, address, s->r[16]);
        return 5568;
    }
    case 0x16dc: { // cpc r1, r21
        pm_sub(s, s->r[1], s->r[21], pm_getflag(s, CARRY), true);
        return 5854;
    }
    case 0x16de: { // cpc r2, r22
        pm_sub(s, s->r[2], s->r[22], pm_getflag(s, CARRY), true);
        return 5856;
    }
    case 0x17ce: { // cpc r1, r29
        pm_sub(s, s->r[1], s->r[29], pm_getflag(s, CARRY), true);
        return 6096;
    }
    case 0x17d0: { // cpc r2, r30
        pm_sub(s, s->r[2], s->r[30], pm_getflag(s, CARRY), true);
        return 6098;
    }
    case 0x18b8: { // cpi r18, 0x7C
        pm_sub(s, s->r[18], 124, 0, false);
        return 6330;
    }
    case 0x1908: { // cpi r19, 0x18
        pm_sub(s, s->r[19], 24, 0, false);
        return 6410;
    }
    case 0x1964: { // cpi r16, 0x60
        pm_sub(s, s->r[16], 96, 0, false);
        return 6502;
    }
    case 0x196e: { // cpi r16, 0x10
        pm_sub(s, s->r[16], 16, 0, false);
        return 6512;
    }
    case 0x19a4: { // cpi r23, 0x0C
        pm_sub(s, s->r[23], 12, 0, false);
        return 6566;
    }
    case 0x19a6: { // brlt .-80
        return (pm_getflag(s, 4) == 1) ? 6488 : 6568;
    }
    case 0x19b0: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6578;
    }
    case 0x19f2: { // st Y, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[16]);
        return 6644;
    }
    case 0x19f6: { // cpi r30, 0x35
        pm_sub(s, s->r[30], 53, 0, false);
        return 6648;
    }
    case 0x19f8: { // cpc r31, r20
        pm_sub(s, s->r[31], s->r[20], pm_getflag(s, CARRY), true);
        return 6650;
    }
    case 0x1a00: { // cpi r18, 0x1F
        pm_sub(s, s->r[18], 31, 0, false);
        return 6658;
    }
    case 0x1ad0: { // cpi r19, 0x06
        pm_sub(s, s->r[19], 6, 0, false);
        return 6866;
    }
    case 0x1b88: { // cpi r17, 0x02
        pm_sub(s, s->r[17], 2, 0, false);
        return 7050;
    }
    case 0x1c1a: { // cpi r16, 0x01
        pm_sub(s, s->r[16], 1, 0, false);
        return 7196;
    }
    case 0x1d64: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7526;
    }
    case 0x1d72: { // cpi r20, 0x00
        pm_sub(s, s->r[20], 0, 0, false);
        return 7540;
    }
    case 0x1d74: { // cpc r21, r26
        pm_sub(s, s->r[21], s->r[26], pm_getflag(s, CARRY), true);
        return 7542;
    }
    case 0x1d76: { // brge .-24
        return (pm_getflag(s, 4) == 0) ? 7520 : 7544;
    }
    case 0x1dc0: { // brge .+106
        return (pm_getflag(s, 4) == 0) ? 7724 : 7618;
    }
    case 0x1dcc: { // cpi r16, 0x2C
        pm_sub(s, s->r[16], 44, 0, false);
        return 7630;
    }
    case 0x1de0: { // brge .+74
        return (pm_getflag(s, 4) == 0) ? 7724 : 7650;
    }
    case 0x1de2: { // cpi r24, 0x00
        pm_sub(s, s->r[24], 0, 0, false);
        return 7652;
    }
    case 0x1de4: { // cpc r25, r26
        pm_sub(s, s->r[25], s->r[26], pm_getflag(s, CARRY), true);
        return 7654;
    }
    case 0x1de6: { // brge .+68
        return (pm_getflag(s, 4) == 0) ? 7724 : 7656;
    }
    case 0x1dea: { // cpi r20, 0x55
        pm_sub(s, s->r[20], 85, 0, false);
        return 7660;
    }
    case 0x1dee: { // brlt .+60
        return (pm_getflag(s, 4) == 1) ? 7724 : 7664;
    }
    case 0x1df0: { // cpi r24, 0x55
        pm_sub(s, s->r[24], 85, 0, false);
        return 7666;
    }
    case 0x1df4: { // brlt .+54
        return (pm_getflag(s, 4) == 1) ? 7724 : 7670;
    }
    case 0x1e0c: { // st Y+, r24
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[24]);
        return 7694;
    }
    case 0x1e0e: { // st Y+, r25
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[25]);
        return 7696;
    }
    case 0x1e38: { // brge .+64
        return (pm_getflag(s, 4) == 0) ? 7802 : 7738;
    }
    case 0x1e52: { // brge .+38
        return (pm_getflag(s, 4) == 0) ? 7802 : 7764;
    }
    case 0x1e9c: { // cpi r20, 0xA1
        pm_sub(s, s->r[20], 161, 0, false);
        return 7838;
    }
    case 0x1eea: { // brge .+76
        return (pm_getflag(s, 4) == 0) ? 7992 : 7916;
    }
    case 0x1efc: { // cpi r20, 0x40
        pm_sub(s, s->r[20], 64, 0, false);
        return 7934;
    }
    case 0x1f00: { // brge .+54
        return (pm_getflag(s, 4) == 0) ? 7992 : 7938;
    }
    case 0x1f04: { // cpi r20, 0xC0
        pm_sub(s, s->r[20], 192, 0, false);
        return 7942;
    }
    case 0x1f08: { // brlt .+46
        return (pm_getflag(s, 4) == 1) ? 7992 : 7946;
    }
    case 0x1f1a: { // st Y, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[20]);
        return 7964;
    }
    case 0x1f42: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 8004;
    }
    case 0x1f44: { // brge .-14
        return (pm_getflag(s, 4) == 0) ? 7992 : 8006;
    }
    case 0x1f5a: { // cpi r20, 0x21
        pm_sub(s, s->r[20], 33, 0, false);
        return 8028;
    }
    case 0x1f5c: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 8030;
    }
    case 0x1f5e: { // brge .-40
        return (pm_getflag(s, 4) == 0) ? 7992 : 8032;
    }
    case 0x1f86: { // st Y+, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[20]);
        return 8072;
    }
    case 0x1f88: { // st Y, r21
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[21]);
        return 8074;
    }
    case 0x1f98: { // brge .-98
        return (pm_getflag(s, 4) == 0) ? 7992 : 8090;
    }
    case 0x1faa: { // cpi r20, 0x31
        pm_sub(s, s->r[20], 49, 0, false);
        return 8108;
    }
    case 0x1fae: { // brge .-120
        return (pm_getflag(s, 4) == 0) ? 7992 : 8112;
    }
    case 0x1fb2: { // cpi r20, 0x2C
        pm_sub(s, s->r[20], 44, 0, false);
        return 8116;
    }
    case 0x1fb6: { // brlt .-128
        return (pm_getflag(s, 4) == 1) ? 7992 : 8120;
    }
    case 0x2058: { // brge .-112
        return (pm_getflag(s, 4) == 0) ? 8170 : 8282;
    }
    case 0x206a: { // cpi r20, 0xF5
        pm_sub(s, s->r[20], 245, 0, false);
        return 8300;
    }
    case 0x206e: { // brge .-36
        return (pm_getflag(s, 4) == 0) ? 8268 : 8304;
    }
    case 0x2076: { // brlt .-44
        return (pm_getflag(s, 4) == 1) ? 8268 : 8312;
    }
    case 0x2186: { // cpi r19, 0x03
        pm_sub(s, s->r[19], 3, 0, false);
        return 8584;
    }
    case 0x2582: { // cpi r30, 0x96
        pm_sub(s, s->r[30], 150, 0, false);
        return 9604;
    }
    case 0x2650: { // cpi r16, 0x2D
        pm_sub(s, s->r[16], 45, 0, false);
        return 9810;
    }
    case 0x265e: { // brlt .+34
        return (pm_getflag(s, 4) == 1) ? 9858 : 9824;
    }
    case 0x2662: { // brge .+30
        return (pm_getflag(s, 4) == 0) ? 9858 : 9828;
    }
    case 0x2682: { // cpi r18, 0x01
        pm_sub(s, s->r[18], 1, 0, false);
        return 9860;
    }
    case 0x268a: { // cpi r18, 0x02
        pm_sub(s, s->r[18], 2, 0, false);
        return 9868;
    }
    case 0x26ba: { // cpi r16, 0x30
        pm_sub(s, s->r[16], 48, 0, false);
        return 9916;
    }
    case 0x26c2: { // cpi r16, 0x41
        pm_sub(s, s->r[16], 65, 0, false);
        return 9924;
    }
    case 0x26c6: { // cpi r16, 0x47
        pm_sub(s, s->r[16], 71, 0, false);
        return 9928;
    }
    case 0x26e2: { // cpi r18, 0x04
        pm_sub(s, s->r[18], 4, 0, false);
        return 9956;
    }
    case 0x2724: { // cpi r16, 0x3A
        pm_sub(s, s->r[16], 58, 0, false);
        return 10022;
    }
    case 0x2780: { // st Z, r19
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[19]);
        return 10114;
    }
    case 0x27ac: { // st -Z, r18
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 10158;
    }
    case 0x27c2: { // brlt .+24
        return (pm_getflag(s, 4) == 1) ? 10204 : 10180;
    }
    case 0x27ee: { // st -Z, r16
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 10224;
    }
    case 0x2892: { // cpi r18, 0x60
        pm_sub(s, s->r[18], 96, 0, false);
        return 10388;
    }
    case 0x2894: { // brlt .+6
        return (pm_getflag(s, 4) == 1) ? 10396 : 10390;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
