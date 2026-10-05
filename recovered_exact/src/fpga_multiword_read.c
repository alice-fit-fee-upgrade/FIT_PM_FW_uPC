#include "legacy_spi_c.h"

void pm_fpga_multiword_read(void) asm("FUN_code_0011e5");
/* Original eight-byte response is returned in R9:R8 ... R15:R14. */
void pm_fpga_multiword_read(void)
{
    PM_WRITE_R22(SPIC_CTRL, 0xd1);
    register uint8_t address asm("r18") = 0xbc;
    register uint8_t dummy asm("r21");
    asm volatile("clr %0" : "=r" (dummy) : "r" (address) : "cc");
    PM_WRITE_R22(PORTD_OUTCLR, 1);
    PM_SPI_SEND_VALUE(SPIC, "r22", address);
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r9");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r8");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r11");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r10");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r13");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r12");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r15");
    PM_SPI_SEND_VALUE(SPIC, "r22", dummy);
    PM_SPI_READ_REGISTER(SPIC, "r14");
    PM_WRITE_R22(PORTD_OUTSET, 1);
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 1008 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_fpga_multiword_read(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x23ca: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9164;
    }
    case 0x23cc: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 9166;
    }
    case 0x23ce: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 9168;
    }
    case 0x23d0: { // ldi r22, 0xD1
        s->r[22] = 209;
        return 9170;
    }
    case 0x23d2: { // sts 0x08C0, r22
        uint16_t address = 2240;
        pm_write(s, address, s->r[22]);
        return 9174;
    }
    case 0x23d6: { // ldi r18, 0xBC
        s->r[18] = 188;
        return 9176;
    }
    case 0x23d8: { // eor r21, r21
        s->r[21] ^= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 9178;
    }
    case 0x23da: { // ldi r22, 0x01
        s->r[22] = 1;
        return 9180;
    }
    case 0x23dc: { // sts 0x0666, r22
        uint16_t address = 1638;
        pm_write(s, address, s->r[22]);
        return 9184;
    }
    case 0x23e0: { // sts 0x08C3, r18
        uint16_t address = 2243;
        pm_write(s, address, s->r[18]);
        return 9188;
    }
    case 0x23e4: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9192;
    }
    case 0x23e8: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9196 : 9194;
    }
    case 0x23ea: { // rjmp .-8
        return 9188;
    }
    case 0x23ec: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9200;
    }
    case 0x23f0: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9204;
    }
    case 0x23f4: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9208 : 9206;
    }
    case 0x23f6: { // rjmp .-8
        return 9200;
    }
    case 0x23f8: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9212;
    }
    case 0x23fc: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9216;
    }
    case 0x2400: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9220 : 9218;
    }
    case 0x2402: { // rjmp .-8
        return 9212;
    }
    case 0x2404: { // lds r9, 0x08C3
        uint16_t address = 2243;
        s->r[9] = pm_read(s, address);
        return 9224;
    }
    case 0x2408: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9228;
    }
    case 0x240c: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9232;
    }
    case 0x2410: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9236 : 9234;
    }
    case 0x2412: { // rjmp .-8
        return 9228;
    }
    case 0x2414: { // lds r8, 0x08C3
        uint16_t address = 2243;
        s->r[8] = pm_read(s, address);
        return 9240;
    }
    case 0x2418: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9244;
    }
    case 0x241c: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9248;
    }
    case 0x2420: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9252 : 9250;
    }
    case 0x2422: { // rjmp .-8
        return 9244;
    }
    case 0x2424: { // lds r11, 0x08C3
        uint16_t address = 2243;
        s->r[11] = pm_read(s, address);
        return 9256;
    }
    case 0x2428: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9260;
    }
    case 0x242c: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9264;
    }
    case 0x2430: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9268 : 9266;
    }
    case 0x2432: { // rjmp .-8
        return 9260;
    }
    case 0x2434: { // lds r10, 0x08C3
        uint16_t address = 2243;
        s->r[10] = pm_read(s, address);
        return 9272;
    }
    case 0x2438: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9276;
    }
    case 0x243c: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9280;
    }
    case 0x2440: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9284 : 9282;
    }
    case 0x2442: { // rjmp .-8
        return 9276;
    }
    case 0x2444: { // lds r13, 0x08C3
        uint16_t address = 2243;
        s->r[13] = pm_read(s, address);
        return 9288;
    }
    case 0x2448: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9292;
    }
    case 0x244c: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9296;
    }
    case 0x2450: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9300 : 9298;
    }
    case 0x2452: { // rjmp .-8
        return 9292;
    }
    case 0x2454: { // lds r12, 0x08C3
        uint16_t address = 2243;
        s->r[12] = pm_read(s, address);
        return 9304;
    }
    case 0x2458: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9308;
    }
    case 0x245c: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9312;
    }
    case 0x2460: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9316 : 9314;
    }
    case 0x2462: { // rjmp .-8
        return 9308;
    }
    case 0x2464: { // lds r15, 0x08C3
        uint16_t address = 2243;
        s->r[15] = pm_read(s, address);
        return 9320;
    }
    case 0x2468: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9324;
    }
    case 0x246c: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9328;
    }
    case 0x2470: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9332 : 9330;
    }
    case 0x2472: { // rjmp .-8
        return 9324;
    }
    case 0x2474: { // lds r14, 0x08C3
        uint16_t address = 2243;
        s->r[14] = pm_read(s, address);
        return 9336;
    }
    case 0x2478: { // ldi r22, 0x01
        s->r[22] = 1;
        return 9338;
    }
    case 0x247a: { // sts 0x0665, r22
        uint16_t address = 1637;
        pm_write(s, address, s->r[22]);
        return 9342;
    }
    case 0x247e: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 9344;
    }
    case 0x2480: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 9346;
    }
    case 0x2482: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9348;
    }
    case 0x2484: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
