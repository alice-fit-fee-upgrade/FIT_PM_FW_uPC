#include "legacy_cpu.h"
#include <avr/io.h>

/* Eight clock edges, with original bit tests and sampling timing preserved. */
void adt7311_byte_rw(void)
{
    register uint8_t count asm("r18") = 8;
    register uint8_t data asm("r16");
    asm volatile("" : "=r" (data));
next_bit:
    {
        register uint8_t data_mask asm("r17") = 8;
        /* Unvalidated C equivalent: if (data & 0x80) PORTA_OUTSET = data_mask;
         * if (!(data & 0x80)) PORTA_OUTCLR = data_mask;
         * Step445 changed binary/layout; original paired skips remain.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
        asm volatile("sbrc %0, 7\n\tsts %2, %1\n\tsbrs %0, 7\n\tsts %3, %1"
            : : "r" (data), "r" (data_mask), "n" (_SFR_MEM_ADDR(PORTA_OUTSET)),
                "n" (_SFR_MEM_ADDR(PORTA_OUTCLR)) : "memory");
    }
    PORTA_OUTCLR = 2;
    data += data;
    asm volatile("" : "+r" (data));
    pm_cpu_nop();
    PORTA_OUTSET = 2;
    register uint8_t sample asm("r17") = PORTA_IN;
    data = PM_COPY_BIT(data, 0, sample, 2);
    asm volatile("" : "+r" (data));
    /* DEC, unlike SUBI, preserves the original carry flag. */
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_bit]" : : "r" (count) : : next_bit);
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 272 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_adt_bit_transfer(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2608: { // ldi r18, 0x08
        s->r[18] = 8;
        return 9738;
    }
    case 0x260a: { // ldi r17, 0x08
        s->r[17] = 8;
        return 9740;
    }
    case 0x260c: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 9746 : 9742;
    }
    case 0x260e: { // sts 0x0605, r17
        uint16_t address = 1541;
        pm_write(s, address, s->r[17]);
        return 9746;
    }
    case 0x2612: { // sbrs r16, 7
        return (!!(s->r[16] & (1u << 7)) == 1) ? 9752 : 9748;
    }
    case 0x2614: { // sts 0x0606, r17
        uint16_t address = 1542;
        pm_write(s, address, s->r[17]);
        return 9752;
    }
    case 0x2618: { // ldi r17, 0x02
        s->r[17] = 2;
        return 9754;
    }
    case 0x261a: { // sts 0x0606, r17
        uint16_t address = 1542;
        pm_write(s, address, s->r[17]);
        return 9758;
    }
    case 0x261e: { // add r16, r16
        s->r[16] = pm_add(s, s->r[16], s->r[16], 0);
        return 9760;
    }
    case 0x2620: { // nop
        return 9762;
    }
    case 0x2622: { // sts 0x0605, r17
        uint16_t address = 1541;
        pm_write(s, address, s->r[17]);
        return 9766;
    }
    case 0x2626: { // lds r17, 0x0608
        uint16_t address = 1544;
        s->r[17] = pm_read(s, address);
        return 9770;
    }
    case 0x262a: { // bst r17, 2
        pm_flag(s, TRANSFER, s->r[17] & (1u << 2));
        return 9772;
    }
    case 0x262c: { // bld r16, 0
        s->r[16] = (s->r[16] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 9774;
    }
    case 0x262e: { // dec r18
        s->r[18]--;
        pm_nzv(s, s->r[18], s->r[18] == 127);
        return 9776;
    }
    case 0x2630: { // brne .-40
        return (pm_getflag(s, 1) == 0) ? 9738 : 9778;
    }
    case 0x2632: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
