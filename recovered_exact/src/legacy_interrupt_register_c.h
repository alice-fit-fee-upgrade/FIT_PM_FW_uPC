#ifndef PM_LEGACY_INTERRUPT_REGISTER_C_H
#define PM_LEGACY_INTERRUPT_REGISTER_C_H
#include "legacy_interrupt.h"
/* Opt-in private R16 ISR frame. File-scope reservation prevents the compiler
 * from allocating saved SREG/R31 as an unrelated scratch. This is a register
 * object, not SRAM. The local profile must mark R31 call-saved to permit it;
 * PUSH/POP/RETI remain exact helpers. Every user requires whole-image matching. */
register uint8_t pm_interrupt_saved_status asm("r31");
#undef PM_ISR_ENTER_R16
#ifdef PM_ISR_EXACT_SREG_READ
/* C value: pm_interrupt_saved_status = SREG; retain original IN because
 * the full C read borrowed R16 before its PUSH (trial421), and pinning R16
 * prevented allocation (422). No standalone behavioral test is claimed.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
#define PM_ISR_ENTER_R16() \
    asm volatile("push r31\n\tin r31, 0x3f\n\tpush r16" \
        : "=r" (pm_interrupt_saved_status) : : "r16", "memory")
#else
#define PM_ISR_ENTER_R16() do { \
    asm volatile("push r31" : : : "memory"); \
    pm_interrupt_saved_status = SREG; \
    asm volatile("" : "+r" (pm_interrupt_saved_status) : : "memory"); \
    asm volatile("push r16" : : : "r16", "memory"); \
} while (0)
#endif
#undef PM_ISR_LEAVE_R16
#define PM_ISR_LEAVE_R16() do { \
    asm volatile("pop r16" : : : "memory"); \
    asm volatile("" : "=r" (pm_interrupt_saved_status) : : "memory"); \
    SREG = pm_interrupt_saved_status; \
    /* C operation: restore_private_frame_and_return_from_interrupt(); */ \
    asm volatile("pop r31\n\treti" : : : "memory"); \
    __builtin_unreachable(); \
} while (0)
#endif

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 800 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_legacy_interrupt_register_c(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0278: { // push r2
        s->stack[s->depth++] = s->r[2];
        return 634;
    }
    case 0x027a: { // push r10
        s->stack[s->depth++] = s->r[10];
        return 636;
    }
    case 0x028a: { // push r26
        s->stack[s->depth++] = s->r[26];
        return 652;
    }
    case 0x028c: { // push r27
        s->stack[s->depth++] = s->r[27];
        return 654;
    }
    case 0x043e: { // pop r27
        s->r[27] = s->stack[--s->depth];
        return 1088;
    }
    case 0x0440: { // pop r26
        s->r[26] = s->stack[--s->depth];
        return 1090;
    }
    case 0x0450: { // pop r10
        s->r[10] = s->stack[--s->depth];
        return 1106;
    }
    case 0x0452: { // pop r2
        s->r[2] = s->stack[--s->depth];
        return 1108;
    }
    case 0x0990: { // push r28
        s->stack[s->depth++] = s->r[28];
        return 2450;
    }
    case 0x0992: { // push r29
        s->stack[s->depth++] = s->r[29];
        return 2452;
    }
    case 0x09d2: { // pop r29
        s->r[29] = s->stack[--s->depth];
        return 2516;
    }
    case 0x09d4: { // pop r28
        s->r[28] = s->stack[--s->depth];
        return 2518;
    }
    case 0x09f6: { // push r1
        s->stack[s->depth++] = s->r[1];
        return 2552;
    }
    case 0x09f8: { // push r0
        s->stack[s->depth++] = s->r[0];
        return 2554;
    }
    case 0x0a2e: { // pop r0
        s->r[0] = s->stack[--s->depth];
        return 2608;
    }
    case 0x0a30: { // pop r1
        s->r[1] = s->stack[--s->depth];
        return 2610;
    }
    case 0x0abe: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 2752;
    }
    case 0x0ada: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 2780;
    }
    case 0x0ade: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    case 0x0de4: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 3558;
    }
    case 0x0e14: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 3606;
    }
    case 0x0f90: { // out 0x3d, r25
        pm_io_write(s, 61, s->r[25]);
        return 3986;
    }
    case 0x0f94: { // out 0x3e, r25
        pm_io_write(s, 62, s->r[25]);
        return 3990;
    }
    case 0x1bf0: { // in r17, 0x00
        s->r[17] = pm_io_read(s, 0);
        return 7154;
    }
    case 0x22ae: { // push r24
        s->stack[s->depth++] = s->r[24];
        return 8880;
    }
    case 0x2306: { // pop r24
        s->r[24] = s->stack[--s->depth];
        return 8968;
    }
    case 0x2310: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 8978;
    }
    case 0x2362: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 9060;
    }
    case 0x2486: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 9352;
    }
    case 0x24ca: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 9420;
    }
    case 0x24d0: { // push r23
        s->stack[s->depth++] = s->r[23];
        return 9426;
    }
    case 0x252a: { // pop r23
        s->r[23] = s->stack[--s->depth];
        return 9516;
    }
    case 0x2536: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 9528;
    }
    case 0x2538: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 9530;
    }
    case 0x258c: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 9614;
    }
    case 0x258e: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 9616;
    }
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
    case 0x2754: { // push r14
        s->stack[s->depth++] = s->r[14];
        return 10070;
    }
    case 0x2756: { // push r15
        s->stack[s->depth++] = s->r[15];
        return 10072;
    }
    case 0x2760: { // push r25
        s->stack[s->depth++] = s->r[25];
        return 10082;
    }
    case 0x280c: { // pop r25
        s->r[25] = s->stack[--s->depth];
        return 10254;
    }
    case 0x2816: { // pop r15
        s->r[15] = s->stack[--s->depth];
        return 10264;
    }
    case 0x2818: { // pop r14
        s->r[14] = s->stack[--s->depth];
        return 10266;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
