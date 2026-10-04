#include <avr/io.h>
void set_status_and_vd8_led(void)
{
    if (!(GPIOR0 & 2)) {
        GPIOR0 &= 0xfe;
        PORTA_OUTSET = 1;
    } else {
        PORTA_OUTCLR = 1;
        GPIOR0 &= 0xfe;
    }
}
