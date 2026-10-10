#ifndef PM_LEGACY_FLAG_OPS_H
#define PM_LEGACY_FLAG_OPS_H

/* Exact in-place private-register operations. These are instruction macros,
 * not GNU C ABI calls. The original raw register mutation stays opaque to GCC:
 * callers must capture the register before C reads it, as in the existing ISR.
 * No return value, stack frame or extra branch is introduced.
 *
 * REFERENCE_C_INFERRED, value-only equivalents:
 *   clear:     value = 0;
 *   increment: value = (uint8_t)(value + 1);
 *   decrement: value = (uint8_t)(value - 1);
 *
 * The exact SREG effects matter even when these expressions have the same value:
 * EOR-self sets Z=1 and N=V=S=0; C/H/T/I are unchanged.
 * INC/DEC set Z/N/V/S (overflow at 0x7f->0x80 / 0x80->0x7f), preserving C/H/T/I.
 * Ordinary C tried in the ISR generated different opcode bytes (LDI/SUBI).
 * The separately compiled transition model in tests/logical_models/legacy_flag_ops.c verifies the actual operations;
 * it is not a claim of whole-path validation of the value-only expressions. */
#define PM_CLEAR_LEGACY_BYTE(reg) \
    asm volatile("eor " reg "," reg : : : "memory", "cc")
#define PM_INCREMENT_LEGACY_BYTE(reg) \
    asm volatile("inc " reg : : : "memory", "cc")
#define PM_DECREMENT_LEGACY_BYTE(reg) \
    asm volatile("dec " reg : : : "memory", "cc")
#endif
