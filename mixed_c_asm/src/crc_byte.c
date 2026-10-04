#include <stdint.h>
typedef struct {
    uint32_t crc;
    uint8_t flags;
    uint8_t reserved[3];
} pm_crc_packet;

/* Original byte 0x17E4: shift state LEFT, consume input byte LSB first.
 * Polynomial is caller supplied; do not substitute a library CRC convention.
 * Final DEC leaves Z=1,N=V=S=0, retaining C from the last LSR and H from
 * the last high-byte ADC. The bridge preserves caller T/I separately. */
pm_crc_packet pm_crc_byte_abi(uint32_t crc, uint32_t polynomial, uint8_t byte)
{
    uint8_t last_half = 0;
    uint8_t last_carry = (uint8_t)(byte >> 7);
    for (uint8_t bit = 0; bit < 8; ++bit) {
        last_half = (uint8_t)((uint8_t)(crc >> 24) & 8u);
        uint8_t feedback = (uint8_t)((crc >> 31) ^ (byte & 1u));
        crc <<= 1;
        if (feedback) crc ^= polynomial;
        byte >>= 1;
    }
    pm_crc_packet p;
    p.crc = crc;
    p.flags = (uint8_t)(2u | last_carry | (last_half << 2));
    p.reserved[0] = p.reserved[1] = p.reserved[2] = 0;
    return p;
}
