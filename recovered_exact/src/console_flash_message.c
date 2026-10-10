#include "legacy_console_call_c.h"
#include <stdint.h>

void cli_send_msg(void)
{
    register const uint8_t *cursor asm("r30");
    asm volatile("" : "=z" (cursor));
next_character:
    {
        register uint8_t byte asm("r16");
        /* C FLASH read equivalent with const __flash uint8_t *cursor:
         * byte = *cursor++;
         * Trials 239-249 could not reproduce the private pointer/register layout;
         * keep exact LPM Z+ instructions. This is explanatory, with no new
         * independent functional-test claim. Existing historical test scope,
         * when available, is documented above. */
        asm volatile("lpm %0, Z+" : "=r" (byte), "+z" (cursor) : : "memory");
        /* Unvalidated C equivalent: if (!byte) goto finished; step473
         * changed layout. Keep the original terminator flags/branch.
 * Validated flag-aware transition model: tests/logical_models/console_flash_message.c,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
        if (!byte) goto finished;
        byte = pm_console_send_character(byte);
    }
    goto next_character;
finished:
    /* Cursor remains advanced, including the consumed terminator. */
    asm volatile("" : : "z" (cursor));
}
