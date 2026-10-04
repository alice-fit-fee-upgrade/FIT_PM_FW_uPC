#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))
#define PRINT_MESSAGE(cursor) asm volatile("rcall cli_send_msg" : "+z" (cursor) : : "memory", "cc")
/* Two volatile Y+ loads must remain between the original CLI/SEI pair. */
#define PRINT_SETTING(format, tail) \
 asm volatile("cli\n\tld r16, Y+\n\tld r17, Y+\n\tsei\n\trcall " format "\n\trcall " tail \
 : "+y" (settings), "=r" (value), "+z" (message) : : "memory", "cc")

void cli_send_channel_cdf_adc(void)
{
    register uint16_t value asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000e78"
                 : "=r" (value) : : "memory", "cc");
    register uint8_t *settings asm("r28") = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    register uint8_t channel asm("r20");
    asm volatile("clr %0" : "=r" (channel) : : "cc");
next_channel:;
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2a68;
    PRINT_MESSAGE(message);
    register uint8_t low asm("r16") = channel;
    asm volatile("clr r17\n\trcall cli_send_uint16\n\trcall cli_send_msg"
                 : "+r" (low), "+z" (message) : : "r17", "memory", "cc");
    PRINT_SETTING("FUN_code_001397", "cli_send_msg");
    PRINT_SETTING("FUN_code_001397", "cli_send_msg");
    PRINT_SETTING("FUN_code_001397", "cli_send_msg");
    PRINT_SETTING("FUN_code_00139f", "cli_send_crlf");
    asm volatile("inc %0" : "+r" (channel) : : "cc");
    asm goto("cpi %0, 12\n\tbrne %l[next_channel]" : : "r" (channel) : "cc" : next_channel);
    message = (const uint8_t *)0x2a92;
    PRINT_MESSAGE(message);
    low = RAM8(0x222f);
    asm volatile("clr r17\n\trcall cli_send_uint16\n\trcall cli_send_msg"
                 : "+r" (low), "+z" (message) : : "r17", "memory", "cc");
    low = RAM8(0x2230);
    asm volatile("" : "+r" (low));
    register uint8_t high asm("r17") = RAM8(0x2231);
    asm volatile("rcall cli_send_uint16\n\trcall cli_send_crlf"
                 : "+r" (low), "+r" (high) : : "memory", "cc");
}
asm(".pushsection .text.cli_send_channel_cdf_adc,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
