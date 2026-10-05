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
 * uint32_t pm_adt8_c(uint8_t command,uint8_t data)
 * {
 *     PORTA_OUTCLR=0x10;
 *     (void)pm_adt_byte_call(command);
 *     uint32_t packet=pm_adt_byte_call(data);
 *     PORTA_OUTSET=0x10;
 *     return packet; // Original 8-bit entry discards RX and preserves all GPRs.
 * }
 */

#define chip_select(address) \
 do { register uint8_t mask asm("r18")=0x10; \
 asm volatile ("" : "+r" (mask)); \
 *(volatile uint8_t *)(address) = mask; } while (0)
void adt7311_8bit_rw(void)
{
    register uint8_t command asm("r16");
    asm volatile ("" : "=r" (command));
    chip_select(_SFR_MEM_ADDR(PORTA_OUTCLR));
    register uint8_t input_data asm("r17");
    asm volatile ("" : "=r" (input_data));
    register uint8_t data asm("r19");
    data = input_data;
    asm volatile ("" : "+r" (data));
    asm volatile ("rcall adt7311_byte_rw" : "+r" (command)
        : : "r17", "r18", "memory", "cc");
    command=data;
    asm volatile ("rcall adt7311_byte_rw" : "+r" (command)
        : : "r17", "r18", "memory", "cc");
    chip_select(_SFR_MEM_ADDR(PORTA_OUTSET));
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 272 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_adt8(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2598: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 9626;
    }
    case 0x259a: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 9628;
    }
    case 0x259c: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9630;
    }
    case 0x259e: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 9632;
    }
    case 0x25a0: { // ldi r18, 0x10
        s->r[18] = 16;
        return 9634;
    }
    case 0x25a2: { // sts 0x0606, r18
        uint16_t address = 1542;
        pm_write(s, address, s->r[18]);
        return 9638;
    }
    case 0x25a6: { // mov r19, r17
        s->r[19] = s->r[17];
        return 9640;
    }
    case 0x25a8: { // rcall .+94
        s->calls[s->call_depth++] = 9642;
        return 9736;
    }
    case 0x25aa: { // mov r16, r19
        s->r[16] = s->r[19];
        return 9644;
    }
    case 0x25ac: { // rcall .+90
        s->calls[s->call_depth++] = 9646;
        return 9736;
    }
    case 0x25ae: { // ldi r18, 0x10
        s->r[18] = 16;
        return 9648;
    }
    case 0x25b0: { // sts 0x0605, r18
        uint16_t address = 1541;
        pm_write(s, address, s->r[18]);
        return 9652;
    }
    case 0x25b4: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 9654;
    }
    case 0x25b6: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9656;
    }
    case 0x25b8: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 9658;
    }
    case 0x25ba: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 9660;
    }
    case 0x25bc: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
