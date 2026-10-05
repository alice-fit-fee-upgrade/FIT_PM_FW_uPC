#include <stdint.h>
register uint16_t pm_crc_address asm("r28");
#define RAM8(address) (*(volatile uint8_t *)(address))
#define WAIT_READY(reg) do { \
 register uint8_t status asm(reg); \
 do { status = RAM8(0x0ac2); asm volatile("" : "+r" (status)); } \
 while (!(status & 0x80u)); \
} while (0)
/* Read the original 24-bit inclusive address range and stream bytes to the
 * exact CRC core. Its unusual byte ordering and live R2:R0 are preserved. */
void FUN_code_000bb9(void)
{
    register uint8_t byte asm("r16") = 0x10;
    asm volatile("" : "+r" (byte));
    RAM8(0x0686) = byte;
    byte = 3;
    asm volatile("" : "+r" (byte));
    RAM8(0x0ac3) = byte;
    register uint8_t dummy asm("r10");
    asm volatile("clr %0" : "=r" (dummy) : : "cc");
    register uint8_t low asm("r28"), middle asm("r29"), high asm("r30");
    asm volatile("" : "=r" (low), "=r" (middle), "=r" (high));
    WAIT_READY("r19");
    RAM8(0x0ac3) = high;
    WAIT_READY("r19");
    RAM8(0x0ac3) = middle;
    WAIT_READY("r19");
    RAM8(0x0ac3) = low;
    /* Separate byte initialization preserves all four original LDI opcodes. */
    register uint8_t crc0 asm("r16") = 0xff;
    asm volatile("" : "+r" (crc0));
    register uint8_t crc1 asm("r17") = 0xff;
    asm volatile("" : "+r" (crc1));
    register uint8_t crc2 asm("r18") = 0xff;
    asm volatile("" : "+r" (crc2));
    register uint8_t crc3 asm("r19") = 0xff;
    asm volatile("" : "+r" (crc3));
    register uint32_t crc asm("r16");
    asm volatile("" : "=r" (crc));
    register uint8_t polynomial0 asm("r23") = 0xb7;
    asm volatile("" : "+r" (polynomial0));
    register uint8_t polynomial1 asm("r24") = 0x1d;
    asm volatile("" : "+r" (polynomial1));
    register uint8_t polynomial2 asm("r25") = 0xc1;
    asm volatile("" : "+r" (polynomial2));
    register uint8_t polynomial3 asm("r26") = 4;
    asm volatile("" : "+r" (polynomial3));
    WAIT_READY("r22");
    RAM8(0x0ac3) = dummy;
next_byte:
    WAIT_READY("r22");
    register uint8_t received asm("r22") = RAM8(0x0ac3);
    asm volatile("" : "+r" (received));
    asm goto("cp r0, r28\n\tcpc r1, r29\n\tcpc r2, r30\n\tbreq %l[last_byte]" : : : "cc" : last_byte);
    RAM8(0x0ac3) = dummy;
    asm volatile("rcall FUN_code_000bf2"
                 : "+r" (crc), "+r" (low), "+r" (middle), "+r" (high), "+r" (received)
                 : "r" (dummy), "r" (polynomial0), "r" (polynomial1), "r" (polynomial2), "r" (polynomial3) : "r20", "memory", "cc");
    asm volatile("" : "=r" (pm_crc_address) : : "memory");
    pm_crc_address += 1;
    asm volatile("" : "+r" (pm_crc_address) : : "memory");
    /* C 24-bit carry equivalent: high += carry_out; exact ADC retains flags. */
    asm volatile("adc r30, r10" : "+r" (high) : "r" (dummy) : "cc");
    goto next_byte;
last_byte:
    asm volatile("rcall FUN_code_000bf2\n\trjmp LAB_code_000c43"
                 : : "r" (crc), "r" (received), "r" (polynomial0), "r" (polynomial1), "r" (polynomial2), "r" (polynomial3) : "memory", "cc");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 704 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_crc_stream_read(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1772: { // ldi r16, 0x10
        s->r[16] = 16;
        return 6004;
    }
    case 0x1774: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 6008;
    }
    case 0x1778: { // ldi r16, 0x03
        s->r[16] = 3;
        return 6010;
    }
    case 0x177a: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 6014;
    }
    case 0x177e: { // eor r10, r10
        s->r[10] ^= s->r[10];
        pm_nzv(s, s->r[10], false);
        return 6016;
    }
    case 0x1780: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6020;
    }
    case 0x1784: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6024 : 6022;
    }
    case 0x1786: { // rjmp .-8
        return 6016;
    }
    case 0x1788: { // sts 0x0AC3, r30
        uint16_t address = 2755;
        pm_write(s, address, s->r[30]);
        return 6028;
    }
    case 0x178c: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6032;
    }
    case 0x1790: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6036 : 6034;
    }
    case 0x1792: { // rjmp .-8
        return 6028;
    }
    case 0x1794: { // sts 0x0AC3, r29
        uint16_t address = 2755;
        pm_write(s, address, s->r[29]);
        return 6040;
    }
    case 0x1798: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6044;
    }
    case 0x179c: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6048 : 6046;
    }
    case 0x179e: { // rjmp .-8
        return 6040;
    }
    case 0x17a0: { // sts 0x0AC3, r28
        uint16_t address = 2755;
        pm_write(s, address, s->r[28]);
        return 6052;
    }
    case 0x17a4: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 6054;
    }
    case 0x17a6: { // ldi r17, 0xFF
        s->r[17] = 255;
        return 6056;
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
    case 0x17b8: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 6076 : 6074;
    }
    case 0x17ba: { // rjmp .-8
        return 6068;
    }
    case 0x17bc: { // sts 0x0AC3, r10
        uint16_t address = 2755;
        pm_write(s, address, s->r[10]);
        return 6080;
    }
    case 0x17c0: { // lds r22, 0x0AC2
        uint16_t address = 2754;
        s->r[22] = pm_read(s, address);
        return 6084;
    }
    case 0x17c4: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 6088 : 6086;
    }
    case 0x17c6: { // rjmp .-8
        return 6080;
    }
    case 0x17c8: { // lds r22, 0x0AC3
        uint16_t address = 2755;
        s->r[22] = pm_read(s, address);
        return 6092;
    }
    case 0x17cc: { // cp r0, r28
        pm_sub(s, s->r[0], s->r[28], 0, false);
        return 6094;
    }
    case 0x17ce: { // cpc r1, r29
        pm_sub(s, s->r[1], s->r[29], pm_getflag(s, CARRY), true);
        return 6096;
    }
    case 0x17d0: { // cpc r2, r30
        pm_sub(s, s->r[2], s->r[30], pm_getflag(s, CARRY), true);
        return 6098;
    }
    case 0x17d2: { // breq .+12
        return (pm_getflag(s, 1) == 1) ? 6112 : 6100;
    }
    case 0x17d4: { // sts 0x0AC3, r10
        uint16_t address = 2755;
        pm_write(s, address, s->r[10]);
        return 6104;
    }
    case 0x17d8: { // rcall .+10
        s->calls[s->call_depth++] = 6106;
        return 6116;
    }
    case 0x17da: { // adiw r28, 0x01
        uint16_t old = pm_pointer(s, 28);
        uint16_t value = old + 1;
        pm_setpointer(s, 28, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 6108;
    }
    case 0x17dc: { // adc r30, r10
        s->r[30] = pm_add(s, s->r[30], s->r[10], pm_getflag(s, CARRY));
        return 6110;
    }
    case 0x17de: { // rjmp .-32
        return 6080;
    }
    case 0x17e0: { // rcall .+2
        s->calls[s->call_depth++] = 6114;
        return 6116;
    }
    case 0x17e2: { // rjmp .+162
        return 6278;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
