#include "legacy_word_ops.h"
#include "legacy_cpu.h"
#include "legacy_cli_guard_c.h"
/* Private Y cursor: register reservation, no SRAM object or GNU frame. */
register uint8_t *settings asm("r28");

void fpga_set_ch_cfd_threshold(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f9c");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000f9c");
    register uint8_t limit_high asm("r24") = 0x75;
    asm volatile("" : "+r" (limit_high));
    /* Reject values at or above 30001 through the original error tail. */
    PM_REJECT_I16_GE(requested, limit_high, 30001, "LAB_code_000f9c");
    limit_high = 1;
    asm volatile("" : "+r" (limit_high));
    /* Reject values below 300 through the original error tail. */
    PM_REJECT_I16_LT(requested, limit_high, 300, "LAB_code_000f9c");
    PM_FPGA_GUARD(requested);
    register uint16_t word asm("r16") = requested;
    asm volatile("" : "+r" (word));
    register uint8_t offset asm("r10");
    offset = channel;
    asm volatile("" : "+r" (offset));
    offset += offset;
    asm volatile("" : "+r" (offset));
    offset += offset;
    asm volatile("" : "+r" (offset));
    register uint8_t address asm("r18") = 0x80;
    asm volatile("" : "+r" (address));
    address += offset;
    asm volatile("" : "+r" (word), "+r" (address) : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : "+r" (word), "+r" (address) : : "memory", "cc");
    pm_cpu_enable_irq();
    settings = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    offset += offset;
    asm volatile("" : "+r" (offset), "+y" (settings));
    /* C pointer value: settings += offset; CLR/ADC retain live flags. */
    asm volatile("clr r11" : : : "r11", "cc");
    {
        register uint8_t address_low asm("r28");
        asm volatile("" : "=r" (address_low) : "y" (settings));
        address_low += offset; asm volatile("" : "+r" (address_low));
    }
    asm volatile("adc r29, r11" : "=y" (settings), "+r" (offset) : : "r11", "cc");
    PM_STORE_WORD_LE_Y(settings, requested);
    asm volatile("rcall FUN_code_001053" : : "r" (requested), "r" (channel) : "memory", "cc");
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2998;
    asm volatile("rcall cli_send_msg" : "+z" (message) : : "memory", "cc");
}
asm(".pushsection .text.fpga_set_ch_cfd_threshold,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
