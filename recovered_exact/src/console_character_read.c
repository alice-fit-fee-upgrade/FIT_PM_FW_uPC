#include "legacy_cpu.h"
#include <avr/io.h>
#define RAM8(address) (*(volatile uint8_t *)(address))

void cli_get_next_char(void)
{
    /* The byte-mode entry jumps past the first two instructions with R19=1. */
    asm volatile("clr r19\n\tpush r17\n\tpush r18\n\tpush r20\n\tpush r30\n\tpush r31" : : : "r19", "memory", "cc");
    register uint8_t *cursor asm("r30") = (uint8_t *)0x2000;
    asm volatile("" : "+z" (cursor));
    register uint8_t value asm("r16"), index asm("r17"), data asm("r18"), flow asm("r20"), mode asm("r19");
    asm volatile("" : "=r" (mode));
wait_character:
    pm_cpu_disable_irq();
    {
        register volatile uint8_t *queue asm("r30");
        asm volatile("" : "=z" (queue) : "z" (cursor) : "memory");
        index = queue[0];
        asm volatile("" : "+r" (index) : "z" (queue) : "memory");
        value = queue[1];
        asm volatile("" : "+r" (value) : : "memory");
    }
    pm_cpu_enable_irq();
    if (value == index) goto wait_character;
    asm volatile("inc %0" : "+r" (index) : : "cc");
    index &= 0x3f;
    asm volatile("" : "+r" (index));
    flow = PORTF_OUT;
    asm goto("sbrs %0, 0\n\trjmp %l[fetch]" : : "r" (flow) : : fetch);
    data = index;
    asm volatile("" : "+r" (data));
    data += 20;
    asm volatile("" : "+r" (data));
    data &= 0x3f;
    asm volatile("" : "+r" (data));
    asm volatile("cpse %1, %2\n\tldi %0, 1" : "+r" (flow) : "r" (value), "r" (data) : "cc");
    PORTF_OUTCLR = flow;
fetch:
    asm volatile("clr %0" : "=r" (data) : : "cc");
    cursor = (uint8_t *)0x2007;
    {
        register uint8_t address_low asm("r30");
        asm volatile("" : "=r" (address_low) : "z" (cursor));
        address_low += index;
        asm volatile("" : "+r" (address_low));
    }
    /* Upper-byte carry remains the original ADC; capture the full result. */
    asm volatile("adc r31, %1" : "=z" (cursor) : "r" (data) : "cc");
    data = *cursor;
    asm volatile("" : "+r" (data));
    RAM8(0x2000) = index;
    if (data != 13) goto case_fold;
    index = RAM8(0x2005);
    /* C equivalent: --index; trial438 failed binary matching.
     * No standalone functional validation; preserve exact DEC flags.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("dec %0" : "+r" (index) : : "cc");
    RAM8(0x2005) = index;
case_fold:
    /* Retain signed BRLT, mode bit and original mask rather than libc folding. */
    asm goto("cpi %0, 0x60\n\tbrlt %l[fold_done]\n\tbreq %l[fold_done]" : : "r" (data) : "cc" : fold_done);
    if (!(mode & 1u)) {
        data &= 0x5f;
        asm volatile("" : "+r" (data));
    }
fold_done:;
    value = data;
    asm volatile("" : : "r" (value));
    asm volatile("pop r31\n\tpop r30\n\tpop r20\n\tpop r18\n\tpop r17" : : : "memory");
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
uint32_t pm_logical_console_character_read(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x283c: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 10302;
    }
    case 0x283e: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 10304;
    }
    case 0x2840: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 10306;
    }
    case 0x2842: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 10308;
    }
    case 0x2844: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 10310;
    }
    case 0x2846: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 10312;
    }
    case 0x2848: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 10314;
    }
    case 0x284a: { // ldi r30, 0x00
        s->r[30] = 0;
        return 10316;
    }
    case 0x284c: { // ldi r31, 0x20
        s->r[31] = 32;
        return 10318;
    }
    case 0x284e: { // cli
        pm_irq(s, false);
        return 10320;
    }
    case 0x2850: { // ld r17, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[17] = pm_read(s, address);
        return 10322;
    }
    case 0x2852: { // ldd r16, Z+1
        uint16_t address = pm_pointer(s, 30) + 1;
        s->r[16] = pm_read(s, address);
        return 10324;
    }
    case 0x2854: { // sei
        pm_irq(s, true);
        return 10326;
    }
    case 0x2856: { // cp r16, r17
        pm_sub(s, s->r[16], s->r[17], 0, false);
        return 10328;
    }
    case 0x2858: { // breq .-12
        return (pm_getflag(s, 1) == 1) ? 10318 : 10330;
    }
    case 0x285a: { // inc r17
        s->r[17]++;
        pm_nzv(s, s->r[17], s->r[17] == 128);
        return 10332;
    }
    case 0x285c: { // andi r17, 0x3F
        s->r[17] &= 63;
        pm_nzv(s, s->r[17], false);
        return 10334;
    }
    case 0x285e: { // lds r20, 0x06A4
        uint16_t address = 1700;
        s->r[20] = pm_read(s, address);
        return 10338;
    }
    case 0x2862: { // sbrs r20, 0
        return (!!(s->r[20] & (1u << 0)) == 1) ? 10342 : 10340;
    }
    case 0x2864: { // rjmp .+14
        return 10356;
    }
    case 0x2866: { // mov r18, r17
        s->r[18] = s->r[17];
        return 10344;
    }
    case 0x2868: { // subi r18, 0xEC
        s->r[18] = pm_sub(s, s->r[18], 236, 0, false);
        return 10346;
    }
    case 0x286a: { // andi r18, 0x3F
        s->r[18] &= 63;
        pm_nzv(s, s->r[18], false);
        return 10348;
    }
    case 0x286c: { // cpse r16, r18
        return (s->r[16] == s->r[18]) ? 10352 : 10350;
    }
    case 0x286e: { // ldi r20, 0x01
        s->r[20] = 1;
        return 10352;
    }
    case 0x2870: { // sts 0x06A6, r20
        uint16_t address = 1702;
        pm_write(s, address, s->r[20]);
        return 10356;
    }
    case 0x2874: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 10358;
    }
    case 0x2876: { // ldi r30, 0x07
        s->r[30] = 7;
        return 10360;
    }
    case 0x2878: { // ldi r31, 0x20
        s->r[31] = 32;
        return 10362;
    }
    case 0x287a: { // add r30, r17
        s->r[30] = pm_add(s, s->r[30], s->r[17], 0);
        return 10364;
    }
    case 0x287c: { // adc r31, r18
        s->r[31] = pm_add(s, s->r[31], s->r[18], pm_getflag(s, CARRY));
        return 10366;
    }
    case 0x287e: { // ld r18, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[18] = pm_read(s, address);
        return 10368;
    }
    case 0x2880: { // sts 0x2000, r17
        uint16_t address = 8192;
        pm_write(s, address, s->r[17]);
        return 10372;
    }
    case 0x2884: { // cpi r18, 0x0D
        pm_sub(s, s->r[18], 13, 0, false);
        return 10374;
    }
    case 0x2886: { // brne .+10
        return (pm_getflag(s, 1) == 0) ? 10386 : 10376;
    }
    case 0x2888: { // lds r17, 0x2005
        uint16_t address = 8197;
        s->r[17] = pm_read(s, address);
        return 10380;
    }
    case 0x288c: { // dec r17
        s->r[17]--;
        pm_nzv(s, s->r[17], s->r[17] == 127);
        return 10382;
    }
    case 0x288e: { // sts 0x2005, r17
        uint16_t address = 8197;
        pm_write(s, address, s->r[17]);
        return 10386;
    }
    case 0x2892: { // cpi r18, 0x60
        pm_sub(s, s->r[18], 96, 0, false);
        return 10388;
    }
    case 0x2894: { // brlt .+6
        return (pm_getflag(s, 4) == 1) ? 10396 : 10390;
    }
    case 0x2896: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 10396 : 10392;
    }
    case 0x2898: { // sbrs r19, 0
        return (!!(s->r[19] & (1u << 0)) == 1) ? 10396 : 10394;
    }
    case 0x289a: { // andi r18, 0x5F
        s->r[18] &= 95;
        pm_nzv(s, s->r[18], false);
        return 10396;
    }
    case 0x289c: { // mov r16, r18
        s->r[16] = s->r[18];
        return 10398;
    }
    case 0x289e: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 10400;
    }
    case 0x28a0: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 10402;
    }
    case 0x28a2: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 10404;
    }
    case 0x28a4: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 10406;
    }
    case 0x28a6: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 10408;
    }
    case 0x28a8: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 10410;
    }
    case 0x28aa: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
