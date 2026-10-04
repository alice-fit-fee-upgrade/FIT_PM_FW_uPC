#include <stdint.h>

void pm_dac_gain_scale(void) asm("FUN_code_001068");
void pm_dac_gain_scale(void)
{
    register uint16_t requested asm("r20");
    asm volatile("" : "=r" (requested));
    register uint16_t coefficient asm("r18") = 20000;
    coefficient -= requested; asm volatile("" : "+r" (coefficient));
    requested = coefficient; asm volatile("" : "+r" (requested));
    coefficient = 0x0272;
    asm volatile("rcall FUN_code_0010a6" : "+r" (coefficient), "+r" (requested) : : "r0", "r1", "r16", "r17", "memory", "cc");
    register uint8_t channel asm("r22");
    asm volatile("" : "=r" (channel));
    channel += channel; asm volatile("" : "+r" (channel));
    channel += channel;
    asm volatile("rcall dac_send_value" : "+r" (channel) : : "memory", "cc");
}
