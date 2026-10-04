#include <avr/io.h>
/* Original 0x0C7E: the order of the atomic GPIOR0 clear and LED write
 * depends on bit 1. Retain that order and clear only bit 0. */
void pm_status_led(void)
{
    if (GPIOR0 & 2u) {
        PORTA_OUTCLR = 1;
        GPIOR0 &= (uint8_t)~1u;
    } else {
        GPIOR0 &= (uint8_t)~1u;
        PORTA_OUTSET = 1;
    }
}
