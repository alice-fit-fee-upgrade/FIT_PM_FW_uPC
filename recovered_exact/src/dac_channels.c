#include <stdint.h>

#define DAC_CHANNEL(name, selector) \
void name(void) { \
    register uint16_t scale asm("r18") = 0x4188; \
    asm volatile("rcall fpga_is_ready" : "+r" (scale) : \
                 : "r0", "r1", "r16", "r17", "memory", "cc"); \
    register uint8_t sign_mask asm("r18") = 0x80; \
    asm volatile("eor r17, %0" : : "r" (sign_mask) : "r17", "cc"); \
    register uint8_t channel asm("r22"); \
    asm volatile("" : "=r" (channel)); \
    channel += channel; \
    asm volatile("" : "+r" (channel)); \
    channel += channel; \
    asm volatile("" : "+r" (channel)); \
    channel |= (selector); \
    asm volatile("rcall dac_send_value" : "+r" (channel) : \
                 : "r16", "r17", "r19", "r20", "r21", "memory", "cc"); \
}
/* Original helper's name is historical: fpga_is_ready performs multiplication. */
DAC_CHANNEL(dac_set_value_2, 2)
DAC_CHANNEL(dac_set_value, 1)
