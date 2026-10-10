#include "legacy_spi_c.h"

/* R22 is the sole allocatable scratch in this entry's original save frame.
 * The value need not stay live after polling: the next instruction replaces it.
 * Keeping it live there makes GCC 7.3 require another register for the test. */
#undef PM_SPI_SEND_AT
#define PM_SPI_SEND_AT(data_register, status_register, scratch, value) do { \
    register uint8_t status asm(scratch); \
    (data_register) = (value); \
    do { \
        status = (status_register); \
        asm volatile("" : "+r" (status)); \
    } while (!(status & 0x80u)); \
} while (0)

/* Legacy caller supplies the little-endian control word in R16..R19. */
void CDCE62005_send_control_settings(void)
{
    SPIC_CTRL = 0xf1;
    PORTF_OUTCLR = 0x10;
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r18");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r19");
    PORTF_OUTSET = 0x10;
}
