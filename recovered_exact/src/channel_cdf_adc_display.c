/* C value equivalent of the retained setting loads and calls:
 * value = (uint16_t)settings[0] | ((uint16_t)settings[1] << 8);
 * settings += 2;
 * print_setting(value); print_message(message);
 * The illustrative calls require the original private register ABI and
 * exact RCALL encoding. The loads keep the two original LD Y+ instructions.
 * This is an explanation, not an independently tested compiled alternative.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
#include "legacy_cpu.h"
#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))
#define PRINT_MESSAGE(cursor) asm volatile("rcall cli_send_msg" : "+z" (cursor) : : "memory", "cc")
/* Two volatile Y+ loads must remain between the original CLI/SEI pair. */
#define PRINT_SETTING(format, tail) do { \
 asm volatile("" : "+y" (settings), "+z" (message) : : "memory"); \
 pm_cpu_disable_irq(); \
 asm volatile("ld r16, Y+\n\tld r17, Y+" : "+y" (settings), "=r" (value) : : "memory"); \
 pm_cpu_enable_irq(); \
 asm volatile("rcall " format "\n\trcall " tail : "+r" (value), "+z" (message) : : "memory", "cc"); \
} while (0)

void cli_send_channel_cdf_adc(void)
{
    register uint16_t value asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000e78"
                 : "=r" (value) : : "memory", "cc");
    register uint8_t *settings asm("r28") = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
    register uint8_t channel asm("r20");
    asm volatile("clr %0" : "=r" (channel) : : "cc");
