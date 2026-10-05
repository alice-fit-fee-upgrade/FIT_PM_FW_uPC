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
 * void pm_system_deinit_c(void)
 * {
 *     REG8(PM_FPGA_TIMER_LOW)=0; REG8(PM_FPGA_TIMER_HIGH)=0;
 *     REG8(PM_FPGA_STATE)=0; REG8(PM_THS_STATE)=0;
 *     REG8(PM_RESTART_REASON)=0; REG8(PM_FPGA_REQUEST)=0; REG8(PM_CLOCK_STATE)=0;
 *     REG8(PM_PORTE_INTCTRL)=1; REG8(PM_PORTF_INTCTRL)=2;
 *     REG8(PM_PORTB_INTCTRL)=0; REG8(PM_PORTD_INTCTRL)=0;
 *     REG8(PM_PORTB_DIRCLR)=0xbf; REG8(PM_SPIC_CTRL)=0;
 *     REG8(PM_PORTC_DIRCLR)=0xff; REG8(PM_PORTD_DIRCLR)=0x41;
 *     REG8(PM_PORTD_OUTCLR)=4; REG8(PM_PORTF_OUTCLR)=0x20;
 *     REG8(PM_PORTF_DIRCLR)=0x20;
 *     REG8(PM_STATUS_FLAGS) &= 0xefu;
 *     REG8(PM_PORTA_OUTSET)=0xa0; REG8(PM_SPID_CTRL)=0; REG8(PM_DMA_CTRL)=0;
 * }
 */

#define RAM(a) (*(volatile uint8_t *)(a))
/* Steps451/456 rejected C scratch/read alternatives. C value equivalent:
 * *(volatile uint8_t *)0x2157; no standalone functional validation.
 * Original R16 helper and constant/flag contracts stay exact. */
#include "legacy_r16.h"
void system_deinit(void)
{
    uint8_t zero = pm_scratch_zero();
    RAM(0x215c)=zero; RAM(0x215d)=zero; RAM(0x215b)=zero;
    RAM(0x2159)=zero; RAM(0x2441)=zero; RAM(0x2006)=zero; RAM(0x2162)=zero;
    PORTE_INTCTRL=1; PORTF_INTCTRL=2;
    zero=pm_scratch_constant(0);
    PORTB_INTCTRL=zero; PORTD_INTCTRL=zero;
    PORTB_DIRCLR=0xbf;
    SPIC_CTRL=pm_scratch_zero();
    PORTC_DIRCLR=0xff; PORTD_DIRCLR=0x41; PORTD_OUTCLR=4;
    PORTF_OUTCLR=0x20; PORTF_DIRCLR=pm_scratch_constant(0x20);
    RAM(0x2157)=pm_read_absolute(0x2157)&0xef;
    PORTA_OUTSET=0xa0;
    zero=pm_scratch_zero();
    SPID_CTRL=zero; DMA_CTRL=zero;
}
