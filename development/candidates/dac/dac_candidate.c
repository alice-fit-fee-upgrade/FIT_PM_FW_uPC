/* Development candidate: unchanged DAC8554 framing and settings order.
 * No readback exists on this input-only interface. See docs/dac/README.md. */
#include "dac_candidate.h"
#define REG8(a) (*(volatile uint8_t *)(a))
#define SPIC_CTRL REG8(0x08c0)
#define SPIC_STATUS REG8(0x08c2)
#define SPIC_DATA REG8(0x08c3)
#define DAC_SELECT REG8(0x0646)
#define DAC_DESELECT REG8(0x0645)
static void send_byte(uint8_t value)
{
    SPIC_DATA=value;
    while (!(SPIC_STATUS & 0x80u)) { }
}
/* Caller owns the bus and masks IRQs; adapter restores original SEI policy.
 * Blocking polling, controller mode and raising all three SYNC lines match
 * the original. Do not silently add a timeout or restore SPI configuration. */
void pm_dev_dac_send_locked(uint8_t control,uint16_t value)
{
    uint8_t group=control & 0x30u;
    uint8_t select=group==0 ? 1u : group==0x10u ? 2u : 4u;
    uint8_t header=((control<<4)&0xc0u)|((control<<1)&6u)|0x10u;
    SPIC_CTRL=0xd5;
    DAC_SELECT=select;
    send_byte(header);
    send_byte((uint8_t)(value>>8));
    send_byte((uint8_t)value);
    DAC_DESELECT=7;
}
/* GNU adapter to the four unchanged legacy preparation functions.
 * kind0 threshold,1 CFD zero,2 ADC zero,3 ADC delay. */
void pm_dev_dac_set_legacy(uint16_t value,uint8_t channel,uint8_t kind);
void pm_dev_dac_apply_stored(void)
{
    const volatile uint8_t *cursor=(const volatile uint8_t *)0x21cf;
    for(uint8_t channel=0;channel<12;++channel) {
        for(uint8_t kind=0;kind<4;++kind) {
            uint8_t low=*cursor++;
            uint8_t high=*cursor++;
            pm_dev_dac_set_legacy((uint16_t)low|((uint16_t)high<<8),channel,kind);
        }
    }
}
