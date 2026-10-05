#include "legacy_cpu.h"
#include <avr/io.h>

void pm_eeprom_page_commit(void) asm("FUN_code_000d07");
void pm_eeprom_page_commit(void)
{
    register uint8_t dirty asm("r19"), low asm("r28"), high asm("r29");
    asm volatile("" : "=r" (dirty), "=r" (low), "=r" (high));
    if (dirty == 0) goto finished;
    register uint8_t command asm("r16") = 0x35;
    asm volatile("" : "+r" (command));
    NVM_CMD = command;
    NVM_ADDR0 = low;
    NVM_ADDR1 = high;
    command = 1;
    asm volatile("" : "+r" (command));
    register uint8_t unlock asm("r17") = 0xd8;
    asm volatile("" : "+r" (unlock));
    pm_cpu_disable_irq();
    asm volatile("sts %0, %1" : : "n" (_SFR_MEM_ADDR(CCP)), "r" (unlock) : "memory");
    NVM_CTRLA = command;
    pm_cpu_enable_irq();
wait_ready:
    command = NVM_STATUS;
    asm volatile("" : "+r" (command));
    if (command & 0x80u) goto wait_ready;
    asm volatile("clr %0" : "=r" (dirty) : : "cc");
finished:
    asm volatile("" : : "r" (dirty));
}
/* Adjacent error jump is another entry, not padding or part of the C return. */
asm(".pushsection .text.FUN_code_000d07,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 288 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_eeprom_page_commit(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1a0e: { // and r19, r19
        s->r[19] &= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 6672;
    }
    case 0x1a10: { // breq .+40
        return (pm_getflag(s, 1) == 1) ? 6714 : 6674;
    }
    case 0x1a12: { // ldi r16, 0x35
        s->r[16] = 53;
        return 6676;
    }
    case 0x1a14: { // sts 0x01CA, r16
        uint16_t address = 458;
        pm_write(s, address, s->r[16]);
        return 6680;
    }
    case 0x1a18: { // sts 0x01C0, r28
        uint16_t address = 448;
        pm_write(s, address, s->r[28]);
        return 6684;
    }
    case 0x1a1c: { // sts 0x01C1, r29
        uint16_t address = 449;
        pm_write(s, address, s->r[29]);
        return 6688;
    }
    case 0x1a20: { // ldi r16, 0x01
        s->r[16] = 1;
        return 6690;
    }
    case 0x1a22: { // ldi r17, 0xD8
        s->r[17] = 216;
        return 6692;
    }
    case 0x1a24: { // cli
        pm_irq(s, false);
        return 6694;
    }
    case 0x1a26: { // sts 0x0034, r17
        uint16_t address = 52;
        pm_write(s, address, s->r[17]);
        return 6698;
    }
    case 0x1a2a: { // sts 0x01CB, r16
        uint16_t address = 459;
        pm_write(s, address, s->r[16]);
        return 6702;
    }
    case 0x1a2e: { // sei
        pm_irq(s, true);
        return 6704;
    }
    case 0x1a30: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 6708;
    }
    case 0x1a34: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 6712 : 6710;
    }
    case 0x1a36: { // rjmp .-8
        return 6704;
    }
    case 0x1a38: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 6714;
    }
    case 0x1a3a: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1a3c: { // rjmp .-1654
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
