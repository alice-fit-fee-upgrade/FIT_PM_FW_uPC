#include "legacy_cpu.h"
#include "legacy_cli.h"

void fpga_set_threshold_calibration(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f3d");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000f3d");
    asm volatile("tst r21\n\tbrmi LAB_code_000f3d\n\tldi r24, 0x0f\n"
                 "cpi r20, 0xa1\n\tcpc r21, r24\n\tbrge LAB_code_000f3d"
                 : : "r" (requested) : "r24", "cc");
    PM_FPGA_GUARD(requested);
    register uint16_t word asm("r16") = requested;
    asm volatile("" : "+r" (word));
    register uint8_t address asm("r18") = 0xb0;
    asm volatile("" : "+r" (address));
    address += channel;
    asm volatile("" : "+r" (word), "+r" (address) : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : "+r" (word), "+r" (address) : : "memory", "cc");
    pm_cpu_enable_irq();
    register uint8_t *settings asm("r28") = (uint8_t *)0x2163;
    asm volatile("" : "+y" (settings));
    register uint8_t offset asm("r23") = channel;
    asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    asm volatile("clr r11\n\tadd r28, %1\n\tadc r29, r11\n\tst Y+, r20\n\tst Y, r21"
                 : "+y" (settings) : "r" (offset), "r" (requested) : "r11", "memory", "cc");
    settings = (uint8_t *)0x21cf;
    register uint8_t settings_offset asm("r10") = channel;
    asm volatile("" : "+r" (settings_offset), "+y" (settings));
    settings_offset += settings_offset; asm volatile("" : "+r" (settings_offset));
    settings_offset += settings_offset; asm volatile("" : "+r" (settings_offset));
    settings_offset += settings_offset; asm volatile("" : "+r" (settings_offset));
    /* C values: settings += settings_offset;
     * requested = settings[0] | ((uint16_t)settings[1] << 8); settings++;
     * Retain exact carry/CLR/LD Y+ encodings; explanation only. */
    asm volatile("clr r11\n\tadd r28, r10\n\tadc r29, r11\n\tld r20, Y+\n\tld r21, Y"
                 : "+y" (settings), "=r" (requested) : "r" (settings_offset) : "r11", "memory", "cc");
    asm volatile("rcall FUN_code_001053\n\trjmp LAB_code_000ff1"
                 : : "r" (requested), "r" (channel) : "memory", "cc");
    __builtin_unreachable();
}
