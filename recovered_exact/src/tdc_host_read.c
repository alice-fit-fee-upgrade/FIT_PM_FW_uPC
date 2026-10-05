#include <avr/io.h>

void ths788_read(void)
{
    register uint8_t command asm("r16"), lo asm("r17"), hi asm("r18");
    register uint8_t device asm("r19"), data_mask asm("r20");
    register uint8_t count asm("r21"), clock asm("r22");
    asm volatile("" : "=r" (command), "=r" (lo), "=r" (hi), "=r" (device));
    if (device < 3) goto selected;
    goto finished;
selected:
    clock = 4;
shift_clock:
    asm volatile("dec %0" : "+r" (device) : : "cc");
    asm goto("brmi %l[clock_selected]" : : "r" (device) : : clock_selected);
    clock += clock; asm volatile("" : "+r" (clock));
    goto shift_clock;
clock_selected:
    PORTB_OUTCLR = clock;
    clock = 1;
    asm volatile("" : "+r" (clock));
    data_mask = 2;
    asm volatile("" : "+r" (data_mask));
    PORTB_OUTSET = data_mask;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    PORTB_OUTCLR = data_mask;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count = 8;
next_command_bit:
    if (!(command & 0x80u)) PORTB_OUTCLR = data_mask;
    asm volatile("" : "+r" (command) : "r" (data_mask), "r" (count) : "memory");
    if (command & 0x80u) PORTB_OUTSET = data_mask;
    /* C value equivalent: command = (command << 1) + carry;
     * ADC preserves the input carry and exact flag state. */
    asm volatile("adc %0, %0" : "+r" (command) : : "cc");
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    /* C counter value: --count; if (count != 0) goto loop_start;
     * DEC/BRNE also preserves C for the serial shift. Plain C decrements
     * need not preserve that flag; this is an explanatory value equivalent. */
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_command_bit]" : : "r" (count) : : next_command_bit);
    PORTB_DIRCLR = data_mask;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    count = 16;
next_response_bit:
    asm volatile("" : : "r" (count));
    PORTB_OUTSET = clock;
    lo += lo;
    asm volatile("" : "+r" (lo));
    /* C value equivalent of ADC after the low-byte shift:
     * uint8_t carry = old_lo >> 7;
     * hi = (uint8_t)((hi << 1) + carry);
     * Here carry is the live SREG.C from lo += lo. A trial that captured
     * it in R0 and expressed the high byte in C failed the fixed-region
     * size assertion (step_218_rejected.log). Keep ADC and its flags exact.
     * This fragment has no separate successful functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("adc %0, %0" : "+r" (hi) : : "cc");
    command = PORTB_IN;
    asm volatile("" : "+r" (command), "+r" (lo));
    if (command & 2u) { lo |= 1u; asm volatile("" : "+r" (lo)); }
    PORTB_OUTCLR = clock;
    /* C counter value: --count; if (count != 0) goto loop_start;
     * DEC/BRNE also preserves C for the serial shift. Plain C decrements
     * need not preserve that flag; this is an explanatory value equivalent. */
    asm volatile("dec %0" : "+r" (count) : : "cc");
    asm goto("brne %l[next_response_bit]" : : "r" (count) : : next_response_bit);
    count = 0x1c;
    asm volatile("" : "+r" (count));
    PORTB_OUTSET = count;
    PORTB_DIRSET = data_mask;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
    PORTB_OUTSET = clock;
    PORTB_OUTCLR = clock;
