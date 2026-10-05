#ifndef PM_LEGACY_SPI_C_H
#define PM_LEGACY_SPI_C_H
#include "legacy_spi.h"

/* Opt-in exact C SPI operations. Legacy register operands are captured without
 * emitting instructions; ordinary C performs MMIO stores, reads and polling.
 * Adopt per translation unit only after a complete zero-difference exact-check.
 * Source-specific archived C comments document any recorded functional tests;
 * those results are not a substitute for exact binary acceptance. */
#define PM_SPI_WAIT_AT(status_register, scratch) do { \
    register uint8_t status asm(scratch); \
    do { \
        status = (status_register); \
        asm volatile("" : "+r" (status)); \
    } while (!(status & 0x80u)); \
} while (0)
#undef PM_SPI_SEND_AT
#define PM_SPI_SEND_AT(data_register, status_register, scratch, value) do { \
    register uint8_t status asm(scratch); \
    (data_register) = (value); \
    do { \
        status = (status_register); \
        asm volatile("" : "+r" (status)); \
    } while (!(status & 0x80u)); \
    asm volatile("" : : "r" (status)); \
} while (0)
#undef PM_WRITE_R22
#define PM_WRITE_R22(peripheral, value) do { \
    register uint8_t mask asm("r22") = (value); \
    asm volatile("" : "+r" (mask)); \
    (peripheral) = mask; \
} while (0)
#undef PM_SPI_READ_REGISTER
#define PM_SPI_READ_REGISTER(peripheral, reg) do { \
    register uint8_t byte asm(reg) = peripheral##_DATA; \
    asm volatile("" : : "r" (byte)); \
} while (0)
#endif
