/* Native outer frame: GCC emits the original entry PUSH under the local
 * call-saved register profile. -fno-ipa-pure-const prevents noreturn inference
 * from dropping that save before the exact shared RET/RETI tail. The tail
 * restores this register in ASM; full FLASH identity validates the pairing. */
#include <avr/io.h>
/* Calls retain private fixed-register inputs/results through zero-byte
 * barriers. Void declarations deliberately introduce no GNU arguments/results;
 * recapture after each call observes the original registers. Every conversion
 * is accepted only if GNU CALL, frames and the entire FLASH remain identical. */
extern void CDCE62005_send_control_settings(void);
extern void FUN_code_001267(void);
#include "legacy_cpu.h"
#include "legacy_interrupt.h"
#include "legacy_r16_c.h"
#define WRITE_STATE(address, constant) do { value = (constant); \
    asm volatile("" : "+r" (value)); PM_RAM8(address) = value; } while (0)

void PORTF_INT1_vect_isr(void)
{
    {
        register uint8_t saved_status asm("r31") = SREG;
        asm volatile("" : "+r" (saved_status) : : "memory");
    }
    asm volatile("push r16\n\tpush r17\n\tpush r18\n\tpush r19" : : : "memory");
    register uint8_t value asm("r16") = PORTF_IN;
    register uint8_t b1 asm("r17"), b2 asm("r18"), status asm("r19");
    asm goto("bst %0, 6\n\tbrtc %l[read_status]\n\tsbrc %0, 5\n\trjmp %l[read_status]" : : "r" (value) : "cc" : read_status);
    value = 0xf5; asm volatile("" : "+r" (value));
    b1 = 0x0f; asm volatile("" : "+r" (b1));
    b2 = 4; asm volatile("" : "+r" (b2));
    status = 0x40;
    do {
        asm volatile("" : "+r" (value), "+r" (b1), "+r" (b2), "+r" (status) :  : "memory");
        CDCE62005_send_control_settings();
        asm volatile("" : "=r" (value), "=r" (b1), "=r" (b2), "=r" (status) : : "memory");
    } while (0);
    value = 0x20; asm volatile("" : "+r" (value));
    PORTF_OUTSET = value;
read_status:
    do {

        FUN_code_001267();
        asm volatile("" : "=r" (value), "=r" (b1), "=r" (b2), "=r" (status) : : "memory");
    } while (0);
    status = __builtin_avr_swap(status); asm volatile("" : "+r" (status));
    status &= 0x0e; asm volatile("" : "+r" (status));
    /* C value equivalent requires the original saved PORTF_IN bit6:
     * status = (status & 0xfeu) | saved_lock_bit;
     * Trial492 incorrectly proposed the current value, which SPI calls
     * already overwrite; it also failed matching and was never accepted.
     * Preserve the live CPU T snapshot rather than re-reading the GPIO.
     * No standalone functional validation is claimed for this fragment.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("bld %0, 0" : "+r" (status) : : "cc");
    value = pm_read_absolute(0x2162);
    value &= 0x0e; asm volatile("" : "+r" (value));
    PM_RAM8(0x2162) = status;
    value ^= status; asm volatile("" : "+r" (value));
    /* Unvalidated C equivalent: if (!value) goto power_state; trial545
     * replaced EOR with CP at0x0b1e; preserve the original XOR result.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("breq %l[power_state]" : : "r" (value) : : power_state);
    value = 0x20; asm volatile("" : "+r" (value));
    if (!(status & (1u << 1))) goto second_alarm;
    PORTA_OUTCLR = value;
    GPIOR0 &= (uint8_t)~(1u << 2);
    goto power_state;
second_alarm:
    if (!(status & (1u << 2))) goto third_alarm;
    /* C equivalent: GPIOR0 |= (1u << 2);
     * Steps351/405 changed encoding/layout, including an opaque status retry.
     * No new standalone functional test is claimed.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("sbi 0, 2" : : : "memory");
third_alarm:
    if (!(status & (1u << 3))) goto power_state;
    value = 0x20; asm volatile("" : "+r" (value));
    PORTA_OUTSET = value;
    GPIOR0 &= (uint8_t)~(1u << 2);
power_state:
    asm goto("brtc %l[fault]" : : : : fault);
    WRITE_STATE(0x215a, 10);
    WRITE_STATE(0x2159, 1);
    goto finished;
fault:
    asm volatile("rcall FUN_code_0005b4" : "=r" (value) : : "memory", "cc");
    WRITE_STATE(0x2441, 2);
finished:
    asm volatile("pop r19\n\tpop r18\n\tpop r17\n\tpop r16" : : : "memory");
    {
        register uint8_t saved_status asm("r31");
        asm volatile("" : "=r" (saved_status) : : "memory");
        SREG = saved_status;
    }
    /* Restore the private frame; ordinary C returns cannot express RETI. */
    asm volatile("pop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 912 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_pll_status_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0ae0: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 2786;
    }
    case 0x0ae2: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 2788;
    }
    case 0x0ae4: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 2790;
    }
    case 0x0ae6: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 2792;
    }
    case 0x0ae8: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 2794;
    }
    case 0x0aea: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 2796;
    }
    case 0x0aec: { // lds r16, 0x06A8
        uint16_t address = 1704;
        s->r[16] = pm_read(s, address);
        return 2800;
    }
    case 0x0af0: { // bst r16, 6
        pm_flag(s, TRANSFER, s->r[16] & (1u << 6));
        return 2802;
    }
    case 0x0af2: { // brtc .+22
        return (pm_getflag(s, 6) == 0) ? 2826 : 2804;
    }
    case 0x0af4: { // sbrc r16, 5
        return (!!(s->r[16] & (1u << 5)) == 0) ? 2808 : 2806;
    }
    case 0x0af6: { // rjmp .+18
        return 2826;
    }
    case 0x0af8: { // ldi r16, 0xF5
        s->r[16] = 245;
        return 2810;
    }
    case 0x0afa: { // ldi r17, 0x0F
        s->r[17] = 15;
        return 2812;
    }
    case 0x0afc: { // ldi r18, 0x04
        s->r[18] = 4;
        return 2814;
    }
    case 0x0afe: { // ldi r19, 0x40
        s->r[19] = 64;
        return 2816;
    }
    case 0x0b00: { // call 0x2486
        s->calls[s->call_depth++] = 2820;
        return 9350;
    }
    case 0x0b04: { // ldi r16, 0x20
        s->r[16] = 32;
        return 2822;
    }
    case 0x0b06: { // sts 0x06A5, r16
        uint16_t address = 1701;
        pm_write(s, address, s->r[16]);
        return 2826;
    }
    case 0x0b0a: { // call 0x24ce
        s->calls[s->call_depth++] = 2830;
        return 9422;
    }
    case 0x0b0e: { // swap r19
        s->r[19] = (s->r[19] >> 4) | (s->r[19] << 4);
        return 2832;
    }
    case 0x0b10: { // andi r19, 0x0E
        s->r[19] &= 14;
        pm_nzv(s, s->r[19], false);
        return 2834;
    }
    case 0x0b12: { // bld r19, 0
        s->r[19] = (s->r[19] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 2836;
    }
    case 0x0b14: { // lds r16, 0x2162
        uint16_t address = 8546;
        s->r[16] = pm_read(s, address);
        return 2840;
    }
    case 0x0b18: { // andi r16, 0x0E
        s->r[16] &= 14;
        pm_nzv(s, s->r[16], false);
        return 2842;
    }
    case 0x0b1a: { // sts 0x2162, r19
        uint16_t address = 8546;
        pm_write(s, address, s->r[19]);
        return 2846;
    }
    case 0x0b1e: { // eor r16, r19
        s->r[16] ^= s->r[19];
        pm_nzv(s, s->r[16], false);
        return 2848;
    }
    case 0x0b20: { // breq .+32
        return (pm_getflag(s, 1) == 1) ? 2882 : 2850;
    }
    case 0x0b22: { // ldi r16, 0x20
        s->r[16] = 32;
        return 2852;
    }
    case 0x0b24: { // sbrs r19, 1
        return (!!(s->r[19] & (1u << 1)) == 1) ? 2856 : 2854;
    }
    case 0x0b26: { // rjmp .+8
        return 2864;
    }
    case 0x0b28: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 2860;
    }
    case 0x0b2c: { // cbi 0x00, 2
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 2));
        return 2862;
    }
    case 0x0b2e: { // rjmp .+18
        return 2882;
    }
    case 0x0b30: { // sbrs r19, 2
        return (!!(s->r[19] & (1u << 2)) == 1) ? 2868 : 2866;
    }
    case 0x0b32: { // rjmp .+2
        return 2870;
    }
    case 0x0b34: { // sbi 0x00, 2
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 2));
        return 2870;
    }
    case 0x0b36: { // sbrs r19, 3
        return (!!(s->r[19] & (1u << 3)) == 1) ? 2874 : 2872;
    }
    case 0x0b38: { // rjmp .+8
        return 2882;
    }
    case 0x0b3a: { // ldi r16, 0x20
        s->r[16] = 32;
        return 2876;
    }
    case 0x0b3c: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 2880;
    }
    case 0x0b40: { // cbi 0x00, 2
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 2));
        return 2882;
    }
    case 0x0b42: { // brtc .+14
        return (pm_getflag(s, 6) == 0) ? 2898 : 2884;
    }
    case 0x0b44: { // ldi r16, 0x0A
        s->r[16] = 10;
        return 2886;
    }
    case 0x0b46: { // sts 0x215A, r16
        uint16_t address = 8538;
        pm_write(s, address, s->r[16]);
        return 2890;
    }
    case 0x0b4a: { // ldi r16, 0x01
        s->r[16] = 1;
        return 2892;
    }
    case 0x0b4c: { // sts 0x2159, r16
        uint16_t address = 8537;
        pm_write(s, address, s->r[16]);
        return 2896;
    }
    case 0x0b50: { // rjmp .+8
        return 2906;
    }
    case 0x0b52: { // rcall .+20
        s->calls[s->call_depth++] = 2900;
        return 2920;
    }
    case 0x0b54: { // ldi r16, 0x02
        s->r[16] = 2;
        return 2902;
    }
    case 0x0b56: { // sts 0x2441, r16
        uint16_t address = 9281;
        pm_write(s, address, s->r[16]);
        return 2906;
    }
    case 0x0b5a: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 2908;
    }
    case 0x0b5c: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 2910;
    }
    case 0x0b5e: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 2912;
    }
    case 0x0b60: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 2914;
    }
    case 0x0b62: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 2916;
    }
    case 0x0b64: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 2918;
    }
    case 0x0b66: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
