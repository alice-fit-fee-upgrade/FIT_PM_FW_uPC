#include <avr/io.h>

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_flash_init_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/flash_spi.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * void pm_flash_spi_init(void)
 * {
 *     PORTD_OUTCLR = 2;
 *     PORTE_OUTSET = 0x10;
 *     PORTE_DIRSET = 0xb0;
 *     SPIE_CTRL = 0x50;
 * }
 */

/* Original entry 0x1664; legacy scratch/output register is R16.
 * Per-file compiler flags restrict temporaries to that register. */
void pm_flash_interface_init(void) asm("FUN_code_000b32");
void pm_flash_interface_init(void)
{
    PORTD_OUTCLR = 0x02;
    PORTE_OUTSET = 0x10;
    PORTE_DIRSET = 0xb0;
    SPIE_CTRL = 0x50;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 144 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_interface_init(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1664: { // ldi r16, 0x02
        s->r[16] = 2;
        return 5734;
    }
    case 0x1666: { // sts 0x0666, r16
        uint16_t address = 1638;
        pm_write(s, address, s->r[16]);
        return 5738;
    }
    case 0x166a: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5740;
    }
    case 0x166c: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5744;
    }
    case 0x1670: { // ldi r16, 0xB0
        s->r[16] = 176;
        return 5746;
    }
    case 0x1672: { // sts 0x0681, r16
        uint16_t address = 1665;
        pm_write(s, address, s->r[16]);
        return 5750;
    }
    case 0x1676: { // ldi r16, 0x50
        s->r[16] = 80;
        return 5752;
    }
    case 0x1678: { // sts 0x0AC0, r16
        uint16_t address = 2752;
        pm_write(s, address, s->r[16]);
        return 5756;
    }
    case 0x167c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
