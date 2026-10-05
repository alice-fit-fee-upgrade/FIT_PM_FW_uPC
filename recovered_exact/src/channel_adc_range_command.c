/* Retained exact call/load helpers: a C call such as
 * value = fpga_read(address); or fpga_write(address, word); expresses the
 * operation, but these entries use private bound registers, original CALL/RCALL
 * widths and shared error tails. These illustrative names are not compiled
 * interfaces and are not independently functionally validated alternatives.
 * Where a historically tested complete C alternative exists, its evidence
 * and bridge scope remain documented beside that helper.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
#include "legacy_cpu.h"
#include "legacy_cli_guard_c.h"
/* Private Y cursor: register reservation, no SRAM object or GNU frame. */
register uint8_t *settings asm("r28");

void fpga_set_adc_range_corr(void)
{
    register uint16_t requested asm("r20"), first asm("r24");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f16");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    asm volatile("cpi r16, 0x20\n\tbrne LAB_code_000f16\n\trcall cli_get_integer\n"
                 "brcs LAB_code_000f16\n\tcpi r16, 0x2c\n\tbrne LAB_code_000f16"
                 : "=r" (requested) : : "r16", "memory", "cc");
    first = requested; asm volatile("" : "+r" (first));
    asm volatile("rcall cli_get_integer\n\tbrcs LAB_code_000f16\n\tcpi r16, 13\n\tbrne LAB_code_000f16"
                 : "=r" (requested) : : "r16", "memory", "cc");
    /* Original signed lower/upper checks for both correction words. */
    register uint8_t limit_high asm("r26") = 0x0c;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested >= 3072) goto LAB_code_000f16;
     * if ((int16_t)first >= 3072) goto LAB_code_000f16;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
asm volatile("cpi r20, 0\n\tcpc r21, r26\n\tbrge LAB_code_000f16\ncpi r24, 0\n\tcpc r25, r26\n\tbrge LAB_code_000f16\n" : : "r" (requested), "r" (first), "r" (limit_high) : "cc");
    limit_high = 5;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested < 1365) goto LAB_code_000f16;
     * if ((int16_t)first < 1365) goto LAB_code_000f16;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
