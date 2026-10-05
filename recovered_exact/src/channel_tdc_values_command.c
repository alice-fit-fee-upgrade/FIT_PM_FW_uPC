/* Retained exact call/load helpers: a C call such as
 * value = fpga_read(address); or fpga_write(address, word); expresses the
 * operation, but these entries use private bound registers, original CALL/RCALL
 * widths and shared error tails. These illustrative names are not compiled
 * interfaces and are not independently functionally validated alternatives.
 * Where a historically tested complete C alternative exists, its evidence
 * and bridge scope remain documented beside that helper. */
#include "legacy_cpu.h"
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
    asm volatile("clr r11" : : : "r11", "cc");
    {
        register uint8_t address_low asm("r28");
        asm volatile("" : "=r" (address_low) : "y" (settings));
        address_low += offset;
        asm volatile("" : "+r" (address_low));
    }
    /* Original ADC consumes carry from the C low-byte addition. */
    asm volatile("adc r29, r11\n\tst Y+, r20\n\tst Y, r21"
                 : "=y" (settings) : "r" (offset), "r" (requested) : "r11", "memory", "cc");
    register uint8_t address asm("r18") = 1;
    asm volatile("" : "+r" (address));
    address += channel; asm volatile("" : "+r" (address));
    register uint16_t word asm("r16") = requested;
    asm volatile("" : : "r" (word), "r" (address) : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : : "r" (word), "r" (address) : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}
asm(".pushsection .text.fpga_set_tdc_values,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
