#include <stdint.h>
/* Byte-addressed data-space registers audited against original opcodes.
 * Hardware SPI supplies clock edges; these functions retain original write
 * and polling order and deliberately add no timeout or input validation. */
#define REG8(address) (*(volatile uint8_t *)(uintptr_t)(address))
#define PORTD_OUTCLR REG8(0x0666)
#define PORTE_DIRSET REG8(0x0681)
#define PORTE_OUTSET REG8(0x0685)
#define PORTE_OUTCLR REG8(0x0686)
#define SPIE_CTRL REG8(0x0ac0)
#define SPIE_STATUS REG8(0x0ac2)
#define SPIE_DATA REG8(0x0ac3)

void pm_flash_spi_init(void)
{
    PORTD_OUTCLR = 2;
    PORTE_OUTSET = 0x10;
    PORTE_DIRSET = 0xb0;
    SPIE_CTRL = 0x50;
}

uint8_t pm_flash_write_enable(void)
{
    PORTE_OUTCLR = 0x10;
    SPIE_DATA = 6;
    uint8_t status;
    do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
    PORTE_OUTSET = 0x10;
    return status;
}

uint8_t pm_flash_send_address(uint8_t command, uint8_t high,
                              uint8_t middle, uint8_t low)
{
    uint8_t status;
    PORTE_OUTCLR = 0x10;
    SPIE_DATA = command;
    do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
    SPIE_DATA = high;
    do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
    SPIE_DATA = middle;
    do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
    SPIE_DATA = low;
    do { status = SPIE_STATUS; } while ((status & 0x80u) == 0);
    return status;
}
