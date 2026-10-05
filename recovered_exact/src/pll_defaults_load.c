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
        asm volatile("lpm %0, Z+\n\tlpm %1, Z+\n\tlpm %2, Z+\n\tlpm %3, Z+"
            : "=r" (b0), "=r" (b1), "=r" (b2), "=r" (b3), "+z" (cursor)
            : "r" (remaining) : "memory");
        /* Retain CALL rather than RCALL and the original interrupt window. */
        asm volatile("cli\n\tcall CDCE62005_send_control_settings\n\tsei"
            : "+r" (b0), "+r" (b1), "+r" (b2), "+r" (b3) : : "memory", "cc");
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
