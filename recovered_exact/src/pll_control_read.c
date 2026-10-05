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
 * uint32_t pm_pll_read_c(void)
 * {
 *     pm_pll_write_unmasked(0x8e);
 *     PORTF_OUTCLR=0x10;
 *     uint8_t b0=pll_byte();
 *     uint8_t b1=pll_byte();
 *     uint8_t b2=pll_byte();
 *     uint8_t b3=pll_byte();
 *     PORTF_OUTSET=0x10;
 *     return (uint32_t)b0 | ((uint32_t)b1<<8) | ((uint32_t)b2<<16) | ((uint32_t)b3<<24);
 * }
 */

void pm_pll_control_read(void) asm("FUN_code_001267");
/* Select the original readback command, then return four bytes in R16..R19. */
void pm_pll_control_read(void)
{
    register uint8_t command asm("r16") = 0x8e;
    asm volatile("clr r17\n\tclr r18\n\tclr r19\n\trcall CDCE62005_send_control_settings"
                 : "+r" (command) : : "r17", "r18", "r19", "memory", "cc");
    PM_WRITE_R22(PORTF_OUTCLR, 0x10);
    register uint8_t dummy asm("r23");
    asm volatile("clr %0" : "=r" (dummy) : : "cc");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r23");
    PM_SPI_READ_REGISTER(SPIC, "r16");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r23");
    PM_SPI_READ_REGISTER(SPIC, "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r23");
    PM_SPI_READ_REGISTER(SPIC, "r18");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r23");
    PM_SPI_READ_REGISTER(SPIC, "r19");
    PM_WRITE_R22(PORTF_OUTSET, 0x10);
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 560 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_pll_control_read(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x24ce: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 9424;
    }
    case 0x24d0: { // push r23
        s->stack[s->depth++] = s->r[23];
        return 9426;
    }
    case 0x24d2: { // ldi r16, 0x8E
        s->r[16] = 142;
        return 9428;
    }
    case 0x24d4: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 9430;
    }
    case 0x24d6: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 9432;
    }
    case 0x24d8: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 9434;
    }
    case 0x24da: { // rcall .-86
        s->calls[s->call_depth++] = 9436;
        return 9350;
    }
    case 0x24dc: { // ldi r22, 0x10
        s->r[22] = 16;
        return 9438;
    }
    case 0x24de: { // sts 0x06A6, r22
        uint16_t address = 1702;
        pm_write(s, address, s->r[22]);
        return 9442;
    }
    case 0x24e2: { // eor r23, r23
        s->r[23] ^= s->r[23];
        pm_nzv(s, s->r[23], false);
        return 9444;
    }
    case 0x24e4: { // sts 0x08C3, r23
        uint16_t address = 2243;
        pm_write(s, address, s->r[23]);
        return 9448;
    }
    case 0x24e8: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9452;
    }
    case 0x24ec: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9456 : 9454;
    }
    case 0x24ee: { // rjmp .-8
        return 9448;
    }
    case 0x24f0: { // lds r16, 0x08C3
        uint16_t address = 2243;
        s->r[16] = pm_read(s, address);
        return 9460;
    }
    case 0x24f4: { // sts 0x08C3, r23
        uint16_t address = 2243;
        pm_write(s, address, s->r[23]);
        return 9464;
    }
    case 0x24f8: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9468;
    }
    case 0x24fc: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9472 : 9470;
    }
    case 0x24fe: { // rjmp .-8
        return 9464;
    }
    case 0x2500: { // lds r17, 0x08C3
        uint16_t address = 2243;
        s->r[17] = pm_read(s, address);
        return 9476;
    }
    case 0x2504: { // sts 0x08C3, r23
        uint16_t address = 2243;
        pm_write(s, address, s->r[23]);
        return 9480;
    }
    case 0x2508: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9484;
    }
    case 0x250c: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9488 : 9486;
    }
    case 0x250e: { // rjmp .-8
        return 9480;
    }
    case 0x2510: { // lds r18, 0x08C3
        uint16_t address = 2243;
        s->r[18] = pm_read(s, address);
        return 9492;
    }
    case 0x2514: { // sts 0x08C3, r23
        uint16_t address = 2243;
        pm_write(s, address, s->r[23]);
        return 9496;
    }
    case 0x2518: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 9500;
    }
    case 0x251c: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 9504 : 9502;
    }
    case 0x251e: { // rjmp .-8
        return 9496;
    }
    case 0x2520: { // lds r19, 0x08C3
        uint16_t address = 2243;
        s->r[19] = pm_read(s, address);
        return 9508;
    }
    case 0x2524: { // ldi r22, 0x10
        s->r[22] = 16;
        return 9510;
    }
    case 0x2526: { // sts 0x06A5, r22
        uint16_t address = 1701;
        pm_write(s, address, s->r[22]);
        return 9514;
    }
    case 0x252a: { // pop r23
        s->r[23] = s->stack[--s->depth];
        return 9516;
    }
    case 0x252c: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 9518;
    }
    case 0x252e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