finished:
    asm volatile("" : : "r" (lo), "r" (hi));
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 928 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_tdc_host_read(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2208: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 8714;
    }
    case 0x220a: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 8716;
    }
    case 0x220c: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 8718;
    }
    case 0x220e: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 8720;
    }
    case 0x2210: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 8722;
    }
    case 0x2212: { // cpi r19, 0x03
        pm_sub(s, s->r[19], 3, 0, false);
        return 8724;
    }
    case 0x2214: { // brcs .+2
        return (pm_getflag(s, 0) == 1) ? 8728 : 8726;
    }
    case 0x2216: { // rjmp .+134
        return 8862;
    }
    case 0x2218: { // ldi r22, 0x04
        s->r[22] = 4;
        return 8730;
    }
    case 0x221a: { // dec r19
        s->r[19]--;
        pm_nzv(s, s->r[19], s->r[19] == 127);
        return 8732;
    }
    case 0x221c: { // brmi .+4
        return (pm_getflag(s, 2) == 1) ? 8738 : 8734;
    }
    case 0x221e: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8736;
    }
    case 0x2220: { // rjmp .-8
        return 8730;
    }
    case 0x2222: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8742;
    }
    case 0x2226: { // ldi r22, 0x01
        s->r[22] = 1;
        return 8744;
    }
    case 0x2228: { // ldi r20, 0x02
        s->r[20] = 2;
        return 8746;
    }
    case 0x222a: { // sts 0x0625, r20
        uint16_t address = 1573;
        pm_write(s, address, s->r[20]);
        return 8750;
    }
    case 0x222e: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8754;
    }
    case 0x2232: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8758;
    }
    case 0x2236: { // sts 0x0626, r20
        uint16_t address = 1574;
        pm_write(s, address, s->r[20]);
        return 8762;
    }
    case 0x223a: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8766;
    }
    case 0x223e: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8770;
    }
    case 0x2242: { // ldi r21, 0x08
        s->r[21] = 8;
        return 8772;
    }
    case 0x2244: { // sbrs r16, 7
        return (!!(s->r[16] & (1u << 7)) == 1) ? 8778 : 8774;
    }
    case 0x2246: { // sts 0x0626, r20
        uint16_t address = 1574;
        pm_write(s, address, s->r[20]);
        return 8778;
    }
    case 0x224a: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 8784 : 8780;
    }
    case 0x224c: { // sts 0x0625, r20
        uint16_t address = 1573;
        pm_write(s, address, s->r[20]);
        return 8784;
    }
    case 0x2250: { // adc r16, r16
        s->r[16] = pm_add(s, s->r[16], s->r[16], pm_getflag(s, CARRY));
        return 8786;
    }
    case 0x2252: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8790;
    }
    case 0x2256: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8794;
    }
    case 0x225a: { // dec r21
        s->r[21]--;
        pm_nzv(s, s->r[21], s->r[21] == 127);
        return 8796;
    }
    case 0x225c: { // brne .-26
        return (pm_getflag(s, 1) == 0) ? 8772 : 8798;
    }
    case 0x225e: { // sts 0x0622, r20
        uint16_t address = 1570;
        pm_write(s, address, s->r[20]);
        return 8802;
    }
    case 0x2262: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8806;
    }
    case 0x2266: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8810;
    }
    case 0x226a: { // ldi r21, 0x10
        s->r[21] = 16;
        return 8812;
    }
    case 0x226c: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8816;
    }
    case 0x2270: { // add r17, r17
        s->r[17] = pm_add(s, s->r[17], s->r[17], 0);
        return 8818;
    }
    case 0x2272: { // adc r18, r18
        s->r[18] = pm_add(s, s->r[18], s->r[18], pm_getflag(s, CARRY));
        return 8820;
    }
    case 0x2274: { // lds r16, 0x0628
        uint16_t address = 1576;
        s->r[16] = pm_read(s, address);
        return 8824;
    }
    case 0x2278: { // sbrc r16, 1
        return (!!(s->r[16] & (1u << 1)) == 0) ? 8828 : 8826;
    }
    case 0x227a: { // ori r17, 0x01
        s->r[17] |= 1;
        pm_nzv(s, s->r[17], false);
        return 8828;
    }
    case 0x227c: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8832;
    }
    case 0x2280: { // dec r21
        s->r[21]--;
        pm_nzv(s, s->r[21], s->r[21] == 127);
        return 8834;
    }
    case 0x2282: { // brne .-24
        return (pm_getflag(s, 1) == 0) ? 8812 : 8836;
    }
    case 0x2284: { // ldi r21, 0x1C
        s->r[21] = 28;
        return 8838;
    }
    case 0x2286: { // sts 0x0625, r21
        uint16_t address = 1573;
        pm_write(s, address, s->r[21]);
        return 8842;
    }
    case 0x228a: { // sts 0x0621, r20
        uint16_t address = 1569;
        pm_write(s, address, s->r[20]);
        return 8846;
    }
    case 0x228e: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8850;
    }
    case 0x2292: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8854;
    }
    case 0x2296: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8858;
    }
    case 0x229a: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8862;
    }
    case 0x229e: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 8864;
    }
    case 0x22a0: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 8866;
    }
    case 0x22a2: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 8868;
    }
    case 0x22a4: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 8870;
    }
    case 0x22a6: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 8872;
    }
    case 0x22a8: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
