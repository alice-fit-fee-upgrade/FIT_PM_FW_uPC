#ifndef PM_LEGACY_SPI_H
#define PM_LEGACY_SPI_H
#include <avr/io.h>

/* Twelve exact instruction bytes: data store, status read, skip, back branch.
 * A named entry register supplies data without a GNU R24 argument call.
 * scratch is the original polling register and must be declared in the profile. */
#define PM_SPI_SEND_REGISTER(peripheral, scratch, reg) \
 do { \
    register uint8_t byte asm(reg); \
    asm volatile ("" : "=r" (byte)); \
    asm volatile ( \
        "sts %[data], %[byte]\n\t" \
        "1: lds " scratch ", %[status]\n\t" \
        "sbrs " scratch ", 7\n\t" \
        "rjmp 1b" \
        : : [data] "n" (_SFR_MEM_ADDR(peripheral##_DATA)), \
            [status] "n" (_SFR_MEM_ADDR(peripheral##_STATUS)), \
            [byte] "r" (byte) \
        : scratch, "memory"); \
 } while (0)
#endif
