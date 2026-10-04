#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))
/* Page programming uses the original 24-bit counter in R22:R20 and limit
 * in R2:R0. R1 is live state, so no GNU arithmetic helper is introduced. */
void FUN_code_000b59(void)
{
    asm volatile("rcall FUN_code_000b8f" : : : "memory", "cc");
    register uint8_t byte asm("r16") = 2;
    asm volatile("rcall FUN_code_000c04" : "+r" (byte) : : "memory", "cc");
    register uint16_t read_index asm("r24") = *(volatile uint16_t *)0x2437;
    asm volatile("" : "+r" (read_index));
next_byte:;
    register uint8_t *buffer asm("r26") = (uint8_t *)0x2235;
    asm volatile("" : "+x" (buffer));
    buffer += read_index;
    asm volatile("ld %0, X" : "=r" (byte) : "x" (buffer) : "memory");
    read_index += 1; asm volatile("" : "+r" (read_index));
    read_index &= 0x01ff; asm volatile("" : "+r" (read_index));
    RAM8(0x0ac3) = byte;
    asm volatile("1: lds r19, 0x0ac2\n\tsbrs r19, 7\n\trjmp 1b" : : : "r19", "memory");
    asm goto("cp r0, r20\n\tcpc r1, r21\n\tcpc r2, r22\n\tbrne %l[continue_page]" : : : "cc" : continue_page);
    RAM8(0x2437) = (uint8_t)read_index;
    RAM8(0x2438) = read_index >> 8;
    byte = 0x10;
    asm volatile("" : "+r" (byte));
    RAM8(0x0685) = byte;
    asm volatile("sec\n\tret" : : : "cc");
    __builtin_unreachable();
continue_page:
    asm volatile("subi r20, 0xff\n\tsbci r21, 0xff\n\tsbci r22, 0xff" : : : "memory", "cc");
    asm goto("tst r20\n\tbrne %l[next_byte]" : : : "cc" : next_byte);
    RAM8(0x2437) = (uint8_t)read_index;
    RAM8(0x2438) = read_index >> 8;
    byte = 0x10;
    asm volatile("" : "+r" (byte));
    RAM8(0x0685) = byte;
    asm volatile("clc");
}
