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

void cli_send_adc_baseline_dispersion(void)
{
    register uint16_t value asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000c87\n"
                 "rcall FUN_code_00108e" : "=r" (value) : : "memory", "cc");
    PM_RETURN_UNLESS_CARRY_CLEAR();
    register uint8_t channel_offset asm("r19");
    asm volatile("clr %0" : "=r" (channel_offset) : : "cc");
next_channel:;
    register uint8_t address asm("r18") = 0x0d;
    asm volatile("" : "+r" (address));
    address += channel_offset;
    asm volatile("" : "+r" (address));
    READ_PRINT(value, address);
    register uint8_t separator asm("r16") = ' ';
    asm volatile("rcall cli_send_buf\n\tinc %1" : "+r" (separator), "+r" (address) : : "memory", "cc");
    READ_PRINT(value, address);
    separator = ' ';
    asm volatile("rcall cli_send_buf" : "+r" (separator) : : "memory", "cc");
    address = 0x4c;
    asm volatile("" : "+r" (address));
    address += channel_offset;
    asm volatile("" : "+r" (address));
    READ_PRINT(value, address);
    separator = ' ';
    asm volatile("rcall cli_send_buf\n\tinc %1" : "+r" (separator), "+r" (address) : : "memory", "cc");
    READ_PRINT(value, address);
    asm volatile("rcall cli_send_crlf" : : : "memory", "cc");
    channel_offset += 2;
    asm volatile("" : "+r" (channel_offset));
    if (channel_offset < 24) goto next_channel;
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}
asm(".pushsection .text.cli_send_adc_baseline_dispersion,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

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
uint32_t pm_logical_channel_baseline_dispersion_display(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x18be: { // rcall .+3964
        s->calls[s->call_depth++] = 6336;
        return 10300;
    }
    case 0x18c0: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6338;
    }
    case 0x18c2: { // brne .+74
        return (pm_getflag(s, 1) == 0) ? 6414 : 6340;
    }
    case 0x18c4: { // rcall .+2134
        s->calls[s->call_depth++] = 6342;
        return 8476;
    }
    case 0x18c6: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 6346 : 6344;
    }
    case 0x18c8: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x18ca: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 6348;
    }
    case 0x18cc: { // ldi r18, 0x0D
        s->r[18] = 13;
        return 6350;
    }
    case 0x18ce: { // add r18, r19
        s->r[18] = pm_add(s, s->r[18], s->r[19], 0);
        return 6352;
    }
    case 0x18d0: { // cli
        pm_irq(s, false);
        return 6354;
    }
    case 0x18d2: { // rcall .+2708
        s->calls[s->call_depth++] = 6356;
        return 9064;
    }
    case 0x18d4: { // sei
        pm_irq(s, true);
        return 6358;
    }
    case 0x18d6: { // rcall .+3702
        s->calls[s->call_depth++] = 6360;
        return 10062;
    }
    case 0x18d8: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6362;
    }
    case 0x18da: { // rcall .+4048
        s->calls[s->call_depth++] = 6364;
        return 10412;
    }
    case 0x18dc: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 6366;
    }
    case 0x18de: { // cli
        pm_irq(s, false);
        return 6368;
    }
    case 0x18e0: { // rcall .+2694
        s->calls[s->call_depth++] = 6370;
        return 9064;
    }
    case 0x18e2: { // sei
        pm_irq(s, true);
        return 6372;
    }
    case 0x18e4: { // rcall .+3688
        s->calls[s->call_depth++] = 6374;
        return 10062;
    }
    case 0x18e6: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6376;
    }
    case 0x18e8: { // rcall .+4034
        s->calls[s->call_depth++] = 6378;
        return 10412;
    }
    case 0x18ea: { // ldi r18, 0x4C
        s->r[18] = 76;
        return 6380;
    }
    case 0x18ec: { // add r18, r19
        s->r[18] = pm_add(s, s->r[18], s->r[19], 0);
        return 6382;
    }
    case 0x18ee: { // cli
        pm_irq(s, false);
        return 6384;
    }
    case 0x18f0: { // rcall .+2678
        s->calls[s->call_depth++] = 6386;
        return 9064;
    }
    case 0x18f2: { // sei
        pm_irq(s, true);
        return 6388;
    }
    case 0x18f4: { // rcall .+3672
        s->calls[s->call_depth++] = 6390;
        return 10062;
    }
    case 0x18f6: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6392;
    }
    case 0x18f8: { // rcall .+4018
        s->calls[s->call_depth++] = 6394;
        return 10412;
    }
    case 0x18fa: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 6396;
    }
    case 0x18fc: { // cli
        pm_irq(s, false);
        return 6398;
    }
    case 0x18fe: { // rcall .+2664
        s->calls[s->call_depth++] = 6400;
        return 9064;
    }
    case 0x1900: { // sei
        pm_irq(s, true);
        return 6402;
    }
    case 0x1902: { // rcall .+3658
        s->calls[s->call_depth++] = 6404;
        return 10062;
    }
    case 0x1904: { // rcall .+3864
        s->calls[s->call_depth++] = 6406;
        return 10270;
    }
    case 0x1906: { // subi r19, 0xFE
        s->r[19] = pm_sub(s, s->r[19], 254, 0, false);
        return 6408;
    }
    case 0x1908: { // cpi r19, 0x18
        pm_sub(s, s->r[19], 24, 0, false);
        return 6410;
    }
    case 0x190a: { // brcs .-64
        return (pm_getflag(s, 0) == 1) ? 6348 : 6412;
    }
    case 0x190c: { // rjmp .+1748
        return 8162;
    }
    case 0x190e: { // rjmp .-1352
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
