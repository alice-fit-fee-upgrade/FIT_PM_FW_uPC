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
