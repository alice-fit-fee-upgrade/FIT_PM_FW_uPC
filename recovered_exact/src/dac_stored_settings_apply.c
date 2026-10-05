/* Private register entries; value/channel barriers preserve the original ABI. */
extern void FUN_code_001053(void);
extern void dac_set_value_2(void);
extern void dac_set_value(void);
extern void FUN_code_001068(void);
#include <stdint.h>
#define LOAD_SETTING(value, cursor) asm volatile("ld r20, Y+\n\tld r21, Y+" : "=r" (value), "+y" (cursor) : : "memory")
#define DAC_CALL(name, value, channel) do { \
 asm volatile("" : "+r" (value), "+r" (channel) : : "memory"); \
 name(); \
 asm volatile("" : "=r" (value), "=r" (channel) : : "memory"); \
} while (0)

/* Apply the four stored words for each channel. The exact save frame is
 * required because callers expect all fourteen original registers restored. */
void FUN_code_0004ef(void)
{
    asm volatile("push r31\n\tpush r30\n\tpush r29\n\tpush r28\n\tpush r23\n\tpush r22\n\tpush r21\n\tpush r20\n\tpush r19\n\tpush r18\n\tpush r17\n\tpush r16\n\tpush r1\n\tpush r0" : : : "memory");
    register uint8_t channel asm("r23");
    asm volatile("clr %0" : "=r" (channel) : : "cc");
    register uint8_t *settings asm("r28") = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
next_channel:;
    register uint16_t word asm("r20");
    register uint8_t selected asm("r22");
    LOAD_SETTING(word, settings);
    selected = channel; asm volatile("" : "+r" (selected), "+r" (channel));
    DAC_CALL(FUN_code_001053, word, selected);
    LOAD_SETTING(word, settings);
    selected = channel; asm volatile("" : "+r" (selected), "+r" (channel));
    DAC_CALL(dac_set_value_2, word, selected);
    LOAD_SETTING(word, settings);
    selected = channel; asm volatile("" : "+r" (selected), "+r" (channel));
    DAC_CALL(dac_set_value, word, selected);
    LOAD_SETTING(word, settings);
    selected = channel; asm volatile("" : "+r" (selected), "+r" (channel));
    DAC_CALL(FUN_code_001068, word, selected);
    asm volatile("inc %0" : "+r" (channel) : : "cc");
    if (channel != 12) goto next_channel;
    asm volatile("pop r0\n\tpop r1\n\tpop r16\n\tpop r17\n\tpop r18\n\tpop r19\n\tpop r20\n\tpop r21\n\tpop r22\n\tpop r23\n\tpop r28\n\tpop r29\n\tpop r30\n\tpop r31" : : : "memory");
    return;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 816 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_dac_stored_settings_apply(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x09de: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 2528;
    }
    case 0x09e0: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 2530;
    }
    case 0x09e2: { // push r29
        s->stack[s->depth++] = s->r[29];
        return 2532;
    }
    case 0x09e4: { // push r28
        s->stack[s->depth++] = s->r[28];
        return 2534;
    }
    case 0x09e6: { // push r23
        s->stack[s->depth++] = s->r[23];
        return 2536;
    }
    case 0x09e8: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 2538;
    }
    case 0x09ea: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 2540;
    }
    case 0x09ec: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 2542;
    }
    case 0x09ee: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 2544;
    }
    case 0x09f0: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 2546;
    }
    case 0x09f2: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 2548;
    }
    case 0x09f4: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 2550;
    }
    case 0x09f6: { // push r1
        s->stack[s->depth++] = s->r[1];
        return 2552;
    }
    case 0x09f8: { // push r0
        s->stack[s->depth++] = s->r[0];
        return 2554;
    }
    case 0x09fa: { // eor r23, r23
        s->r[23] ^= s->r[23];
        pm_nzv(s, s->r[23], false);
        return 2556;
    }
    case 0x09fc: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 2558;
    }
    case 0x09fe: { // ldi r29, 0x21
        s->r[29] = 33;
        return 2560;
    }
    case 0x0a00: { // ld r20, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[20] = pm_read(s, address);
        return 2562;
    }
    case 0x0a02: { // ld r21, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[21] = pm_read(s, address);
        return 2564;
    }
    case 0x0a04: { // mov r22, r23
        s->r[22] = s->r[23];
        return 2566;
    }
    case 0x0a06: { // call 0x20a6
        s->calls[s->call_depth++] = 2570;
        return 8358;
    }
    case 0x0a0a: { // ld r20, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[20] = pm_read(s, address);
        return 2572;
    }
    case 0x0a0c: { // ld r21, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[21] = pm_read(s, address);
        return 2574;
    }
    case 0x0a0e: { // mov r22, r23
        s->r[22] = s->r[23];
        return 2576;
    }
    case 0x0a10: { // call 0x20ec
        s->calls[s->call_depth++] = 2580;
        return 8428;
    }
    case 0x0a14: { // ld r20, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[20] = pm_read(s, address);
        return 2582;
    }
    case 0x0a16: { // ld r21, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[21] = pm_read(s, address);
        return 2584;
    }
    case 0x0a18: { // mov r22, r23
        s->r[22] = s->r[23];
        return 2586;
    }
    case 0x0a1a: { // call 0x2104
        s->calls[s->call_depth++] = 2590;
        return 8452;
    }
    case 0x0a1e: { // ld r20, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[20] = pm_read(s, address);
        return 2592;
    }
    case 0x0a20: { // ld r21, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[21] = pm_read(s, address);
        return 2594;
    }
    case 0x0a22: { // mov r22, r23
        s->r[22] = s->r[23];
        return 2596;
    }
    case 0x0a24: { // call 0x20d0
        s->calls[s->call_depth++] = 2600;
        return 8400;
    }
    case 0x0a28: { // inc r23
        s->r[23]++;
        pm_nzv(s, s->r[23], s->r[23] == 128);
        return 2602;
    }
    case 0x0a2a: { // cpi r23, 0x0C
        pm_sub(s, s->r[23], 12, 0, false);
        return 2604;
    }
    case 0x0a2c: { // brne .-46
        return (pm_getflag(s, 1) == 0) ? 2560 : 2606;
    }
    case 0x0a2e: { // pop r0
        s->r[0] = s->stack[--s->depth];
        return 2608;
    }
    case 0x0a30: { // pop r1
        s->r[1] = s->stack[--s->depth];
        return 2610;
    }
    case 0x0a32: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 2612;
    }
    case 0x0a34: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 2614;
    }
    case 0x0a36: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 2616;
    }
    case 0x0a38: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 2618;
    }
    case 0x0a3a: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 2620;
    }
    case 0x0a3c: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 2622;
    }
    case 0x0a3e: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 2624;
    }
    case 0x0a40: { // pop r23
        s->r[23] = s->stack[--s->depth];
        return 2626;
    }
    case 0x0a42: { // pop r28
        s->r[28] = s->stack[--s->depth];
        return 2628;
    }
    case 0x0a44: { // pop r29
        s->r[29] = s->stack[--s->depth];
        return 2630;
    }
    case 0x0a46: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 2632;
    }
    case 0x0a48: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 2634;
    }
    case 0x0a4a: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
