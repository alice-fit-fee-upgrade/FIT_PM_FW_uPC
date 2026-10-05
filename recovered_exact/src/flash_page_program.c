#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))
/* Page programming uses the original 24-bit counter in R22:R20 and limit
 * in R2:R0. R1 is live state, so no GNU arithmetic helper is introduced. */
void FUN_code_000b59(void)
{
    asm volatile("rcall FUN_code_000b8f" : : : "memory", "cc");
    register uint8_t byte asm("r16") = 2;
    asm volatile("rcall FUN_code_000c04" : "+r" (byte) : : "memory", "cc");
    register uint16_t read_index asm("r24") = *(volatile uint16_t *)0x2437;
    asm volatile("" : "+r" (read_index));
next_byte:;
    register uint8_t *buffer asm("r26") = (uint8_t *)0x2235;
    asm volatile("" : "+x" (buffer));
    buffer += read_index;
    asm volatile("" : "+x" (buffer) : : "memory");
    byte = *buffer;
    asm volatile("" : "+r" (byte) : : "memory");
    read_index += 1; asm volatile("" : "+r" (read_index));
    read_index &= 0x01ff; asm volatile("" : "+r" (read_index));
    RAM8(0x0ac3) = byte;
    {
        register uint8_t spi_status asm("r19");
        do {
            spi_status = RAM8(0x0ac2);
            asm volatile("" : "+r" (spi_status));
        } while (!(spi_status & 0x80u));
    }
    asm goto("cp r0, r20\n\tcpc r1, r21\n\tcpc r2, r22\n\tbrne %l[continue_page]" : : : "cc" : continue_page);
    RAM8(0x2437) = (uint8_t)read_index;
    RAM8(0x2438) = read_index >> 8;
    byte = 0x10;
    asm volatile("" : "+r" (byte));
    RAM8(0x0685) = byte;
    asm volatile("sec" : : : "cc");
    return;
continue_page:
    {
        register __uint24 current_address asm("r20");
        asm volatile("" : "=r" (current_address) : : "memory");
        ++current_address;
        asm volatile("" : "+r" (current_address) : : "memory");
    }
    asm goto("tst r20\n\tbrne %l[next_byte]" : : : "cc" : next_byte);
    RAM8(0x2437) = (uint8_t)read_index;
    RAM8(0x2438) = read_index >> 8;
    byte = 0x10;
    asm volatile("" : "+r" (byte));
    RAM8(0x0685) = byte;
    asm volatile("clc");
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 592 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_page_program(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x16b2: { // rcall .+106
        s->calls[s->call_depth++] = 5812;
        return 5918;
    }
    case 0x16b4: { // ldi r16, 0x02
        s->r[16] = 2;
        return 5814;
    }
    case 0x16b6: { // rcall .+336
        s->calls[s->call_depth++] = 5816;
        return 6152;
    }
    case 0x16b8: { // lds r24, 0x2437
        uint16_t address = 9271;
        s->r[24] = pm_read(s, address);
        return 5820;
    }
    case 0x16bc: { // lds r25, 0x2438
        uint16_t address = 9272;
        s->r[25] = pm_read(s, address);
        return 5824;
    }
    case 0x16c0: { // ldi r26, 0x35
        s->r[26] = 53;
        return 5826;
    }
    case 0x16c2: { // ldi r27, 0x22
        s->r[27] = 34;
        return 5828;
    }
    case 0x16c4: { // add r26, r24
        s->r[26] = pm_add(s, s->r[26], s->r[24], 0);
        return 5830;
    }
    case 0x16c6: { // adc r27, r25
        s->r[27] = pm_add(s, s->r[27], s->r[25], pm_getflag(s, CARRY));
        return 5832;
    }
    case 0x16c8: { // ld r16, X
        uint16_t address = pm_pointer(s, 26) + 0;
        s->r[16] = pm_read(s, address);
        return 5834;
    }
    case 0x16ca: { // adiw r24, 0x01
        uint16_t old = pm_pointer(s, 24);
        uint16_t value = old + 1;
        pm_setpointer(s, 24, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 5836;
    }
    case 0x16cc: { // andi r25, 0x01
        s->r[25] &= 1;
        pm_nzv(s, s->r[25], false);
        return 5838;
    }
    case 0x16ce: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 5842;
    }
    case 0x16d2: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 5846;
    }
    case 0x16d6: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 5850 : 5848;
    }
    case 0x16d8: { // rjmp .-8
        return 5842;
    }
    case 0x16da: { // cp r0, r20
        pm_sub(s, s->r[0], s->r[20], 0, false);
        return 5852;
    }
    case 0x16dc: { // cpc r1, r21
        pm_sub(s, s->r[1], s->r[21], pm_getflag(s, CARRY), true);
        return 5854;
    }
    case 0x16de: { // cpc r2, r22
        pm_sub(s, s->r[2], s->r[22], pm_getflag(s, CARRY), true);
        return 5856;
    }
    case 0x16e0: { // brne .+18
        return (pm_getflag(s, 1) == 0) ? 5876 : 5858;
    }
    case 0x16e2: { // sts 0x2437, r24
        uint16_t address = 9271;
        pm_write(s, address, s->r[24]);
        return 5862;
    }
    case 0x16e6: { // sts 0x2438, r25
        uint16_t address = 9272;
        pm_write(s, address, s->r[25]);
        return 5866;
    }
    case 0x16ea: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5868;
    }
    case 0x16ec: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5872;
    }
    case 0x16f0: { // sec
        pm_flag(s, CARRY, true);
        return 5874;
    }
    case 0x16f2: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x16f4: { // subi r20, 0xFF
        s->r[20] = pm_sub(s, s->r[20], 255, 0, false);
        return 5878;
    }
    case 0x16f6: { // sbci r21, 0xFF
        s->r[21] = pm_sub(s, s->r[21], 255, pm_getflag(s, CARRY), true);
        return 5880;
    }
    case 0x16f8: { // sbci r22, 0xFF
        s->r[22] = pm_sub(s, s->r[22], 255, pm_getflag(s, CARRY), true);
        return 5882;
    }
    case 0x16fa: { // and r20, r20
        s->r[20] &= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 5884;
    }
    case 0x16fc: { // brne .-62
        return (pm_getflag(s, 1) == 0) ? 5824 : 5886;
    }
    case 0x16fe: { // sts 0x2437, r24
        uint16_t address = 9271;
        pm_write(s, address, s->r[24]);
        return 5890;
    }
    case 0x1702: { // sts 0x2438, r25
        uint16_t address = 9272;
        pm_write(s, address, s->r[25]);
        return 5894;
    }
    case 0x1706: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5896;
    }
    case 0x1708: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5900;
    }
    case 0x170c: { // clc
        pm_flag(s, CARRY, false);
        return 5902;
    }
    case 0x170e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
