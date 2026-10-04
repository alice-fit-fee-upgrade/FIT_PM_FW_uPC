#include "pm_recovered.h"
/* 0x2130..0x214B: unsigned coefficient times signed input /256,
 * round using low-product bit7, retain modulo-65536 result. */
uint16_t pm_scale_signed(uint16_t coefficient, uint16_t input)
{
    int32_t value = input < 0x8000u ? (int32_t)input : (int32_t)input - 65536;
    int32_t product = value * coefficient;
    /* Only bits 8..23 survive the 16-bit return. Unsigned shifting of
     * the modulo-2^32 product has exactly those bits for either sign;
     * no implementation-defined signed shift or runtime division is needed. */
    uint32_t rounded_bits = (uint32_t)product + 128u;
    return (uint16_t)(rounded_bits >> 8);
}
/* 0x214C..0x2173: preserve ORIGINAL staged saturation. Carry from the
 * penultimate addition is not used by the final test; retain the original
 * staged order rather than infer an alternative saturation contract. */
uint16_t pm_scale_unsigned(uint16_t coefficient, uint16_t input)
{
    uint16_t low = (uint16_t)((coefficient & 255u) * (input & 255u));
    uint16_t result = (uint16_t)((low >> 8) + ((low & 128u) != 0));
    result = (uint16_t)(result + (coefficient >> 8) * (input & 255u));
    result = (uint16_t)(result + (coefficient & 255u) * (input >> 8));
    uint16_t top = (uint16_t)((coefficient >> 8) * (input >> 8));
    if (top > 255u || (result >> 8) + top > 255u) return 65535u;
    return (uint16_t)(result + (top << 8));
}

/* Original unsigned-scaler CPU ABI: low word is the result, byte 2 is
 * SREG. The high-byte ADC half carry survives the early saturation path.
 * T and I belong to the caller. This adapter computes flags in C rather
 * than relying on whichever instructions the compiler chooses. */
uint32_t pm_scale_unsigned_abi(uint16_t coefficient, uint16_t input,
                             uint8_t entry_sreg)
{
    uint16_t low = (uint16_t)((coefficient & 255u) * (input & 255u));
    uint16_t middle = (uint16_t)((low >> 8) + ((low & 128u) != 0));
    middle = (uint16_t)(middle + (coefficient >> 8) * (input & 255u));
    uint16_t cross = (uint16_t)((coefficient & 255u) * (input >> 8));
    uint8_t carry = (uint16_t)((middle & 255u) + (cross & 255u)) > 255u;
    uint8_t half = ((middle >> 8) & 15u) + ((cross >> 8) & 15u) + carry > 15u;
    middle = (uint16_t)(middle + cross);
    uint16_t top = (uint16_t)((coefficient >> 8) * (input >> 8));
    uint8_t flags;
    uint16_t value;
    if (top >> 8) {
        uint8_t negative = (uint8_t)(top >> 15);
        flags = (uint8_t)(negative * 0x15u + half * 0x20u);
        value = 65535u;
    } else {
        uint8_t a = (uint8_t)(middle >> 8), b = (uint8_t)top;
        uint16_t sum = (uint16_t)a + b;
        uint8_t result = (uint8_t)sum;
        uint8_t negative = result >> 7;
        uint8_t overflow = (uint8_t)((~(a ^ b) & (a ^ result)) >> 7);
        flags = (uint8_t)((sum > 255u) | ((result == 0) << 1) |
                (negative << 2) | (overflow << 3) |
                ((negative ^ overflow) << 4) |
                ((((a & 15u) + (b & 15u)) > 15u) << 5));
        value = sum > 255u ? 65535u : (uint16_t)((middle & 255u) | (sum << 8));
    }
    return (uint32_t)value | ((uint32_t)(flags | (entry_sreg & 0xc0u)) << 16);
}
