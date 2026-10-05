#include <stdint.h>

/* Original 0x2130: historical symbol fpga_is_ready is a signed scaler.
 * Keep MUL/MULSU, rounding and carry propagation exact; C expresses the
 * low-byte result copy and final high-byte sum. R1 is intentionally live.
 * Archived, binary-different C alternative below passed 796,432 direct
 * functional comparisons with its ASM bridge (docs/mixed_c_asm_checkpoint.md).
 * The bridge restores the private registers/flags; no timing, asynchronous
 * IRQ or hardware equivalence is established. Not compiled in this baseline.
 * source_recovery/src/scaling.c:
 * uint16_t pm_scale_signed(uint16_t coefficient, uint16_t input)
 * {
 *     int32_t value = input < 0x8000u ? (int32_t)input : (int32_t)input - 65536;
 *     int32_t product = value * coefficient;
 *     // Only bits 8..23 survive the 16-bit return. Unsigned shifting of * the modulo-2^32 product has exactly those bits for either sign; * no implementation-defined signed shift or runtime division is needed.
 *     uint32_t rounded_bits = (uint32_t)product + 128u;
 *     return (uint16_t)(rounded_bits >> 8);
 * }
 */
void fpga_is_ready(void)
{
    register uint8_t low asm("r16"), high asm("r17"), product_high asm("r1"), product_low asm("r0");
    asm volatile("clr r17\n\tmul r18, r20\n\tsbrc r0, 7\n\tinc r1"
                 : "=r" (high), "=r" (product_high) : : "r0", "cc");
    low = product_high;
    asm volatile("" : "+r" (low));
    asm volatile("mul r19, r20\n\tadd r16, r0\n\tadc r17, r1\n"
                 "mulsu r21, r18\n\tadd r16, r0\n\tadc r17, r1\n\tmulsu r21, r19"
                 : "+r" (low), "+r" (high), "=r" (product_low) : : "r1", "cc");
    high += product_low;
    asm volatile("" : : "r" (low), "r" (high));
}
