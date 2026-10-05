/* Retained exact call/load helpers: a C call such as
 * value = fpga_read(address); or fpga_write(address, word); expresses the
 * operation, but these entries use private bound registers, original CALL/RCALL
 * widths and shared error tails. These illustrative names are not compiled
 * interfaces and are not independently functionally validated alternatives.
 * Where a historically tested complete C alternative exists, its evidence
 * and bridge scope remain documented beside that helper. */
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
    register uint8_t limit_high asm("r26") = 0x0c;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested >= 3072) goto LAB_code_000f16;
     * if ((int16_t)first >= 3072) goto LAB_code_000f16;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim. */
asm volatile("cpi r20, 0\n\tcpc r21, r26\n\tbrge LAB_code_000f16\ncpi r24, 0\n\tcpc r25, r26\n\tbrge LAB_code_000f16\n" : : "r" (requested), "r" (first), "r" (limit_high) : "cc");
    limit_high = 5;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested < 1365) goto LAB_code_000f16;
     * if ((int16_t)first < 1365) goto LAB_code_000f16;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim. */
asm volatile("cpi r20, 0x55\n\tcpc r21, r26\n\tbrlt LAB_code_000f16\ncpi r24, 0x55\n\tcpc r25, r26\n\tbrlt LAB_code_000f16" : : "r" (requested), "r" (first), "r" (limit_high) : "cc");
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
    asm volatile("" : : "r" (word), "r" (address) : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : : "r" (word), "r" (address) : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}
asm(".pushsection .text.fpga_set_adc_range_corr,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
