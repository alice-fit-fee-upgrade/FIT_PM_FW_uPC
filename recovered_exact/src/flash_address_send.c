#include "legacy_spi.h"

void pm_flash_address_send(void) asm("FUN_code_000c04");
void pm_flash_address_send(void)
{
    PORTE_OUTCLR = 0x10;
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r16"); /* Command. */
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r22"); /* Address high byte. */
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r21");
    PM_SPI_SEND_REGISTER(SPIE, "r19", "r20"); /* Address low byte. */
}
