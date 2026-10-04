#include "legacy_cli.h"

void fpga_set_tdc_values(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f3d");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000f3d");
    asm volatile("tst r21\n\tbrmi LAB_code_000f3d\n\tldi r24, 0x10\n"
                 "cpi r20, 0\n\tcpc r21, r24\n\tbrge LAB_code_000f3d"
                 : : "r" (requested) : "r24", "cc");
    PM_FPGA_GUARD(requested);
    register uint8_t *settings asm("r28") = (uint8_t *)0x21b7;
    asm volatile("" : "+y" (settings));
    register uint8_t offset asm("r23") = channel;
    asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    asm volatile("clr r11\n\tadd r28, %1\n\tadc r29, r11\n\tst Y+, r20\n\tst Y, r21"
                 : "+y" (settings) : "r" (offset), "r" (requested) : "r11", "memory", "cc");
    register uint8_t address asm("r18") = 1;
    asm volatile("" : "+r" (address));
    address += channel; asm volatile("" : "+r" (address));
    register uint16_t word asm("r16") = requested;
    asm volatile("cli\n\trcall fpga_msg_send_t2\n\tsei\n\trjmp LAB_code_000ff1"
                 : : "r" (word), "r" (address) : "memory", "cc");
    __builtin_unreachable();
}
asm(".pushsection .text.fpga_set_tdc_values,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
