#include <stdint.h>

void pm_dac_offset_scale(void) asm("FUN_code_001053");
void pm_dac_offset_scale(void)
{
    register uint16_t coefficient asm("r18") = 0x020c;
    register uint16_t value asm("r16");
    asm volatile("rcall FUN_code_0010a6" : "+r" (coefficient), "=r" (value) : : "r0", "r1", "memory", "cc");
    register const uint8_t *cursor asm("r30") = (const uint8_t *)0x2163;
    asm volatile("" : "+z" (cursor));
    register uint16_t correction asm("r20");
    asm volatile("clr r20" : "=r" (correction) : : "cc");
    register uint8_t channel asm("r22");
    asm volatile("" : "=r" (channel));
    channel += channel; asm volatile("" : "+r" (channel));
    asm volatile("add r30, %1\n\tadc r31, r20" : "+z" (cursor) : "r" (channel), "r" (correction) : "cc");
    channel += channel; asm volatile("" : "+r" (channel));
    channel |= 3; asm volatile("" : "+r" (channel));
    asm volatile("ld r20, Z+\n\tld r21, Z" : "=r" (correction), "+z" (cursor) : : "memory");
    value += correction; asm volatile("" : "+r" (value));
    value = ~value;
    asm volatile("rcall dac_send_value" : "+r" (value), "+r" (channel) : : "memory", "cc");
}
