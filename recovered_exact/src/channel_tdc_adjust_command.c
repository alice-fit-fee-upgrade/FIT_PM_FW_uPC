#include "legacy_cpu.h"
#include "legacy_cli_guard_c.h"
/* Private Y cursor: register reservation, no SRAM object or GNU frame. */
register uint8_t *settings asm("r28");

void pm_channel_tdc_adjust(void) asm("FUN_code_000f70");
void pm_channel_tdc_adjust(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f9c");
    register uint8_t channel asm("r19") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000f9c");
    register uint8_t limit_high asm("r24") = 0;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested >= 64) goto LAB_code_000f9c;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
asm volatile("cpi r20, 0x40\n\tcpc r21, r24\n\tbrge LAB_code_000f9c\n" : : "r" (requested), "r" (limit_high) : "cc");
    limit_high = 0xff;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested < -64) goto LAB_code_000f9c;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
asm volatile("cpi r20, 0xc0\n\tcpc r21, r24\n\tbrlt LAB_code_000f9c" : : "r" (requested), "r" (limit_high) : "cc");
    PM_FPGA_GUARD(requested);
    settings = (uint8_t *)0x217b;
    asm volatile("clr r11" : : : "r11", "cc");
    {
        register uint8_t address_low asm("r28");
        asm volatile("" : "=r" (address_low) : "y" (settings));
        address_low += channel;
        asm volatile("" : "+r" (address_low));
    }
    /* Original ADC consumes carry from the C low-byte addition. */
    asm volatile("adc r29, r11"
                 : "=y" (settings) : "r" (channel) : "r11", "cc");
    { register uint8_t low asm("r20");
      asm volatile("" : "=r" (low) : "r" (requested));
      *settings = low; asm volatile("" : : : "memory"); }
    register uint8_t command_index asm("r21") = channel;
    asm volatile("" : "+r" (command_index));
    command_index &= 3; asm volatile("" : "+r" (command_index));
    register uint8_t stride asm("r22") = 0x20;
    asm volatile("mul %0, %1" : : "r" (command_index), "r" (stride) : "r0", "r1", "cc");
    register uint8_t command asm("r16") = 0x0c;
    register uint8_t product_low asm("r0");
    asm volatile("" : "=r" (product_low));
    command += product_low;
    asm volatile("" : "+r" (command));
    channel >>= 2; asm volatile("" : "+r" (channel));
    register uint8_t data asm("r17") = (uint8_t)requested;
    register uint8_t high asm("r18");
    /* C value: high = 0; retain CLR's original flags and exact short call. */
    asm volatile("clr %0" : "=r" (high) : "r" (command), "r" (data), "r" (channel) : "cc");
    pm_cpu_disable_irq();
    asm volatile("rcall ths788_write" : : "r" (high), "r" (command), "r" (data), "r" (channel) : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}
asm(".pushsection .text.FUN_code_000f70,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 720 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_channel_tdc_adjust_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1ee0: { // rcall .+1874
        s->calls[s->call_depth++] = 7906;
        return 9780;
    }
    case 0x1ee2: { // brcs .+84
        return (pm_getflag(s, 0) == 1) ? 7992 : 7908;
    }
    case 0x1ee4: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7910;
    }
    case 0x1ee6: { // brne .+80
        return (pm_getflag(s, 1) == 0) ? 7992 : 7912;
    }
    case 0x1ee8: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 7914;
    }
    case 0x1eea: { // brge .+76
        return (pm_getflag(s, 4) == 0) ? 7992 : 7916;
    }
    case 0x1eec: { // mov r19, r20
        s->r[19] = s->r[20];
        return 7918;
    }
    case 0x1eee: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7920;
    }
    case 0x1ef0: { // brne .+70
        return (pm_getflag(s, 1) == 0) ? 7992 : 7922;
    }
    case 0x1ef2: { // rcall .+1856
        s->calls[s->call_depth++] = 7924;
        return 9780;
    }
    case 0x1ef4: { // brcs .+66
        return (pm_getflag(s, 0) == 1) ? 7992 : 7926;
    }
    case 0x1ef6: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 7928;
    }
    case 0x1ef8: { // brne .+62
        return (pm_getflag(s, 1) == 0) ? 7992 : 7930;
    }
    case 0x1efa: { // ldi r24, 0x00
        s->r[24] = 0;
        return 7932;
    }
    case 0x1efc: { // cpi r20, 0x40
        pm_sub(s, s->r[20], 64, 0, false);
        return 7934;
    }
    case 0x1efe: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 7936;
    }
    case 0x1f00: { // brge .+54
        return (pm_getflag(s, 4) == 0) ? 7992 : 7938;
    }
    case 0x1f02: { // ldi r24, 0xFF
        s->r[24] = 255;
        return 7940;
    }
    case 0x1f04: { // cpi r20, 0xC0
        pm_sub(s, s->r[20], 192, 0, false);
        return 7942;
    }
    case 0x1f06: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 7944;
    }
    case 0x1f08: { // brlt .+46
        return (pm_getflag(s, 4) == 1) ? 7992 : 7946;
    }
    case 0x1f0a: { // rcall .+528
        s->calls[s->call_depth++] = 7948;
        return 8476;
    }
    case 0x1f0c: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7952 : 7950;
    }
    case 0x1f0e: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1f10: { // ldi r28, 0x7B
        s->r[28] = 123;
        return 7954;
    }
    case 0x1f12: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7956;
    }
    case 0x1f14: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 7958;
    }
    case 0x1f16: { // add r28, r19
        s->r[28] = pm_add(s, s->r[28], s->r[19], 0);
        return 7960;
    }
    case 0x1f18: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 7962;
    }
    case 0x1f1a: { // st Y, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[20]);
        return 7964;
    }
    case 0x1f1c: { // mov r21, r19
        s->r[21] = s->r[19];
        return 7966;
    }
    case 0x1f1e: { // andi r21, 0x03
        s->r[21] &= 3;
        pm_nzv(s, s->r[21], false);
        return 7968;
    }
    case 0x1f20: { // ldi r22, 0x20
        s->r[22] = 32;
        return 7970;
    }
    case 0x1f22: { // mul r21, r22
        uint16_t product = (s->r[21]) * (int)s->r[22];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 7972;
    }
    case 0x1f24: { // ldi r16, 0x0C
        s->r[16] = 12;
        return 7974;
    }
    case 0x1f26: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 7976;
    }
    case 0x1f28: { // lsr r19
        bool carry = s->r[19] & 1;
        s->r[19] = (s->r[19] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[19], !!(s->r[19] & 128) ^ carry);
        return 7978;
    }
    case 0x1f2a: { // lsr r19
        bool carry = s->r[19] & 1;
        s->r[19] = (s->r[19] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[19], !!(s->r[19] & 128) ^ carry);
        return 7980;
    }
    case 0x1f2c: { // mov r17, r20
        s->r[17] = s->r[20];
        return 7982;
    }
    case 0x1f2e: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 7984;
    }
    case 0x1f30: { // cli
        pm_irq(s, false);
        return 7986;
    }
    case 0x1f32: { // rcall .+576
        s->calls[s->call_depth++] = 7988;
        return 8564;
    }
    case 0x1f34: { // sei
        pm_irq(s, true);
        return 7990;
    }
    case 0x1f36: { // rjmp .+170
        return 8162;
    }
    case 0x1f38: { // rjmp .-2930
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
