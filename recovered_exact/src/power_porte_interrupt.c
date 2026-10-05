#include "legacy_cpu.h"
#include <avr/io.h>
#include "legacy_interrupt.h"
#include "legacy_r16_c.h"
#define SET_VALUE(constant) do { value=(constant); asm volatile("" : "+r" (value)); } while (0)

void PORTE_INT0_vect_isr(void)
{
    {
        register uint8_t saved_status asm("r31") = SREG;
        asm volatile("" : "+r" (saved_status) : : "memory");
    }
    pm_cpu_disable_irq();
    asm volatile("push r16\n\tpush r17\n\tpush r18" : : : "memory");
    register uint8_t value asm("r16"), flags asm("r17"), changed asm("r18");
    asm volatile("clr %0" : "=r" (flags) : : "cc");
    value = PORTE_IN;
    asm volatile("" : "+r" (value), "+r" (flags));
    flags = PM_COPY_BIT(flags, 0, value, 1);
    asm volatile("" : "+r" (flags), "+r" (value));
    /* Unvalidated C equivalent: flags = PM_COPY_BIT(flags, 3, value, 3);
     * Combined trial500 and isolated525 changed layout. The first bit copy
     * passed in524. Final BST captures live T for the later BRTC.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("bst %0, 3\n\tbld %1, 3\n\tbst %0, 2"
        : "+r" (value), "+r" (flags) : : "cc");
    value = pm_read_absolute(0x2157);
    changed = value; asm volatile("" : "+r" (changed));
    value &= 0xf6; asm volatile("" : "+r" (value));
    value |= flags; asm volatile("" : "+r" (value));
    PM_RAM8(0x2157) = value;
    changed ^= flags; asm volatile("" : "+r" (changed));
    if (!(changed & (1u << 0))) goto watch_retry;
    if (flags & (1u << 0)) goto power_on;
    SET_VALUE(4);
    PORTE_OUTCLR = value;
    GPIOR0 |= (1u << 1);
    asm volatile("rcall system_deinit" : "=r" (value) : : "memory", "cc");
    goto update_led;
power_on:
    flags = 0xd0; asm volatile("" : "+r" (flags));
    changed = 7; asm volatile("" : "+r" (changed));
store_retry:
    SET_VALUE(1);
    PM_RAM8(0x215b) = value;
    PM_RAM8(0x215c) = flags;
    PM_RAM8(0x215d) = changed;
    GPIOR0 &= (uint8_t)~(1u << 1);
    goto update_led;
watch_retry:
    if (!(changed & (1u << 3))) goto finished;
    if (flags & (1u << 3)) goto finished;
    value = pm_read_absolute(0x215b);
    if (value != 2) goto deinitialize;
    value = pm_read_absolute(0x2442);
    if (value == 0) goto deinitialize;
    asm volatile("dec %0" : "+r" (value) : : "cc");
    PM_RAM8(0x2442) = value;
    SET_VALUE(4);
    PORTE_OUTCLR = value;
    flags = 0x88; asm volatile("" : "+r" (flags));
    changed = 0x13; asm volatile("" : "+r" (changed));
    goto store_retry;
deinitialize:
    asm volatile("rcall system_deinit" : "=r" (value) : : "memory", "cc");
    asm goto("brtc %l[finished]" : : : : finished);
    GPIOR0 |= (1u << 1);
update_led:
    asm volatile("rcall set_status_and_vd8_led" : "=r" (value) : : "memory", "cc");
finished:
    asm volatile("pop r18\n\tpop r17\n\tpop r16" : : : "memory");
    {
        register uint8_t saved_status asm("r31");
        asm volatile("" : "=r" (saved_status) : : "memory");
        SREG = saved_status;
    }
    /* C operation: restore_private_frame_and_return_from_interrupt();
     * POP/RETI retain the original interrupt frame and return contract. */
    asm volatile("pop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 1008 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_power_porte_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0b96: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 2968;
    }
    case 0x0b98: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 2970;
    }
    case 0x0b9a: { // cli
        pm_irq(s, false);
        return 2972;
    }
    case 0x0b9c: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 2974;
    }
    case 0x0b9e: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 2976;
    }
    case 0x0ba0: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 2978;
    }
    case 0x0ba2: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 2980;
    }
    case 0x0ba4: { // lds r16, 0x0688
        uint16_t address = 1672;
        s->r[16] = pm_read(s, address);
        return 2984;
    }
    case 0x0ba8: { // bst r16, 1
        pm_flag(s, TRANSFER, s->r[16] & (1u << 1));
        return 2986;
    }
    case 0x0baa: { // bld r17, 0
        s->r[17] = (s->r[17] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 2988;
    }
    case 0x0bac: { // bst r16, 3
        pm_flag(s, TRANSFER, s->r[16] & (1u << 3));
        return 2990;
    }
    case 0x0bae: { // bld r17, 3
        s->r[17] = (s->r[17] & ~(1u << 3)) | (pm_getflag(s, TRANSFER) << 3);
        return 2992;
    }
    case 0x0bb0: { // bst r16, 2
        pm_flag(s, TRANSFER, s->r[16] & (1u << 2));
        return 2994;
    }
    case 0x0bb2: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 2998;
    }
    case 0x0bb6: { // mov r18, r16
        s->r[18] = s->r[16];
        return 3000;
    }
    case 0x0bb8: { // andi r16, 0xF6
        s->r[16] &= 246;
        pm_nzv(s, s->r[16], false);
        return 3002;
    }
    case 0x0bba: { // or r16, r17
        s->r[16] |= s->r[17];
        pm_nzv(s, s->r[16], false);
        return 3004;
    }
    case 0x0bbc: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 3008;
    }
    case 0x0bc0: { // eor r18, r17
        s->r[18] ^= s->r[17];
        pm_nzv(s, s->r[18], false);
        return 3010;
    }
    case 0x0bc2: { // sbrs r18, 0
        return (!!(s->r[18] & (1u << 0)) == 1) ? 3014 : 3012;
    }
    case 0x0bc4: { // rjmp .+38
        return 3052;
    }
    case 0x0bc6: { // sbrc r17, 0
        return (!!(s->r[17] & (1u << 0)) == 0) ? 3018 : 3016;
    }
    case 0x0bc8: { // rjmp .+12
        return 3030;
    }
    case 0x0bca: { // ldi r16, 0x04
        s->r[16] = 4;
        return 3020;
    }
    case 0x0bcc: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 3024;
    }
    case 0x0bd0: { // sbi 0x00, 1
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 1));
        return 3026;
    }
    case 0x0bd2: { // rcall .+194
        s->calls[s->call_depth++] = 3028;
        return 3222;
    }
    case 0x0bd4: { // rjmp .+70
        return 3100;
    }
    case 0x0bd6: { // ldi r17, 0xD0
        s->r[17] = 208;
        return 3032;
    }
    case 0x0bd8: { // ldi r18, 0x07
        s->r[18] = 7;
        return 3034;
    }
    case 0x0bda: { // ldi r16, 0x01
        s->r[16] = 1;
        return 3036;
    }
    case 0x0bdc: { // sts 0x215B, r16
        uint16_t address = 8539;
        pm_write(s, address, s->r[16]);
        return 3040;
    }
    case 0x0be0: { // sts 0x215C, r17
        uint16_t address = 8540;
        pm_write(s, address, s->r[17]);
        return 3044;
    }
    case 0x0be4: { // sts 0x215D, r18
        uint16_t address = 8541;
        pm_write(s, address, s->r[18]);
        return 3048;
    }
    case 0x0be8: { // cbi 0x00, 1
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 1));
        return 3050;
    }
    case 0x0bea: { // rjmp .+48
        return 3100;
    }
    case 0x0bec: { // sbrs r18, 3
        return (!!(s->r[18] & (1u << 3)) == 1) ? 3056 : 3054;
    }
    case 0x0bee: { // rjmp .+46
        return 3102;
    }
    case 0x0bf0: { // sbrc r17, 3
        return (!!(s->r[17] & (1u << 3)) == 0) ? 3060 : 3058;
    }
    case 0x0bf2: { // rjmp .+42
        return 3102;
    }
    case 0x0bf4: { // lds r16, 0x215B
        uint16_t address = 8539;
        s->r[16] = pm_read(s, address);
        return 3064;
    }
    case 0x0bf8: { // cpi r16, 0x02
        pm_sub(s, s->r[16], 2, 0, false);
        return 3066;
    }
    case 0x0bfa: { // brne .+26
        return (pm_getflag(s, 1) == 0) ? 3094 : 3068;
    }
    case 0x0bfc: { // lds r16, 0x2442
        uint16_t address = 9282;
        s->r[16] = pm_read(s, address);
        return 3072;
    }
    case 0x0c00: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3074;
    }
    case 0x0c02: { // breq .+18
        return (pm_getflag(s, 1) == 1) ? 3094 : 3076;
    }
    case 0x0c04: { // dec r16
        s->r[16]--;
        pm_nzv(s, s->r[16], s->r[16] == 127);
        return 3078;
    }
    case 0x0c06: { // sts 0x2442, r16
        uint16_t address = 9282;
        pm_write(s, address, s->r[16]);
        return 3082;
    }
    case 0x0c0a: { // ldi r16, 0x04
        s->r[16] = 4;
        return 3084;
    }
    case 0x0c0c: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 3088;
    }
    case 0x0c10: { // ldi r17, 0x88
        s->r[17] = 136;
        return 3090;
    }
    case 0x0c12: { // ldi r18, 0x13
        s->r[18] = 19;
        return 3092;
    }
    case 0x0c14: { // rjmp .-60
        return 3034;
    }
    case 0x0c16: { // rcall .+126
        s->calls[s->call_depth++] = 3096;
        return 3222;
    }
    case 0x0c18: { // brtc .+4
        return (pm_getflag(s, 6) == 0) ? 3102 : 3098;
    }
    case 0x0c1a: { // sbi 0x00, 1
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 1));
        return 3100;
    }
    case 0x0c1c: { // rcall .+96
        s->calls[s->call_depth++] = 3102;
        return 3198;
    }
    case 0x0c1e: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 3104;
    }
    case 0x0c20: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 3106;
    }
    case 0x0c22: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 3108;
    }
    case 0x0c24: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 3110;
    }
    case 0x0c26: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3112;
    }
    case 0x0c28: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
