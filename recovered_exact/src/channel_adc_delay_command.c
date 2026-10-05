#include "legacy_word_ops.h"
#include "legacy_cpu.h"
#include "legacy_cli_guard_c.h"
/* Private Y cursor: register reservation, no SRAM object or GNU frame. */
register uint8_t *settings asm("r28");

void fpga_set_ch_adc_delay(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f9c");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000f9c");
    asm volatile("tst r21\n\tbrmi LAB_code_000f9c\n\tldi r24, 0x4e\n"
                 "cpi r20, 0x21\n\tcpc r21, r24\n\tbrge LAB_code_000f9c" : : "r" (requested) : "r24", "cc");
    PM_FPGA_GUARD(requested);
    register uint16_t word asm("r16") = requested;
    asm volatile("" : "+r" (word));
    register uint8_t offset asm("r23") = channel;
    asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    register uint8_t address asm("r18") = 0x83;
    asm volatile("" : "+r" (address));
    address += offset;
    asm volatile("" : "+r" (word), "+r" (address) : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : "+r" (word), "+r" (address) : : "memory", "cc");
    pm_cpu_enable_irq();
    settings = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += 6; asm volatile("" : "+r" (offset));
    asm volatile("clr r11" : : : "r11", "cc");
    {
        register uint8_t address_low asm("r28");
        asm volatile("" : "=r" (address_low) : "y" (settings));
        address_low += offset;
        asm volatile("" : "+r" (address_low));
    }
    /* Original ADC consumes carry from the C low-byte addition. */
    asm volatile("adc r29, r11" : "=y" (settings) : "r" (offset) : "r11", "cc");
    PM_STORE_WORD_LE_Y(settings, requested);
    asm volatile("rcall FUN_code_001068\n\trjmp LAB_code_000ff1" : : "r" (requested), "r" (channel) : "memory", "cc");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 672 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_channel_adc_delay_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1f3a: { // rcall .+1784
        s->calls[s->call_depth++] = 7996;
        return 9780;
    }
    case 0x1f3c: { // brcs .-6
        return (pm_getflag(s, 0) == 1) ? 7992 : 7998;
    }
    case 0x1f3e: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8000;
    }
    case 0x1f40: { // brne .-10
        return (pm_getflag(s, 1) == 0) ? 7992 : 8002;
    }
    case 0x1f42: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 8004;
    }
    case 0x1f44: { // brge .-14
        return (pm_getflag(s, 4) == 0) ? 7992 : 8006;
    }
    case 0x1f46: { // mov r22, r20
        s->r[22] = s->r[20];
        return 8008;
    }
    case 0x1f48: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 8010;
    }
    case 0x1f4a: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 7992 : 8012;
    }
    case 0x1f4c: { // rcall .+1766
        s->calls[s->call_depth++] = 8014;
        return 9780;
    }
    case 0x1f4e: { // brcs .-24
        return (pm_getflag(s, 0) == 1) ? 7992 : 8016;
    }
    case 0x1f50: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 8018;
    }
    case 0x1f52: { // brne .-28
        return (pm_getflag(s, 1) == 0) ? 7992 : 8020;
    }
    case 0x1f54: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8022;
    }
    case 0x1f56: { // brmi .-32
        return (pm_getflag(s, 2) == 1) ? 7992 : 8024;
    }
    case 0x1f58: { // ldi r24, 0x4E
        s->r[24] = 78;
        return 8026;
    }
    case 0x1f5a: { // cpi r20, 0x21
        pm_sub(s, s->r[20], 33, 0, false);
        return 8028;
    }
    case 0x1f5c: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 8030;
    }
    case 0x1f5e: { // brge .-40
        return (pm_getflag(s, 4) == 0) ? 7992 : 8032;
    }
    case 0x1f60: { // rcall .+442
        s->calls[s->call_depth++] = 8034;
        return 8476;
    }
    case 0x1f62: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 8038 : 8036;
    }
    case 0x1f64: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1f66: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 8040;
    }
    case 0x1f68: { // mov r23, r22
        s->r[23] = s->r[22];
        return 8042;
    }
    case 0x1f6a: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8044;
    }
    case 0x1f6c: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8046;
    }
    case 0x1f6e: { // ldi r18, 0x83
        s->r[18] = 131;
        return 8048;
    }
    case 0x1f70: { // add r18, r23
        s->r[18] = pm_add(s, s->r[18], s->r[23], 0);
        return 8050;
    }
    case 0x1f72: { // cli
        pm_irq(s, false);
        return 8052;
    }
    case 0x1f74: { // rcall .+920
        s->calls[s->call_depth++] = 8054;
        return 8974;
    }
    case 0x1f76: { // sei
        pm_irq(s, true);
        return 8056;
    }
    case 0x1f78: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 8058;
    }
    case 0x1f7a: { // ldi r29, 0x21
        s->r[29] = 33;
        return 8060;
    }
    case 0x1f7c: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8062;
    }
    case 0x1f7e: { // subi r23, 0xFA
        s->r[23] = pm_sub(s, s->r[23], 250, 0, false);
        return 8064;
    }
    case 0x1f80: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 8066;
    }
    case 0x1f82: { // add r28, r23
        s->r[28] = pm_add(s, s->r[28], s->r[23], 0);
        return 8068;
    }
    case 0x1f84: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 8070;
    }
    case 0x1f86: { // st Y+, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[20]);
        return 8072;
    }
    case 0x1f88: { // st Y, r21
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[21]);
        return 8074;
    }
    case 0x1f8a: { // rcall .+324
        s->calls[s->call_depth++] = 8076;
        return 8400;
    }
    case 0x1f8c: { // rjmp .+84
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
