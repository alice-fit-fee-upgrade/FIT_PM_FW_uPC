#include "legacy_cli.h"

void fpga_set_ch_cfd_zero(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000ff5");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000ff5");
    asm volatile("ldi r24, 1\n\tcpi r20, 0xf5\n\tcpc r21, r24\n\tbrge LAB_code_001026\n"
                 "ldi r24, 0xfe\n\tcpi r20, 0x0c\n\tcpc r21, r24\n\tbrlt LAB_code_001026"
                 : : "r" (requested) : "r24", "cc");
    PM_FPGA_GUARD(requested);
    register uint16_t word asm("r16") = requested;
    asm volatile("" : "+r" (word));
    register uint8_t offset asm("r23") = channel;
    asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    register uint8_t address asm("r18") = 0x81;
    asm volatile("" : "+r" (address));
    address += offset;
    asm volatile("cli\n\trcall fpga_msg_send_t2\n\tsei" : "+r" (word), "+r" (address) : : "memory", "cc");
    register uint8_t *settings asm("r28") = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += 2; asm volatile("" : "+r" (offset));
    asm volatile("clr r11\n\tadd r28, %1\n\tadc r29, r11" : "+y" (settings) : "r" (offset) : "r11", "cc");
    asm volatile("st Y+, r20\n\tst Y, r21" : "+y" (settings) : "r" (requested) : "memory");
    asm volatile("rcall dac_set_value_2\n\trjmp LAB_code_000ff1" : : "r" (requested), "r" (channel) : "memory", "cc");
    __builtin_unreachable();
}
