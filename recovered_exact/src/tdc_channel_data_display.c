#include "legacy_cpu.h"
#include <stdint.h>
#include "legacy_cli_guard_c.h"
#define READ_WORD(value, address) do { \
    asm volatile("" : "+r" (address) : : "memory"); \
    pm_cpu_disable_irq(); \
    asm volatile("rcall fpga_msg_read_t1" : "=r" (value), "+r" (address) : : "memory", "cc"); \
    pm_cpu_enable_irq(); \
} while (0)
/* Rejected step356 C fragment: sign_high = 0;
 * if (low & 0x80u) sign_high = (uint8_t)~sign_high;
 * The separate C conditional/complement changed full FLASH bytes.
 * Step383 subsequently reproduced the original SBRC/COM in C by making
 * low a read/write R16 operand; this passed whole-image exact-check.
 * No standalone behavioral-test claim is made.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
#define PRINT_SIGNED_BYTE(low) do { \
    register uint8_t sign_high asm("r17"); \
    /* C zero value retained as flag-setting CLR; low must be ready first. */ \
    asm volatile("clr %0" : "=r" (sign_high), "+r" (low) : : "cc", "memory"); \
    if ((low) & 0x80u) { sign_high = (uint8_t)~sign_high; \
    asm volatile("" : "+r" (sign_high)); } \
    asm volatile("rcall cli_send_int16" : "+r" (low), "+r" (sign_high) : : "memory", "cc"); \
    } while (0)
#define SEND_SPACE(low) do { (low) = ' '; asm volatile("rcall cli_send_buf" : "+r" (low) : : "memory", "cc"); } while (0)

/* Preserve the original signed byte display and unusual TDC coarse/fine
 * rounding; no arithmetic correction is made to the recovered baseline. */
void cli_send_tdc_data(void)
{
    register uint16_t word asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000c87\n"
                 "rcall FUN_code_00108e" : "=r" (word) : : "memory", "cc");
    PM_RETURN_UNLESS_CARRY_CLEAR();
    register uint8_t address asm("r18") = 0x3e;
    asm volatile("" : "+r" (address));
    READ_WORD(word, address);
    register uint8_t high asm("r17");
    asm volatile("" : "=r" (high));
    register uint8_t saved_high asm("r20") = high;
    asm volatile("" : "+r" (saved_high));
    register uint8_t low asm("r16");
    asm volatile("" : "=r" (low));
    PRINT_SIGNED_BYTE(low);
    SEND_SPACE(low);
    low = saved_high;
    PRINT_SIGNED_BYTE(low);
    SEND_SPACE(low);
    address = 0x3f; asm volatile("" : "+r" (address));
    READ_WORD(word, address);
    asm volatile("" : "=r" (low));
    PRINT_SIGNED_BYTE(low);
    asm volatile("rcall cli_send_crlf" : : : "memory", "cc");
    register uint8_t *calibration asm("r28") = (uint8_t *)0x217b;
    asm volatile("" : "+y" (calibration));
    register uint8_t channel asm("r23");
    asm volatile("clr %0" : "=r" (channel) : : "cc");
