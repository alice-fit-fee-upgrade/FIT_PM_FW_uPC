#include "legacy_spi_c.h"

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_flash_wait_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/flash_spi.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 * For flash_wait_ready only the SPI status transaction is described; the delay
 * and outer busy loop remained ASM in the historical tested implementation.
 *
 * uint16_t pm_flash_read_status_transaction(void)
 * {
 *     uint8_t status;
 *     PORTE_OUTCLR = 0x10;
 *     SPIE_DATA = 5;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     SPIE_DATA = 0;
 *     do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
 *     uint8_t data = SPIE_DATA;
 *     PORTE_OUTSET = 0x10;
 *     return (uint16_t)(((uint16_t)status << 8) | data);
 * }
 */

void pm_flash_wait_ready(void) asm("FUN_code_000b9d");
void pm_flash_wait_ready(void)
{
retry:
    {
        register uint16_t delay asm("r24") = 0x0640;
        /* Unvalidated C equivalent:
         * do { compiler_memory_barrier(); --delay; } while (delay);
         * Steps488/537/538 failed allocation/layout, including O1 and a
         * global R25:R24 reservation. Keep exact SBIW/BRNE and delay flags.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
        asm volatile("1: sbiw %0, 1\n\tbrne 1b" : "+w" (delay) : : "cc");
    }
    register uint8_t command asm("r16") = 0x10;
    asm volatile("" : "+r" (command));
    PORTE_OUTCLR = command;
    command = 5;
    asm volatile("" : "+r" (command));
    SPIE_DATA = command;
    PM_SPI_WAIT_AT(SPIE_STATUS, "r19");
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r24");
    register uint8_t status asm("r24");
    status = SPIE_DATA;
    asm volatile("" : "+r" (status));
    command = 0x10;
    asm volatile("" : "+r" (command));
    PORTE_OUTSET = command;
    if (status & (1u << 0)) goto retry;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 336 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_wait_ready(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x173a: { // ldi r24, 0x40
        s->r[24] = 64;
        return 5948;
    }
    case 0x173c: { // ldi r25, 0x06
        s->r[25] = 6;
        return 5950;
    }
    case 0x173e: { // sbiw r24, 0x01
        uint16_t old = pm_pointer(s, 24);
        uint16_t value = old - 1;
        pm_setpointer(s, 24, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 5952;
    }
    case 0x1740: { // brne .-4
        return (pm_getflag(s, 1) == 0) ? 5950 : 5954;
    }
    case 0x1742: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5956;
    }
    case 0x1744: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 5960;
    }
    case 0x1748: { // ldi r16, 0x05
        s->r[16] = 5;
        return 5962;
    }
    case 0x174a: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 5966;
    }
    case 0x174e: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 5970;
    }
    case 0x1752: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 5974 : 5972;
    }
    case 0x1754: { // rjmp .-8
        return 5966;
    }
    case 0x1756: { // sts 0x0AC3, r24
        uint16_t address = 2755;
        pm_write(s, address, s->r[24]);
        return 5978;
    }
    case 0x175a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 5982;
    }
    case 0x175e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 5986 : 5984;
    }
    case 0x1760: { // rjmp .-8
        return 5978;
    }
    case 0x1762: { // lds r24, 0x0AC3
        uint16_t address = 2755;
        s->r[24] = pm_read(s, address);
        return 5990;
    }
    case 0x1766: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5992;
    }
    case 0x1768: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5996;
    }
    case 0x176c: { // sbrc r24, 0
        return (!!(s->r[24] & (1u << 0)) == 0) ? 6000 : 5998;
    }
    case 0x176e: { // rjmp .-54
        return 5946;
    }
    case 0x1770: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
