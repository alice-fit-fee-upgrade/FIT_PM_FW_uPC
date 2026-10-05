/* Native outer frame: GCC emits the original entry PUSH under the local
 * call-saved register profile. -fno-ipa-pure-const prevents noreturn inference
 * from dropping that save before the exact shared RET/RETI tail. The tail
 * restores this register in ASM; full FLASH identity validates the pairing. */
/* C equivalent of the retained cursor-read helper:
 * ready++; data = *--cursor; read_index = *--cursor;
 * Steps365–370 failed allocation or changed fixed layout/bytes, including
 * one local Z-allocation retry. No standalone functional test is claimed.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
#include <avr/io.h>

void PORTF_INT0_vect_isr(void)
{
    {
        register uint8_t saved_status asm("r31") = SREG;
        asm volatile("" : "+r" (saved_status) : : "memory");
    }
    asm volatile("push r31\n\tpush r30\n\tpush r29\n\tpush r18\n\tpush r17\n\tpush r16" : : : "memory");
    register uint8_t *cursor asm("r30") = (uint8_t *)0x2004;
    asm volatile("" : "+z" (cursor));
    register uint8_t ready asm("r29");
    asm volatile("clr %0" : "=r" (ready) : : "cc");
    register uint8_t read_index asm("r16") = PORTF_IN;
    register uint8_t data asm("r17"), control asm("r18");
    asm goto("sbrc %0, 1\n\trjmp %l[store_ready]" : : "r" (read_index) : : store_ready);
    asm volatile("inc %0\n\tld %1, -Z\n\tld %2, -Z"
        : "+r" (ready), "=r" (data), "=r" (read_index), "+z" (cursor) : : "memory", "cc");
    if (read_index == data) goto advance_ready;
    control = USARTF0_STATUS;
    asm goto("sbrs %0, 5\n\trjmp %l[enable_tx]" : : "r" (control) : : enable_tx);
    asm volatile("clr %0\n\tinc %1" : "=r" (data), "+r" (read_index) : : "cc");
    cursor = (uint8_t *)0x2047;
    {
        register uint8_t address_low asm("r30");
        asm volatile("" : "=r" (address_low) : "z" (cursor));
        address_low += read_index;
        asm volatile("" : "+r" (address_low));
    }
    /* Upper-byte carry remains the original ADC; capture the full result. */
    asm volatile("adc r31, %1" : "=z" (cursor) : "r" (data) : "cc");
    data = *cursor;
    asm volatile("" : "+r" (data));
    USARTF0_DATA = data;
    cursor = (uint8_t *)0x2002;
    asm volatile("" : "+z" (cursor));
    *cursor = read_index;
enable_tx:
    control = USARTF0_CTRLA;
    control |= 2;
    asm volatile("" : "+r" (control));
    USARTF0_CTRLA = control;
advance_ready:
    cursor += 2;
    asm volatile("" : "+z" (cursor));
