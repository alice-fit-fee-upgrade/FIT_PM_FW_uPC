#include "legacy_console_call_c.h"
#include <stdint.h>

/* Legacy result is R21:R20, delimiter R16, validity in carry.
 * Original accepts up to four uppercase hex digits and wraps at 16 bits. */
void cli_get_hex(void)
{
    register uint8_t count asm("r18"), character asm("r16");
    register uint16_t result asm("r20");
    asm volatile("push r18\n\tclr r18\n\tclr r20\n\tclr r21"
                 : "=r" (count), "=r" (result) : : "memory", "cc");
next_digit:;
    character = pm_console_read_character();
    register uint8_t digit asm("r22") = character;
    asm volatile("" : "+r" (digit), "+r" (character));
    /* Rejected C range alternative (step_235_rejected.log):
     * if (character < '0') goto done;
     * if (character < ':') goto decode;
     * if (character < 'A') goto done;
     * if (character >= 'G') goto done;
     * It changed complete binary/layout matching. No independent functional
     * test is claimed for this fragment; retain original branches below.
 * Validated flag-aware transition model: tests/logical_models/console_hex_parser.c,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("cpi r16, 0x30\n\tbrcs %l[done]\n\tcpi r16, 0x3a\n\tbrcs %l[decode]\n"
             "cpi r16, 0x41\n\tbrcs %l[done]\n\tcpi r16, 0x47\n\tbrcc %l[done]"
             : : "r" (character) : "cc" : done, decode);
    digit -= 7; asm volatile("" : "+r" (digit));
decode:
    digit -= '0'; asm volatile("" : "+r" (digit));
    result += result; asm volatile("" : "+r" (result));
    result += result; asm volatile("" : "+r" (result));
    result += result; asm volatile("" : "+r" (result));
    result += result; asm volatile("" : "+r" (result));
    result |= digit; asm volatile("" : "+r" (result));
    /* C equivalent: ++count; trial436 changed the exact encoding/layout.
     * Unvalidated standalone; retain original INC and its flags.
 * Validated flag-aware transition model: tests/logical_models/console_hex_parser.c,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("inc %0" : "+r" (count) : : "cc");
    if (count != 4) goto next_digit;
    character = pm_console_read_character();
done:
    asm volatile("tst r18\n\tbreq 1f\n\tclc\n\trjmp 2f\n1:\n\tsec\n2:\n"
                 "pop r18" : : "r" (count), "r" (result) : "memory", "cc");
    return;
}
