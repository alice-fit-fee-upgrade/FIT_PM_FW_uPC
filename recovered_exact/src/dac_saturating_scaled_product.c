#include <stdint.h>

/* Archived C alternative for the exact multiply/round/saturate ASM below.
 * pm_scale_unsigned_abi was integrated with a private-ABI ASM bridge in the
 * historical binary-different mixed build. Recorded functional validation:
 * docs/mixed_unsigned_checkpoint.md and docs/mixed_recovery_wave_checkpoint.md
 * (796,432 direct comparisons). The bridge restores registers, R1:R0 and SREG;
 * this returned value/flags packet alone is not the original CPU ABI.
 * Dependencies: source_recovery/src/scaling.c and pm_recovered.h.
 * No timing, asynchronous IRQ or hardware equivalence is established.
 * This alternative is a comment, not an accepted binary-exact implementation.
 *
 * uint32_t pm_scale_unsigned_abi(uint16_t coefficient, uint16_t input,
 *                              uint8_t entry_sreg)
 * {
 *     uint16_t low = (uint16_t)((coefficient & 255u) * (input & 255u));
 *     uint16_t middle = (uint16_t)((low >> 8) + ((low & 128u) != 0));
 *     middle = (uint16_t)(middle + (coefficient >> 8) * (input & 255u));
 *     uint16_t cross = (uint16_t)((coefficient & 255u) * (input >> 8));
 *     uint8_t carry = (uint16_t)((middle & 255u) + (cross & 255u)) > 255u;
 *     uint8_t half = ((middle >> 8) & 15u) + ((cross >> 8) & 15u) + carry > 15u;
 *     middle = (uint16_t)(middle + cross);
 *     uint16_t top = (uint16_t)((coefficient >> 8) * (input >> 8));
 *     uint8_t flags;
 *     uint16_t value;
 *     if (top >> 8) {
 *         uint8_t negative = (uint8_t)(top >> 15);
 *         flags = (uint8_t)(negative * 0x15u + half * 0x20u);
 *         value = 65535u;
 *     } else {
 *         uint8_t a = (uint8_t)(middle >> 8), b = (uint8_t)top;
 *         uint16_t sum = (uint16_t)a + b;
 *         uint8_t result = (uint8_t)sum;
 *         uint8_t negative = result >> 7;
 *         uint8_t overflow = (uint8_t)((~(a ^ b) & (a ^ result)) >> 7);
 *         flags = (uint8_t)((sum > 255u) | ((result == 0) << 1) |
 *                 (negative << 2) | (overflow << 3) |
 *                 ((negative ^ overflow) << 4) |
 *                 ((((a & 15u) + (b & 15u)) > 15u) << 5));
 *         value = sum > 255u ? 65535u : (uint16_t)((middle & 255u) | (sum << 8));
 *     }
 *     return (uint32_t)value | ((uint32_t)(flags | (entry_sreg & 0xc0u)) << 16);
 * }
 */

/* Exact scaled product uses MUL and live R1, including its original rounding.
 * C represents the overflow result; the arithmetic/flag helper is unchanged. */
void FUN_code_0010a6(void)
{
    register uint16_t result asm("r16");
    asm volatile("clr r17\n\tmul r18, r20\n\tsbrc r0, 7\n\tinc r1\n\tmov r16, r1\n"
                 "mul r19, r20\n\tadd r16, r0\n\tadc r17, r1\n"
                 "mul r18, r21\n\tadd r16, r0\n\tadc r17, r1\n\tmul r19, r21"
                 : "=r" (result) : : "r0", "r1", "cc");
    asm goto("tst r1\n\tbrne %l[overflow]\n\tadd r17, r0\n\tbrcs %l[overflow]" : : : "cc" : overflow);
    asm volatile("ret"); __builtin_unreachable();
overflow:
    result = UINT16_MAX;
    asm volatile("" : : "r" (result));
}
