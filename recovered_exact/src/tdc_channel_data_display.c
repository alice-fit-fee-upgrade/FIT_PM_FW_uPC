#include "legacy_cpu.h"
#include <stdint.h>
#define READ_WORD(value, address) do { \
    asm volatile("" : "+r" (address) : : "memory"); \
    pm_cpu_disable_irq(); \
    asm volatile("rcall fpga_msg_read_t1" : "=r" (value), "+r" (address) : : "memory", "cc"); \
    pm_cpu_enable_irq(); \
} while (0)
/* Rejected step356 C fragment: sign_high = 0;
 * if (low & 0x80u) sign_high = (uint8_t)~sign_high;
 * The separate C conditional/complement changed full FLASH bytes.
 * This fragment has no independent successful functional-test claim. */
#define PRINT_SIGNED_BYTE(low) asm volatile("clr r17\n\tsbrc r16, 7\n\tcom r17\n\trcall cli_send_int16" : "+r" (low) : : "r17", "memory", "cc")
#define SEND_SPACE(low) do { (low) = ' '; asm volatile("rcall cli_send_buf" : "+r" (low) : : "memory", "cc"); } while (0)

/* Preserve the original signed byte display and unusual TDC coarse/fine
 * rounding; no arithmetic correction is made to the recovered baseline. */
void cli_send_tdc_data(void)
{
    register uint16_t word asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000c87\n"
                 "rcall FUN_code_00108e\n\tbrcc 1f\n\tret\n1:" : "=r" (word) : : "memory", "cc");
    register uint8_t address asm("r18") = 0x3e;
    asm volatile("" : "+r" (address));
    READ_WORD(word, address);
    register uint8_t high asm("r17");
    asm volatile("" : "=r" (high));
    register uint8_t saved_high asm("r20") = high;
    asm volatile("" : "+r" (saved_high));
    register uint8_t low asm("r16");
    asm volatile("" : "=r" (low));
    PRINT_SIGNED_BYTE(low);
    SEND_SPACE(low);
    low = saved_high;
    PRINT_SIGNED_BYTE(low);
    SEND_SPACE(low);
    address = 0x3f; asm volatile("" : "+r" (address));
    READ_WORD(word, address);
    asm volatile("" : "=r" (low));
    PRINT_SIGNED_BYTE(low);
    asm volatile("rcall cli_send_crlf" : : : "memory", "cc");
    register uint8_t *calibration asm("r28") = (uint8_t *)0x217b;
    asm volatile("" : "+y" (calibration));
    register uint8_t channel asm("r23");
    asm volatile("clr %0" : "=r" (channel) : : "cc");
next_channel:
    address = 0x40; asm volatile("" : "+r" (address));
    address += channel; asm volatile("" : "+r" (address));
    READ_WORD(word, address);
    /* Exact threshold/odd-bit corrections, two ASRs and carry injection. */
    asm volatile("rcall cli_send_32bit_hex\n\tcpi r16, 0x60\n\tbrlt 1f\n"
                 "sbrs r17, 0\n\tsubi r17, 2\n\trjmp 2f\n1:\n"
                 "cpi r16, 0x10\n\tbrcc 2f\n\tsbrc r17, 0\n\tsubi r17, 0xfe\n2:\n"
                 "asr r17\n\tasr r17\n\tbrcc 3f\n\tori r16, 0x80\n3:\n\tpush r16"
                 : "+r" (word) : : "memory", "cc");
    SEND_SPACE(low);
    asm volatile("pop r16\n\trcall cli_send_int16" : "=r" (low) : : "memory", "cc");
    SEND_SPACE(low);
    asm volatile("clr r24\n\tld r16, Y+\n\tadd r16, r16\n\tasr r16"
                 : "=r" (low), "+y" (calibration) : : "r24", "memory", "cc");
    PRINT_SIGNED_BYTE(low);
    SEND_SPACE(low);
    asm volatile("rcall cli_send_crlf\n\tinc %0" : "+r" (channel) : : "memory", "cc");
    asm goto("cpi %0, 12\n\tbrlt %l[next_channel]" : : "r" (channel) : "cc" : next_channel);
    asm volatile("rcall cli_send_crlf\n\trjmp LAB_code_000ff1" : : : "memory", "cc");
    __builtin_unreachable();
}
asm(".pushsection .text.cli_send_tdc_data,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
