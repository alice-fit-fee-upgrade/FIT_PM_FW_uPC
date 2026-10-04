#include "legacy_cli.h"

void fpga_set_ch_cfd_threshold(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f9c");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000f9c");
    asm volatile("ldi r24, 0x75\n\tcpi r20, 0x31\n\tcpc r21, r24\n\tbrge LAB_code_000f9c\n"
                 "ldi r24, 1\n\tcpi r20, 0x2c\n\tcpc r21, r24\n\tbrlt LAB_code_000f9c"
                 : : "r" (requested) : "r24", "cc");
    PM_FPGA_GUARD(requested);
    register uint16_t word asm("r16") = requested;
    asm volatile("" : "+r" (word));
    register uint8_t offset asm("r10");
    asm volatile("mov %0, %1\n\tlsl %0\n\tlsl %0" : "=r" (offset) : "r" (channel) : "cc");
    register uint8_t address asm("r18") = 0x80;
    asm volatile("" : "+r" (address));
    address += offset;
    asm volatile("cli\n\trcall fpga_msg_send_t2\n\tsei" : "+r" (word), "+r" (address) : : "memory", "cc");
    register uint8_t *settings asm("r28") = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    asm volatile("lsl %0\n\tclr r11\n\tadd r28, %0\n\tadc r29, r11"
        : "+r" (offset), "+y" (settings) : : "r11", "cc");
    asm volatile("st Y+, r20\n\tst Y, r21" : "+y" (settings) : "r" (requested) : "memory");
    asm volatile("rcall FUN_code_001053" : : "r" (requested), "r" (channel) : "memory", "cc");
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2998;
    asm volatile("rcall cli_send_msg" : "+z" (message) : : "memory", "cc");
}
asm(".pushsection .text.fpga_set_ch_cfd_threshold,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
