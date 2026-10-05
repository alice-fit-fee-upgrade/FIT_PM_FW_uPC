#include "legacy_spi_c.h"

/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_reads_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/device_reads.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * void pm_fpga_stamp_c(void)
 * {
 *     SPIC_CTRL=0xd1;
 *     PORTD_OUTCLR=1;
 *     fpga_byte(0x3d); fpga_byte(0x40);
 *     uint16_t address=0x2b92;
 *     for (uint8_t words=0;words!=2;++words) {
 *         uint8_t low=pgm_read_byte(address++);
 *         uint8_t high=pgm_read_byte(address++);
 *         fpga_byte(high); fpga_byte(low);
 *     }
 *     PORTD_OUTSET=1;
 * }
 */

void fpga_send_mcu_ts(void)
{
    register const uint8_t *cursor asm("r30") = (const uint8_t *)0x2b92;
    /* Keep the original pointer setup before configuring SPI. */
    asm volatile("" : "+z" (cursor));
    PM_WRITE_R22(SPIC_CTRL, 0xd1);
    register uint8_t address asm("r18") = 0x3d;
    register uint8_t offset asm("r21") = 0x40;
    asm volatile("" : "+r" (address), "+r" (offset));
    PM_WRITE_R22(PORTD_OUTCLR, 1);
    PM_SPI_SEND_VALUE(SPIC, "r22", address);
    PM_SPI_SEND_VALUE(SPIC, "r22", offset);
next_word:
    /* C FLASH read equivalent with const __flash uint8_t *cursor:
     * address = *cursor++; offset = *cursor++;
     * Trials 239-249 could not reproduce the private pointer/register layout;
     * keep exact LPM Z+ instructions. This is explanatory, with no new
     * independent functional-test claim. Existing historical test scope,
     * when available, is documented above. */
    asm volatile("lpm %0, Z+\n\tlpm %1, Z+"
                 : "=r" (address), "=r" (offset), "+z" (cursor) : : "memory");
    PM_SPI_SEND_VALUE(SPIC, "r22", offset);
    PM_SPI_SEND_VALUE(SPIC, "r22", address);
    /* Original loop tests only ZL, sending precisely two words. */
    { register uint8_t address_low asm("r30");
      asm volatile("" : "=r" (address_low) : "z" (cursor));
      if (address_low != 0x96) goto next_word; }
    PM_WRITE_R22(PORTD_OUTSET, 1);
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 656 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_fpga_timestamp_send(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2530: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9522;
    }
    case 0x2532: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 9524;
    }
    case 0x2534: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 9526;
    }
    case 0x2536: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 9528;
    }
    case 0x2538: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 9530;
    }
    case 0x253a: { // ldi r30, 0x92
        s->r[30] = 146;
        return 9532;
    }
    case 0x253c: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 9534;
    }
    case 0x253e: { // ldi r22, 0xD1
        s->r[22] = 209;
        return 9536;
    }
    case 0x2540: { // sts 0x08C0, r22
        uint16_t address = 2240;
        pm_write(s, address, s->r[22]);
        return 9540;
    }
    case 0x2544: { // ldi r18, 0x3D
        s->r[18] = 61;
        return 9542;
    }
    case 0x2546: { // ldi r21, 0x40
        s->r[21] = 64;
        return 9544;
    }
    case 0x2548: { // ldi r22, 0x01
        s->r[22] = 1;
        return 9546;
    }
    case 0x254a: { // sts 0x0666, r22
        uint16_t address = 1638;
        pm_write(s, address, s->r[22]);
        return 9550;
    }
    case 0x254e: { // sts 0x08C3, r18
        uint16_t address = 2243;
        pm_write(s, address, s->r[18]);
        return 9554;
    }
    case 0x2552: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9558;
    }
    case 0x2556: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9562 : 9560;
    }
    case 0x2558: { // rjmp .-8
        return 9554;
    }
    case 0x255a: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9566;
    }
    case 0x255e: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9570;
    }
    case 0x2562: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9574 : 9572;
    }
    case 0x2564: { // rjmp .-8
        return 9566;
    }
    case 0x2566: { // lpm r18, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[18] = pm_golden_flash[address];
        return 9576;
    }
    case 0x2568: { // lpm r21, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[21] = pm_golden_flash[address];
        return 9578;
    }
    case 0x256a: { // sts 0x08C3, r21
        uint16_t address = 2243;
        pm_write(s, address, s->r[21]);
        return 9582;
    }
    case 0x256e: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9586;
    }
    case 0x2572: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9590 : 9588;
    }
    case 0x2574: { // rjmp .-8
        return 9582;
    }
    case 0x2576: { // sts 0x08C3, r18
        uint16_t address = 2243;
        pm_write(s, address, s->r[18]);
        return 9594;
    }
    case 0x257a: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9598;
    }
    case 0x257e: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9602 : 9600;
    }
    case 0x2580: { // rjmp .-8
        return 9594;
    }
    case 0x2582: { // cpi r30, 0x96
        pm_sub(s, s->r[30], 150, 0, false);
        return 9604;
    }
    case 0x2584: { // brne .-32
        return (pm_getflag(s, 1) == 0) ? 9574 : 9606;
    }
    case 0x2586: { // ldi r22, 0x01
        s->r[22] = 1;
        return 9608;
    }
    case 0x2588: { // sts 0x0665, r22
        uint16_t address = 1637;
        pm_write(s, address, s->r[22]);
        return 9612;
    }
    case 0x258c: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 9614;
    }
    case 0x258e: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 9616;
    }
    case 0x2590: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 9618;
    }
    case 0x2592: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 9620;
    }
    case 0x2594: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9622;
    }
    case 0x2596: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
