#include <stdint.h>
#define RAM8(a) (*(volatile uint8_t *)(a))
/* GNU-callable adapter retains original CLI / ASM serializer / SEI. */
extern void pm_fpga_settings_write(uint8_t reg, uint16_t value);
static uint16_t word_at(uint16_t address)
{
    uint8_t low = RAM8(address);
    uint8_t high = RAM8((uint16_t)(address + 1u));
    return (uint16_t)low | ((uint16_t)high << 8);
}
void pm_fpga_settings_reset_c(void)
{
    uint16_t address = 0x21cf;
    for (uint8_t reg = 0x80; reg != 0xb0; ++reg, address += 2)
        pm_fpga_settings_write(reg, word_at(address));
    address = 0x2163;
    for (uint8_t reg = 0xb0; reg != 0xbc; ++reg, address += 2)
        pm_fpga_settings_write(reg, word_at(address));
    pm_fpga_settings_write(0xbe, RAM8(0x2441));
}

void pm_fpga_settings_init_c(void)
{
    pm_fpga_settings_write(0x7c, 0);
    pm_fpga_settings_write(0, RAM8(0x222f));
    uint16_t address = 0x21b7;
    for (uint8_t reg = 1; reg != 13; ++reg, address += 2)
        pm_fpga_settings_write(reg, word_at(address));
    address = 0x2187;
    for (uint8_t reg = 0x25; reg != 0x3d; ++reg, address += 2)
        pm_fpga_settings_write(reg, word_at(address));
    pm_fpga_settings_write(0x3d, word_at(0x2230));
    pm_fpga_settings_write(0xbc, word_at(0x2160));
    pm_fpga_settings_write(0xbd, word_at(0x2232));
    pm_fpga_settings_write(0x7c, 0x0fff);
}
