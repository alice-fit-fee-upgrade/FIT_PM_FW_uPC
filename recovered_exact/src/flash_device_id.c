#include "legacy_spi_c.h"

void pm_flash_device_id(void) asm("FUN_code_000c20");
/* JEDEC ID bytes return in original R18, R19, R16 order. */
void pm_flash_device_id(void)
{
    PORTE_OUTCLR = 0x10;
    SPIE_DATA = 0x9f;
    register uint8_t dummy asm("r20");
    asm volatile("clr %0" : "=r" (dummy) : : "cc");
    PM_SPI_WAIT_AT(SPIE_STATUS, "r19");
    PM_SPI_SEND_VALUE(SPIE, "r19", dummy);
    PM_SPI_READ_REGISTER(SPIE, "r18");
    PM_SPI_SEND_VALUE(SPIE, "r19", dummy);
    PM_SPI_READ_REGISTER(SPIE, "r19");
    /* Final polling deliberately reuses and overwrites the dummy register. */
    SPIE_DATA = dummy;
    PM_SPI_WAIT_AT(SPIE_STATUS, "r20");
    PM_SPI_READ_REGISTER(SPIE, "r16");
    register uint8_t mask asm("r20") = 0x10;
    asm volatile("" : "+r" (mask));
    PORTE_OUTSET = mask;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 416 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_device_id(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1840: { // ldi r16, 0x10
        s->r[16] = 16;
        return 6210;
    }
    case 0x1842: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 6214;
    }
    case 0x1846: { // ldi r16, 0x9F
        s->r[16] = 159;
        return 6216;
    }
    case 0x1848: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 6220;
    }
    case 0x184c: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 6222;
    }
    case 0x184e: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6226;
    }
    case 0x1852: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6230 : 6228;
    }
    case 0x1854: { // rjmp .-8
        return 6222;
    }
    case 0x1856: { // sts 0x0AC3, r20
        uint16_t address = 2755;
        pm_write(s, address, s->r[20]);
        return 6234;
    }
    case 0x185a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6238;
    }
    case 0x185e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6242 : 6240;
    }
    case 0x1860: { // rjmp .-8
        return 6234;
    }
    case 0x1862: { // lds r18, 0x0AC3
        uint16_t address = 2755;
        s->r[18] = pm_read(s, address);
        return 6246;
    }
    case 0x1866: { // sts 0x0AC3, r20
        uint16_t address = 2755;
        pm_write(s, address, s->r[20]);
        return 6250;
    }
    case 0x186a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6254;
    }
    case 0x186e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6258 : 6256;
    }
    case 0x1870: { // rjmp .-8
        return 6250;
    }
    case 0x1872: { // lds r19, 0x0AC3
        uint16_t address = 2755;
        s->r[19] = pm_read(s, address);
        return 6262;
    }
    case 0x1876: { // sts 0x0AC3, r20
        uint16_t address = 2755;
        pm_write(s, address, s->r[20]);
        return 6266;
    }
    case 0x187a: { // lds r20, 0x0AC2
        uint16_t address = 2754;
        s->r[20] = pm_read(s, address);
        return 6270;
    }
    case 0x187e: { // sbrs r20, 7
        return (!!(s->r[20] & (1u << 7)) == 1) ? 6274 : 6272;
    }
    case 0x1880: { // rjmp .-8
        return 6266;
    }
    case 0x1882: { // lds r16, 0x0AC3
        uint16_t address = 2755;
        s->r[16] = pm_read(s, address);
        return 6278;
    }
    case 0x1886: { // ldi r20, 0x10
        s->r[20] = 16;
        return 6280;
    }
    case 0x1888: { // sts 0x0685, r20
        uint16_t address = 1669;
        pm_write(s, address, s->r[20]);
        return 6284;
    }
    case 0x188c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
