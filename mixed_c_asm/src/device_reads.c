#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
extern void pm_pll_write_unmasked(uint32_t value);
static uint8_t pll_byte(void)
{
    SPIC_DATA=0;
    while (!(SPIC_STATUS & 0x80u)) { }
    return SPIC_DATA;
}
uint32_t pm_pll_read_c(void)
{
    pm_pll_write_unmasked(0x8e);
    PORTF_OUTCLR=0x10;
    uint8_t b0=pll_byte();
    uint8_t b1=pll_byte();
    uint8_t b2=pll_byte();
    uint8_t b3=pll_byte();
    PORTF_OUTSET=0x10;
    return (uint32_t)b0 | ((uint32_t)b1<<8) | ((uint32_t)b2<<16) | ((uint32_t)b3<<24);
}
/* Packet from original bit-banged byte primitive: RX, last GPIO, SREG, zero. */
extern uint32_t pm_adt_byte_call(uint8_t tx);
uint32_t pm_adt8_c(uint8_t command,uint8_t data)
{
    PORTA_OUTCLR=0x10;
    (void)pm_adt_byte_call(command);
    uint32_t packet=pm_adt_byte_call(data);
    PORTA_OUTSET=0x10;
    return packet; /* Original 8-bit entry discards RX and preserves all GPRs. */
}
uint32_t pm_adt16_c(uint8_t command,uint16_t data)
{
    PORTA_OUTCLR=0x10;
    (void)pm_adt_byte_call(command);
    uint8_t high=(uint8_t)pm_adt_byte_call((uint8_t)(data>>8));
    uint32_t last=pm_adt_byte_call((uint8_t)data);
    PORTA_OUTSET=0x10;
    return (last&0x00ff0000UL) | ((uint32_t)high<<8) | (uint8_t)last;
}
uint32_t pm_adt_faults_c(void)
{
    PORTA_OUTCLR=0x10;
    (void)pm_adt_byte_call(0xff);
    (void)pm_adt_byte_call(0xff);
    (void)pm_adt_byte_call(0xff);
    uint32_t packet=pm_adt_byte_call(0xff);
    PORTA_OUTSET=0x10;
    return packet;
}
static void fpga_byte(uint8_t value)
{
    SPIC_DATA=value;
    while (!(SPIC_STATUS&0x80u)) { }
}
void pm_fpga_stamp_c(void)
{
    SPIC_CTRL=0xd1;
    PORTD_OUTCLR=1;
    fpga_byte(0x3d); fpga_byte(0x40);
    uint16_t address=0x2b92;
    for (uint8_t words=0;words!=2;++words) {
        uint8_t low=pgm_read_byte(address++);
        uint8_t high=pgm_read_byte(address++);
        fpga_byte(high); fpga_byte(low);
    }
    PORTD_OUTSET=1;
}
