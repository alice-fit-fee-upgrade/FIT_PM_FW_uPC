#include "legacy_spi_c.h"

/* R22 is the sole allocatable scratch in this entry's original save frame.
 * The value need not stay live after polling: the next instruction replaces it.
 * Keeping it live there makes GCC 7.3 require another register for the test. */
#undef PM_SPI_SEND_AT
#define PM_SPI_SEND_AT(data_register, status_register, scratch, value) do { \
    register uint8_t status asm(scratch); \
    (data_register) = (value); \
    do { \
        status = (status_register); \
        asm volatile("" : "+r" (status)); \
    } while (!(status & 0x80u)); \
} while (0)

/* Legacy caller supplies the little-endian control word in R16..R19. */
void CDCE62005_send_control_settings(void)
{
    SPIC_CTRL = 0xf1;
    PORTF_OUTCLR = 0x10;
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r18");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r19");
    PORTF_OUTSET = 0x10;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 400 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_pll_control_write(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2486: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 9352;
    }
    case 0x2488: { // ldi r22, 0xF1
        s->r[22] = 241;
        return 9354;
    }
    case 0x248a: { // sts 0x08C0, r22
        uint16_t address = 2240;
        pm_write(s, address, s->r[22]);
        return 9358;
    }
    case 0x248e: { // ldi r22, 0x10
        s->r[22] = 16;
        return 9360;
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
    case 0x249e: { // rjmp .-8
        return 9368;
    }
    case 0x24a0: { // sts 0x08C3, r17
        uint16_t address = 2243;
        pm_write(s, address, s->r[17]);
        return 9380;
    }
    case 0x24a4: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9384;
    }
    case 0x24a8: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9388 : 9386;
    }
    case 0x24aa: { // rjmp .-8
        return 9380;
    }
    case 0x24ac: { // sts 0x08C3, r18
        uint16_t address = 2243;
        pm_write(s, address, s->r[18]);
        return 9392;
    }
    case 0x24b0: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9396;
    }
    case 0x24b4: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9400 : 9398;
    }
    case 0x24b6: { // rjmp .-8
        return 9392;
    }
    case 0x24b8: { // sts 0x08C3, r19
        uint16_t address = 2243;
        pm_write(s, address, s->r[19]);
        return 9404;
    }
    case 0x24bc: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9408;
    }
    case 0x24c0: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9412 : 9410;
    }
    case 0x24c2: { // rjmp .-8
        return 9404;
    }
    case 0x24c4: { // ldi r22, 0x10
        s->r[22] = 16;
        return 9414;
    }
    case 0x24c6: { // sts 0x06A5, r22
        uint16_t address = 1701;
        pm_write(s, address, s->r[22]);
        return 9418;
    }
    case 0x24ca: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 9420;
    }
    case 0x24cc: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
