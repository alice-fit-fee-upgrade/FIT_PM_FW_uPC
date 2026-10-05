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

void fpga_set_trg_settings(void)
{
    register uint16_t requested asm("r20");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 0x20\n\tbrne LAB_code_000eb0\n"
                 "rcall cli_get_integer\n\tbrcs LAB_code_000eb0\n\ttst r21\n\tbrne LAB_code_000eb0"
        : "=r" (requested) : : "r16", "memory", "cc");
    PM_FPGA_GUARD(requested);
    RAM8(0x222f) = (uint8_t)requested;
    register uint8_t address asm("r18") = 0;
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
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 272 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_trigger_settings_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1d92: { // rcall .+2728
        s->calls[s->call_depth++] = 7572;
        return 10300;
    }
    case 0x1d94: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7574;
    }
    case 0x1d96: { // brne .-56
        return (pm_getflag(s, 1) == 0) ? 7520 : 7576;
    }
    case 0x1d98: { // rcall .+2202
        s->calls[s->call_depth++] = 7578;
        return 9780;
    }
    case 0x1d9a: { // brcs .-60
        return (pm_getflag(s, 0) == 1) ? 7520 : 7580;
    }
    case 0x1d9c: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7582;
    }
    case 0x1d9e: { // brne .-64
        return (pm_getflag(s, 1) == 0) ? 7520 : 7584;
    }
    case 0x1da0: { // rcall .+890
        s->calls[s->call_depth++] = 7586;
        return 8476;
    }
    case 0x1da2: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7590 : 7588;
    }
    case 0x1da4: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1da6: { // sts 0x222F, r20
        uint16_t address = 8751;
        pm_write(s, address, s->r[20]);
        return 7594;
    }
    case 0x1daa: { // ldi r18, 0x00
        s->r[18] = 0;
        return 7596;
    }
    case 0x1dac: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 7598;
    }
    case 0x1dae: { // cli
        pm_irq(s, false);
        return 7600;
    }
    case 0x1db0: { // rcall .+1372
        s->calls[s->call_depth++] = 7602;
        return 8974;
    }
    case 0x1db2: { // sei
        pm_irq(s, true);
        return 7604;
    }
    case 0x1db4: { // rjmp .+556
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
