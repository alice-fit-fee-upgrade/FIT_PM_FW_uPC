/* Retained exact call/load helpers: a C call such as
 * value = fpga_read(address); or fpga_write(address, word); expresses the
 * operation, but these entries use private bound registers, original CALL/RCALL
 * widths and shared error tails. These illustrative names are not compiled
 * interfaces and are not independently functionally validated alternatives.
 * Where a historically tested complete C alternative exists, its evidence
 * and bridge scope remain documented beside that helper. */
#include "legacy_cpu.h"
#include <stdint.h>
/* Private console/FPGA ABI: payload in R17:R16, register address in R18.
 * Carry, interrupt windows and the original wide CALL remain exact helpers. */
#define READ_PRINT(value, address) do { \
 asm volatile("" : "+r" (address) : : "memory"); \
 pm_cpu_disable_irq(); \
 asm volatile("rcall fpga_msg_read_t1" : "=r" (value), "+r" (address) : : "memory", "cc"); \
 pm_cpu_enable_irq(); \
 asm volatile("rcall cli_send_uint16" : "+r" (value) : : "memory", "cc"); \
} while (0)

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
