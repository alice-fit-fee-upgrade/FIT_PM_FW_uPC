/* Private void entry; bound-register barriers preserve the original ABI. */
extern void fpga_msg_send_t2(void);
#include "legacy_cpu.h"
#include <avr/io.h>
/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_settings_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/fpga_settings.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * void pm_fpga_settings_reset_c(void)
 * {
 *     uint16_t address = 0x21cf;
 *     for (uint8_t reg = 0x80; reg != 0xb0; ++reg, address += 2)
 *         pm_fpga_settings_write(reg, word_at(address));
 *     address = 0x2163;
 *     for (uint8_t reg = 0xb0; reg != 0xbc; ++reg, address += 2)
 *         pm_fpga_settings_write(reg, word_at(address));
 *     pm_fpga_settings_write(0xbe, RAM8(0x2441));
 * }
 */

#define RAM8(address) (*(volatile uint8_t *)(address))
#define SEND_SETTING(lo, hi, address) do { \
    asm volatile("" : "+r" (lo), "+r" (hi), "+r" (address) : : "memory"); \
    pm_cpu_disable_irq(); \
    fpga_msg_send_t2(); \
    asm volatile("" : "=r" (lo), "=r" (hi), "=r" (address) : : "memory"); \
    pm_cpu_enable_irq(); \
} while (0)

void fpga_settings_reset(void)
{
    register uint8_t address asm("r18") = 0x80;
    asm volatile("" : "+r" (address));
    register const uint8_t *cursor asm("r28") = (const uint8_t *)0x21cf;
    asm volatile("" : "+y" (cursor));
first_bank:
    {
        register uint8_t lo asm("r16"), hi asm("r17");
        asm volatile("ld %0, Y+\n\tld %1, Y+" : "=r" (lo), "=r" (hi), "+y" (cursor) : : "memory");
        SEND_SETTING(lo, hi, address);
    }
    asm volatile("inc %0" : "+r" (address) : : "cc");
    if (address != 0xb0) goto first_bank;
    cursor = (const uint8_t *)0x2163;
    asm volatile("" : "+y" (cursor));
second_bank:
    {
        register uint8_t lo asm("r16"), hi asm("r17");
        asm volatile("ld %0, Y+\n\tld %1, Y+" : "=r" (lo), "=r" (hi), "+y" (cursor) : : "memory");
        SEND_SETTING(lo, hi, address);
    }
    asm volatile("inc %0" : "+r" (address) : : "cc");
    if (address != 0xbc) goto second_bank;
    register uint8_t lo asm("r16") = RAM8(0x2441);
    register uint8_t hi asm("r17");
    asm volatile("clr %0" : "=r" (hi) : "r" (lo) : "cc");
    address = 0xbe;
    SEND_SETTING(lo, hi, address);
}
