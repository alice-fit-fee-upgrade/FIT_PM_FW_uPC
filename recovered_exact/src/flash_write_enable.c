#include "legacy_spi_c.h"
#include <avr/io.h>
/* Archived C alternative for the exact implementation below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_flash_enable_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/flash_spi.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint8_t pm_flash_write_enable(void)
 * {
 *     PORTE_OUTCLR = 0x10;
 *     SPIE_DATA = 6;
 *     uint8_t status;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     PORTE_OUTSET = 0x10;
 *     return status;
 * }
 */

/* Exact polling fragment; reserve legacy R16 across the helper so the
 * following C write reloads its historical scratch value. */
static inline void wait_spie_complete(void)
{
    PM_SPI_WAIT_AT(SPIE_STATUS, "r19");
    /* The original helper also invalidates the R16 constant, forcing reload. */
    asm volatile("" : : : "r16", "memory");
}
void pm_flash_write_enable(void) asm("FUN_code_000b8f");
void pm_flash_write_enable(void)
{
    PORTE_OUTCLR = 0x10;
    SPIE_DATA = 0x06;
    wait_spie_complete();
    PORTE_OUTSET = 0x10;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 160 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_write_enable(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x171e: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5920;
    }
    case 0x1720: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 5924;
    }
    case 0x1724: { // ldi r16, 0x06
        s->r[16] = 6;
        return 5926;
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
    case 0x1732: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5940;
    }
    case 0x1734: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5944;
    }
    case 0x1738: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
