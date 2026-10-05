#include "legacy_word_ops.h"
#include "legacy_cpu.h"
#include "legacy_cli_guard_c.h"
/* Private Y cursor: register reservation, no SRAM object or GNU frame. */
register uint8_t *settings asm("r28");

void fpga_set_ch_cfd_threshold(void)
{
    register uint16_t requested asm("r20");
    PM_PARSE_CHANNEL(requested, "LAB_code_000f9c");
    register uint8_t channel asm("r22") = (uint8_t)requested;
    asm volatile("" : "+r" (channel));
    PM_PARSE_VALUE(requested, "LAB_code_000f9c");
    register uint8_t limit_high asm("r24") = 0x75;
    asm volatile("" : "+r" (limit_high));
    /* Reject values at or above 30001 through the original error tail. */
    PM_REJECT_I16_GE(requested, limit_high, 30001, "LAB_code_000f9c");
    limit_high = 1;
    asm volatile("" : "+r" (limit_high));
    /* Reject values below 300 through the original error tail. */
    PM_REJECT_I16_LT(requested, limit_high, 300, "LAB_code_000f9c");
    PM_FPGA_GUARD(requested);
    register uint16_t word asm("r16") = requested;
    asm volatile("" : "+r" (word));
    register uint8_t offset asm("r10");
    offset = channel;
    asm volatile("" : "+r" (offset));
    offset += offset;
    asm volatile("" : "+r" (offset));
    offset += offset;
    asm volatile("" : "+r" (offset));
    register uint8_t address asm("r18") = 0x80;
    asm volatile("" : "+r" (address));
    address += offset;
    asm volatile("" : "+r" (word), "+r" (address) : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : "+r" (word), "+r" (address) : : "memory", "cc");
    pm_cpu_enable_irq();
    settings = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    offset += offset;
    asm volatile("" : "+r" (offset), "+y" (settings));
    /* C pointer value: settings += offset; CLR/ADC retain live flags. */
    asm volatile("clr r11" : : : "r11", "cc");
    {
        register uint8_t address_low asm("r28");
        asm volatile("" : "=r" (address_low) : "y" (settings));
        address_low += offset; asm volatile("" : "+r" (address_low));
    }
    asm volatile("adc r29, r11" : "=y" (settings), "+r" (offset) : : "r11", "cc");
    PM_STORE_WORD_LE_Y(settings, requested);
    asm volatile("rcall FUN_code_001053" : : "r" (requested), "r" (channel) : "memory", "cc");
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2998;
    asm volatile("rcall cli_send_msg" : "+z" (message) : : "memory", "cc");
}
asm(".pushsection .text.fpga_set_ch_cfd_threshold,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 752 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_channel_cfd_threshold_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1f8e: { // rcall .+1700
        s->calls[s->call_depth++] = 8080;
        return 9780;
    }
    case 0x1f90: { // brcs .-90
        return (pm_getflag(s, 0) == 1) ? 7992 : 8082;
    }
    case 0x1f92: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8084;
    }
    case 0x1f94: { // brne .-94
        return (pm_getflag(s, 1) == 0) ? 7992 : 8086;
    }
    case 0x1f96: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 8088;
    }
    case 0x1f98: { // brge .-98
        return (pm_getflag(s, 4) == 0) ? 7992 : 8090;
    }
    case 0x1f9a: { // mov r22, r20
        s->r[22] = s->r[20];
        return 8092;
    }
    case 0x1f9c: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 8094;
    }
    case 0x1f9e: { // brne .-104
        return (pm_getflag(s, 1) == 0) ? 7992 : 8096;
    }
    case 0x1fa0: { // rcall .+1682
        s->calls[s->call_depth++] = 8098;
        return 9780;
    }
    case 0x1fa2: { // brcs .-108
        return (pm_getflag(s, 0) == 1) ? 7992 : 8100;
    }
    case 0x1fa4: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 8102;
    }
    case 0x1fa6: { // brne .-112
        return (pm_getflag(s, 1) == 0) ? 7992 : 8104;
    }
    case 0x1fa8: { // ldi r24, 0x75
        s->r[24] = 117;
        return 8106;
    }
    case 0x1faa: { // cpi r20, 0x31
        pm_sub(s, s->r[20], 49, 0, false);
        return 8108;
    }
    case 0x1fac: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 8110;
    }
    case 0x1fae: { // brge .-120
        return (pm_getflag(s, 4) == 0) ? 7992 : 8112;
    }
    case 0x1fb0: { // ldi r24, 0x01
        s->r[24] = 1;
        return 8114;
    }
    case 0x1fb2: { // cpi r20, 0x2C
        pm_sub(s, s->r[20], 44, 0, false);
        return 8116;
    }
    case 0x1fb4: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 8118;
    }
    case 0x1fb6: { // brlt .-128
        return (pm_getflag(s, 4) == 1) ? 7992 : 8120;
    }
    case 0x1fb8: { // rcall .+354
        s->calls[s->call_depth++] = 8122;
        return 8476;
    }
    case 0x1fba: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 8126 : 8124;
    }
    case 0x1fbc: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1fbe: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 8128;
    }
    case 0x1fc0: { // mov r10, r22
        s->r[10] = s->r[22];
        return 8130;
    }
    case 0x1fc2: { // add r10, r10
        s->r[10] = pm_add(s, s->r[10], s->r[10], 0);
        return 8132;
    }
    case 0x1fc4: { // add r10, r10
        s->r[10] = pm_add(s, s->r[10], s->r[10], 0);
        return 8134;
    }
    case 0x1fc6: { // ldi r18, 0x80
        s->r[18] = 128;
        return 8136;
    }
    case 0x1fc8: { // add r18, r10
        s->r[18] = pm_add(s, s->r[18], s->r[10], 0);
        return 8138;
    }
    case 0x1fca: { // cli
        pm_irq(s, false);
        return 8140;
    }
    case 0x1fcc: { // rcall .+832
        s->calls[s->call_depth++] = 8142;
        return 8974;
    }
    case 0x1fce: { // sei
        pm_irq(s, true);
        return 8144;
    }
    case 0x1fd0: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 8146;
    }
    case 0x1fd2: { // ldi r29, 0x21
        s->r[29] = 33;
        return 8148;
    }
    case 0x1fd4: { // add r10, r10
        s->r[10] = pm_add(s, s->r[10], s->r[10], 0);
        return 8150;
    }
    case 0x1fd6: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 8152;
    }
    case 0x1fd8: { // add r28, r10
        s->r[28] = pm_add(s, s->r[28], s->r[10], 0);
        return 8154;
    }
    case 0x1fda: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 8156;
    }
    case 0x1fdc: { // st Y+, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[20]);
        return 8158;
    }
    case 0x1fde: { // st Y, r21
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[21]);
        return 8160;
    }
    case 0x1fe0: { // rcall .+196
        s->calls[s->call_depth++] = 8162;
        return 8358;
    }
    case 0x1fe2: { // ldi r30, 0x98
        s->r[30] = 152;
        return 8164;
    }
    case 0x1fe4: { // ldi r31, 0x29
        s->r[31] = 41;
        return 8166;
    }
    case 0x1fe6: { // rcall .+2110
        s->calls[s->call_depth++] = 8168;
        return 10278;
    }
    case 0x1fe8: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1fea: { // rjmp .-3108
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
