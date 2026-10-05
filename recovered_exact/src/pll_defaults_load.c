/* Calls retain private fixed-register inputs/results through zero-byte
 * barriers. Void declarations deliberately introduce no GNU arguments/results;
 * recapture after each call observes the original registers. Every conversion
 * is accepted only if GNU CALL, frames and the entire FLASH remain identical. */
extern void CDCE62005_send_control_settings(void);
#include "legacy_cpu.h"
#include <avr/io.h>

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_system_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/system_control.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * void pm_pll_control_reset_c(void)
 * {
 *     uint16_t address=0x2916;
 *     for (uint8_t remaining=10; remaining!=0; --remaining) {
 *         uint8_t b0=pgm_read_byte(address++);
 *         uint8_t b1=pgm_read_byte(address++);
 *         uint8_t b2=pgm_read_byte(address++);
 *         uint8_t b3=pgm_read_byte(address++);
 *         uint32_t value=(uint32_t)b0 | ((uint32_t)b1<<8)
 *                      | ((uint32_t)b2<<16) | ((uint32_t)b3<<24);
 *         pm_pll_write_c(value);
 *     }
 *     PORTF_INTCTRL=0x0a;
 *     PORTB_OUTCLR=0x20;
 * }
 */

void CDCE62005_control_rst(void)
{
    register const uint8_t *cursor asm("r30") = (const uint8_t *)0x2916;
    asm volatile("" : "+z" (cursor));
    register uint8_t remaining asm("r20") = 10;
next_setting:
    {
        register uint8_t b0 asm("r16"), b1 asm("r17"), b2 asm("r18"), b3 asm("r19");
        /* C FLASH read equivalent with const __flash uint8_t *cursor:
         * b0 = *cursor++; b1 = *cursor++; b2 = *cursor++; b3 = *cursor++;
         * Trials 239-249 could not reproduce the private pointer/register layout;
         * keep exact LPM Z+ instructions. This is explanatory, with no new
         * independent functional-test claim. Existing historical test scope,
         * when available, is documented above. */
        asm volatile("lpm %0, Z+\n\tlpm %1, Z+\n\tlpm %2, Z+\n\tlpm %3, Z+"
            : "=r" (b0), "=r" (b1), "=r" (b2), "=r" (b3), "+z" (cursor)
            : "r" (remaining) : "memory");
        /* Retain CALL rather than RCALL and the original interrupt window. */
        asm volatile("" : "+r" (b0), "+r" (b1), "+r" (b2), "+r" (b3) : : "memory");
        pm_cpu_disable_irq();
        do {
        asm volatile("" : "+r" (b0), "+r" (b1), "+r" (b2), "+r" (b3) :  : "memory");
        CDCE62005_send_control_settings();
        asm volatile("" : "=r" (b0), "=r" (b1), "=r" (b2), "=r" (b3) : : "memory");
    } while (0);
        pm_cpu_enable_irq();
    }
    asm volatile("dec %0" : "+r" (remaining) : : "cc");
    asm goto("brne %l[next_setting]" : : "r" (remaining) : : next_setting);
    register uint8_t mask asm("r16") = 0x0a;
    asm volatile("" : "+r" (mask));
    PORTF_INTCTRL = mask;
    mask = 0x20;
    asm volatile("" : "+r" (mask));
    PORTB_OUTCLR = mask;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 496 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_pll_defaults_load(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0ddc: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 3550;
    }
    case 0x0dde: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 3552;
    }
    case 0x0de0: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 3554;
    }
    case 0x0de2: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 3556;
    }
    case 0x0de4: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 3558;
    }
    case 0x0de6: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 3560;
    }
    case 0x0de8: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 3562;
    }
    case 0x0dea: { // ldi r30, 0x16
        s->r[30] = 22;
        return 3564;
    }
    case 0x0dec: { // ldi r31, 0x29
        s->r[31] = 41;
        return 3566;
    }
    case 0x0dee: { // ldi r20, 0x0A
        s->r[20] = 10;
        return 3568;
    }
    case 0x0df0: { // lpm r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_golden_flash[address];
        return 3570;
    }
    case 0x0df2: { // lpm r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_golden_flash[address];
        return 3572;
    }
    case 0x0df4: { // lpm r18, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[18] = pm_golden_flash[address];
        return 3574;
    }
    case 0x0df6: { // lpm r19, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[19] = pm_golden_flash[address];
        return 3576;
    }
    case 0x0df8: { // cli
        pm_irq(s, false);
        return 3578;
    }
    case 0x0dfa: { // call 0x2486
        s->calls[s->call_depth++] = 3582;
        return 9350;
    }
    case 0x0dfe: { // sei
        pm_irq(s, true);
        return 3584;
    }
    case 0x0e00: { // dec r20
        s->r[20]--;
        pm_nzv(s, s->r[20], s->r[20] == 127);
        return 3586;
    }
    case 0x0e02: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 3568 : 3588;
    }
    case 0x0e04: { // ldi r16, 0x0A
        s->r[16] = 10;
        return 3590;
    }
    case 0x0e06: { // sts 0x06A9, r16
        uint16_t address = 1705;
        pm_write(s, address, s->r[16]);
        return 3594;
    }
    case 0x0e0a: { // ldi r16, 0x20
        s->r[16] = 32;
        return 3596;
    }
    case 0x0e0c: { // sts 0x0626, r16
        uint16_t address = 1574;
        pm_write(s, address, s->r[16]);
        return 3600;
    }
    case 0x0e10: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3602;
    }
    case 0x0e12: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 3604;
    }
    case 0x0e14: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 3606;
    }
    case 0x0e16: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 3608;
    }
    case 0x0e18: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 3610;
    }
    case 0x0e1a: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 3612;
    }
    case 0x0e1c: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 3614;
    }
    case 0x0e1e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
