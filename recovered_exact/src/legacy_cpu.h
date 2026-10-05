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

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 208 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_legacy_cpu(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0a56: { // bst r16, 3
        pm_flag(s, TRANSFER, s->r[16] & (1u << 3));
        return 2648;
    }
    case 0x0a5e: { // bld r16, 4
        s->r[16] = (s->r[16] & ~(1u << 4)) | (pm_getflag(s, TRANSFER) << 4);
        return 2656;
    }
    case 0x0af0: { // bst r16, 6
        pm_flag(s, TRANSFER, s->r[16] & (1u << 6));
        return 2802;
    }
    case 0x0b12: { // bld r19, 0
        s->r[19] = (s->r[19] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 2836;
    }
    case 0x0ba8: { // bst r16, 1
        pm_flag(s, TRANSFER, s->r[16] & (1u << 1));
        return 2986;
    }
    case 0x0baa: { // bld r17, 0
        s->r[17] = (s->r[17] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 2988;
    }
    case 0x0bae: { // bld r17, 3
        s->r[17] = (s->r[17] & ~(1u << 3)) | (pm_getflag(s, TRANSFER) << 3);
        return 2992;
    }
    case 0x0bb0: { // bst r16, 2
        pm_flag(s, TRANSFER, s->r[16] & (1u << 2));
        return 2994;
    }
    case 0x167e: { // cli
        pm_irq(s, false);
        return 5760;
    }
    case 0x16ae: { // sei
        pm_irq(s, true);
        return 5808;
    }
    case 0x2620: { // nop
        return 9762;
    }
    case 0x262a: { // bst r17, 2
        pm_flag(s, TRANSFER, s->r[17] & (1u << 2));
        return 9772;
    }
    case 0x262c: { // bld r16, 0
        s->r[16] = (s->r[16] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 9774;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
