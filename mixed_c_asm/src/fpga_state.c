#include <stdint.h>
#define REG8(a) (*(volatile uint8_t *)(a))
/* Original 0x0A9C: preserve state writes and the final AND/CPI flags. */
uint16_t pm_fpga_set_state(uint8_t state, uint8_t incoming_sreg)
{
    REG8(0x215b) = state;
    REG8(0x0626) = 0x80;
    uint8_t previous = REG8(0x2159);
    uint8_t flags;
    if (previous == 0) {
        flags = (incoming_sreg & 0xe1u) | 2u;
    } else {
        uint8_t result = (uint8_t)(previous - 5u);
        uint8_t n = result >> 7;
        uint8_t v = ((previous ^ 5u) & (previous ^ result)) >> 7;
        flags = (incoming_sreg & 0xc0u) | (previous < 5u)
              | ((result == 0) << 1) | (n << 2) | (v << 3)
              | ((n ^ v) << 4) | ((previous % 16u < 5u) << 5);
    }
    if (previous == 0 || previous == 5) {
        previous = 4;
        REG8(0x2159) = previous;
    }
    return (uint16_t)previous | ((uint16_t)flags << 8);
}
