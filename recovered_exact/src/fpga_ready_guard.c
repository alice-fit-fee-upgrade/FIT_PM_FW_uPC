#include <stdint.h>
/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_status_gate_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/status_gate.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint8_t pm_status_gate_allowed(uint8_t status)
 * {
 *     return (uint8_t)((status & 0x10u) != 0);
 * }
 */

#define RAM8(address) (*(volatile uint8_t *)(address))

/* Carry is the result: clear means ready, set means the error was printed. */
void FUN_code_00108e(void)
{
    asm volatile("clc" : : : "cc");
    register uint8_t status asm("r16") = RAM8(0x2157);
    /* Step474 plain C `if (status & 0x10u) return;` changed the binary.
     * No new standalone validation; retain the exact skip and carry result. */
    asm volatile("sbrc %0, 4\n\tret" : : "r" (status));
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2ab4;
    asm volatile("rcall cli_send_msg\n\tsec" : "+z" (message) : : "memory", "cc");
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
uint32_t pm_logical_fpga_ready_guard(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x211c: { // clc
        pm_flag(s, CARRY, false);
        return 8478;
    }
    case 0x211e: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 8482;
    }
    case 0x2122: { // sbrc r16, 4
        return (!!(s->r[16] & (1u << 4)) == 0) ? 8486 : 8484;
    }
    case 0x2124: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x2126: { // ldi r30, 0xB4
        s->r[30] = 180;
        return 8488;
    }
    case 0x2128: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 8490;
    }
    case 0x212a: { // rcall .+1786
        s->calls[s->call_depth++] = 8492;
        return 10278;
    }
    case 0x212c: { // sec
        pm_flag(s, CARRY, true);
        return 8494;
    }
    case 0x212e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
