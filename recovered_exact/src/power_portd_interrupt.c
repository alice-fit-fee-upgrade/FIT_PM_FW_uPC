#include "legacy_cpu.h"
#include "legacy_interrupt_register_c.h"
#include "legacy_r16_c.h"

void PORTD_INT0_vect_isr(void)
{
    PM_ISR_ENTER_R16();
    register uint8_t value asm("r16") = PORTD_IN;
    /* C flags equivalent: saved_t = (value >> 3) & 1; disable_irq();
     * Keep BST because the later BLD consumes CPU T across this IRQ window. */
    asm volatile("bst %0, 3" : : "r" (value) : "cc");
    pm_cpu_disable_irq();
    value = pm_read_absolute(0x2157);
    asm volatile("bld %0, 4" : "+r" (value) : : "cc");
    PM_RAM8(0x2157) = value;
    pm_cpu_enable_irq();
    asm goto("brtc %l[cleared]" : : : : cleared);
    pm_cpu_disable_irq();
    value = 5;
    asm volatile("rcall FUN_code_00054e" : "+r" (value) : : "memory", "cc");
    pm_cpu_enable_irq();
    goto leave;
cleared:
    PORTE_INTCTRL = 1;
    PM_RAM8(0x2158) = pm_scratch_zero();
    value = pm_read_absolute(0x2157);
    PM_RAM8(0x2157) = value & 0x7f;
    PORTA_OUTSET = 0xc0;
    PM_RAM8(0x2441) = 1;
leave:
    PM_ISR_LEAVE_R16();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 496 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_power_portd_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0a4c: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 2638;
    }
    case 0x0a4e: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 2640;
    }
    case 0x0a50: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 2642;
    }
    case 0x0a52: { // lds r16, 0x0668
        uint16_t address = 1640;
        s->r[16] = pm_read(s, address);
        return 2646;
    }
    case 0x0a56: { // bst r16, 3
        pm_flag(s, TRANSFER, s->r[16] & (1u << 3));
        return 2648;
    }
    case 0x0a58: { // cli
        pm_irq(s, false);
        return 2650;
    }
    case 0x0a5a: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 2654;
    }
    case 0x0a5e: { // bld r16, 4
        s->r[16] = (s->r[16] & ~(1u << 4)) | (pm_getflag(s, TRANSFER) << 4);
        return 2656;
    }
    case 0x0a60: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 2660;
    }
    case 0x0a64: { // sei
        pm_irq(s, true);
        return 2662;
    }
    case 0x0a66: { // brtc .+10
        return (pm_getflag(s, 6) == 0) ? 2674 : 2664;
    }
    case 0x0a68: { // cli
        pm_irq(s, false);
        return 2666;
    }
    case 0x0a6a: { // ldi r16, 0x05
        s->r[16] = 5;
        return 2668;
    }
    case 0x0a6c: { // rcall .+46
        s->calls[s->call_depth++] = 2670;
        return 2716;
    }
    case 0x0a6e: { // sei
        pm_irq(s, true);
        return 2672;
    }
    case 0x0a70: { // rjmp .+34
        return 2708;
    }
    case 0x0a72: { // ldi r16, 0x01
        s->r[16] = 1;
        return 2676;
    }
    case 0x0a74: { // sts 0x0689, r16
        uint16_t address = 1673;
        pm_write(s, address, s->r[16]);
        return 2680;
    }
    case 0x0a78: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 2682;
    }
    case 0x0a7a: { // sts 0x2158, r16
        uint16_t address = 8536;
        pm_write(s, address, s->r[16]);
        return 2686;
    }
    case 0x0a7e: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 2690;
    }
    case 0x0a82: { // andi r16, 0x7F
        s->r[16] &= 127;
        pm_nzv(s, s->r[16], false);
        return 2692;
    }
    case 0x0a84: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 2696;
    }
    case 0x0a88: { // ldi r16, 0xC0
        s->r[16] = 192;
        return 2698;
    }
    case 0x0a8a: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 2702;
    }
    case 0x0a8e: { // ldi r16, 0x01
        s->r[16] = 1;
        return 2704;
    }
    case 0x0a90: { // sts 0x2441, r16
        uint16_t address = 9281;
        pm_write(s, address, s->r[16]);
        return 2708;
    }
    case 0x0a94: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 2710;
    }
    case 0x0a96: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 2712;
    }
    case 0x0a98: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 2714;
    }
    case 0x0a9a: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
