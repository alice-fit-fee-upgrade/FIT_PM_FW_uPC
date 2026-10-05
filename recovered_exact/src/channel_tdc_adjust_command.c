#include "legacy_cpu.h"
#include "legacy_cli.h"

void pm_channel_tdc_adjust(void) asm("FUN_code_000f70");
void pm_channel_tdc_adjust(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f9c");
    register uint8_t channel asm("r19") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000f9c");
    register uint8_t limit_high asm("r24") = 0;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested >= 64) goto LAB_code_000f9c;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim. */
asm volatile("cpi r20, 0x40\n\tcpc r21, r24\n\tbrge LAB_code_000f9c\n" : : "r" (requested), "r" (limit_high) : "cc");
    limit_high = 0xff;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested < -64) goto LAB_code_000f9c;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim. */
asm volatile("cpi r20, 0xc0\n\tcpc r21, r24\n\tbrlt LAB_code_000f9c" : : "r" (requested), "r" (limit_high) : "cc");
    PM_FPGA_GUARD(requested);
    register uint8_t *settings asm("r28") = (uint8_t *)0x217b;
    asm volatile("clr r11\n\tadd r28, %1\n\tadc r29, r11"
                 : "+y" (settings) : "r" (channel) : "r11", "cc");
    asm volatile("st Y, r20" : : "y" (settings), "r" (requested) : "memory");
    register uint8_t command_index asm("r21") = channel;
    asm volatile("" : "+r" (command_index));
    command_index &= 3; asm volatile("" : "+r" (command_index));
    register uint8_t stride asm("r22") = 0x20;
    asm volatile("mul %0, %1" : : "r" (command_index), "r" (stride) : "r0", "r1", "cc");
    register uint8_t command asm("r16") = 0x0c;
    register uint8_t product_low asm("r0");
    asm volatile("" : "=r" (product_low));
    command += product_low;
    asm volatile("" : "+r" (command));
    channel >>= 2; asm volatile("" : "+r" (channel));
    register uint8_t data asm("r17") = (uint8_t)requested;
    register uint8_t high asm("r18");
    /* C value: high = 0; retain CLR's original flags and exact short call. */
    asm volatile("clr %0" : "=r" (high) : "r" (command), "r" (data), "r" (channel) : "cc");
    pm_cpu_disable_irq();
    asm volatile("rcall ths788_write" : : "r" (high), "r" (command), "r" (data), "r" (channel) : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}
asm(".pushsection .text.FUN_code_000f70,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