asm volatile("cpi r20, 0x55\n\tcpc r21, r26\n\tbrlt LAB_code_000f16\ncpi r24, 0x55\n\tcpc r25, r26\n\tbrlt LAB_code_000f16" : : "r" (requested), "r" (first), "r" (limit_high) : "cc");
    PM_FPGA_GUARD(requested);
    settings = (uint8_t *)0x2187;
    asm volatile("" : "+y" (settings));
    channel += channel; asm volatile("" : "+r" (channel));
    register uint8_t offset asm("r23") = channel;
    asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    asm volatile("clr r11" : : : "r11", "cc");
    {
        register uint8_t address_low asm("r28");
        asm volatile("" : "=r" (address_low) : "y" (settings));
        address_low += offset;
        asm volatile("" : "+r" (address_low));
    }
    /* Original ADC consumes carry from the C low-byte addition. */
    asm volatile("adc r29, r11\n"
                 "st Y+, r24\n\tst Y+, r25\n\tst Y+, r20"
                 : "=y" (settings) : "r" (offset), "r" (first), "r" (requested) : "r11", "memory", "cc");
    { register uint8_t high asm("r21");
      asm volatile("" : "=r" (high) : "r" (requested));
      *settings = high; asm volatile("" : : : "memory"); }
    register uint8_t address asm("r18") = 0x25;
    asm volatile("" : "+r" (address));
    address += channel; asm volatile("" : "+r" (address));
    register uint16_t word asm("r16") = first;
    asm volatile("" : "+r" (word), "+r" (address) : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : "+r" (word), "+r" (address) : : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("inc %0" : "+r" (address) : : "cc");
    word = requested;
    asm volatile("" : : "r" (word), "r" (address) : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : : "r" (word), "r" (address) : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}
asm(".pushsection .text.fpga_set_adc_range_corr,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 960 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_channel_adc_range_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1db6: { // rcall .+2172
        s->calls[s->call_depth++] = 7608;
        return 9780;
    }
    case 0x1db8: { // brcs .+114
        return (pm_getflag(s, 0) == 1) ? 7724 : 7610;
    }
    case 0x1dba: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7612;
    }
    case 0x1dbc: { // brne .+110
        return (pm_getflag(s, 1) == 0) ? 7724 : 7614;
    }
    case 0x1dbe: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 7616;
    }
    case 0x1dc0: { // brge .+106
        return (pm_getflag(s, 4) == 0) ? 7724 : 7618;
    }
    case 0x1dc2: { // mov r22, r20
        s->r[22] = s->r[20];
        return 7620;
    }
    case 0x1dc4: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7622;
    }
    case 0x1dc6: { // brne .+100
        return (pm_getflag(s, 1) == 0) ? 7724 : 7624;
    }
    case 0x1dc8: { // rcall .+2154
        s->calls[s->call_depth++] = 7626;
        return 9780;
    }
    case 0x1dca: { // brcs .+96
        return (pm_getflag(s, 0) == 1) ? 7724 : 7628;
    }
    case 0x1dcc: { // cpi r16, 0x2C
        pm_sub(s, s->r[16], 44, 0, false);
        return 7630;
    }
    case 0x1dce: { // brne .+92
        return (pm_getflag(s, 1) == 0) ? 7724 : 7632;
    }
    case 0x1dd0: { // movw r24, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 24, pair);
        return 7634;
    }
    case 0x1dd2: { // rcall .+2144
        s->calls[s->call_depth++] = 7636;
        return 9780;
    }
    case 0x1dd4: { // brcs .+86
        return (pm_getflag(s, 0) == 1) ? 7724 : 7638;
    }
    case 0x1dd6: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 7640;
    }
    case 0x1dd8: { // brne .+82
        return (pm_getflag(s, 1) == 0) ? 7724 : 7642;
    }
    case 0x1dda: { // ldi r26, 0x0C
        s->r[26] = 12;
        return 7644;
    }
    case 0x1ddc: { // cpi r20, 0x00
        pm_sub(s, s->r[20], 0, 0, false);
        return 7646;
    }
    case 0x1dde: { // cpc r21, r26
        pm_sub(s, s->r[21], s->r[26], pm_getflag(s, CARRY), true);
        return 7648;
    }
    case 0x1de0: { // brge .+74
        return (pm_getflag(s, 4) == 0) ? 7724 : 7650;
    }
    case 0x1de2: { // cpi r24, 0x00
        pm_sub(s, s->r[24], 0, 0, false);
        return 7652;
    }
    case 0x1de4: { // cpc r25, r26
        pm_sub(s, s->r[25], s->r[26], pm_getflag(s, CARRY), true);
        return 7654;
    }
    case 0x1de6: { // brge .+68
        return (pm_getflag(s, 4) == 0) ? 7724 : 7656;
    }
    case 0x1de8: { // ldi r26, 0x05
        s->r[26] = 5;
        return 7658;
    }
    case 0x1dea: { // cpi r20, 0x55
        pm_sub(s, s->r[20], 85, 0, false);
        return 7660;
    }
    case 0x1dec: { // cpc r21, r26
        pm_sub(s, s->r[21], s->r[26], pm_getflag(s, CARRY), true);
        return 7662;
    }
    case 0x1dee: { // brlt .+60
        return (pm_getflag(s, 4) == 1) ? 7724 : 7664;
    }
    case 0x1df0: { // cpi r24, 0x55
        pm_sub(s, s->r[24], 85, 0, false);
        return 7666;
    }
    case 0x1df2: { // cpc r25, r26
        pm_sub(s, s->r[25], s->r[26], pm_getflag(s, CARRY), true);
        return 7668;
    }
    case 0x1df4: { // brlt .+54
        return (pm_getflag(s, 4) == 1) ? 7724 : 7670;
    }
    case 0x1df6: { // rcall .+804
        s->calls[s->call_depth++] = 7672;
        return 8476;
    }
    case 0x1df8: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7676 : 7674;
    }
    case 0x1dfa: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1dfc: { // ldi r28, 0x87
        s->r[28] = 135;
        return 7678;
    }
    case 0x1dfe: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7680;
    }
    case 0x1e00: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 7682;
    }
    case 0x1e02: { // mov r23, r22
        s->r[23] = s->r[22];
        return 7684;
    }
    case 0x1e04: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 7686;
    }
    case 0x1e06: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 7688;
    }
    case 0x1e08: { // add r28, r23
        s->r[28] = pm_add(s, s->r[28], s->r[23], 0);
        return 7690;
    }
    case 0x1e0a: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 7692;
    }
    case 0x1e0c: { // st Y+, r24
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[24]);
        return 7694;
    }
    case 0x1e0e: { // st Y+, r25
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[25]);
        return 7696;
    }
    case 0x1e10: { // st Y+, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[20]);
        return 7698;
    }
    case 0x1e12: { // st Y, r21
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[21]);
        return 7700;
    }
    case 0x1e14: { // ldi r18, 0x25
        s->r[18] = 37;
        return 7702;
    }
    case 0x1e16: { // add r18, r22
        s->r[18] = pm_add(s, s->r[18], s->r[22], 0);
        return 7704;
    }
    case 0x1e18: { // movw r16, r24
        uint16_t pair = pm_pointer(s, 24);
        pm_setpointer(s, 16, pair);
        return 7706;
    }
    case 0x1e1a: { // cli
        pm_irq(s, false);
        return 7708;
    }
    case 0x1e1c: { // rcall .+1264
        s->calls[s->call_depth++] = 7710;
        return 8974;
    }
    case 0x1e1e: { // sei
        pm_irq(s, true);
        return 7712;
    }
    case 0x1e20: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 7714;
    }
    case 0x1e22: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 7716;
    }
    case 0x1e24: { // cli
        pm_irq(s, false);
        return 7718;
    }
    case 0x1e26: { // rcall .+1254
        s->calls[s->call_depth++] = 7720;
        return 8974;
    }
    case 0x1e28: { // sei
        pm_irq(s, true);
        return 7722;
    }
    case 0x1e2a: { // rjmp .+438
        return 8162;
    }
    case 0x1e2c: { // rjmp .-2662
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
