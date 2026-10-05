#include "legacy_cpu.h"
#include "legacy_spi_c.h"

void dac_send_value(void)
{
    register uint8_t header asm("r22"), scratch asm("r23"), select asm("r24");
    asm volatile("" : "=r" (header));
    pm_cpu_disable_irq();
    scratch = 0xd5;
    asm volatile("" : "+r" (scratch));
    SPIC_CTRL = scratch;
    /* Earlier copy/constant attempts403/431 failed; steps495/496/539
     * subsequently matched using an opaque R22/R23 register boundary.
     * Only the second BREQ remains ASM. Rejected C equivalent:
     * if (!scratch) goto selected;
     * Trials540/543 emitted an extra test or replaced SUBI with CPI
     * (0x22c2, step543_instruction_diff.txt). No standalone functional
     * validation is claimed for those rejected fragments.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    scratch = header;
    asm volatile("" : "+r" (scratch), "+r" (header));
    select = 1; asm volatile("" : "+r" (select));
    scratch &= 0x30;
    if (!scratch) goto selected;
    select += select; asm volatile("" : "+r" (select));
    scratch -= 0x10; asm volatile("" : "+r" (scratch));
    asm goto("breq %l[selected]" : : "r" (scratch) : : selected);
    select += select; asm volatile("" : "+r" (select));
selected:
    scratch = header;
    asm volatile("" : "+r" (scratch));
    header = __builtin_avr_swap(header); asm volatile("" : "+r" (header));
    header &= 0xc0;
    asm volatile("" : "+r" (header));
    scratch += scratch;
    asm volatile("" : "+r" (scratch));
    scratch &= 6;
    asm volatile("" : "+r" (scratch));
    header |= scratch;
    asm volatile("" : "+r" (header));
    header |= 0x10;
    asm volatile("" : "+r" (header));
    PORTC_OUTCLR = select;
    /* Polling overwrites the first outgoing byte register, as in the original. */
    SPIC_DATA = header;
    PM_SPI_WAIT_AT(SPIC_STATUS, "r22");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r17");
    PM_SPI_SEND_REGISTER(SPIC, "r22", "r16");
    header = 7;
    asm volatile("" : "+r" (header));
    PORTC_OUTSET = header;
    pm_cpu_enable_irq();
}

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
uint32_t pm_logical_dac_spi_send(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x22aa: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 8876;
    }
    case 0x22ac: { // push r23
        s->stack[s->depth++] = s->r[23];
        return 8878;
    }
    case 0x22ae: { // push r24
        s->stack[s->depth++] = s->r[24];
        return 8880;
    }
    case 0x22b0: { // cli
        pm_irq(s, false);
        return 8882;
    }
    case 0x22b2: { // ldi r23, 0xD5
        s->r[23] = 213;
        return 8884;
    }
    case 0x22b4: { // sts 0x08C0, r23
        uint16_t address = 2240;
        pm_write(s, address, s->r[23]);
        return 8888;
    }
    case 0x22b8: { // mov r23, r22
        s->r[23] = s->r[22];
        return 8890;
    }
    case 0x22ba: { // ldi r24, 0x01
        s->r[24] = 1;
        return 8892;
    }
    case 0x22bc: { // andi r23, 0x30
        s->r[23] &= 48;
        pm_nzv(s, s->r[23], false);
        return 8894;
    }
    case 0x22be: { // breq .+8
        return (pm_getflag(s, 1) == 1) ? 8904 : 8896;
    }
    case 0x22c0: { // add r24, r24
        s->r[24] = pm_add(s, s->r[24], s->r[24], 0);
        return 8898;
    }
    case 0x22c2: { // subi r23, 0x10
        s->r[23] = pm_sub(s, s->r[23], 16, 0, false);
        return 8900;
    }
    case 0x22c4: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 8904 : 8902;
    }
    case 0x22c6: { // add r24, r24
        s->r[24] = pm_add(s, s->r[24], s->r[24], 0);
        return 8904;
    }
    case 0x22c8: { // mov r23, r22
        s->r[23] = s->r[22];
        return 8906;
    }
    case 0x22ca: { // swap r22
        s->r[22] = (s->r[22] >> 4) | (s->r[22] << 4);
        return 8908;
    }
    case 0x22cc: { // andi r22, 0xC0
        s->r[22] &= 192;
        pm_nzv(s, s->r[22], false);
        return 8910;
    }
    case 0x22ce: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8912;
    }
    case 0x22d0: { // andi r23, 0x06
        s->r[23] &= 6;
        pm_nzv(s, s->r[23], false);
        return 8914;
    }
    case 0x22d2: { // or r22, r23
        s->r[22] |= s->r[23];
        pm_nzv(s, s->r[22], false);
        return 8916;
    }
    case 0x22d4: { // ori r22, 0x10
        s->r[22] |= 16;
        pm_nzv(s, s->r[22], false);
        return 8918;
    }
    case 0x22d6: { // sts 0x0646, r24
        uint16_t address = 1606;
        pm_write(s, address, s->r[24]);
        return 8922;
    }
    case 0x22da: { // sts 0x08C3, r22
        uint16_t address = 2243;
        pm_write(s, address, s->r[22]);
        return 8926;
    }
    case 0x22de: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 8930;
    }
    case 0x22e2: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 8934 : 8932;
    }
    case 0x22e4: { // rjmp .-8
        return 8926;
    }
    case 0x22e6: { // sts 0x08C3, r17
        uint16_t address = 2243;
        pm_write(s, address, s->r[17]);
        return 8938;
    }
    case 0x22ea: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 8942;
    }
    case 0x22ee: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 8946 : 8944;
    }
    case 0x22f0: { // rjmp .-8
        return 8938;
    }
    case 0x22f2: { // sts 0x08C3, r16
        uint16_t address = 2243;
        pm_write(s, address, s->r[16]);
        return 8950;
    }
    case 0x22f6: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 8954;
    }
    case 0x22fa: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 8958 : 8956;
    }
    case 0x22fc: { // rjmp .-8
        return 8950;
    }
    case 0x22fe: { // ldi r22, 0x07
        s->r[22] = 7;
        return 8960;
    }
    case 0x2300: { // sts 0x0645, r22
        uint16_t address = 1605;
        pm_write(s, address, s->r[22]);
        return 8964;
    }
    case 0x2304: { // sei
        pm_irq(s, true);
        return 8966;
    }
    case 0x2306: { // pop r24
        s->r[24] = s->stack[--s->depth];
        return 8968;
    }
    case 0x2308: { // pop r23
        s->r[23] = s->stack[--s->depth];
        return 8970;
    }
    case 0x230a: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 8972;
    }
    case 0x230c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
