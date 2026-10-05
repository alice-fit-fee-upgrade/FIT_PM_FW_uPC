/* Retained exact call/load helpers: a C call such as
 * value = fpga_read(address); or fpga_write(address, word); expresses the
 * operation, but these entries use private bound registers, original CALL/RCALL
 * widths and shared error tails. These illustrative names are not compiled
 * interfaces and are not independently functionally validated alternatives.
 * Where a historically tested complete C alternative exists, its evidence
 * and bridge scope remain documented beside that helper.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
#include "legacy_cpu.h"
#include <stdint.h>
#include "legacy_cli_guard_c.h"
#define RAM8(address) (*(volatile uint8_t *)(address))

void fpga_set_trg_charge_lvls(void)
{
    register uint16_t requested asm("r20");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 0x20\n\tbrne LAB_code_000eb0\n"
                 "rcall cli_get_integer\n\tbrcs LAB_code_000e78\n\ttst r21\n\tbrmi LAB_code_000e78\n"
                 "ldi r26, 0x10\n\tcpi r20, 0\n\tcpc r21, r26\n\tbrge LAB_code_000eb0"
        : "=r" (requested) : : "r16", "r26", "memory", "cc");
    PM_FPGA_GUARD(requested);
    RAM8(0x2230) = (uint8_t)requested;
    RAM8(0x2231) = requested >> 8;
    register uint8_t address asm("r18") = 0x3d;
    asm volatile("" : "+r" (address));
    register uint16_t word asm("r16") = requested;
    asm volatile("" : : "r" (word), "r" (address) : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall fpga_msg_send_t2" : : "r" (word), "r" (address) : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 352 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_trigger_charge_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1d62: { // rcall .+2776
        s->calls[s->call_depth++] = 7524;
        return 10300;
    }
    case 0x1d64: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7526;
    }
    case 0x1d66: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 7520 : 7528;
    }
    case 0x1d68: { // rcall .+2250
        s->calls[s->call_depth++] = 7530;
        return 9780;
    }
    case 0x1d6a: { // brcs .-124
        return (pm_getflag(s, 0) == 1) ? 7408 : 7532;
    }
    case 0x1d6c: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7534;
    }
    case 0x1d6e: { // brmi .-128
        return (pm_getflag(s, 2) == 1) ? 7408 : 7536;
    }
    case 0x1d70: { // ldi r26, 0x10
        s->r[26] = 16;
        return 7538;
    }
    case 0x1d72: { // cpi r20, 0x00
        pm_sub(s, s->r[20], 0, 0, false);
        return 7540;
    }
    case 0x1d74: { // cpc r21, r26
        pm_sub(s, s->r[21], s->r[26], pm_getflag(s, CARRY), true);
        return 7542;
    }
    case 0x1d76: { // brge .-24
        return (pm_getflag(s, 4) == 0) ? 7520 : 7544;
    }
    case 0x1d78: { // rcall .+930
        s->calls[s->call_depth++] = 7546;
        return 8476;
    }
    case 0x1d7a: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7550 : 7548;
    }
    case 0x1d7c: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1d7e: { // sts 0x2230, r20
        uint16_t address = 8752;
        pm_write(s, address, s->r[20]);
        return 7554;
    }
    case 0x1d82: { // sts 0x2231, r21
        uint16_t address = 8753;
        pm_write(s, address, s->r[21]);
        return 7558;
    }
    case 0x1d86: { // ldi r18, 0x3D
        s->r[18] = 61;
        return 7560;
    }
    case 0x1d88: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 7562;
    }
    case 0x1d8a: { // cli
        pm_irq(s, false);
        return 7564;
    }
    case 0x1d8c: { // rcall .+1408
        s->calls[s->call_depth++] = 7566;
        return 8974;
    }
    case 0x1d8e: { // sei
        pm_irq(s, true);
        return 7568;
    }
    case 0x1d90: { // rjmp .+592
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
