/* Calls retain private fixed-register inputs/results through zero-byte
 * barriers. Void declarations deliberately introduce no GNU arguments/results;
 * recapture after each call observes the original registers. Every conversion
 * is accepted only if GNU CALL, frames and the entire FLASH remain identical. */
extern void cli_send_buf(void);
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
/* Private console/FPGA ABI: payload in R17:R16, register address in R18.
 * Carry, interrupt windows and the original wide CALL remain exact helpers. */
#define READ_PRINT(value, address) do { \
 asm volatile("" : "+r" (address) : : "memory"); \
 pm_cpu_disable_irq(); \
 asm volatile("rcall fpga_msg_read_t1" : "=r" (value), "+r" (address) : : "memory", "cc"); \
 pm_cpu_enable_irq(); \
 asm volatile("rcall cli_send_uint16" : "+r" (value) : : "memory", "cc"); \
} while (0)

void cli_send_ch_mean_amplitude(void)
{
    register uint16_t value asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000c87\n"
                 "rcall FUN_code_00108e"
                 : "=r" (value) : : "memory", "cc");
    PM_RETURN_UNLESS_CARRY_CLEAR();
    register uint8_t address asm("r18") = 0x64;
    asm volatile("" : "+r" (address));
next_channel:
    READ_PRINT(value, address);
    register uint8_t separator asm("r16") = ' ';
    do {
        asm volatile("" : "+r" (separator) :  : "memory");
        cli_send_buf();
        asm volatile("" : "=r" (separator) : : "memory");
    } while (0);
    asm volatile("inc %0" : "+r" (address) : : "cc");
    READ_PRINT(value, address);
    asm volatile("rcall cli_send_crlf\n\tinc %0" : "+r" (address) : : "memory", "cc");
    if (address < 0x7c) goto next_channel;
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 368 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_channel_mean_amplitude_display(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x188e: { // rcall .+4012
        s->calls[s->call_depth++] = 6288;
        return 10300;
    }
    case 0x1890: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6290;
    }
    case 0x1892: { // brne .+122
        return (pm_getflag(s, 1) == 0) ? 6414 : 6292;
    }
    case 0x1894: { // rcall .+2182
        s->calls[s->call_depth++] = 6294;
        return 8476;
    }
    case 0x1896: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 6298 : 6296;
    }
    case 0x1898: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x189a: { // ldi r18, 0x64
        s->r[18] = 100;
        return 6300;
    }
    case 0x189c: { // cli
        pm_irq(s, false);
        return 6302;
    }
    case 0x189e: { // rcall .+2760
        s->calls[s->call_depth++] = 6304;
        return 9064;
    }
    case 0x18a0: { // sei
        pm_irq(s, true);
        return 6306;
    }
    case 0x18a2: { // rcall .+3754
        s->calls[s->call_depth++] = 6308;
        return 10062;
    }
    case 0x18a4: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6310;
    }
    case 0x18a6: { // call 0x28ac
        s->calls[s->call_depth++] = 6314;
        return 10412;
    }
    case 0x18aa: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 6316;
    }
    case 0x18ac: { // cli
        pm_irq(s, false);
        return 6318;
    }
    case 0x18ae: { // rcall .+2744
        s->calls[s->call_depth++] = 6320;
        return 9064;
    }
    case 0x18b0: { // sei
        pm_irq(s, true);
        return 6322;
    }
    case 0x18b2: { // rcall .+3738
        s->calls[s->call_depth++] = 6324;
        return 10062;
    }
    case 0x18b4: { // rcall .+3944
        s->calls[s->call_depth++] = 6326;
        return 10270;
    }
    case 0x18b6: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 6328;
    }
    case 0x18b8: { // cpi r18, 0x7C
        pm_sub(s, s->r[18], 124, 0, false);
        return 6330;
    }
    case 0x18ba: { // brcs .-32
        return (pm_getflag(s, 0) == 1) ? 6300 : 6332;
    }
    case 0x18bc: { // rjmp .+1828
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
