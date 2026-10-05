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
 * uint32_t pm_adt16_c(uint8_t command,uint16_t data)
 * {
 *     PORTA_OUTCLR=0x10;
 *     (void)pm_adt_byte_call(command);
 *     uint8_t high=(uint8_t)pm_adt_byte_call((uint8_t)(data>>8));
 *     uint32_t last=pm_adt_byte_call((uint8_t)data);
 *     PORTA_OUTSET=0x10;
 *     return (last&0x00ff0000UL) | ((uint32_t)high<<8) | (uint8_t)last;
 * }
 */

#define CS_REGISTER(address, reg) \
 do { register uint8_t mask asm(reg)=0x10; \
 asm volatile ("" : "+r" (mask)); \
 *(volatile uint8_t *)(address) = mask; } while (0)
#define TRANSFER(value) \
 asm volatile ("rcall adt7311_byte_rw" : "+r" (value) \
     : : "r17", "r18", "memory", "cc")
void adt7311_16bit_rw(void)
{
    register uint8_t command asm("r16");
    asm volatile ("" : "=r" (command));
    CS_REGISTER(_SFR_MEM_ADDR(PORTA_OUTCLR), "r20");
    register uint8_t tx_low asm("r17");
    register uint8_t tx_high asm("r18");
    asm volatile ("" : "=r" (tx_low), "=r" (tx_high));
    register uint8_t low asm("r20");
    register uint8_t high asm("r21");
    low = tx_low;
    asm volatile ("" : "+r" (low));
    high = tx_high;
    asm volatile ("" : "+r" (high));
    TRANSFER(command);
    command=high;
    TRANSFER(command);
    high=command;
    command=low;
    TRANSFER(command);
    low=command;
    CS_REGISTER(_SFR_MEM_ADDR(PORTA_OUTSET), "r18");
    asm volatile ("" : : "r" (low), "r" (high));
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 320 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_adt16(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x25be: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 9664;
    }
    case 0x25c0: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 9666;
    }
    case 0x25c2: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9668;
    }
    case 0x25c4: { // ldi r20, 0x10
        s->r[20] = 16;
        return 9670;
    }
    case 0x25c6: { // sts 0x0606, r20
        uint16_t address = 1542;
        pm_write(s, address, s->r[20]);
        return 9674;
    }
    case 0x25ca: { // mov r20, r17
        s->r[20] = s->r[17];
        return 9676;
    }
    case 0x25cc: { // mov r21, r18
        s->r[21] = s->r[18];
        return 9678;
    }
    case 0x25ce: { // rcall .+56
        s->calls[s->call_depth++] = 9680;
        return 9736;
    }
    case 0x25d0: { // mov r16, r21
        s->r[16] = s->r[21];
        return 9682;
    }
    case 0x25d2: { // rcall .+52
        s->calls[s->call_depth++] = 9684;
        return 9736;
    }
    case 0x25d4: { // mov r21, r16
        s->r[21] = s->r[16];
        return 9686;
    }
    case 0x25d6: { // mov r16, r20
        s->r[16] = s->r[20];
        return 9688;
    }
    case 0x25d8: { // rcall .+46
        s->calls[s->call_depth++] = 9690;
        return 9736;
    }
    case 0x25da: { // mov r20, r16
        s->r[20] = s->r[16];
        return 9692;
    }
    case 0x25dc: { // ldi r18, 0x10
        s->r[18] = 16;
        return 9694;
    }
    case 0x25de: { // sts 0x0605, r18
        uint16_t address = 1541;
        pm_write(s, address, s->r[18]);
        return 9698;
    }
    case 0x25e2: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9700;
    }
    case 0x25e4: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 9702;
    }
    case 0x25e6: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 9704;
    }
    case 0x25e8: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
