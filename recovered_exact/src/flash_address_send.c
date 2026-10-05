#include "legacy_spi_c.h"

/* Archived C alternative for the exact implementation below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_flash_address_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/flash_spi.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint8_t pm_flash_send_address(uint8_t command, uint8_t high,
 *                               uint8_t middle, uint8_t low)
 * {
 *     uint8_t status;
 *     PORTE_OUTCLR = 0x10;
 *     SPIE_DATA = command;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     SPIE_DATA = high;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     SPIE_DATA = middle;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     SPIE_DATA = low;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     return status;
 * }
 */

void pm_flash_address_send(void) asm("FUN_code_000c04");
void pm_flash_address_send(void)
{
    PORTE_OUTCLR = 0x10;
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r16"); /* Command. */
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r22"); /* Address high byte. */
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r21");
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r20"); /* Address low byte. */
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 304 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_address_send(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1808: { // ldi r17, 0x10
        s->r[17] = 16;
        return 6154;
    }
    case 0x180a: { // sts 0x0686, r17
        uint16_t address = 1670;
        pm_write(s, address, s->r[17]);
        return 6158;
    }
    case 0x180e: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 6162;
    }
    case 0x1812: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6166;
    }
    case 0x1816: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6170 : 6168;
    }
    case 0x1818: { // rjmp .-8
        return 6162;
    }
    case 0x181a: { // sts 0x0AC3, r22
        uint16_t address = 2755;
        pm_write(s, address, s->r[22]);
        return 6174;
    }
    case 0x181e: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6178;
    }
    case 0x1822: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6182 : 6180;
    }
    case 0x1824: { // rjmp .-8
        return 6174;
    }
    case 0x1826: { // sts 0x0AC3, r21
        uint16_t address = 2755;
        pm_write(s, address, s->r[21]);
        return 6186;
    }
    case 0x182a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6190;
    }
    case 0x182e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6194 : 6192;
    }
    case 0x1830: { // rjmp .-8
        return 6186;
    }
    case 0x1832: { // sts 0x0AC3, r20
        uint16_t address = 2755;
        pm_write(s, address, s->r[20]);
        return 6198;
    }
    case 0x1836: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6202;
    }
    case 0x183a: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6206 : 6204;
    }
    case 0x183c: { // rjmp .-8
        return 6198;
    }
    case 0x183e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
