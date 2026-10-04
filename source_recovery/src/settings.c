#include "pm_recovered.h"
#include "pm_addresses.h"
#define READ(a) b->read8(b->context, (a))
#define WRITE(a,v) b->write8(b->context, (a), (uint8_t)(v))
static uint16_t word_at(const pm_bus *b, uint16_t address)
{
    uint8_t low = READ(address);
    uint8_t high = READ((uint16_t)(address + 1u));
    return (uint16_t)((uint16_t)high << 8 | low);
}
static void atomic_fpga_write(const pm_bus *b, uint8_t reg, uint16_t value)
{
    b->irq(b->context, false);
    pm_fpga_write16(b, reg, value);
    b->irq(b->context, true);
}
/* 0x08E4..0x0989: addresses intentionally follow actual code, even though
 * preceding analysis assumed differently aligned 16-bit EEPROM fields. */
void pm_fpga_settings_init(const pm_bus *b)
{
    atomic_fpga_write(b, 0x7c, 0);
    atomic_fpga_write(b, 0, READ(PM_GATE_BYTE)); /* one byte, not a 16-bit load */
    uint16_t address = PM_TDC_SETTINGS;
    for (uint8_t reg = 1; reg != 13; ++reg, address += 2)
        atomic_fpga_write(b, reg, word_at(b, address));
    address = PM_RANGE_CORRECTION;
    for (uint8_t reg = 0x25; reg != 0x3d; ++reg, address += 2)
        atomic_fpga_write(b, reg, word_at(b, address));
    atomic_fpga_write(b, 0x3d, word_at(b, PM_TRIGGER_WORD));
    atomic_fpga_write(b, 0xbc, word_at(b, PM_TEMPERATURE));
    atomic_fpga_write(b, 0xbd, word_at(b, PM_BOARD_ID));
    atomic_fpga_write(b, 0x7c, 0x0fff); /* not 0xffff from old README */
}
/* 0x098A..0x09DD */
void pm_fpga_settings_reset(const pm_bus *b)
{
    uint16_t address = PM_CHANNEL_SETTINGS;
    for (uint8_t reg = 0x80; reg != 0xb0; ++reg, address += 2)
        atomic_fpga_write(b, reg, word_at(b, address));
    address = PM_CALIBRATION;
    for (uint8_t reg = 0xb0; reg != 0xbc; ++reg, address += 2)
        atomic_fpga_write(b, reg, word_at(b, address));
    atomic_fpga_write(b, 0xbe, READ(PM_RESTART_REASON));
}
/* 0x0C96..0x0D0F: retain write ordering and read-modify-write flags. */
void pm_system_deinit(const pm_bus *b)
{
    WRITE(PM_FPGA_TIMER_LOW,0); WRITE(PM_FPGA_TIMER_HIGH,0); WRITE(PM_FPGA_STATE,0); WRITE(PM_THS_STATE,0);
    WRITE(PM_RESTART_REASON,0); WRITE(PM_FPGA_REQUEST,0); WRITE(PM_CLOCK_STATE,0);
    WRITE(PM_PORTE_INTCTRL,1); WRITE(PM_PORTF_INTCTRL,2); WRITE(PM_PORTB_INTCTRL,0); WRITE(PM_PORTD_INTCTRL,0);
    WRITE(PM_PORTB_DIRCLR,0xbf); WRITE(PM_SPIC_CTRL,0); WRITE(PM_PORTC_DIRCLR,0xff);
    WRITE(PM_PORTD_DIRCLR,0x41); WRITE(PM_PORTD_OUTCLR,4);
    WRITE(PM_PORTF_OUTCLR,0x20); WRITE(PM_PORTF_DIRCLR,0x20);
    WRITE(PM_STATUS_FLAGS,READ(PM_STATUS_FLAGS)&0xefu);
    WRITE(PM_PORTA_OUTSET,0xa0); WRITE(PM_SPID_CTRL,0); WRITE(PM_DMA_CTRL,0);
}
