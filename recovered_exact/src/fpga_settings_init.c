#include <avr/io.h>
#define RAM8(address) (*(volatile uint8_t *)(address))
#define SEND_SETTING(lo, hi, address) \
    asm volatile("cli\n\tcall fpga_msg_send_t2\n\tsei" \
        : "+r" (lo), "+r" (hi), "+r" (address) : : "memory", "cc")
#define READ_WORD(lo, hi, cursor) \
    asm volatile("ld %0, Y+\n\tld %1, Y+" \
        : "=r" (lo), "=r" (hi), "+y" (cursor) : : "memory")
#define NEXT_ADDRESS(address) asm volatile("inc %0" : "+r" (address) : : "cc")

void fpga_settings_init(void)
{
    /* Original asymmetric frame saves R30 but not YL; preserve it as recorded. */
    asm volatile("push r18\n\tpush r17\n\tpush r16\n\tpush r29\n\tpush r30" : : : "memory");
    register uint8_t address asm("r18") = 0x7c;
    register uint8_t lo asm("r16"), hi asm("r17");
    asm volatile("clr %0\n\tclr %1" : "=r" (lo), "=r" (hi) : "r" (address) : "cc");
    SEND_SETTING(lo, hi, address);
    lo = RAM8(0x222f);
    asm volatile("clr %0\n\tclr %1" : "=r" (hi), "=r" (address) : "r" (lo) : "cc");
    SEND_SETTING(lo, hi, address);
    register const uint8_t *cursor asm("r28") = (const uint8_t *)0x21b7;
    asm volatile("" : "+y" (cursor));
first_bank:
    READ_WORD(lo, hi, cursor);
    NEXT_ADDRESS(address);
    SEND_SETTING(lo, hi, address);
    asm goto("cpi %0, 0x0c\n\tbrne %l[first_bank]" : : "r" (address) : "cc" : first_bank);
    address = 0x24;
    asm volatile("" : "+r" (address));
    cursor = (const uint8_t *)0x2187;
    asm volatile("" : "+y" (cursor));
second_bank:
    READ_WORD(lo, hi, cursor);
    NEXT_ADDRESS(address);
    SEND_SETTING(lo, hi, address);
    asm goto("cpi %0, 0x3c\n\tbrne %l[second_bank]" : : "r" (address) : "cc" : second_bank);
    NEXT_ADDRESS(address);
    lo = RAM8(0x2230); hi = RAM8(0x2231);
    SEND_SETTING(lo, hi, address);
    lo = RAM8(0x2160); hi = RAM8(0x2161);
    address = 0xbc;
    SEND_SETTING(lo, hi, address);
    lo = RAM8(0x2232); hi = RAM8(0x2233);
    address = 0xbd;
    SEND_SETTING(lo, hi, address);
    address = 0x7c;
    lo = 0xff; hi = 0x0f;
    SEND_SETTING(lo, hi, address);
    asm volatile("pop r30\n\tpop r29\n\tpop r16\n\tpop r17\n\tpop r18\n\tret" : : : "memory");
    __builtin_unreachable();
}