next_channel:;
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2a68;
    PRINT_MESSAGE(message);
    register uint8_t low asm("r16") = channel;
    asm volatile("clr r17\n\trcall cli_send_uint16\n\trcall cli_send_msg"
                 : "+r" (low), "+z" (message) : : "r17", "memory", "cc");
    PRINT_SETTING("FUN_code_001397", "cli_send_msg");
    PRINT_SETTING("FUN_code_001397", "cli_send_msg");
    PRINT_SETTING("FUN_code_001397", "cli_send_msg");
    PRINT_SETTING("FUN_code_00139f", "cli_send_crlf");
    asm volatile("inc %0" : "+r" (channel) : : "cc");
    if (channel != 12) goto next_channel;
    message = (const uint8_t *)0x2a92;
    PRINT_MESSAGE(message);
    low = RAM8(0x222f);
    asm volatile("clr r17\n\trcall cli_send_uint16\n\trcall cli_send_msg"
                 : "+r" (low), "+z" (message) : : "r17", "memory", "cc");
    low = RAM8(0x2230);
    asm volatile("" : "+r" (low));
    register uint8_t high asm("r17") = RAM8(0x2231);
    asm volatile("rcall cli_send_uint16\n\trcall cli_send_crlf"
                 : "+r" (low), "+r" (high) : : "memory", "cc");
}
asm(".pushsection .text.cli_send_channel_cdf_adc,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 848 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_channel_cdf_adc_display(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1cf2: { // rcall .+2888
        s->calls[s->call_depth++] = 7412;
        return 10300;
    }
    case 0x1cf4: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 7414;
    }
    case 0x1cf6: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 7408 : 7416;
    }
    case 0x1cf8: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 7418;
    }
    case 0x1cfa: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7420;
    }
    case 0x1cfc: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 7422;
    }
    case 0x1cfe: { // ldi r30, 0x68
        s->r[30] = 104;
        return 7424;
    }
    case 0x1d00: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 7426;
    }
    case 0x1d02: { // rcall .+2850
        s->calls[s->call_depth++] = 7428;
        return 10278;
    }
    case 0x1d04: { // mov r16, r20
        s->r[16] = s->r[20];
        return 7430;
    }
    case 0x1d06: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 7432;
    }
    case 0x1d08: { // rcall .+2628
        s->calls[s->call_depth++] = 7434;
        return 10062;
    }
    case 0x1d0a: { // rcall .+2842
        s->calls[s->call_depth++] = 7436;
        return 10278;
    }
    case 0x1d0c: { // cli
        pm_irq(s, false);
        return 7438;
    }
    case 0x1d0e: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 7440;
    }
    case 0x1d10: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 7442;
    }
    case 0x1d12: { // sei
        pm_irq(s, true);
        return 7444;
    }
    case 0x1d14: { // rcall .+2584
        s->calls[s->call_depth++] = 7446;
        return 10030;
    }
    case 0x1d16: { // rcall .+2830
        s->calls[s->call_depth++] = 7448;
        return 10278;
    }
    case 0x1d18: { // cli
        pm_irq(s, false);
        return 7450;
    }
    case 0x1d1a: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 7452;
    }
    case 0x1d1c: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 7454;
    }
    case 0x1d1e: { // sei
        pm_irq(s, true);
        return 7456;
    }
    case 0x1d20: { // rcall .+2572
        s->calls[s->call_depth++] = 7458;
        return 10030;
    }
    case 0x1d22: { // rcall .+2818
        s->calls[s->call_depth++] = 7460;
        return 10278;
    }
    case 0x1d24: { // cli
        pm_irq(s, false);
        return 7462;
    }
    case 0x1d26: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 7464;
    }
    case 0x1d28: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 7466;
    }
    case 0x1d2a: { // sei
        pm_irq(s, true);
        return 7468;
    }
    case 0x1d2c: { // rcall .+2560
        s->calls[s->call_depth++] = 7470;
        return 10030;
    }
    case 0x1d2e: { // rcall .+2806
        s->calls[s->call_depth++] = 7472;
        return 10278;
    }
    case 0x1d30: { // cli
        pm_irq(s, false);
        return 7474;
    }
    case 0x1d32: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 7476;
    }
    case 0x1d34: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 7478;
    }
    case 0x1d36: { // sei
        pm_irq(s, true);
        return 7480;
    }
    case 0x1d38: { // rcall .+2564
        s->calls[s->call_depth++] = 7482;
        return 10046;
    }
    case 0x1d3a: { // rcall .+2786
        s->calls[s->call_depth++] = 7484;
        return 10270;
    }
    case 0x1d3c: { // inc r20
        s->r[20]++;
        pm_nzv(s, s->r[20], s->r[20] == 128);
        return 7486;
    }
    case 0x1d3e: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 7488;
    }
    case 0x1d40: { // brne .-68
        return (pm_getflag(s, 1) == 0) ? 7422 : 7490;
    }
    case 0x1d42: { // ldi r30, 0x92
        s->r[30] = 146;
        return 7492;
    }
    case 0x1d44: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 7494;
    }
    case 0x1d46: { // rcall .+2782
        s->calls[s->call_depth++] = 7496;
        return 10278;
    }
    case 0x1d48: { // lds r16, 0x222F
        uint16_t address = 8751;
        s->r[16] = pm_read(s, address);
        return 7500;
    }
    case 0x1d4c: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 7502;
    }
    case 0x1d4e: { // rcall .+2558
        s->calls[s->call_depth++] = 7504;
        return 10062;
    }
    case 0x1d50: { // rcall .+2772
        s->calls[s->call_depth++] = 7506;
        return 10278;
    }
    case 0x1d52: { // lds r16, 0x2230
        uint16_t address = 8752;
        s->r[16] = pm_read(s, address);
        return 7510;
    }
    case 0x1d56: { // lds r17, 0x2231
        uint16_t address = 8753;
        s->r[17] = pm_read(s, address);
        return 7514;
    }
    case 0x1d5a: { // rcall .+2546
        s->calls[s->call_depth++] = 7516;
        return 10062;
    }
    case 0x1d5c: { // rcall .+2752
        s->calls[s->call_depth++] = 7518;
        return 10270;
    }
    case 0x1d5e: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1d60: { // rjmp .-2458
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
