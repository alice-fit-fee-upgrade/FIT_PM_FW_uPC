#include "legacy_spi_c.h"

/* Legacy input R18 is an address; output word is returned in R17:R16.
 * R21 starts with the caller's bits, as in the original (no invented clear). */
void fpga_msg_read_t1(void)
{
    PM_WRITE_R22(SPIC_CTRL, 0xd1);
    register uint8_t mask asm("r22") = 1;
    register uint8_t address asm("r18");
    register uint8_t address_low asm("r21");
    asm volatile("" : "=r" (address), "=r" (address_low));
    asm volatile("" : "+r" (address), "+r" (address_low), "+r" (mask));
    address >>= 1;
    asm volatile("ror %1" : "+r" (address), "+r" (address_low) : : "cc");
    address >>= 1;
    asm volatile("ror %1" : "+r" (address), "+r" (address_low) : : "cc");
    address |= 0x80;
    asm volatile("" : "+r" (address));
    asm volatile("" : "+r" (mask));
    PORTD_OUTCLR = mask;
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r18");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r21");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_READ_REGISTER(SPIC, "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    PM_SPI_READ_REGISTER(SPIC, "r16");
    PM_WRITE_R22(PORTD_OUTSET, 1);
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 576 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_fpga_word_read(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2368: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9066;
    }
    case 0x236a: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 9068;
    }
    case 0x236c: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 9070;
    }
    case 0x236e: { // ldi r22, 0xD1
        s->r[22] = 209;
        return 9072;
    }
    case 0x2370: { // sts 0x08C0, r22
        uint16_t address = 2240;
        pm_write(s, address, s->r[22]);
        return 9076;
    }
    case 0x2374: { // ldi r22, 0x01
        s->r[22] = 1;
        return 9078;
    }
    case 0x2376: { // lsr r18
        bool carry = s->r[18] & 1;
        s->r[18] = (s->r[18] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[18], !!(s->r[18] & 128) ^ carry);
        return 9080;
    }
    case 0x2378: { // ror r21
        bool carry = s->r[21] & 1;
        s->r[21] = (s->r[21] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[21], !!(s->r[21] & 128) ^ carry);
        return 9082;
    }
    case 0x237a: { // lsr r18
        bool carry = s->r[18] & 1;
        s->r[18] = (s->r[18] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[18], !!(s->r[18] & 128) ^ carry);
        return 9084;
    }
    case 0x237c: { // ror r21
        bool carry = s->r[21] & 1;
        s->r[21] = (s->r[21] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[21], !!(s->r[21] & 128) ^ carry);
        return 9086;
    }
    case 0x237e: { // ori r18, 0x80
        s->r[18] |= 128;
        pm_nzv(s, s->r[18], false);
        return 9088;
    }
    case 0x2380: { // sts 0x0666, r22
        uint16_t address = 1638;
        pm_write(s, address, s->r[22]);
        return 9092;
    }
    case 0x2384: { // sts 0x08C3, r18
        uint16_t address = 2243;
        pm_write(s, address, s->r[18]);
        return 9096;
    }
    case 0x2388: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9100;
    }
    case 0x238c: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9104 : 9102;
    }
    case 0x238e: { // rjmp .-8
        return 9096;
    }
    case 0x2390: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9108;
    }
    case 0x2394: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9112;
    }
    case 0x2398: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9116 : 9114;
    }
    case 0x239a: { // rjmp .-8
        return 9108;
    }
    case 0x239c: { // sts 0x08C3, r17
        uint16_t address = 2243;
        pm_write(s, address, s->r[17]);
        return 9120;
    }
    case 0x23a0: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9124;
    }
    case 0x23a4: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9128 : 9126;
    }
    case 0x23a6: { // rjmp .-8
        return 9120;
    }
    case 0x23a8: { // lds r17, 0x08C3
        uint16_t address = 2243;
        s->r[17] = pm_read(s, address);
        return 9132;
    }
    case 0x23ac: { // sts 0x08C3, r16
        uint16_t address = 2243;
        pm_write(s, address, s->r[16]);
        return 9136;
    }
    case 0x23b0: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9140;
    }
    case 0x23b4: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9144 : 9142;
    }
    case 0x23b6: { // rjmp .-8
        return 9136;
    }
    case 0x23b8: { // lds r16, 0x08C3
        uint16_t address = 2243;
        s->r[16] = pm_read(s, address);
        return 9148;
    }
    case 0x23bc: { // ldi r22, 0x01
        s->r[22] = 1;
        return 9150;
    }
    case 0x23be: { // sts 0x0665, r22
        uint16_t address = 1637;
        pm_write(s, address, s->r[22]);
        return 9154;
    }
    case 0x23c2: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 9156;
    }
    case 0x23c4: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 9158;
    }
    case 0x23c6: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9160;
    }
    case 0x23c8: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
