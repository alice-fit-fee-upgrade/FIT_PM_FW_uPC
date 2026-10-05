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
 * prevented allocation (422). No standalone behavioral test is claimed. */
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
