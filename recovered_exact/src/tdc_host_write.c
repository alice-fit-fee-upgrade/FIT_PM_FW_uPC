#include <avr/io.h>

/* Legacy R19 selects a TDC/broadcast and the additional command bit.
 * The payload uses R16/R18/R17 in the original carry-dependent order. */
void ths788_write(void)
{
    register uint8_t b0 asm("r16"), b1 asm("r17"), b2 asm("r18");
    register uint8_t device asm("r19"), data_mask asm("r20");
    register uint8_t count asm("r21"), clock asm("r22");
    asm volatile("" : "=r" (b0), "=r" (b1), "=r" (b2), "=r" (device));
    /* Exact selection loop: DEC/BRMI retains its original carry behavior. */
    count = device;
    asm volatile("" : "+r" (count), "+r" (device));
    device &= 3;
    asm volatile("" : "+r" (device));
    /* C value selection: clock = device == 3 ? 0x1c : (4u << device);
     * The exact loop additionally leaves device at its decremented value and
     * preserves live flags; this is an explanatory value description. */
    asm goto("cpi %0, 3\n\tbrlo %l[single_strobe]" : : "r" (device), "r" (count) : "cc" : single_strobe);
    clock = 0x1c; asm volatile("" : "+r" (clock));
    goto clock_selected;
single_strobe:
    clock = 4; asm volatile("" : "+r" (clock));
shift_clock:
    asm volatile("dec %0" : "+r" (device) : : "cc");
    asm goto("brmi %l[clock_selected]" : : "r" (device) : : clock_selected);
    clock += clock; asm volatile("" : "+r" (clock));
    goto shift_clock;
clock_selected:
    clock |= 2;
    asm volatile("" : "+r" (clock));
    PORTB_OUTCLR = clock;
    clock = 1;
    asm volatile("" : "+r" (clock));
    data_mask = 2;
    asm volatile("" : "+r" (data_mask));
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count &= 4;
    asm volatile("" : "+r" (count));
    /* C branch value equivalent: if (count == 0) goto command_clock;
     * BREQ consumes flags from the exact ANDI emitted for the C mask. */
    /* Input-only boundary retry542 also changed layout. */
    /* Step446 `if (!count) goto command_clock;` changed the binary/layout.
     * Unvalidated standalone; consume original ANDI flags directly.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("breq %l[command_clock]" : : "r" (count) : : command_clock);
    PORTB_OUTSET = data_mask;
command_clock:
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count = 24;
next_bit:
    if (!(b0 & 0x80u)) PORTB_OUTCLR = data_mask;
    asm volatile("" : "+r" (b0) : "r" (data_mask), "r" (count) : "memory");
    if (b0 & 0x80u) PORTB_OUTSET = data_mask;
    /* C values: shift the b1/b2/b0 serial packet with input carry.
     * Exact ADC instructions also propagate SREG.C between the bytes. */
    asm volatile("adc %1, %1\n\tadc %2, %2\n\tadc %0, %0"
        : "+r" (b0), "+r" (b1), "+r" (b2) : : "cc");
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_bit]" : : "r" (count) : : next_bit);
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count = 0x1c;
    asm volatile("" : "+r" (count));
    PORTB_OUTSET = count;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 912 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_tdc_host_write(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2174: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 8566;
    }
    case 0x2176: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 8568;
    }
    case 0x2178: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8570;
    }
    case 0x217a: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 8572;
    }
    case 0x217c: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 8574;
    }
    case 0x217e: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 8576;
    }
    case 0x2180: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 8578;
    }
    case 0x2182: { // mov r21, r19
        s->r[21] = s->r[19];
        return 8580;
    }
    case 0x2184: { // andi r19, 0x03
        s->r[19] &= 3;
        pm_nzv(s, s->r[19], false);
        return 8582;
    }
    case 0x2186: { // cpi r19, 0x03
        pm_sub(s, s->r[19], 3, 0, false);
        return 8584;
    }
    case 0x2188: { // brcs .+4
        return (pm_getflag(s, 0) == 1) ? 8590 : 8586;
    }
    case 0x218a: { // ldi r22, 0x1C
        s->r[22] = 28;
        return 8588;
    }
    case 0x218c: { // rjmp .+10
        return 8600;
    }
    case 0x218e: { // ldi r22, 0x04
        s->r[22] = 4;
        return 8592;
    }
    case 0x2190: { // dec r19
        s->r[19]--;
        pm_nzv(s, s->r[19], s->r[19] == 127);
        return 8594;
    }
    case 0x2192: { // brmi .+4
        return (pm_getflag(s, 2) == 1) ? 8600 : 8596;
    }
    case 0x2194: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8598;
    }
    case 0x2196: { // rjmp .-8
        return 8592;
    }
    case 0x2198: { // ori r22, 0x02
        s->r[22] |= 2;
        pm_nzv(s, s->r[22], false);
        return 8602;
    }
    case 0x219a: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8606;
    }
    case 0x219e: { // ldi r22, 0x01
        s->r[22] = 1;
        return 8608;
    }
    case 0x21a0: { // ldi r20, 0x02
        s->r[20] = 2;
        return 8610;
    }
    case 0x21a2: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8614;
    }
    case 0x21a6: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8618;
    }
    case 0x21aa: { // andi r21, 0x04
        s->r[21] &= 4;
        pm_nzv(s, s->r[21], false);
        return 8620;
    }
    case 0x21ac: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 8626 : 8622;
    }
    case 0x21ae: { // sts 0x0625, r20
        uint16_t address = 1573;
        pm_write(s, address, s->r[20]);
        return 8626;
    }
    case 0x21b2: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8630;
    }
    case 0x21b6: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8634;
    }
    case 0x21ba: { // ldi r21, 0x18
        s->r[21] = 24;
        return 8636;
    }
    case 0x21bc: { // sbrs r16, 7
        return (!!(s->r[16] & (1u << 7)) == 1) ? 8642 : 8638;
    }
    case 0x21be: { // sts 0x0626, r20
        uint16_t address = 1574;
        pm_write(s, address, s->r[20]);
        return 8642;
    }
    case 0x21c2: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 8648 : 8644;
    }
    case 0x21c4: { // sts 0x0625, r20
        uint16_t address = 1573;
        pm_write(s, address, s->r[20]);
        return 8648;
    }
    case 0x21c8: { // adc r17, r17
        s->r[17] = pm_add(s, s->r[17], s->r[17], pm_getflag(s, CARRY));
        return 8650;
    }
    case 0x21ca: { // adc r18, r18
        s->r[18] = pm_add(s, s->r[18], s->r[18], pm_getflag(s, CARRY));
        return 8652;
    }
    case 0x21cc: { // adc r16, r16
        s->r[16] = pm_add(s, s->r[16], s->r[16], pm_getflag(s, CARRY));
        return 8654;
    }
    case 0x21ce: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8658;
    }
    case 0x21d2: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8662;
    }
    case 0x21d6: { // dec r21
        s->r[21]--;
        pm_nzv(s, s->r[21], s->r[21] == 127);
        return 8664;
    }
    case 0x21d8: { // brne .-30
        return (pm_getflag(s, 1) == 0) ? 8636 : 8666;
    }
    case 0x21da: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8670;
    }
    case 0x21de: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8674;
    }
    case 0x21e2: { // ldi r21, 0x1C
        s->r[21] = 28;
        return 8676;
    }
    case 0x21e4: { // sts 0x0625, r21
        uint16_t address = 1573;
        pm_write(s, address, s->r[21]);
        return 8680;
    }
    case 0x21e8: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8684;
    }
    case 0x21ec: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8688;
    }
    case 0x21f0: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8692;
    }
    case 0x21f4: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8696;
    }
    case 0x21f8: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 8698;
    }
    case 0x21fa: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 8700;
    }
    case 0x21fc: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 8702;
    }
    case 0x21fe: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 8704;
    }
    case 0x2200: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 8706;
    }
    case 0x2202: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 8708;
    }
    case 0x2204: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 8710;
    }
    case 0x2206: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
