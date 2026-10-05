#include "legacy_spi_c.h"

/* R18 is the incoming address; never borrow it for peripheral constants. */

void fpga_msg_send_t2(void)
{
    PM_WRITE_R22(SPIC_CTRL, 0xd1);
    register uint8_t address asm("r18");
    register uint8_t address_low asm("r21");
    asm volatile("" : "=r" (address));
    /* Preserve the original address packing and its flag effects. */
    asm volatile("clr %1" : "+r" (address), "=r" (address_low) : : "cc");
    address >>= 1;
    asm volatile("ror %1" : "+r" (address), "+r" (address_low) : : "cc");
    address >>= 1;
    asm volatile("ror %1" : "+r" (address), "+r" (address_low) : : "cc");
    PM_WRITE_R22(PORTD_OUTCLR, 1);
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r18");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r21");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    PM_WRITE_R22(PORTD_OUTSET, 1);
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 544 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_fpga_word_write(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x230e: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8976;
    }
    case 0x2310: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 8978;
    }
    case 0x2312: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 8980;
    }
    case 0x2314: { // ldi r22, 0xD1
        s->r[22] = 209;
        return 8982;
    }
    case 0x2316: { // sts 0x08C0, r22
        uint16_t address = 2240;
        pm_write(s, address, s->r[22]);
        return 8986;
    }
    case 0x231a: { // eor r21, r21
        s->r[21] ^= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8988;
    }
    case 0x231c: { // lsr r18
        bool carry = s->r[18] & 1;
        s->r[18] = (s->r[18] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[18], !!(s->r[18] & 128) ^ carry);
        return 8990;
    }
    case 0x231e: { // ror r21
        bool carry = s->r[21] & 1;
        s->r[21] = (s->r[21] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[21], !!(s->r[21] & 128) ^ carry);
        return 8992;
    }
    case 0x2320: { // lsr r18
        bool carry = s->r[18] & 1;
        s->r[18] = (s->r[18] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[18], !!(s->r[18] & 128) ^ carry);
        return 8994;
    }
    case 0x2322: { // ror r21
        bool carry = s->r[21] & 1;
        s->r[21] = (s->r[21] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[21], !!(s->r[21] & 128) ^ carry);
        return 8996;
    }
    case 0x2324: { // ldi r22, 0x01
        s->r[22] = 1;
        return 8998;
    }
    case 0x2326: { // sts 0x0666, r22
        uint16_t address = 1638;
        pm_write(s, address, s->r[22]);
        return 9002;
    }
    case 0x232a: { // sts 0x08C3, r18
        uint16_t address = 2243;
        pm_write(s, address, s->r[18]);
        return 9006;
    }
    case 0x232e: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9010;
    }
    case 0x2332: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9014 : 9012;
    }
    case 0x2334: { // rjmp .-8
        return 9006;
    }
    case 0x2336: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9018;
    }
    case 0x233a: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9022;
    }
    case 0x233e: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9026 : 9024;
    }
    case 0x2340: { // rjmp .-8
        return 9018;
    }
    case 0x2342: { // sts 0x08C3, r17
        uint16_t address = 2243;
        pm_write(s, address, s->r[17]);
        return 9030;
    }
    case 0x2346: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9034;
    }
    case 0x234a: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9038 : 9036;
    }
    case 0x234c: { // rjmp .-8
        return 9030;
    }
    case 0x234e: { // sts 0x08C3, r16
        uint16_t address = 2243;
        pm_write(s, address, s->r[16]);
        return 9042;
    }
    case 0x2352: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9046;
    }
    case 0x2356: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9050 : 9048;
    }
    case 0x2358: { // rjmp .-8
        return 9042;
    }
    case 0x235a: { // ldi r22, 0x01
        s->r[22] = 1;
        return 9052;
    }
    case 0x235c: { // sts 0x0665, r22
        uint16_t address = 1637;
        pm_write(s, address, s->r[22]);
        return 9056;
    }
    case 0x2360: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 9058;
    }
    case 0x2362: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 9060;
    }
    case 0x2364: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9062;
    }
    case 0x2366: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
