#include <stdint.h>
/* Application 0x2720..0x2728: R16 -> ASCII; terminal send is at 0x272a.
 * C ABI differs from original register ABI. This is a standalone recovery
 * candidate, not linked into the exact image. Preserve low-nibble truncation.
 */
uint8_t pm_hex_digit(uint8_t value)
{
    uint8_t digit = (uint8_t)((value & 15u) + 0x30u);
    if (digit >= 0x3au)
        digit = (uint8_t)(digit + 7u);
    return digit;
}
