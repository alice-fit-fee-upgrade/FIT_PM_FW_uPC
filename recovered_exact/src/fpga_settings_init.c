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
 * void pm_fpga_settings_init_c(void)
 * {
 *     pm_fpga_settings_write(0x7c, 0);
 *     pm_fpga_settings_write(0, RAM8(0x222f));
 *     uint16_t address = 0x21b7;
 *     for (uint8_t reg = 1; reg != 13; ++reg, address += 2)
 *         pm_fpga_settings_write(reg, word_at(address));
 *     address = 0x2187;
 *     for (uint8_t reg = 0x25; reg != 0x3d; ++reg, address += 2)
 *         pm_fpga_settings_write(reg, word_at(address));
 *     pm_fpga_settings_write(0x3d, word_at(0x2230));
 *     pm_fpga_settings_write(0xbc, word_at(0x2160));
 *     pm_fpga_settings_write(0xbd, word_at(0x2232));
 *     pm_fpga_settings_write(0x7c, 0x0fff);
 * }
 */

#define RAM8(address) (*(volatile uint8_t *)(address))
#define SEND_SETTING(lo, hi, address) do { \
    asm volatile("" : "+r" (lo), "+r" (hi), "+r" (address) : : "memory"); \
    pm_cpu_disable_irq(); \
    asm volatile("call fpga_msg_send_t2" \
        : "+r" (lo), "+r" (hi), "+r" (address) : : "memory", "cc"); \
    pm_cpu_enable_irq(); \
} while (0)
#define READ_WORD(lo, hi, cursor) \
    asm volatile("ld %0, Y+\n\tld %1, Y+" \
        : "=r" (lo), "=r" (hi), "+y" (cursor) : : "memory")
#define NEXT_ADDRESS(address) asm volatile("inc %0" : "+r" (address) : : "cc")

void fpga_settings_init(void)
{
    /* Original asymmetric frame saves R30 but not YL; preserve it as recorded. */
    asm volatile("push r18\n\tpush r17\n\tpush r16\n\tpush r29\n\tpush r30" : : : "memory");
    register uint8_t address asm("r18") = 0x7c;
    register uint8_t lo asm("r16"), hi asm("r17");
    asm volatile("clr %0\n\tclr %1" : "=r" (lo), "=r" (hi) : "r" (address) : "cc");
    SEND_SETTING(lo, hi, address);
    lo = RAM8(0x222f);
    asm volatile("clr %0\n\tclr %1" : "=r" (hi), "=r" (address) : "r" (lo) : "cc");
    SEND_SETTING(lo, hi, address);
    register const uint8_t *cursor asm("r28") = (const uint8_t *)0x21b7;
    asm volatile("" : "+y" (cursor));
first_bank:
    READ_WORD(lo, hi, cursor);
    NEXT_ADDRESS(address);
    SEND_SETTING(lo, hi, address);
    if (address != 0x0c) goto first_bank;
    address = 0x24;
    asm volatile("" : "+r" (address));
    cursor = (const uint8_t *)0x2187;
    asm volatile("" : "+y" (cursor));
second_bank:
    READ_WORD(lo, hi, cursor);
    NEXT_ADDRESS(address);
    SEND_SETTING(lo, hi, address);
    if (address != 0x3c) goto second_bank;
    NEXT_ADDRESS(address);
    lo = RAM8(0x2230); hi = RAM8(0x2231);
    SEND_SETTING(lo, hi, address);
    lo = RAM8(0x2160); hi = RAM8(0x2161);
    address = 0xbc;
    SEND_SETTING(lo, hi, address);
    lo = RAM8(0x2232); hi = RAM8(0x2233);
    address = 0xbd;
    SEND_SETTING(lo, hi, address);
    address = 0x7c;
    lo = 0xff; hi = 0x0f;
    SEND_SETTING(lo, hi, address);
    asm volatile("pop r30\n\tpop r29\n\tpop r16\n\tpop r17\n\tpop r18\n\tret" : : : "memory");
    __builtin_unreachable();
}
