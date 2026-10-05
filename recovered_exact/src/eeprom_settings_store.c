/* C store equivalent of the retained protected instruction: CCP = previous;
 * GCC may choose OUT instead of the original four-byte STS. Keep STS and
 * the original protected-write window; the comment is explanatory only. */
#include "legacy_cpu.h"
#include <avr/io.h>
#include <stdint.h>
register uint8_t *eeprom asm("r28");
#define SET_VALUE(constant) do { value=(constant); asm volatile("" : "+r" (value)); } while (0)

void eeprom_settings_save(void)
{
    register uint8_t value asm("r16"), previous asm("r17"), offset asm("r18"), dirty asm("r19"), end_high asm("r20");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000cd6" : "=r" (value) : : "memory", "cc");
wait_nvm:
    value = NVM_STATUS;
    asm volatile("" : "+r" (value));
    if (value & 0x80u) goto wait_nvm;
    SET_VALUE(0x36); NVM_CMD = value;
    SET_VALUE(1);
    previous = 0xd8; asm volatile("" : "+r" (previous));
    asm volatile("" : : "r" (previous), "r" (value) : "memory");
    pm_cpu_disable_irq();
    asm volatile("sts %0, %1" : : "n" (_SFR_MEM_ADDR(CCP)), "r" (previous) : "memory");
    NVM_CTRLA = value;
    pm_cpu_enable_irq();
    register const uint8_t *settings asm("r30") = (const uint8_t *)0x2163;
    asm volatile("" : "+z" (settings));
    eeprom = (uint8_t *)0x0fff;
    asm volatile("" : "+y" (eeprom));
    end_high = 0x22; asm volatile("" : "+r" (end_high));
wait_buffer:
    value = NVM_STATUS;
    asm volatile("" : "+r" (value));
    if (value & 0x80u) goto wait_buffer;
    GPIOR0 |= (1u << 3);
    /* C value: dirty = 0; retain flag-setting CLR rather than LDI/MOV zero. */
    asm volatile("clr %0" : "=r" (dirty) : : "cc");
next_byte:
    eeprom += 1;
    asm volatile("" : "+y" (eeprom) : : "memory");
    asm volatile("ld %0, Z+" : "=r" (value), "+z" (settings) : : "memory");
    previous = *eeprom;
    asm volatile("" : "+r" (previous) : : "memory");
    previous ^= value; asm volatile("" : "+r" (previous));
    /* Unvalidated C equivalent: if (!previous) goto unchanged; trial544
     * replaced EOR with CP at0x19ee. Preserve XOR result/flags exactly.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("breq %l[unchanged]" : : "r" (previous) : : unchanged);
    *eeprom = value;
    asm volatile("" : : : "memory");
    dirty = 1; asm volatile("" : "+r" (dirty));
unchanged:
    asm goto("cpi r30, 0x35\n\tcpc r31, %1\n\tbreq %l[finished]"
        : : "z" (settings), "r" (end_high) : "cc" : finished);
    offset = (uint8_t)(uintptr_t)eeprom;
    asm volatile("" : "+r" (offset));
    offset &= 0x1f; asm volatile("" : "+r" (offset));
    if (offset != 0x1f) goto next_byte;
    asm volatile("rcall FUN_code_000d07" : "+r" (dirty), "=r" (value), "=r" (previous)
        : "y" (eeprom) : "memory", "cc");
    goto next_byte;
finished:
    asm volatile("rcall FUN_code_000d07" : : "y" (eeprom), "r" (dirty) : "memory", "cc");
    GPIOR0 &= (uint8_t)~(1u << 3);
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 688 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_eeprom_settings_store(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x19ae: { // rcall .+3724
        s->calls[s->call_depth++] = 6576;
        return 10300;
    }
    case 0x19b0: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6578;
    }
    case 0x19b2: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 6572 : 6580;
    }
    case 0x19b4: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 6584;
    }
    case 0x19b8: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 6588 : 6586;
    }
    case 0x19ba: { // rjmp .-8
        return 6580;
    }
    case 0x19bc: { // ldi r16, 0x36
        s->r[16] = 54;
        return 6590;
    }
    case 0x19be: { // sts 0x01CA, r16
        uint16_t address = 458;
        pm_write(s, address, s->r[16]);
        return 6594;
    }
    case 0x19c2: { // ldi r16, 0x01
        s->r[16] = 1;
        return 6596;
    }
    case 0x19c4: { // ldi r17, 0xD8
        s->r[17] = 216;
        return 6598;
    }
    case 0x19c6: { // cli
        pm_irq(s, false);
        return 6600;
    }
    case 0x19c8: { // sts 0x0034, r17
        uint16_t address = 52;
        pm_write(s, address, s->r[17]);
        return 6604;
    }
    case 0x19cc: { // sts 0x01CB, r16
        uint16_t address = 459;
        pm_write(s, address, s->r[16]);
        return 6608;
    }
    case 0x19d0: { // sei
        pm_irq(s, true);
        return 6610;
    }
    case 0x19d2: { // ldi r30, 0x63
        s->r[30] = 99;
        return 6612;
    }
    case 0x19d4: { // ldi r31, 0x21
        s->r[31] = 33;
        return 6614;
    }
    case 0x19d6: { // ldi r28, 0xFF
        s->r[28] = 255;
        return 6616;
    }
    case 0x19d8: { // ldi r29, 0x0F
        s->r[29] = 15;
        return 6618;
    }
    case 0x19da: { // ldi r20, 0x22
        s->r[20] = 34;
        return 6620;
    }
    case 0x19dc: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 6624;
    }
    case 0x19e0: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 6628 : 6626;
    }
    case 0x19e2: { // rjmp .-8
        return 6620;
    }
    case 0x19e4: { // sbi 0x00, 3
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 3));
        return 6630;
    }
    case 0x19e6: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 6632;
    }
    case 0x19e8: { // adiw r28, 0x01
        uint16_t old = pm_pointer(s, 28);
        uint16_t value = old + 1;
        pm_setpointer(s, 28, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 6634;
    }
    case 0x19ea: { // ld r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_read(s, address);
        return 6636;
    }
    case 0x19ec: { // ld r17, Y
        uint16_t address = pm_pointer(s, 28) + 0;
        s->r[17] = pm_read(s, address);
        return 6638;
    }
    case 0x19ee: { // eor r17, r16
        s->r[17] ^= s->r[16];
        pm_nzv(s, s->r[17], false);
        return 6640;
    }
    case 0x19f0: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 6646 : 6642;
    }
    case 0x19f2: { // st Y, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[16]);
        return 6644;
    }
    case 0x19f4: { // ldi r19, 0x01
        s->r[19] = 1;
        return 6646;
    }
    case 0x19f6: { // cpi r30, 0x35
        pm_sub(s, s->r[30], 53, 0, false);
        return 6648;
    }
    case 0x19f8: { // cpc r31, r20
        pm_sub(s, s->r[31], s->r[20], pm_getflag(s, CARRY), true);
        return 6650;
    }
    case 0x19fa: { // breq .+12
        return (pm_getflag(s, 1) == 1) ? 6664 : 6652;
    }
    case 0x19fc: { // mov r18, r28
        s->r[18] = s->r[28];
        return 6654;
    }
    case 0x19fe: { // andi r18, 0x1F
        s->r[18] &= 31;
        pm_nzv(s, s->r[18], false);
        return 6656;
    }
    case 0x1a00: { // cpi r18, 0x1F
        pm_sub(s, s->r[18], 31, 0, false);
        return 6658;
    }
    case 0x1a02: { // brne .-28
        return (pm_getflag(s, 1) == 0) ? 6632 : 6660;
    }
    case 0x1a04: { // rcall .+8
        s->calls[s->call_depth++] = 6662;
        return 6670;
    }
    case 0x1a06: { // rjmp .-32
        return 6632;
    }
    case 0x1a08: { // rcall .+4
        s->calls[s->call_depth++] = 6666;
        return 6670;
    }
    case 0x1a0a: { // cbi 0x00, 3
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 3));
        return 6668;
    }
    case 0x1a0c: { // rjmp .+1492
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
