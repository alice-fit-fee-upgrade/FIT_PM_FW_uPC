#include "legacy_cpu.h"
#include "legacy_cli.h"

void fpga_set_ch_adc_delay(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f9c");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000f9c");
    asm volatile("tst r21\n\tbrmi LAB_code_000f9c\n\tldi r24, 0x4e\n"
                 "cpi r20, 0x21\n\tcpc r21, r24\n\tbrge LAB_code_000f9c" : : "r" (requested) : "r24", "cc");
    PM_FPGA_GUARD(requested);
    register uint16_t word asm("r16") = requested;
    asm volatile("" : "+r" (word));
    register uint8_t offset asm("r23") = channel;
    asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    register uint8_t address asm("r18") = 0x83;
    asm volatile("" : "+r" (address));
    address += offset;
    asm volatile("" : "+r" (word), "+r" (address) : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : "+r" (word), "+r" (address) : : "memory", "cc");
    pm_cpu_enable_irq();
    register uint8_t *settings asm("r28") = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += 6; asm volatile("" : "+r" (offset));
    asm volatile("clr r11" : : : "r11", "cc");
    {
        register uint8_t address_low asm("r28");
        asm volatile("" : "=r" (address_low) : "y" (settings));
        address_low += offset;
        asm volatile("" : "+r" (address_low));
    }
    /* Original ADC consumes carry from the C low-byte addition. */
    asm volatile("adc r29, r11" : "=y" (settings) : "r" (offset) : "r11", "cc");
    asm volatile("st Y+, r20\n\tst Y, r21" : "+y" (settings) : "r" (requested) : "memory");
    asm volatile("rcall FUN_code_001068\n\trjmp LAB_code_000ff1" : : "r" (requested), "r" (channel) : "memory", "cc");
    __builtin_unreachable();
}
