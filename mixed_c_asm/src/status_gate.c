#include <stdint.h>
/* Original 0x211C tests RAM 0x2157 bit 4. No interpretation of the
 * remaining status bits, and no changes to the original failure message. */
uint8_t pm_status_gate_allowed(uint8_t status)
{
    return (uint8_t)((status & 0x10u) != 0);
}
