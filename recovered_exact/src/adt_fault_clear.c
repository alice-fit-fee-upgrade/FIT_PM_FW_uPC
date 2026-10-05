#include <avr/io.h>

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_reads_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/device_reads.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint32_t pm_adt_faults_c(void)
 * {
 *     PORTA_OUTCLR=0x10;
 *     (void)pm_adt_byte_call(0xff);
 *     (void)pm_adt_byte_call(0xff);
 *     (void)pm_adt_byte_call(0xff);
 *     uint32_t packet=pm_adt_byte_call(0xff);
 *     PORTA_OUTSET=0x10;
 *     return packet;
 * }
 */

/* No GNU argument call: byte transfer uses the original R16/17/18 contract. */
#define PM_ADT_SEND_BYTE(byte) \
 do { register uint8_t value asm("r16")=(byte); \
 asm volatile ("rcall adt7311_byte_rw" : "+r" (value) \
     : : "r17", "r18", "memory", "cc"); } while (0)

void adt7311_faults_clr(void)
{
    PORTA_OUTCLR=0x10;
    PM_ADT_SEND_BYTE(0xff);
    PM_ADT_SEND_BYTE(0xff);
    PM_ADT_SEND_BYTE(0xff);
    PM_ADT_SEND_BYTE(0xff);
    PORTA_OUTSET=0x10;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 208 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_adt_fault_clear(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x25ea: { // ldi r16, 0x10
        s->r[16] = 16;
        return 9708;
    }
    case 0x25ec: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 9712;
    }
    case 0x25f0: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 9714;
    }
    case 0x25f2: { // rcall .+20
        s->calls[s->call_depth++] = 9716;
        return 9736;
    }
    case 0x25f4: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 9718;
    }
    case 0x25f6: { // rcall .+16
        s->calls[s->call_depth++] = 9720;
        return 9736;
    }
    case 0x25f8: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 9722;
    }
    case 0x25fa: { // rcall .+12
        s->calls[s->call_depth++] = 9724;
        return 9736;
    }
    case 0x25fc: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 9726;
    }
    case 0x25fe: { // rcall .+8
        s->calls[s->call_depth++] = 9728;
        return 9736;
    }
    case 0x2600: { // ldi r16, 0x10
        s->r[16] = 16;
        return 9730;
    }
    case 0x2602: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 9734;
    }
    case 0x2606: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
