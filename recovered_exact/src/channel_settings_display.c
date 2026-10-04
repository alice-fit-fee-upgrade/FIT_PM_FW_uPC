#include <stdint.h>
#define MESSAGE(cursor) asm volatile("rcall cli_send_msg" : "+z" (cursor) : : "memory", "cc")
#define OFFSET_POINTER(pointer, offset, zero) \
 asm volatile("add r28, %1\n\tadc r29, %2" : "+y" (pointer) : "r" (offset), "r" (zero) : "cc")
#define READ_PAIR(last_load, formatter, tail) \
 asm volatile("ld r16, Y+\n\t" last_load "\n\trcall " formatter "\n\trcall " tail \
 : "=r" (word), "+y" (settings), "+z" (message) : : "memory", "cc")

void channels_read(void)
{
    register uint16_t word asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000e30" : "=r" (word) : : "memory", "cc");
    register uint8_t channel asm("r20"), zero asm("r21");
    asm volatile("clr %0\n\tclr %1" : "=r" (channel), "=r" (zero) : : "cc");
next_channel:;
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2ac6;
    MESSAGE(message);
    register uint8_t low asm("r16") = channel;
    asm volatile("" : "+r" (low), "+r" (channel));
    asm volatile("clr r17\n\trcall cli_send_uint16\n\trcall cli_send_msg" : "+r" (low), "+z" (message) : : "r17", "memory", "cc");
    register uint8_t *settings asm("r28") = (uint8_t *)0x2163;
    asm volatile("" : "+y" (settings));
    register uint8_t offset asm("r22") = channel;
    asm volatile("" : "+r" (offset), "+r" (channel));
    offset += offset; asm volatile("" : "+r" (offset), "+r" (channel));
    OFFSET_POINTER(settings, offset, zero);
    READ_PAIR("ld r17, Y", "cli_send_uint16", "cli_send_msg");
    register uint8_t tdc_channel asm("r23") = channel;
    asm volatile("" : "+r" (tdc_channel), "+r" (channel));
    tdc_channel &= 3; asm volatile("" : "+r" (tdc_channel), "+r" (channel));
    register uint8_t spacing asm("r24") = 0x20;
    asm volatile("mul %0, %1" : : "r" (tdc_channel), "r" (spacing) : "r0", "r1", "cc");
    low = 0x0c;
    asm volatile("add %0, r0" : "+r" (low) : : "cc");
    register uint8_t device asm("r19") = channel;
    asm volatile("" : "+r" (device), "+r" (channel));
    asm volatile("lsr %0\n\tlsr %0\n\tcli\n\trcall ths788_read\n\tsei"
                 : "+r" (device), "=r" (word) : "r" (low) : "memory", "cc");
    register uint8_t high asm("r17");
    asm volatile("" : "=r" (high));
    low = high;
    asm volatile("clr r17\n\tsbrc r16, 7\n\tldi r17, 0xff\n\trcall cli_send_int16\n\trcall cli_send_msg"
                 : "+r" (low), "+z" (message) : : "r17", "memory", "cc");
    settings = (uint8_t *)0x21b7;
    asm volatile("" : "+y" (settings));
    offset = channel; asm volatile("" : "+r" (offset), "+r" (channel));
    offset += offset; asm volatile("" : "+r" (offset), "+r" (channel));
    OFFSET_POINTER(settings, offset, zero);
    READ_PAIR("ld r17, Y+", "cli_send_int16", "cli_send_msg");
    settings = (uint8_t *)0x2187;
    asm volatile("" : "+y" (settings));
    offset = channel; asm volatile("" : "+r" (offset), "+r" (channel));
    offset += offset; asm volatile("" : "+r" (offset), "+r" (channel));
    offset += offset; asm volatile("" : "+r" (offset), "+r" (channel));
    OFFSET_POINTER(settings, offset, zero);
    asm volatile("ld r16, Y+\n\tld r17, Y+\n\trcall cli_send_uint16" : "=r" (word), "+y" (settings) : : "memory", "cc");
    low = ' ';
    asm volatile("rcall cli_send_buf" : "+r" (low) : : "memory", "cc");
    READ_PAIR("ld r17, Y", "cli_send_uint16", "cli_send_crlf");
    asm volatile("inc %0" : "+r" (channel) : : "cc");
    asm goto("cpi %0, 12\n\tbreq 1f\n\trjmp %l[next_channel]\n1:" : : "r" (channel) : "cc" : next_channel);
}
asm(".pushsection .text.channels_read,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
