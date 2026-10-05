#include <avr/io.h>
#include "legacy_cpu.h"
#include "legacy_cli_guard_c.h"
/* Private Y cursor: register reservation, no SRAM object or GNU frame. */
register uint8_t *settings asm("r28");

void fpga_set_adc_zero(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000ff5");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000ff5");
    register uint8_t limit_high asm("r24") = 1;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested >= 501) goto LAB_code_000ff5;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
asm volatile("cpi r20, 0xf5\n\tcpc r21, r24\n\tbrge LAB_code_000ff5\n" : : "r" (requested), "r" (limit_high) : "cc");
    limit_high = 0xfe;
    asm volatile("" : "+r" (limit_high));
    /* C value equivalent (not compiled):
     * if ((int16_t)requested < -500) goto LAB_code_000ff5;
     * Keep exact CPI/CPC flags and shared ASM error tails. This explanation
     * has no separate successful functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
asm volatile("cpi r20, 0x0c\n\tcpc r21, r24\n\tbrlt LAB_code_000ff5" : : "r" (requested), "r" (limit_high) : "cc");
    PM_FPGA_GUARD(requested);
    GPIOR0 |= (1u << 3);
    register uint16_t word asm("r16") = requested;
    asm volatile("" : "+r" (word));
    register uint8_t offset asm("r23") = channel;
    asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    register uint8_t address asm("r18") = 0x82;
    asm volatile("" : "+r" (address));
    address += offset;
    asm volatile("" : "+r" (word), "+r" (address) : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : "+r" (word), "+r" (address) : : "memory", "cc");
    pm_cpu_enable_irq();
    settings = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += 4; asm volatile("" : "+r" (offset));
    asm volatile("clr r11" : : : "r11", "cc");
    {
        register uint8_t address_low asm("r28");
        asm volatile("" : "=r" (address_low) : "y" (settings));
        address_low += offset;
        asm volatile("" : "+r" (address_low));
    }
    /* Original ADC consumes carry from the C low-byte addition. */
    asm volatile("adc r29, r11" : "=y" (settings) : "r" (offset) : "r11", "cc");
    pm_cpu_disable_irq();
    asm volatile("st Y+, r20" : "+y" (settings) : "r" (requested) : "memory");
    { register uint8_t high asm("r21");
      asm volatile("" : "=r" (high) : "r" (requested));
      *settings = high; asm volatile("" : : : "memory"); }
    pm_cpu_enable_irq();
    asm volatile("rcall dac_set_value\n\tcbi 0, 3\n\trjmp LAB_code_000ff1" : : "r" (requested), "r" (channel) : "memory", "cc");
    __builtin_unreachable();
}

asm(".pushsection .text.fpga_set_adc_zero,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 784 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_channel_adc_zero_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1fec: { // rcall .+1606
        s->calls[s->call_depth++] = 8174;
        return 9780;
    }
    case 0x1fee: { // brcs .-6
        return (pm_getflag(s, 0) == 1) ? 8170 : 8176;
    }
    case 0x1ff0: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8178;
    }
    case 0x1ff2: { // brne .-10
        return (pm_getflag(s, 1) == 0) ? 8170 : 8180;
    }
    case 0x1ff4: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 8182;
    }
    case 0x1ff6: { // brge .-14
        return (pm_getflag(s, 4) == 0) ? 8170 : 8184;
    }
    case 0x1ff8: { // mov r22, r20
        s->r[22] = s->r[20];
        return 8186;
    }
    case 0x1ffa: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 8188;
    }
    case 0x1ffc: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 8170 : 8190;
    }
    case 0x1ffe: { // rcall .+1588
        s->calls[s->call_depth++] = 8192;
        return 9780;
    }
    case 0x2000: { // brcs .-24
        return (pm_getflag(s, 0) == 1) ? 8170 : 8194;
    }
    case 0x2002: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 8196;
    }
    case 0x2004: { // brne .-28
        return (pm_getflag(s, 1) == 0) ? 8170 : 8198;
    }
    case 0x2006: { // ldi r24, 0x01
        s->r[24] = 1;
        return 8200;
    }
    case 0x2008: { // cpi r20, 0xF5
        pm_sub(s, s->r[20], 245, 0, false);
        return 8202;
    }
    case 0x200a: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 8204;
    }
    case 0x200c: { // brge .-36
        return (pm_getflag(s, 4) == 0) ? 8170 : 8206;
    }
    case 0x200e: { // ldi r24, 0xFE
        s->r[24] = 254;
        return 8208;
    }
    case 0x2010: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 8210;
    }
    case 0x2012: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 8212;
    }
    case 0x2014: { // brlt .-44
        return (pm_getflag(s, 4) == 1) ? 8170 : 8214;
    }
    case 0x2016: { // rcall .+260
        s->calls[s->call_depth++] = 8216;
        return 8476;
    }
    case 0x2018: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 8220 : 8218;
    }
    case 0x201a: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x201c: { // sbi 0x00, 3
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 3));
        return 8222;
    }
    case 0x201e: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 8224;
    }
    case 0x2020: { // mov r23, r22
        s->r[23] = s->r[22];
        return 8226;
    }
    case 0x2022: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8228;
    }
    case 0x2024: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8230;
    }
    case 0x2026: { // ldi r18, 0x82
        s->r[18] = 130;
        return 8232;
    }
    case 0x2028: { // add r18, r23
        s->r[18] = pm_add(s, s->r[18], s->r[23], 0);
        return 8234;
    }
    case 0x202a: { // cli
        pm_irq(s, false);
        return 8236;
    }
    case 0x202c: { // rcall .+736
        s->calls[s->call_depth++] = 8238;
        return 8974;
    }
    case 0x202e: { // sei
        pm_irq(s, true);
        return 8240;
    }
    case 0x2030: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 8242;
    }
    case 0x2032: { // ldi r29, 0x21
        s->r[29] = 33;
        return 8244;
    }
    case 0x2034: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8246;
    }
    case 0x2036: { // subi r23, 0xFC
        s->r[23] = pm_sub(s, s->r[23], 252, 0, false);
        return 8248;
    }
    case 0x2038: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 8250;
    }
    case 0x203a: { // add r28, r23
        s->r[28] = pm_add(s, s->r[28], s->r[23], 0);
        return 8252;
    }
    case 0x203c: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 8254;
    }
    case 0x203e: { // cli
        pm_irq(s, false);
        return 8256;
    }
    case 0x2040: { // st Y+, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[20]);
        return 8258;
    }
    case 0x2042: { // st Y, r21
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[21]);
        return 8260;
    }
    case 0x2044: { // sei
        pm_irq(s, true);
        return 8262;
    }
    case 0x2046: { // rcall .+188
        s->calls[s->call_depth++] = 8264;
        return 8452;
    }
    case 0x2048: { // cbi 0x00, 3
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 3));
        return 8266;
    }
    case 0x204a: { // rjmp .-106
        return 8162;
    }
    case 0x204c: { // rjmp .-3206
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
