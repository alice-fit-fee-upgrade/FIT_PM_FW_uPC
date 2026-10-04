#include <stdint.h>
/* Private console/FPGA ABI: payload in R17:R16, register address in R18.
 * Carry, interrupt windows and the original wide CALL remain exact helpers. */
#define READ_PRINT(value, address) \
 asm volatile("cli\n\trcall fpga_msg_read_t1\n\tsei\n\trcall cli_send_uint16" \
 : "=r" (value), "+r" (address) : : "memory", "cc")

void cli_send_adc_baseline_dispersion(void)
{
    register uint16_t value asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000c87\n"
                 "rcall FUN_code_00108e\n\tbrcc 1f\n\tret\n1:" : "=r" (value) : : "memory", "cc");
    register uint8_t channel_offset asm("r19");
    asm volatile("clr %0" : "=r" (channel_offset) : : "cc");
next_channel:;
    register uint8_t address asm("r18") = 0x0d;
    asm volatile("" : "+r" (address));
    address += channel_offset;
    asm volatile("" : "+r" (address));
    READ_PRINT(value, address);
    register uint8_t separator asm("r16") = ' ';
    asm volatile("rcall cli_send_buf\n\tinc %1" : "+r" (separator), "+r" (address) : : "memory", "cc");
    READ_PRINT(value, address);
    separator = ' ';
    asm volatile("rcall cli_send_buf" : "+r" (separator) : : "memory", "cc");
    address = 0x4c;
    asm volatile("" : "+r" (address));
    address += channel_offset;
    asm volatile("" : "+r" (address));
    READ_PRINT(value, address);
    separator = ' ';
    asm volatile("rcall cli_send_buf\n\tinc %1" : "+r" (separator), "+r" (address) : : "memory", "cc");
    READ_PRINT(value, address);
    asm volatile("rcall cli_send_crlf" : : : "memory", "cc");
    channel_offset += 2;
    asm volatile("" : "+r" (channel_offset));
    asm goto("cpi %0, 24\n\tbrcs %l[next_channel]" : : "r" (channel_offset) : "cc" : next_channel);
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}
asm(".pushsection .text.cli_send_adc_baseline_dispersion,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
