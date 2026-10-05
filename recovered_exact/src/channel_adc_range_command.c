#include "legacy_cpu.h"
#include "legacy_cli.h"

void fpga_set_adc_range_corr(void)
{
    register uint16_t requested asm("r20"), first asm("r24");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f16");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    asm volatile("cpi r16, 0x20\n\tbrne LAB_code_000f16\n\trcall cli_get_integer\n"
                 "brcs LAB_code_000f16\n\tcpi r16, 0x2c\n\tbrne LAB_code_000f16"
                 : "=r" (requested) : : "r16", "memory", "cc");
    first = requested; asm volatile("" : "+r" (first));
    asm volatile("rcall cli_get_integer\n\tbrcs LAB_code_000f16\n\tcpi r16, 13\n\tbrne LAB_code_000f16"
                 : "=r" (requested) : : "r16", "memory", "cc");
    /* Original signed lower/upper checks for both correction words. */
    asm volatile("ldi r26, 0x0c\n\tcpi r20, 0\n\tcpc r21, r26\n\tbrge LAB_code_000f16\n"
                 "cpi r24, 0\n\tcpc r25, r26\n\tbrge LAB_code_000f16\n"
                 "ldi r26, 5\n\tcpi r20, 0x55\n\tcpc r21, r26\n\tbrlt LAB_code_000f16\n"
                 "cpi r24, 0x55\n\tcpc r25, r26\n\tbrlt LAB_code_000f16"
                 : : "r" (requested), "r" (first) : "r26", "cc");
    PM_FPGA_GUARD(requested);
    register uint8_t *settings asm("r28") = (uint8_t *)0x2187;
    asm volatile("" : "+y" (settings));
    channel += channel; asm volatile("" : "+r" (channel));
    register uint8_t offset asm("r23") = channel;
    asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    asm volatile("clr r11\n\tadd r28, %1\n\tadc r29, r11\n"
                 "st Y+, r24\n\tst Y+, r25\n\tst Y+, r20\n\tst Y, r21"
                 : "+y" (settings) : "r" (offset), "r" (first), "r" (requested) : "r11", "memory", "cc");
    register uint8_t address asm("r18") = 0x25;
    asm volatile("" : "+r" (address));
    address += channel; asm volatile("" : "+r" (address));
    register uint16_t word asm("r16") = first;
    asm volatile("" : "+r" (word), "+r" (address) : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : "+r" (word), "+r" (address) : : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("inc %0" : "+r" (address) : : "cc");
    word = requested;
    asm volatile("cli\n\trcall fpga_msg_send_t2\n\tsei\n\trjmp LAB_code_000ff1"
                 : : "r" (word), "r" (address) : "memory", "cc");
    __builtin_unreachable();
}
asm(".pushsection .text.fpga_set_adc_range_corr,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