store_ready:
    *cursor = ready;
    asm volatile("pop r16\n\tpop r17\n\tpop r18\n\tpop r29\n\tpop r30\n\tpop r31" : : : "memory");
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
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 752 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_cts_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0e20: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 3618;
    }
    case 0x0e22: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 3620;
    }
    case 0x0e24: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 3622;
    }
    case 0x0e26: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 3624;
    }
    case 0x0e28: { // push r29
        s->stack[s->depth++] = s->r[29];
        return 3626;
    }
    case 0x0e2a: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 3628;
    }
    case 0x0e2c: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 3630;
    }
    case 0x0e2e: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 3632;
    }
    case 0x0e30: { // ldi r30, 0x04
        s->r[30] = 4;
        return 3634;
    }
    case 0x0e32: { // ldi r31, 0x20
        s->r[31] = 32;
        return 3636;
    }
    case 0x0e34: { // eor r29, r29
        s->r[29] ^= s->r[29];
        pm_nzv(s, s->r[29], false);
        return 3638;
    }
    case 0x0e36: { // lds r16, 0x06A8
        uint16_t address = 1704;
        s->r[16] = pm_read(s, address);
        return 3642;
    }
    case 0x0e3a: { // sbrc r16, 1
        return (!!(s->r[16] & (1u << 1)) == 0) ? 3646 : 3644;
    }
    case 0x0e3c: { // rjmp .+54
        return 3700;
    }
    case 0x0e3e: { // inc r29
        s->r[29]++;
        pm_nzv(s, s->r[29], s->r[29] == 128);
        return 3648;
    }
    case 0x0e40: { // ld r17, -Z
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[17] = pm_read(s, address);
        return 3650;
    }
    case 0x0e42: { // ld r16, -Z
        pm_setpointer(s, 30, pm_pointer(s, 30) - 1);
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[16] = pm_read(s, address);
        return 3652;
    }
    case 0x0e44: { // cp r16, r17
        pm_sub(s, s->r[16], s->r[17], 0, false);
        return 3654;
    }
    case 0x0e46: { // breq .+42
        return (pm_getflag(s, 1) == 1) ? 3698 : 3656;
    }
    case 0x0e48: { // lds r18, 0x0BA1
        uint16_t address = 2977;
        s->r[18] = pm_read(s, address);
        return 3660;
    }
    case 0x0e4c: { // sbrs r18, 5
        return (!!(s->r[18] & (1u << 5)) == 1) ? 3664 : 3662;
    }
    case 0x0e4e: { // rjmp .+24
        return 3688;
    }
    case 0x0e50: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 3666;
    }
    case 0x0e52: { // inc r16
        s->r[16]++;
        pm_nzv(s, s->r[16], s->r[16] == 128);
        return 3668;
    }
    case 0x0e54: { // ldi r30, 0x47
        s->r[30] = 71;
        return 3670;
    }
    case 0x0e56: { // ldi r31, 0x20
        s->r[31] = 32;
        return 3672;
    }
    case 0x0e58: { // add r30, r16
        s->r[30] = pm_add(s, s->r[30], s->r[16], 0);
        return 3674;
    }
    case 0x0e5a: { // adc r31, r17
        s->r[31] = pm_add(s, s->r[31], s->r[17], pm_getflag(s, CARRY));
        return 3676;
    }
    case 0x0e5c: { // ld r17, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[17] = pm_read(s, address);
        return 3678;
    }
    case 0x0e5e: { // sts 0x0BA0, r17
        uint16_t address = 2976;
        pm_write(s, address, s->r[17]);
        return 3682;
    }
    case 0x0e62: { // ldi r30, 0x02
        s->r[30] = 2;
        return 3684;
    }
    case 0x0e64: { // ldi r31, 0x20
        s->r[31] = 32;
        return 3686;
    }
    case 0x0e66: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 3688;
    }
    case 0x0e68: { // lds r18, 0x0BA3
        uint16_t address = 2979;
        s->r[18] = pm_read(s, address);
        return 3692;
    }
    case 0x0e6c: { // ori r18, 0x02
        s->r[18] |= 2;
        pm_nzv(s, s->r[18], false);
        return 3694;
    }
    case 0x0e6e: { // sts 0x0BA3, r18
        uint16_t address = 2979;
        pm_write(s, address, s->r[18]);
        return 3698;
    }
    case 0x0e72: { // adiw r30, 0x02
        uint16_t old = pm_pointer(s, 30);
        uint16_t value = old + 2;
        pm_setpointer(s, 30, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 3700;
    }
    case 0x0e74: { // st Z, r29
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[29]);
        return 3702;
    }
    case 0x0e76: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 3704;
    }
    case 0x0e78: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 3706;
    }
    case 0x0e7a: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 3708;
    }
    case 0x0e7c: { // pop r29
        s->r[29] = s->stack[--s->depth];
        return 3710;
    }
    case 0x0e7e: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 3712;
    }
    case 0x0e80: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3714;
    }
    case 0x0e82: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 3716;
    }
    case 0x0e84: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3718;
    }
    case 0x0e86: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