next_channel:
    address = 0x40; asm volatile("" : "+r" (address));
    address += channel; asm volatile("" : "+r" (address));
    READ_WORD(word, address);
    /* Exact threshold/odd-bit corrections; C shifts emit the original ASRs.
     * The final carry is consumed by the unchanged injection helper. */
    asm volatile("rcall cli_send_32bit_hex\n\tcpi r16, 0x60\n\tbrlt 1f\n"
                 "sbrs r17, 0\n\tsubi r17, 2\n\trjmp 2f\n1:\n"
                 "cpi r16, 0x10\n\tbrcc 2f\n\tsbrc r17, 0\n\tsubi r17, 0xfe\n2:\n"
                 : "+r" (word) : : "memory", "cc");
    { register uint8_t rounded_high asm("r17");
      asm volatile("" : "=r" (rounded_high));
      rounded_high = (uint8_t)((int8_t)rounded_high >> 1);
      asm volatile("" : "+r" (rounded_high));
      rounded_high = (uint8_t)((int8_t)rounded_high >> 1);
      asm volatile("" : "+r" (rounded_high));
      asm volatile("brcc 3f\n\tori r16, 0x80\n3:\n\tpush r16"
                   : "=r" (word) : "r" (rounded_high) : "memory", "cc"); }
    SEND_SPACE(low);
    asm volatile("pop r16\n\trcall cli_send_int16" : "=r" (low) : : "memory", "cc");
    SEND_SPACE(low);
    asm volatile("clr r24\n\tld r16, Y+"
                 : "=r" (low), "+y" (calibration) : : "r24", "memory", "cc");
    low += low; asm volatile("" : "+r" (low));
    low = (uint8_t)((int8_t)low >> 1); asm volatile("" : "+r" (low));
    PRINT_SIGNED_BYTE(low);
    SEND_SPACE(low);
    /* C equivalent: cli_send_crlf(); ++channel; trial439 changed layout.
     * Unvalidated standalone; private call ABI and INC flags remain exact.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("rcall cli_send_crlf\n\tinc %0" : "+r" (channel) : : "memory", "cc");
    /* Step449 C equivalent: if ((int8_t)channel < 12) goto next_channel;
     * Failed exact matching; unvalidated standalone, retain signed flags.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("cpi %0, 12\n\tbrlt %l[next_channel]" : : "r" (channel) : "cc" : next_channel);
    asm volatile("rcall cli_send_crlf\n\trjmp LAB_code_000ff1" : : : "memory", "cc");
    __builtin_unreachable();
}
asm(".pushsection .text.cli_send_tdc_data,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 1264 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_tdc_channel_data_display(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1910: { // rcall .+3882
        s->calls[s->call_depth++] = 6418;
        return 10300;
    }
    case 0x1912: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6420;
    }
    case 0x1914: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 6414 : 6422;
    }
    case 0x1916: { // rcall .+2052
        s->calls[s->call_depth++] = 6424;
        return 8476;
    }
    case 0x1918: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 6428 : 6426;
    }
    case 0x191a: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x191c: { // ldi r18, 0x3E
        s->r[18] = 62;
        return 6430;
    }
    case 0x191e: { // cli
        pm_irq(s, false);
        return 6432;
    }
    case 0x1920: { // rcall .+2630
        s->calls[s->call_depth++] = 6434;
        return 9064;
    }
    case 0x1922: { // sei
        pm_irq(s, true);
        return 6436;
    }
    case 0x1924: { // mov r20, r17
        s->r[20] = s->r[17];
        return 6438;
    }
    case 0x1926: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 6440;
    }
    case 0x1928: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 6444 : 6442;
    }
    case 0x192a: { // com r17
        s->r[17] = ~s->r[17];
        pm_nzv(s, s->r[17], false);
        pm_flag(s, CARRY, true);
        return 6444;
    }
    case 0x192c: { // rcall .+3608
        s->calls[s->call_depth++] = 6446;
        return 10054;
    }
    case 0x192e: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6448;
    }
    case 0x1930: { // rcall .+3962
        s->calls[s->call_depth++] = 6450;
        return 10412;
    }
    case 0x1932: { // mov r16, r20
        s->r[16] = s->r[20];
        return 6452;
    }
    case 0x1934: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 6454;
    }
    case 0x1936: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 6458 : 6456;
    }
    case 0x1938: { // com r17
        s->r[17] = ~s->r[17];
        pm_nzv(s, s->r[17], false);
        pm_flag(s, CARRY, true);
        return 6458;
    }
    case 0x193a: { // rcall .+3594
        s->calls[s->call_depth++] = 6460;
        return 10054;
    }
    case 0x193c: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6462;
    }
    case 0x193e: { // rcall .+3948
        s->calls[s->call_depth++] = 6464;
        return 10412;
    }
    case 0x1940: { // ldi r18, 0x3F
        s->r[18] = 63;
        return 6466;
    }
    case 0x1942: { // cli
        pm_irq(s, false);
        return 6468;
    }
    case 0x1944: { // rcall .+2594
        s->calls[s->call_depth++] = 6470;
        return 9064;
    }
    case 0x1946: { // sei
        pm_irq(s, true);
        return 6472;
    }
    case 0x1948: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 6474;
    }
    case 0x194a: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 6478 : 6476;
    }
    case 0x194c: { // com r17
        s->r[17] = ~s->r[17];
        pm_nzv(s, s->r[17], false);
        pm_flag(s, CARRY, true);
        return 6478;
    }
    case 0x194e: { // rcall .+3574
        s->calls[s->call_depth++] = 6480;
        return 10054;
    }
    case 0x1950: { // rcall .+3788
        s->calls[s->call_depth++] = 6482;
        return 10270;
    }
    case 0x1952: { // ldi r28, 0x7B
        s->r[28] = 123;
        return 6484;
    }
    case 0x1954: { // ldi r29, 0x21
        s->r[29] = 33;
        return 6486;
    }
    case 0x1956: { // eor r23, r23
        s->r[23] ^= s->r[23];
        pm_nzv(s, s->r[23], false);
        return 6488;
    }
    case 0x1958: { // ldi r18, 0x40
        s->r[18] = 64;
        return 6490;
    }
    case 0x195a: { // add r18, r23
        s->r[18] = pm_add(s, s->r[18], s->r[23], 0);
        return 6492;
    }
    case 0x195c: { // cli
        pm_irq(s, false);
        return 6494;
    }
    case 0x195e: { // rcall .+2568
        s->calls[s->call_depth++] = 6496;
        return 9064;
    }
    case 0x1960: { // sei
        pm_irq(s, true);
        return 6498;
    }
    case 0x1962: { // rcall .+3476
        s->calls[s->call_depth++] = 6500;
        return 9976;
    }
    case 0x1964: { // cpi r16, 0x60
        pm_sub(s, s->r[16], 96, 0, false);
        return 6502;
    }
    case 0x1966: { // brlt .+6
        return (pm_getflag(s, 4) == 1) ? 6510 : 6504;
    }
    case 0x1968: { // sbrs r17, 0
        return (!!(s->r[17] & (1u << 0)) == 1) ? 6508 : 6506;
    }
    case 0x196a: { // subi r17, 0x02
        s->r[17] = pm_sub(s, s->r[17], 2, 0, false);
        return 6508;
    }
    case 0x196c: { // rjmp .+8
        return 6518;
    }
    case 0x196e: { // cpi r16, 0x10
        pm_sub(s, s->r[16], 16, 0, false);
        return 6512;
    }
    case 0x1970: { // brcc .+4
        return (pm_getflag(s, 0) == 0) ? 6518 : 6514;
    }
    case 0x1972: { // sbrc r17, 0
        return (!!(s->r[17] & (1u << 0)) == 0) ? 6518 : 6516;
    }
    case 0x1974: { // subi r17, 0xFE
        s->r[17] = pm_sub(s, s->r[17], 254, 0, false);
        return 6518;
    }
    case 0x1976: { // asr r17
        bool carry = s->r[17] & 1;
        s->r[17] = (s->r[17] >> 1) | (s->r[17] & 128);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[17], !!(s->r[17] & 128) ^ carry);
        return 6520;
    }
    case 0x1978: { // asr r17
        bool carry = s->r[17] & 1;
        s->r[17] = (s->r[17] >> 1) | (s->r[17] & 128);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[17], !!(s->r[17] & 128) ^ carry);
        return 6522;
    }
    case 0x197a: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 6526 : 6524;
    }
    case 0x197c: { // ori r16, 0x80
        s->r[16] |= 128;
        pm_nzv(s, s->r[16], false);
        return 6526;
    }
    case 0x197e: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 6528;
    }
    case 0x1980: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6530;
    }
    case 0x1982: { // rcall .+3880
        s->calls[s->call_depth++] = 6532;
        return 10412;
    }
    case 0x1984: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 6534;
    }
    case 0x1986: { // rcall .+3518
        s->calls[s->call_depth++] = 6536;
        return 10054;
    }
    case 0x1988: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6538;
    }
    case 0x198a: { // rcall .+3872
        s->calls[s->call_depth++] = 6540;
        return 10412;
    }
    case 0x198c: { // eor r24, r24
        s->r[24] ^= s->r[24];
        pm_nzv(s, s->r[24], false);
        return 6542;
    }
    case 0x198e: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 6544;
    }
    case 0x1990: { // add r16, r16
        s->r[16] = pm_add(s, s->r[16], s->r[16], 0);
        return 6546;
    }
    case 0x1992: { // asr r16
        bool carry = s->r[16] & 1;
        s->r[16] = (s->r[16] >> 1) | (s->r[16] & 128);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[16], !!(s->r[16] & 128) ^ carry);
        return 6548;
    }
    case 0x1994: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 6550;
    }
    case 0x1996: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 6554 : 6552;
    }
    case 0x1998: { // com r17
        s->r[17] = ~s->r[17];
        pm_nzv(s, s->r[17], false);
        pm_flag(s, CARRY, true);
        return 6554;
    }
    case 0x199a: { // rcall .+3498
        s->calls[s->call_depth++] = 6556;
        return 10054;
    }
    case 0x199c: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6558;
    }
    case 0x199e: { // rcall .+3852
        s->calls[s->call_depth++] = 6560;
        return 10412;
    }
    case 0x19a0: { // rcall .+3708
        s->calls[s->call_depth++] = 6562;
        return 10270;
    }
    case 0x19a2: { // inc r23
        s->r[23]++;
        pm_nzv(s, s->r[23], s->r[23] == 128);
        return 6564;
    }
    case 0x19a4: { // cpi r23, 0x0C
        pm_sub(s, s->r[23], 12, 0, false);
        return 6566;
    }
    case 0x19a6: { // brlt .-80
        return (pm_getflag(s, 4) == 1) ? 6488 : 6568;
    }
    case 0x19a8: { // rcall .+3700
        s->calls[s->call_depth++] = 6570;
        return 10270;
    }
    case 0x19aa: { // rjmp .+1590
        return 8162;
    }
    case 0x19ac: { // rjmp .-1510
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
