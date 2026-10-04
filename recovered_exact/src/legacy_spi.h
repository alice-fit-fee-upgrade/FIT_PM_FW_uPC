#ifndef PM_LEGACY_SPI_H
#define PM_LEGACY_SPI_H
#include <avr/io.h>

/* Twelve exact instruction bytes: data store, status read, skip, back branch.
 * A named entry register supplies data without a GNU R24 argument call.
 * scratch is the original polling register and must be declared in the profile. */
#define PM_SPI_SEND_AT(data_register, status_register, scratch, value) \
    asm volatile ( \
        "sts %[data], %[byte]\n\t" \
        "1: lds " scratch ", %[status]\n\t" \
        "sbrs " scratch ", 7\n\t" \
        "rjmp 1b" \
        : : [data] "n" (_SFR_MEM_ADDR(data_register)), \
            [status] "n" (_SFR_MEM_ADDR(status_register)), \
            [byte] "r" (value) \
        : scratch, "memory")
#define PM_SPI_SEND_VALUE(peripheral, scratch, value) \
    PM_SPI_SEND_AT(peripheral##_DATA, peripheral##_STATUS, scratch, value)
#define PM_SPI_SEND_REGISTER(peripheral, scratch, reg) \
 do { \
    register uint8_t byte asm(reg); \
    asm volatile ("" : "=r" (byte)); \
    PM_SPI_SEND_AT(peripheral##_DATA, peripheral##_STATUS, scratch, byte); \
 } while (0)
/* Keep peripheral stores on the original scratch register, preserving inputs. */
#define PM_WRITE_R22(peripheral, value) do { \
    register uint8_t mask asm("r22") = (value); \
    asm volatile("sts %0, %1" : : "n" (_SFR_MEM_ADDR(peripheral)), \
                 "r" (mask) : "memory"); \
} while (0)

#define PM_SPI_READ_REGISTER(peripheral, reg) do { \
    register uint8_t byte asm(reg); \
    asm volatile("lds %0, %1" : "=r" (byte) \
                 : "n" (_SFR_MEM_ADDR(peripheral##_DATA)) : "memory"); \
} while (0)
#endif
