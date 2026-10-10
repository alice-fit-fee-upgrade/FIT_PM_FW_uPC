#ifndef PM_LEGACY_CPU_H
#define PM_LEGACY_CPU_H
#include <stdint.h>

/* GNU AVR bit insertion: all other destination bits are preserved.
 * Source/target bit indices must be compile-time constants (0..7).
 * GCC documents map nibbles here:
 * https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/AVR-Built-in-Functions.html
 * Whole-image exact-check also verifies the instruction order and SREG.T. */
#define PM_COPY_BIT(destination, target_bit, source, source_bit) \
    __builtin_avr_insert_bits( \
        (0xffffffffUL & ~(0xfUL << (4u * (target_bit)))) | \
        ((uint32_t)(source_bit) << (4u * (target_bit))), \
        (source), (destination))

/* Compiler barriers retain MMIO/ordinary-memory ordering around the AVR
 * primitives. They emit no instructions; CLI/SEI/NOP come from GCC built-ins. */
static inline void pm_cpu_disable_irq(void)
{
    asm volatile("" : : : "memory");
    __builtin_avr_cli();
    asm volatile("" : : : "memory");
}
static inline void pm_cpu_enable_irq(void)
{
    asm volatile("" : : : "memory");
    __builtin_avr_sei();
    asm volatile("" : : : "memory");
}
static inline void pm_cpu_nop(void)
{
    asm volatile("" : : : "memory");
    __builtin_avr_nop();
    asm volatile("" : : : "memory");
}
#endif
