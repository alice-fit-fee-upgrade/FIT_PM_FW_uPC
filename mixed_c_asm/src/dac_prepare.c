#include "pm_recovered.h"
/* Eight-byte return packet occupies R18..R25 under the GNU AVR ABI.
 * Field order is deliberate; original register effects are part of recovery. */
typedef struct {
    uint16_t value;
    uint16_t input;
    uint16_t product;
    uint8_t control;
    uint8_t reserved;
} pm_dac_packet;

/* Original byte 0x20D0: modulo subtraction precedes unsigned scaling. */
pm_dac_packet pm_dac_prepare_inverse(uint16_t input, uint8_t channel)
{
    pm_dac_packet p;
    p.input = (uint16_t)(20000u - input);
    p.value = pm_scale_unsigned(0x0272u, p.input);
    p.product = (uint16_t)(2u * (p.input >> 8));
    p.control = (uint8_t)(channel * 4u);
    p.reserved = 0;
    return p;
}
