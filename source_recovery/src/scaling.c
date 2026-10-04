#include "pm_recovered.h"
/* 0x2130..0x214B: unsigned coefficient times signed input /256,
 * round using low-product bit7, retain modulo-65536 result. */
uint16_t pm_scale_signed(uint16_t coefficient, uint16_t input)
{
    int32_t value = input < 0x8000u ? (int32_t)input : (int32_t)input - 65536;
    int32_t product = value * coefficient;
    /* Division rounds toward zero in C; AVR truncates low byte toward -infinity. */
    int32_t rounded = product + 128;
    int32_t result = rounded >= 0 ? rounded / 256 : -((-rounded + 255) / 256);
    return (uint16_t)result;
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
