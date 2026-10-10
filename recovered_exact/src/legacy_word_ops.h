#ifndef PM_LEGACY_WORD_OPS_H
#define PM_LEGACY_WORD_OPS_H
#include <stdint.h>

/* Exact private-ABI operations, expanded at the call site. Not ordinary GNU
 * calls. Signed limits use the original preloaded high byte and an immediate
 * low byte; the caller must preload high == ((uint16_t)limit >> 8).
 * error_label is an assembler symbol string for an original shared error tail.
 * Logical C: if ((int16_t)value >= limit) goto error;
 *           if ((int16_t)value < limit) goto error;
 * SREG is modified exactly by CPI/CPC; no registers or memory are written. */
#define PM_REJECT_I16_GE(value, high, limit, error_label) \
    asm volatile("cpi %A0, %2\n\tcpc %B0, %1\n\tbrge " error_label \
        : : "r" (value), "r" (high), "n" ((uint16_t)(limit) & 0xffu) : "cc")
#define PM_REJECT_I16_LT(value, high, limit, error_label) \
    asm volatile("cpi %A0, %2\n\tcpc %B0, %1\n\tbrlt " error_label \
        : : "r" (value), "r" (high), "n" ((uint16_t)(limit) & 0xffu) : "cc")

/* Original settings store: Y addresses the low byte, value is R21:R20.
 * Writes little-endian bytes; advances Y by one, leaving it at the high byte.
 * Logical C: *cursor++ = (uint8_t)value; *cursor = (uint8_t)(value >> 8);
 * No interrupt masking is added; the caller retains its original IRQ window.
 * ST Y+ remains exact ASM; the high-byte store is ordinary C. */
#define PM_STORE_WORD_LE_Y(cursor, value) do { \
    asm volatile("st Y+, %A1" : "+y" (cursor) : "r" (value) : "memory"); \
    register uint8_t pm_store_high asm("r21"); \
    asm volatile("" : "=r" (pm_store_high) : "r" (value)); \
    *(cursor) = pm_store_high; \
    asm volatile("" : : : "memory"); \
} while (0)
#endif
