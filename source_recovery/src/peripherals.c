#include "pm_recovered.h"
#include "pm_addresses.h"
#define READ(a) b->read8(b->context, (a))
#define WRITE(a,v) b->write8(b->context, (a), (uint8_t)(v))
static void spi_wait(const pm_bus *b)
{
    while ((READ(PM_SPIC_STATUS) & 0x80u) == 0) { }
}
static void spi_write(const pm_bus *b, uint8_t data)
{
    WRITE(PM_SPIC_DATA, data);
    spi_wait(b);
}
uint8_t pm_adt7311_byte(const pm_bus *b, uint8_t byte)
{
    for (uint8_t i = 0; i != 8; ++i) {
        WRITE((byte & 128u) ? PM_PORTA_OUTSET : PM_PORTA_OUTCLR, 8);
        WRITE(PM_PORTA_OUTCLR, 2);
        byte = (uint8_t)(byte << 1);
        /* Original NOP remains in AVR build; callback overhead changes timing. */
#ifdef __AVR__
        __asm__ __volatile__("nop");
#endif
        WRITE(PM_PORTA_OUTSET, 2);
        byte |= (READ(PM_PORTA_IN) >> 2) & 1u;
    }
    return byte;
}
void pm_adt7311_write8(const pm_bus *b, uint8_t cmd, uint8_t data)
{
    WRITE(PM_PORTA_OUTCLR, 16);
    (void)pm_adt7311_byte(b, cmd);
    (void)pm_adt7311_byte(b, data);
    WRITE(PM_PORTA_OUTSET, 16);
}
uint16_t pm_adt7311_exchange16(const pm_bus *b, uint8_t cmd, uint16_t data)
{
    WRITE(PM_PORTA_OUTCLR, 16);
    (void)pm_adt7311_byte(b, cmd);
    uint8_t high = pm_adt7311_byte(b, (uint8_t)(data >> 8));
    uint8_t low = pm_adt7311_byte(b, (uint8_t)data);
    WRITE(PM_PORTA_OUTSET, 16);
    return (uint16_t)((uint16_t)high << 8 | low);
}
void pm_adt7311_clear(const pm_bus *b)
{
    WRITE(PM_PORTA_OUTCLR, 16);
    for (uint8_t i = 0; i != 4; ++i) (void)pm_adt7311_byte(b, 255);
    WRITE(PM_PORTA_OUTSET, 16);
}
void pm_fpga_write16(const pm_bus *b, uint8_t reg, uint16_t data)
{
    WRITE(PM_SPIC_CTRL, 0xd1);
    WRITE(PM_PORTD_OUTCLR, 1);
    spi_write(b, reg >> 2);
    spi_write(b, (uint8_t)(reg << 6));
    spi_write(b, (uint8_t)(data >> 8));
    spi_write(b, (uint8_t)data);
    WRITE(PM_PORTD_OUTSET, 1);
}
uint16_t pm_fpga_read16(const pm_bus *b, uint8_t reg, uint16_t dummy, uint8_t r21)
{
    WRITE(PM_SPIC_CTRL, 0xd1);
    WRITE(PM_PORTD_OUTCLR, 1);
    spi_write(b, (reg >> 2) | 128u);
    spi_write(b, (uint8_t)((reg << 6) | (r21 >> 2)));
    spi_write(b, (uint8_t)(dummy >> 8));
    uint8_t high = READ(PM_SPIC_DATA);
    spi_write(b, (uint8_t)dummy);
    uint8_t low = READ(PM_SPIC_DATA);
    WRITE(PM_PORTD_OUTSET, 1);
    return (uint16_t)((uint16_t)high << 8 | low);
}
void pm_fpga_read_bc(const pm_bus *b, uint8_t result[8])
{
    WRITE(PM_SPIC_CTRL, 0xd1);
    WRITE(PM_PORTD_OUTCLR, 1);
    spi_write(b, 0xbc);
    spi_write(b, 0);
    for (uint8_t i = 0; i != 8; ++i) {
        spi_write(b, 0);
        result[i] = READ(PM_SPIC_DATA);
    }
    WRITE(PM_PORTD_OUTSET, 1);
}
void pm_pll_write32(const pm_bus *b, uint32_t value)
{
    WRITE(PM_SPIC_CTRL, 0xf1);
    WRITE(PM_PORTF_OUTCLR, 16);
    for (uint8_t i = 0; i != 4; ++i) {
        spi_write(b, (uint8_t)value);
        value >>= 8;
    }
    WRITE(PM_PORTF_OUTSET, 16);
}
uint32_t pm_pll_read32(const pm_bus *b)
{
    pm_pll_write32(b, 0x8e);
    WRITE(PM_PORTF_OUTCLR, 16);
    uint32_t result = 0;
    for (uint8_t i = 0; i != 4; ++i) {
        spi_write(b, 0);
        result |= (uint32_t)READ(PM_SPIC_DATA) << (8u * i);
    }
    WRITE(PM_PORTF_OUTSET, 16);
    return result;
}
void pm_dac_send(const pm_bus *b, uint8_t control, uint16_t data)
{
    b->irq(b->context, false);
    WRITE(PM_SPIC_CTRL, 0xd5);
    uint8_t group = control & 0x30u;
    uint8_t select = group == 0 ? 1 : group == 0x10 ? 2 : 4;
    uint8_t header = (uint8_t)(((control << 4) & 0xc0u) | ((control << 1) & 6u) | 16u);
    WRITE(PM_PORTC_OUTCLR, select);
    spi_write(b, header);
    spi_write(b, (uint8_t)(data >> 8));
    spi_write(b, (uint8_t)data);
    WRITE(PM_PORTC_OUTSET, 7);
    b->irq(b->context, true); /* Original unconditionally enables interrupts. */
}
void pm_ths_write24(const pm_bus *b, uint8_t control, uint8_t r16, uint8_t r17, uint8_t r18)
{
    uint8_t select = control & 3u;
    WRITE(PM_PORTB_OUTCLR, (select == 3 ? 28 : (4u << select)) | 2u);
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    if (control & 4u) WRITE(PM_PORTB_OUTSET, 2);
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    uint32_t data = (uint32_t)r16 << 16 | (uint16_t)r18 << 8 | r17;
    /* ADC chain rotates R17 -> R18 -> R16; incoming Carry is overwritten
     * before this loop. The 24 transmitted bits are unaffected by injected bits. */
    for (uint8_t i = 0; i != 24; ++i) {
        WRITE((data & 0x800000ul) ? PM_PORTB_OUTSET : PM_PORTB_OUTCLR, 2);
        data <<= 1;
        WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    }
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    WRITE(PM_PORTB_OUTSET, 28);
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
}
uint16_t pm_ths_read16(const pm_bus *b, uint8_t select, uint8_t cmd, uint16_t incoming)
{
    if (select >= 3) return incoming;
    WRITE(PM_PORTB_OUTCLR, 4u << select);
    WRITE(PM_PORTB_OUTSET, 2);
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    WRITE(PM_PORTB_OUTCLR, 2);
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    for (uint8_t i = 0; i != 8; ++i) {
        WRITE((cmd & 128u) ? PM_PORTB_OUTSET : PM_PORTB_OUTCLR, 2);
        cmd = (uint8_t)(cmd << 1);
        WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    }
    WRITE(PM_PORTB_DIRCLR, 2);
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    uint16_t value = incoming;
    for (uint8_t i = 0; i != 16; ++i) {
        WRITE(PM_PORTB_OUTSET, 1);
        value = (uint16_t)((value << 1) | ((READ(PM_PORTB_IN) >> 1) & 1u));
        WRITE(PM_PORTB_OUTCLR, 1);
    }
    WRITE(PM_PORTB_OUTSET, 28); WRITE(PM_PORTB_DIRSET, 2);
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    WRITE(PM_PORTB_OUTSET, 1); WRITE(PM_PORTB_OUTCLR, 1);
    return value;
}
