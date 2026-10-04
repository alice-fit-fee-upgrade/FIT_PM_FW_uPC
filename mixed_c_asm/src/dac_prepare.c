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

/* Original byte 0x20A6: calibration is loaded by the ABI bridge so its
 * two volatile RAM reads and the original final Z pointer remain explicit. */
pm_dac_packet pm_dac_prepare_calibrated(uint16_t input, uint8_t channel,
                                        uint16_t calibration)
{
    pm_dac_packet p;
    p.value = (uint16_t)~(uint16_t)(pm_scale_unsigned(0x020cu, input) + calibration);
    p.input = calibration;
    p.product = (uint16_t)(2u * (input >> 8));
    p.control = (uint8_t)((uint8_t)(channel * 4u) | 3u);
    p.reserved = 0;
    return p;
}
