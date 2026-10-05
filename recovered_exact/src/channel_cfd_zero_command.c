#include "legacy_word_ops.h"
#include "legacy_cpu.h"
#include "legacy_cli_guard_c.h"
/* Private Y cursor: register reservation, no SRAM object or GNU frame. */
register uint8_t *settings asm("r28");

void fpga_set_ch_cfd_zero(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000ff5");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000ff5");
    register uint8_t limit_high asm("r24") = 1;
    asm volatile("" : "+r" (limit_high));
    /* Reject values at or above 501 through the original error tail. */
    PM_REJECT_I16_GE(requested, limit_high, 501, "LAB_code_001026");
    limit_high = 0xfe;
    asm volatile("" : "+r" (limit_high));
    /* Reject values below -500 through the original error tail. */
    PM_REJECT_I16_LT(requested, limit_high, -500, "LAB_code_001026");
    PM_FPGA_GUARD(requested);
    register uint16_t word asm("r16") = requested;
    asm volatile("" : "+r" (word));
    register uint8_t offset asm("r23") = channel;
    asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += offset; asm volatile("" : "+r" (offset));
    register uint8_t address asm("r18") = 0x81;
    asm volatile("" : "+r" (address));
    address += offset;
    asm volatile("" : "+r" (word), "+r" (address) : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : "+r" (word), "+r" (address) : : "memory", "cc");
    pm_cpu_enable_irq();
    settings = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    offset += offset; asm volatile("" : "+r" (offset));
    offset += 2; asm volatile("" : "+r" (offset));
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
    asm volatile("rcall dac_set_value_2\n\trjmp LAB_code_000ff1" : : "r" (requested), "r" (channel) : "memory", "cc");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 704 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_channel_cfd_zero_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x204e: { // rcall .+1508
        s->calls[s->call_depth++] = 8272;
        return 9780;
    }
    case 0x2050: { // brcs .-104
        return (pm_getflag(s, 0) == 1) ? 8170 : 8274;
    }
    case 0x2052: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8276;
    }
    case 0x2054: { // brne .-108
        return (pm_getflag(s, 1) == 0) ? 8170 : 8278;
    }
    case 0x2056: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 8280;
    }
    case 0x2058: { // brge .-112
        return (pm_getflag(s, 4) == 0) ? 8170 : 8282;
    }
    case 0x205a: { // mov r22, r20
        s->r[22] = s->r[20];
        return 8284;
    }
    case 0x205c: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 8286;
    }
    case 0x205e: { // brne .-118
        return (pm_getflag(s, 1) == 0) ? 8170 : 8288;
    }
    case 0x2060: { // rcall .+1490
        s->calls[s->call_depth++] = 8290;
        return 9780;
    }
    case 0x2062: { // brcs .-122
        return (pm_getflag(s, 0) == 1) ? 8170 : 8292;
    }
    case 0x2064: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 8294;
    }
    case 0x2066: { // brne .-126
        return (pm_getflag(s, 1) == 0) ? 8170 : 8296;
    }
    case 0x2068: { // ldi r24, 0x01
        s->r[24] = 1;
        return 8298;
    }
    case 0x206a: { // cpi r20, 0xF5
        pm_sub(s, s->r[20], 245, 0, false);
        return 8300;
    }
    case 0x206c: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 8302;
    }
    case 0x206e: { // brge .-36
        return (pm_getflag(s, 4) == 0) ? 8268 : 8304;
    }
    case 0x2070: { // ldi r24, 0xFE
        s->r[24] = 254;
        return 8306;
    }
    case 0x2072: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 8308;
    }
    case 0x2074: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 8310;
    }
    case 0x2076: { // brlt .-44
        return (pm_getflag(s, 4) == 1) ? 8268 : 8312;
    }
    case 0x2078: { // rcall .+162
        s->calls[s->call_depth++] = 8314;
        return 8476;
    }
    case 0x207a: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 8318 : 8316;
    }
    case 0x207c: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x207e: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 8320;
    }
    case 0x2080: { // mov r23, r22
        s->r[23] = s->r[22];
        return 8322;
    }
    case 0x2082: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8324;
    }
    case 0x2084: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8326;
    }
    case 0x2086: { // ldi r18, 0x81
        s->r[18] = 129;
        return 8328;
    }
    case 0x2088: { // add r18, r23
        s->r[18] = pm_add(s, s->r[18], s->r[23], 0);
        return 8330;
    }
    case 0x208a: { // cli
        pm_irq(s, false);
        return 8332;
    }
    case 0x208c: { // rcall .+640
        s->calls[s->call_depth++] = 8334;
        return 8974;
    }
    case 0x208e: { // sei
        pm_irq(s, true);
        return 8336;
    }
    case 0x2090: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 8338;
    }
    case 0x2092: { // ldi r29, 0x21
        s->r[29] = 33;
        return 8340;
    }
    case 0x2094: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8342;
    }
    case 0x2096: { // subi r23, 0xFE
        s->r[23] = pm_sub(s, s->r[23], 254, 0, false);
        return 8344;
    }
    case 0x2098: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 8346;
    }
    case 0x209a: { // add r28, r23
        s->r[28] = pm_add(s, s->r[28], s->r[23], 0);
        return 8348;
    }
    case 0x209c: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 8350;
    }
    case 0x209e: { // st Y+, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[20]);
        return 8352;
    }
    case 0x20a0: { // st Y, r21
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[21]);
        return 8354;
    }
    case 0x20a2: { // rcall .+72
        s->calls[s->call_depth++] = 8356;
        return 8428;
    }
    case 0x20a4: { // rjmp .-196
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
