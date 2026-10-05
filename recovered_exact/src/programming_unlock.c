/* Original private console entries: fixed-register barriers, no GNU result ABI. */
extern void cli_get_next_char(void);
extern void cli_get_next_byte(void);
extern void cli_send_msg(void);
#include <stdint.h>

/* Compare the eight original FLASH password bytes; retain carry-result ABI. */
void unlock_programming(void)
{
    register uint8_t received asm("r16");
    cli_get_next_char();
    asm volatile("" : "=r" (received) : : "memory");
    /* Native C comparison; the password_error tail retains the Carry ABI. */
    if (received != 13) goto password_error;
    register const uint8_t *password asm("r30") = (const uint8_t *)0x2b76;
    asm volatile("" : "+z" (password));
next_byte:;
    register uint8_t expected asm("r17");
    /* C value equivalent: expected = flash_byte(password++);
     * Exact LPM Z+ remains; earlier __flash pointer trials needed other code. */
    asm volatile("lpm %0, Z+" : "=r" (expected), "+z" (password) : : "memory");
    cli_get_next_byte();
    asm volatile("" : "=r" (received));
    asm volatile("cp %0, %1\n\tbrne LAB_code_000ad0" : : "r" (received), "r" (expected) : "cc");
    { register uint8_t address_low asm("r30");
      asm volatile("" : "=r" (address_low) : "z" (password));
      if (address_low != 0x7e) goto next_byte; }
    password = (const uint8_t *)0x2998;
    asm volatile("" : "+z" (password));
    cli_send_msg();
    /* C status equivalent: return_success_with_carry_clear();
     * The private carry-result entry cannot use a normal C return value. */
    asm volatile("clc" : : : "memory", "cc");
    return;
password_error:
    asm volatile("sec" : : : "memory", "cc");
    return;
}
asm(".pushsection .text.unlock_programming,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 304 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_programming_unlock(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x157a: { // call 0x283c
        s->calls[s->call_depth++] = 5502;
        return 10300;
    }
    case 0x157e: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 5504;
    }
    case 0x1580: { // brne .+30
        return (pm_getflag(s, 1) == 0) ? 5536 : 5506;
    }
    case 0x1582: { // ldi r30, 0x76
        s->r[30] = 118;
        return 5508;
    }
    case 0x1584: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 5510;
    }
    case 0x1586: { // lpm r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_golden_flash[address];
        return 5512;
    }
    case 0x1588: { // call 0x2836
        s->calls[s->call_depth++] = 5516;
        return 10294;
    }
    case 0x158c: { // cp r16, r17
        pm_sub(s, s->r[16], s->r[17], 0, false);
        return 5518;
    }
    case 0x158e: { // brne .+16
        return (pm_getflag(s, 1) == 0) ? 5536 : 5520;
    }
    case 0x1590: { // cpi r30, 0x7E
        pm_sub(s, s->r[30], 126, 0, false);
        return 5522;
    }
    case 0x1592: { // brne .-14
        return (pm_getflag(s, 1) == 0) ? 5510 : 5524;
    }
    case 0x1594: { // ldi r30, 0x98
        s->r[30] = 152;
        return 5526;
    }
    case 0x1596: { // ldi r31, 0x29
        s->r[31] = 41;
        return 5528;
    }
    case 0x1598: { // call 0x2826
        s->calls[s->call_depth++] = 5532;
        return 10278;
    }
    case 0x159c: { // clc
        pm_flag(s, CARRY, false);
        return 5534;
    }
    case 0x159e: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x15a0: { // sec
        pm_flag(s, CARRY, true);
        return 5538;
    }
    case 0x15a2: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x15a4: { // rjmp .-478
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
