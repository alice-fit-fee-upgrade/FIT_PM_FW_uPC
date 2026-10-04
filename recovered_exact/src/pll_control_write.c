#include "legacy_spi.h"

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
