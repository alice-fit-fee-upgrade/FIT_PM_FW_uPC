#ifndef PM_LEGACY_INTERRUPT_H
#define PM_LEGACY_INTERRUPT_H
#include <avr/io.h>
/* Private legacy ISR frame: called only by the original vector table.
 * Compiler profile must reserve R31 and allow R16 without a GNU prologue. */
#define PM_ISR_ENTER_R16() \
    asm volatile("push r31\n\tin r31, 0x3f\n\tpush r16" \
                 : : : "r16", "r31", "memory")
#define PM_ISR_LEAVE_R16() do { \
    asm volatile("pop r16\n\tout 0x3f, r31\n\tpop r31\n\treti" : : : "memory"); \
    __builtin_unreachable(); \
} while (0)
#define PM_RAM8(address) (*(volatile uint8_t *)(address))
#endif
