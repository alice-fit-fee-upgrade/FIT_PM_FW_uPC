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
