/* C value equivalent of the retained queue loads:
 * read_index = *cursor++; write_index = *cursor++;
 * Exact LD Z+ encodings and the private register/frame contract remain ASM.
 * Earlier pointer trials changed the generated code; this comment describes
 * the values only, with no independent functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
#include "legacy_cpu.h"
#include <avr/io.h>

void cli_send_buf(void)
{
    asm volatile("push r30\n\tpush r20\n\tpush r19\n\tpush r18\n\tpush r17" : : : "memory");
    register uint8_t character asm("r16");
    asm volatile("" : "=r" (character));
    register uint8_t *cursor asm("r30") = (uint8_t *)0x2002;
    asm volatile("" : "+z" (cursor));
    register uint8_t read_index asm("r17"), write_index asm("r18"), ready asm("r19"), control asm("r20");
retry:
    pm_cpu_disable_irq();
    asm volatile("ld %0, Z+\n\tld %1, Z+"
        : "=r" (read_index), "=r" (write_index), "+z" (cursor) : : "memory");
    asm goto("cp %0, %1\n\tbreq %l[direct_send]" : : "r" (read_index), "r" (write_index) : "cc" : direct_send);
    asm volatile("inc %0" : "+r" (write_index) : : "cc");
    asm goto("cp %0, %1\n\tbrne %l[enqueue]" : : "r" (read_index), "r" (write_index) : "cc" : enqueue);
    pm_cpu_enable_irq();
    cursor -= 2;
    asm volatile("" : "+z" (cursor));
    goto retry;
enqueue:
    cursor = (uint8_t *)0x2047;
    asm volatile("clr %0" : "=r" (read_index) : "z" (cursor) : "cc");
    {
        register uint8_t address_low asm("r30");
        asm volatile("" : "=r" (address_low) : "z" (cursor));
        address_low += write_index;
        asm volatile("" : "+r" (address_low));
    }
    /* Upper-byte carry remains the original ADC; capture the full result. */
    asm volatile("adc r31, %1" : "=z" (cursor) : "r" (read_index) : "cc");
    *cursor = character;
    cursor = (uint8_t *)0x2003;
    asm volatile("" : "+z" (cursor));
    *cursor = write_index;
    pm_cpu_enable_irq();
    goto finished;
direct_send:
    asm volatile("inc %0" : "+r" (write_index) : : "cc");
    ready = *cursor;
    asm goto("tst %0\n\tbreq %l[enqueue]" : : "r" (ready) : "cc" : enqueue);
    control = USARTF0_STATUS;
    asm goto("sbrs %0, 5\n\trjmp %l[enqueue]" : : "r" (control) : : enqueue);
    USARTF0_DATA = character;
    control = USARTF0_CTRLA;
    control |= 2;
    asm volatile("" : "+r" (control));
    USARTF0_CTRLA = control;
    pm_cpu_enable_irq();
finished:
    asm volatile("pop r17\n\tpop r18\n\tpop r19\n\tpop r20\n\tpop r30" : : : "memory");
    return;
}

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
uint32_t pm_logical_console_character_send(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x28ac: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 10414;
    }
    case 0x28ae: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 10416;
    }
    case 0x28b0: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 10418;
    }
    case 0x28b2: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 10420;
    }
    case 0x28b4: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 10422;
    }
    case 0x28b6: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 10424;
    }
    case 0x28b8: { // ldi r30, 0x02
        s->r[30] = 2;
        return 10426;
    }
    case 0x28ba: { // ldi r31, 0x20
        s->r[31] = 32;
        return 10428;
    }
    case 0x28bc: { // cli
        pm_irq(s, false);
        return 10430;
    }
    case 0x28be: { // ld r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_read(s, address);
        return 10432;
    }
    case 0x28c0: { // ld r18, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[18] = pm_read(s, address);
        return 10434;
    }
    case 0x28c2: { // cp r17, r18
        pm_sub(s, s->r[17], s->r[18], 0, false);
        return 10436;
    }
    case 0x28c4: { // breq .+34
        return (pm_getflag(s, 1) == 1) ? 10472 : 10438;
    }
    case 0x28c6: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 10440;
    }
    case 0x28c8: { // cp r17, r18
        pm_sub(s, s->r[17], s->r[18], 0, false);
        return 10442;
    }
    case 0x28ca: { // brne .+6
        return (pm_getflag(s, 1) == 0) ? 10450 : 10444;
    }
    case 0x28cc: { // sei
        pm_irq(s, true);
        return 10446;
    }
    case 0x28ce: { // sbiw r30, 0x02
        uint16_t old = pm_pointer(s, 30);
        uint16_t value = old - 2;
        pm_setpointer(s, 30, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 10448;
    }
    case 0x28d0: { // rjmp .-22
        return 10428;
    }
    case 0x28d2: { // ldi r30, 0x47
        s->r[30] = 71;
        return 10452;
    }
    case 0x28d4: { // ldi r31, 0x20
        s->r[31] = 32;
        return 10454;
    }
    case 0x28d6: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 10456;
    }
    case 0x28d8: { // add r30, r18
        s->r[30] = pm_add(s, s->r[30], s->r[18], 0);
        return 10458;
    }
    case 0x28da: { // adc r31, r17
        s->r[31] = pm_add(s, s->r[31], s->r[17], pm_getflag(s, CARRY));
        return 10460;
    }
    case 0x28dc: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 10462;
    }
    case 0x28de: { // ldi r30, 0x03
        s->r[30] = 3;
        return 10464;
    }
    case 0x28e0: { // ldi r31, 0x20
        s->r[31] = 32;
        return 10466;
    }
    case 0x28e2: { // st Z, r18
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 10468;
    }
    case 0x28e4: { // sei
        pm_irq(s, true);
        return 10470;
    }
    case 0x28e6: { // rjmp .+32
        return 10504;
    }
    case 0x28e8: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 10474;
    }
    case 0x28ea: { // ld r19, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[19] = pm_read(s, address);
        return 10476;
    }
    case 0x28ec: { // and r19, r19
        s->r[19] &= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 10478;
    }
    case 0x28ee: { // breq .-30
        return (pm_getflag(s, 1) == 1) ? 10450 : 10480;
    }
    case 0x28f0: { // lds r20, 0x0BA1
        uint16_t address = 2977;
        s->r[20] = pm_read(s, address);
        return 10484;
    }
    case 0x28f4: { // sbrs r20, 5
        return (!!(s->r[20] & (1u << 5)) == 1) ? 10488 : 10486;
    }
    case 0x28f6: { // rjmp .-38
        return 10450;
    }
    case 0x28f8: { // sts 0x0BA0, r16
        uint16_t address = 2976;
        pm_write(s, address, s->r[16]);
        return 10492;
    }
    case 0x28fc: { // lds r20, 0x0BA3
        uint16_t address = 2979;
        s->r[20] = pm_read(s, address);
        return 10496;
    }
    case 0x2900: { // ori r20, 0x02
        s->r[20] |= 2;
        pm_nzv(s, s->r[20], false);
        return 10498;
    }
    case 0x2902: { // sts 0x0BA3, r20
        uint16_t address = 2979;
        pm_write(s, address, s->r[20]);
        return 10502;
    }
    case 0x2906: { // sei
        pm_irq(s, true);
        return 10504;
    }
    case 0x2908: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 10506;
    }
    case 0x290a: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 10508;
    }
    case 0x290c: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 10510;
    }
    case 0x290e: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 10512;
    }
    case 0x2910: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 10514;
    }
    case 0x2912: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 10516;
    }
    case 0x2914: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
