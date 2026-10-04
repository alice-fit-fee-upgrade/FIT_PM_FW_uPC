#include <stdint.h>
/* Private console/FPGA ABI: payload in R17:R16, register address in R18.
 * Carry, interrupt windows and the original wide CALL remain exact helpers. */
#define READ_PRINT(value, address) \
 asm volatile("cli\n\trcall fpga_msg_read_t1\n\tsei\n\trcall cli_send_uint16" \
 : "=r" (value), "+r" (address) : : "memory", "cc")

void cli_send_ch_mean_amplitude(void)
{
    register uint16_t value asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000c87\n"
                 "rcall FUN_code_00108e\n\tbrcc 1f\n\tret\n1:"
                 : "=r" (value) : : "memory", "cc");
    register uint8_t address asm("r18") = 0x64;
    asm volatile("" : "+r" (address));
next_channel:
    READ_PRINT(value, address);
    register uint8_t separator asm("r16") = ' ';
    asm volatile("call cli_send_buf" : "+r" (separator) : : "memory", "cc");
    asm volatile("inc %0" : "+r" (address) : : "cc");
    READ_PRINT(value, address);
    asm volatile("rcall cli_send_crlf\n\tinc %0" : "+r" (address) : : "memory", "cc");
    asm goto("cpi %0, 0x7c\n\tbrcs %l[next_channel]" : : "r" (address) : "cc" : next_channel);
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}
