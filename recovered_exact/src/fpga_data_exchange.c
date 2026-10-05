/* Private entry calls below intentionally have no GNU argument/result ABI.
 * Adjacent fixed-register setup/capture preserves each historical contract;
 * these declarations emit the original wide CALL, not a new C API. The whole
 * image check verifies CALL width, register moves and every saved frame. */
extern void FUN_code_0011e5(void);
extern void fpga_msg_read_t1(void);
extern void fpga_msg_send_t2(void);
#include "legacy_cpu.h"
#include <avr/io.h>
#include <stdint.h>
register uint16_t pm_exchange_cursor asm("r28");

/* Original byte range 0x115c..0x12e9. C expresses the register values,
 * MMIO reads, masks and channel derivation. Exact helpers preserve live carry,
 * signed/unsigned comparisons, IRQ gates and private call contracts.
 * This conversion is accepted only by complete-FLASH exact-check; no new
 * independent functional tests of rejected C alternatives are claimed. */
void fpga_data_exchange(void)
{
    register uint8_t r16 asm("r16");
    register uint8_t r17 asm("r17");
    register uint8_t r18 asm("r18");
    register uint8_t r19 asm("r19");
    register uint8_t r20 asm("r20");
    register uint8_t r22 asm("r22");
    register uint8_t r23 asm("r23");
    register uint8_t r24 asm("r24");
    register uint8_t r28 asm("r28");
    register uint8_t r29 asm("r29");
    register uint8_t r30 asm("r30");
    register uint8_t r31 asm("r31");
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    pm_cpu_disable_irq();
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x2006 = r16;
    r16 = PORTB_OUT;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    if ((r16 & (1u << 7))) goto L_0008b8;
L_0008b6:
    pm_cpu_enable_irq();
    return;
L_0008b8:
    FUN_code_0011e5();
    pm_cpu_enable_irq();
    r18 = 0x80;
    asm volatile("" : "+r" (r18));
    r28 = 0xcf;
    asm volatile("" : "+r" (r28));
    r29 = 0x21;
    asm volatile("" : "+r" (r29));
L_0008be:
    {
        register uint32_t upper_bitmap asm("r12");
        asm volatile("" : "=r" (upper_bitmap) : : "memory");
        upper_bitmap >>= 1;
        asm volatile("" : "+r" (upper_bitmap) : : "memory");
    }
    /* C full value: bitmap >>= 1; lower RORs inject upper-word carry.
     * GNU uint64_t shifting calls __lshrdi3 and moves the private R8:R15
     * packet through GNU argument registers. Keep the lower exact carry chain;
     * no independent functional-test claim for this explanatory alternative.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("ror         r11" : : : "memory", "cc");
    asm volatile("ror         r10" : : : "memory", "cc");
    asm volatile("ror         r9" : : : "memory", "cc");
    asm volatile("ror         r8" : : : "memory", "cc");
    asm goto("brbs 0, %l[L_0008c9]" : : : "memory", "cc" : L_0008c9);
    asm volatile("" : "=r" (pm_exchange_cursor) : : "memory");
    pm_exchange_cursor += 2;
    asm volatile("" : "+r" (pm_exchange_cursor) : : "memory");
    goto L_00096b;
L_0008c9:
    pm_cpu_disable_irq();
    r20 = PORTE_INTCTRL;
    asm volatile("" : "+r" (r20));
    asm volatile("" : "=r" (r20));
    if (!(r20 & (1u << 3))) goto L_0008b6;
    fpga_msg_read_t1();
    pm_cpu_enable_irq();
    asm volatile("cpi         r18,0xb0" : : : "memory", "cc");
    asm goto("brbs 0, %l[L_0008d4]" : : : "memory", "cc" : L_0008d4);
    goto L_00094c;
L_0008d4:
    asm volatile("" : "=r" (r18));
    r19 = r18;
    asm volatile("" : "+r" (r19));
    asm volatile("" : "=r" (r19));
    r19 &= 0x3;
    if (r19) goto L_0008f5;
    r24 = 0x75;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r16,0x31" : : : "memory", "cc");
    asm volatile("cpc         r17,r24" : : : "memory", "cc");
    asm goto("brbc 0, %l[L_0008e2]" : : : "memory", "cc" : L_0008e2);
    r24 = 0x1;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r16,0x2c" : : : "memory", "cc");
    asm volatile("cpc         r17,r24" : : : "memory", "cc");
    asm goto("brbc 0, %l[L_0008ec]" : : : "memory", "cc" : L_0008ec);
    r16 = 0x2c;
    asm volatile("" : "+r" (r16));
    r17 = 0x1;
    asm volatile("" : "+r" (r17));
    goto L_0008e4;
L_0008e2:
    r16 = 0x30;
    asm volatile("" : "+r" (r16));
    r17 = 0x75;
    asm volatile("" : "+r" (r17));
L_0008e4:
    pm_cpu_disable_irq();
    r20 = PORTE_INTCTRL;
    asm volatile("" : "+r" (r20));
    asm volatile("" : "=r" (r20));
    if (!(r20 & (1u << 3))) goto L_0008b6;
    fpga_msg_send_t2();
    pm_cpu_enable_irq();
L_0008ec:
    /* Unvalidated C equivalent: *destination++ = r16; *destination++ = r17;
     * Step440 changed layout with typed Y operands; keep private stores.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("st          Y+,r16" : : : "memory", "cc");
    asm volatile("st          Y+,r17" : : : "memory", "cc");
    {
        register uint16_t copied_word asm("r16");
        register uint16_t destination_word asm("r20");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    asm volatile("" : "=r" (r18));
    r22 = r18;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 &= 0x3f;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 >>= 1;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 >>= 1;
    asm volatile("" : "+r" (r22));
    asm volatile("rcall       FUN_code_001053" : : : "memory", "cc");
    goto L_00096b;
L_0008f5:
    asm volatile("cpi         r19,0x1" : : : "memory", "cc");
    asm goto("brbc 1, %l[L_000915]" : : : "memory", "cc" : L_000915);
    r24 = 0x1;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r16,0xf5" : : : "memory", "cc");
    asm volatile("cpc         r17,r24" : : : "memory", "cc");
    asm goto("brbc 4, %l[L_000902]" : : : "memory", "cc" : L_000902);
    r24 = 0xfe;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r16,0xc" : : : "memory", "cc");
    asm volatile("cpc         r17,r24" : : : "memory", "cc");
    asm goto("brbc 4, %l[L_00090c]" : : : "memory", "cc" : L_00090c);
    r16 = 0xc;
    asm volatile("" : "+r" (r16));
    r17 = 0xfe;
    asm volatile("" : "+r" (r17));
    goto L_000904;
L_000902:
    r16 = 0xf4;
    asm volatile("" : "+r" (r16));
    r17 = 0x1;
    asm volatile("" : "+r" (r17));
L_000904:
    pm_cpu_disable_irq();
    r20 = PORTE_INTCTRL;
    asm volatile("" : "+r" (r20));
    asm volatile("" : "=r" (r20));
    if (!(r20 & (1u << 3))) goto L_0008b6;
    fpga_msg_send_t2();
    pm_cpu_enable_irq();
L_00090c:
    asm volatile("st          Y+,r16" : : : "memory", "cc");
    asm volatile("st          Y+,r17" : : : "memory", "cc");
    {
        register uint16_t copied_word asm("r16");
        register uint16_t destination_word asm("r20");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    asm volatile("" : "=r" (r18));
    r22 = r18;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 &= 0x3f;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 >>= 1;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 >>= 1;
    asm volatile("" : "+r" (r22));
    asm volatile("rcall       dac_set_value_2" : : : "memory", "cc");
    goto L_00096b;
L_000915:
    asm volatile("cpi         r19,0x2" : : : "memory", "cc");
    asm goto("brbc 1, %l[L_000935]" : : : "memory", "cc" : L_000935);
    r24 = 0x1;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r16,0xf5" : : : "memory", "cc");
    asm volatile("cpc         r17,r24" : : : "memory", "cc");
    asm goto("brbc 4, %l[L_000922]" : : : "memory", "cc" : L_000922);
    r24 = 0xfe;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r16,0xc" : : : "memory", "cc");
    asm volatile("cpc         r17,r24" : : : "memory", "cc");
    asm goto("brbc 4, %l[L_00092c]" : : : "memory", "cc" : L_00092c);
    r16 = 0xc;
    asm volatile("" : "+r" (r16));
    r17 = 0xfe;
    asm volatile("" : "+r" (r17));
    goto L_000924;
L_000922:
    r16 = 0xf4;
    asm volatile("" : "+r" (r16));
    r17 = 0x1;
    asm volatile("" : "+r" (r17));
L_000924:
    pm_cpu_disable_irq();
    r20 = PORTE_INTCTRL;
    asm volatile("" : "+r" (r20));
    asm volatile("" : "=r" (r20));
    if (!(r20 & (1u << 3))) goto L_0008b6;
    fpga_msg_send_t2();
    pm_cpu_enable_irq();
L_00092c:
    asm volatile("st          Y+,r16" : : : "memory", "cc");
    asm volatile("st          Y+,r17" : : : "memory", "cc");
    {
        register uint16_t copied_word asm("r16");
        register uint16_t destination_word asm("r20");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    asm volatile("" : "=r" (r18));
    r22 = r18;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 &= 0x3f;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 >>= 1;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 >>= 1;
    asm volatile("" : "+r" (r22));
    asm volatile("rcall       dac_set_value" : : : "memory", "cc");
    goto L_00096b;
L_000935:
    r24 = 0x4e;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r16,0x21" : : : "memory", "cc");
    asm volatile("cpc         r17,r24" : : : "memory", "cc");
    asm goto("brbs 0, %l[L_00093b]" : : : "memory", "cc" : L_00093b);
    r16 = 0x20;
    asm volatile("" : "+r" (r16));
    r17 = 0x4e;
    asm volatile("" : "+r" (r17));
L_00093b:
    pm_cpu_disable_irq();
    r20 = PORTE_INTCTRL;
    asm volatile("" : "+r" (r20));
    asm volatile("" : "=r" (r20));
    if (!(r20 & (1u << 3))) goto L_0008b6;
    fpga_msg_send_t2();
    pm_cpu_enable_irq();
    asm volatile("st          Y+,r16" : : : "memory", "cc");
    asm volatile("st          Y+,r17" : : : "memory", "cc");
    {
        register uint16_t copied_word asm("r16");
        register uint16_t destination_word asm("r20");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    asm volatile("" : "=r" (r18));
    r22 = r18;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 &= 0x3f;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 >>= 1;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 >>= 1;
    asm volatile("" : "+r" (r22));
    asm volatile("rcall       FUN_code_001068" : : : "memory", "cc");
    goto L_00096b;
L_00094c:
    r24 = 0xf;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r16,0xa1" : : : "memory", "cc");
    asm volatile("cpc         r17,r24" : : : "memory", "cc");
    asm goto("brbs 0, %l[L_00095a]" : : : "memory", "cc" : L_00095a);
    r16 = 0xa0;
    asm volatile("" : "+r" (r16));
    r17 = 0xf;
    asm volatile("" : "+r" (r17));
    pm_cpu_disable_irq();
    r20 = PORTE_INTCTRL;
    asm volatile("" : "+r" (r20));
    asm volatile("" : "=r" (r20));
    if (!(r20 & (1u << 3))) goto L_0008b6;
    fpga_msg_send_t2();
    pm_cpu_enable_irq();
L_00095a:
    asm volatile("st          Y+,r16" : : : "memory", "cc");
    asm volatile("st          Y+,r17" : : : "memory", "cc");
    asm volatile("" : "=r" (r18));
    r22 = r18;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r22 &= 0xf;
    asm volatile("" : "+r" (r22));
    asm volatile("" : "=r" (r22));
    r23 = r22;
    asm volatile("" : "+r" (r23));
    r30 = 0xcf;
    asm volatile("" : "+r" (r30));
    r31 = 0x21;
    asm volatile("" : "+r" (r31));
    asm volatile("" : "=r" (r23));
    r23 += r23;
    asm volatile("" : "+r" (r23));
    asm volatile("" : "=r" (r23));
    r23 += r23;
    asm volatile("" : "+r" (r23));
    asm volatile("" : "=r" (r23));
    r23 += r23;
    asm volatile("" : "+r" (r23));
    asm volatile("eor         r24,r24" : : : "memory", "cc");
    asm volatile("" : "=r" (r23), "=r" (r30));
    r30 += r23;
    asm volatile("" : "+r" (r30));
    asm volatile("adc         r31,r24" : : : "memory", "cc");
    asm volatile("ld          r20,Z+" : : : "memory", "cc");
    {
        register uint8_t *cursor asm("r30");
        register uint8_t high asm("r21");
        asm volatile("" : "=z" (cursor));
        high = *cursor;
        asm volatile("" : "+r" (high) : : "memory");
    }
    asm volatile("rcall       FUN_code_001053" : : : "memory", "cc");
    /* Original deliberate jump to the immediately following instruction. */
    asm volatile("rjmp 1f\n1:");
L_00096b:
    asm volatile("inc         r18" : : : "memory", "cc");
    asm volatile("cpi         r18,0xb0" : : : "memory", "cc");
    asm goto("brbc 1, %l[L_000971]" : : : "memory", "cc" : L_000971);
    r28 = 0x63;
    asm volatile("" : "+r" (r28));
    r29 = 0x21;
    asm volatile("" : "+r" (r29));
    goto L_0008be;
L_000971:
    asm volatile("cpi         r18,0xbc" : : : "memory", "cc");
    asm goto("brbs 1, %l[L_000974]" : : : "memory", "cc" : L_000974);
    goto L_0008be;
L_000974:
    return;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 2944 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_fpga_data_exchange(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x115c: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 4446;
    }
    case 0x115e: { // cli
        pm_irq(s, false);
        return 4448;
    }
    case 0x1160: { // sts 0x2006, r16
        uint16_t address = 8198;
        pm_write(s, address, s->r[16]);
        return 4452;
    }
    case 0x1164: { // lds r16, 0x0624
        uint16_t address = 1572;
        s->r[16] = pm_read(s, address);
        return 4456;
    }
    case 0x1168: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 4460 : 4458;
    }
    case 0x116a: { // rjmp .+4
        return 4464;
    }
    case 0x116c: { // sei
        pm_irq(s, true);
        return 4462;
    }
    case 0x116e: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1170: { // call 0x23ca
        s->calls[s->call_depth++] = 4468;
        return 9162;
    }
    case 0x1174: { // sei
        pm_irq(s, true);
        return 4470;
    }
    case 0x1176: { // ldi r18, 0x80
        s->r[18] = 128;
        return 4472;
    }
    case 0x1178: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 4474;
    }
    case 0x117a: { // ldi r29, 0x21
        s->r[29] = 33;
        return 4476;
    }
    case 0x117c: { // lsr r15
        bool carry = s->r[15] & 1;
        s->r[15] = (s->r[15] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[15], !!(s->r[15] & 128) ^ carry);
        return 4478;
    }
    case 0x117e: { // ror r14
        bool carry = s->r[14] & 1;
        s->r[14] = (s->r[14] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[14], !!(s->r[14] & 128) ^ carry);
        return 4480;
    }
    case 0x1180: { // ror r13
        bool carry = s->r[13] & 1;
        s->r[13] = (s->r[13] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[13], !!(s->r[13] & 128) ^ carry);
        return 4482;
    }
    case 0x1182: { // ror r12
        bool carry = s->r[12] & 1;
        s->r[12] = (s->r[12] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[12], !!(s->r[12] & 128) ^ carry);
        return 4484;
    }
    case 0x1184: { // ror r11
        bool carry = s->r[11] & 1;
        s->r[11] = (s->r[11] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[11], !!(s->r[11] & 128) ^ carry);
        return 4486;
    }
    case 0x1186: { // ror r10
        bool carry = s->r[10] & 1;
        s->r[10] = (s->r[10] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[10], !!(s->r[10] & 128) ^ carry);
        return 4488;
    }
    case 0x1188: { // ror r9
        bool carry = s->r[9] & 1;
        s->r[9] = (s->r[9] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[9], !!(s->r[9] & 128) ^ carry);
        return 4490;
    }
    case 0x118a: { // ror r8
        bool carry = s->r[8] & 1;
        s->r[8] = (s->r[8] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[8], !!(s->r[8] & 128) ^ carry);
        return 4492;
    }
    case 0x118c: { // brcs .+4
        return (pm_getflag(s, 0) == 1) ? 4498 : 4494;
    }
    case 0x118e: { // adiw r28, 0x02
        uint16_t old = pm_pointer(s, 28);
        uint16_t value = old + 2;
        pm_setpointer(s, 28, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 4496;
    }
    case 0x1190: { // rjmp .+324
        return 4822;
    }
    case 0x1192: { // cli
        pm_irq(s, false);
        return 4500;
    }
    case 0x1194: { // lds r20, 0x0689
        uint16_t address = 1673;
        s->r[20] = pm_read(s, address);
        return 4504;
    }
    case 0x1198: { // sbrs r20, 3
        return (!!(s->r[20] & (1u << 3)) == 1) ? 4508 : 4506;
    }
    case 0x119a: { // rjmp .-48
        return 4460;
    }
    case 0x119c: { // call 0x2368
        s->calls[s->call_depth++] = 4512;
        return 9064;
    }
    case 0x11a0: { // sei
        pm_irq(s, true);
        return 4514;
    }
    case 0x11a2: { // cpi r18, 0xB0
        pm_sub(s, s->r[18], 176, 0, false);
        return 4516;
    }
    case 0x11a4: { // brcs .+2
        return (pm_getflag(s, 0) == 1) ? 4520 : 4518;
    }
    case 0x11a6: { // rjmp .+240
        return 4760;
    }
    case 0x11a8: { // mov r19, r18
        s->r[19] = s->r[18];
        return 4522;
    }
    case 0x11aa: { // andi r19, 0x03
        s->r[19] &= 3;
        pm_nzv(s, s->r[19], false);
        return 4524;
    }
    case 0x11ac: { // brne .+60
        return (pm_getflag(s, 1) == 0) ? 4586 : 4526;
    }
    case 0x11ae: { // ldi r24, 0x75
        s->r[24] = 117;
        return 4528;
    }
    case 0x11b0: { // cpi r16, 0x31
        pm_sub(s, s->r[16], 49, 0, false);
        return 4530;
    }
    case 0x11b2: { // cpc r17, r24
        pm_sub(s, s->r[17], s->r[24], pm_getflag(s, CARRY), true);
        return 4532;
    }
    case 0x11b4: { // brcc .+14
        return (pm_getflag(s, 0) == 0) ? 4548 : 4534;
    }
    case 0x11b6: { // ldi r24, 0x01
        s->r[24] = 1;
        return 4536;
    }
    case 0x11b8: { // cpi r16, 0x2C
        pm_sub(s, s->r[16], 44, 0, false);
        return 4538;
    }
    case 0x11ba: { // cpc r17, r24
        pm_sub(s, s->r[17], s->r[24], pm_getflag(s, CARRY), true);
        return 4540;
    }
    case 0x11bc: { // brcc .+26
        return (pm_getflag(s, 0) == 0) ? 4568 : 4542;
    }
    case 0x11be: { // ldi r16, 0x2C
        s->r[16] = 44;
        return 4544;
    }
    case 0x11c0: { // ldi r17, 0x01
        s->r[17] = 1;
        return 4546;
    }
    case 0x11c2: { // rjmp .+4
        return 4552;
    }
    case 0x11c4: { // ldi r16, 0x30
        s->r[16] = 48;
        return 4550;
    }
    case 0x11c6: { // ldi r17, 0x75
        s->r[17] = 117;
        return 4552;
    }
    case 0x11c8: { // cli
        pm_irq(s, false);
        return 4554;
    }
    case 0x11ca: { // lds r20, 0x0689
        uint16_t address = 1673;
        s->r[20] = pm_read(s, address);
        return 4558;
    }
    case 0x11ce: { // sbrs r20, 3
        return (!!(s->r[20] & (1u << 3)) == 1) ? 4562 : 4560;
    }
    case 0x11d0: { // rjmp .-102
        return 4460;
    }
    case 0x11d2: { // call 0x230e
        s->calls[s->call_depth++] = 4566;
        return 8974;
    }
    case 0x11d6: { // sei
        pm_irq(s, true);
        return 4568;
    }
    case 0x11d8: { // st Y+, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[16]);
        return 4570;
    }
    case 0x11da: { // st Y+, r17
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[17]);
        return 4572;
    }
    case 0x11dc: { // movw r20, r16
        uint16_t pair = pm_pointer(s, 16);
        pm_setpointer(s, 20, pair);
        return 4574;
    }
    case 0x11de: { // mov r22, r18
        s->r[22] = s->r[18];
        return 4576;
    }
    case 0x11e0: { // andi r22, 0x3F
        s->r[22] &= 63;
        pm_nzv(s, s->r[22], false);
        return 4578;
    }
    case 0x11e2: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 4580;
    }
    case 0x11e4: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 4582;
    }
    case 0x11e6: { // rcall .+3774
        s->calls[s->call_depth++] = 4584;
        return 8358;
    }
    case 0x11e8: { // rjmp .+236
        return 4822;
    }
    case 0x11ea: { // cpi r19, 0x01
        pm_sub(s, s->r[19], 1, 0, false);
        return 4588;
    }
    case 0x11ec: { // brne .+60
        return (pm_getflag(s, 1) == 0) ? 4650 : 4590;
    }
    case 0x11ee: { // ldi r24, 0x01
        s->r[24] = 1;
        return 4592;
    }
    case 0x11f0: { // cpi r16, 0xF5
        pm_sub(s, s->r[16], 245, 0, false);
        return 4594;
    }
    case 0x11f2: { // cpc r17, r24
        pm_sub(s, s->r[17], s->r[24], pm_getflag(s, CARRY), true);
        return 4596;
    }
    case 0x11f4: { // brge .+14
        return (pm_getflag(s, 4) == 0) ? 4612 : 4598;
    }
    case 0x11f6: { // ldi r24, 0xFE
        s->r[24] = 254;
        return 4600;
    }
    case 0x11f8: { // cpi r16, 0x0C
        pm_sub(s, s->r[16], 12, 0, false);
        return 4602;
    }
    case 0x11fa: { // cpc r17, r24
        pm_sub(s, s->r[17], s->r[24], pm_getflag(s, CARRY), true);
        return 4604;
    }
    case 0x11fc: { // brge .+26
        return (pm_getflag(s, 4) == 0) ? 4632 : 4606;
    }
    case 0x11fe: { // ldi r16, 0x0C
        s->r[16] = 12;
        return 4608;
    }
    case 0x1200: { // ldi r17, 0xFE
        s->r[17] = 254;
        return 4610;
    }
    case 0x1202: { // rjmp .+4
        return 4616;
    }
    case 0x1204: { // ldi r16, 0xF4
        s->r[16] = 244;
        return 4614;
    }
    case 0x1206: { // ldi r17, 0x01
        s->r[17] = 1;
        return 4616;
    }
    case 0x1208: { // cli
        pm_irq(s, false);
        return 4618;
    }
    case 0x120a: { // lds r20, 0x0689
        uint16_t address = 1673;
        s->r[20] = pm_read(s, address);
        return 4622;
    }
    case 0x120e: { // sbrs r20, 3
        return (!!(s->r[20] & (1u << 3)) == 1) ? 4626 : 4624;
    }
    case 0x1210: { // rjmp .-166
        return 4460;
    }
    case 0x1212: { // call 0x230e
        s->calls[s->call_depth++] = 4630;
        return 8974;
    }
    case 0x1216: { // sei
        pm_irq(s, true);
        return 4632;
    }
    case 0x1218: { // st Y+, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[16]);
        return 4634;
    }
    case 0x121a: { // st Y+, r17
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[17]);
        return 4636;
    }
    case 0x121c: { // movw r20, r16
        uint16_t pair = pm_pointer(s, 16);
        pm_setpointer(s, 20, pair);
        return 4638;
    }
    case 0x121e: { // mov r22, r18
        s->r[22] = s->r[18];
        return 4640;
    }
    case 0x1220: { // andi r22, 0x3F
        s->r[22] &= 63;
        pm_nzv(s, s->r[22], false);
        return 4642;
    }
    case 0x1222: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 4644;
    }
    case 0x1224: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 4646;
    }
    case 0x1226: { // rcall .+3780
        s->calls[s->call_depth++] = 4648;
        return 8428;
    }
    case 0x1228: { // rjmp .+172
        return 4822;
    }
    case 0x122a: { // cpi r19, 0x02
        pm_sub(s, s->r[19], 2, 0, false);
        return 4652;
    }
    case 0x122c: { // brne .+60
        return (pm_getflag(s, 1) == 0) ? 4714 : 4654;
    }
    case 0x122e: { // ldi r24, 0x01
        s->r[24] = 1;
        return 4656;
    }
    case 0x1230: { // cpi r16, 0xF5
        pm_sub(s, s->r[16], 245, 0, false);
        return 4658;
    }
    case 0x1232: { // cpc r17, r24
        pm_sub(s, s->r[17], s->r[24], pm_getflag(s, CARRY), true);
        return 4660;
    }
    case 0x1234: { // brge .+14
        return (pm_getflag(s, 4) == 0) ? 4676 : 4662;
    }
    case 0x1236: { // ldi r24, 0xFE
        s->r[24] = 254;
        return 4664;
    }
    case 0x1238: { // cpi r16, 0x0C
        pm_sub(s, s->r[16], 12, 0, false);
        return 4666;
    }
    case 0x123a: { // cpc r17, r24
        pm_sub(s, s->r[17], s->r[24], pm_getflag(s, CARRY), true);
        return 4668;
    }
    case 0x123c: { // brge .+26
        return (pm_getflag(s, 4) == 0) ? 4696 : 4670;
    }
    case 0x123e: { // ldi r16, 0x0C
        s->r[16] = 12;
        return 4672;
    }
    case 0x1240: { // ldi r17, 0xFE
        s->r[17] = 254;
        return 4674;
    }
    case 0x1242: { // rjmp .+4
        return 4680;
    }
    case 0x1244: { // ldi r16, 0xF4
        s->r[16] = 244;
        return 4678;
    }
    case 0x1246: { // ldi r17, 0x01
        s->r[17] = 1;
        return 4680;
    }
    case 0x1248: { // cli
        pm_irq(s, false);
        return 4682;
    }
    case 0x124a: { // lds r20, 0x0689
        uint16_t address = 1673;
        s->r[20] = pm_read(s, address);
        return 4686;
    }
    case 0x124e: { // sbrs r20, 3
        return (!!(s->r[20] & (1u << 3)) == 1) ? 4690 : 4688;
    }
    case 0x1250: { // rjmp .-230
        return 4460;
    }
    case 0x1252: { // call 0x230e
        s->calls[s->call_depth++] = 4694;
        return 8974;
    }
    case 0x1256: { // sei
        pm_irq(s, true);
        return 4696;
    }
    case 0x1258: { // st Y+, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[16]);
        return 4698;
    }
    case 0x125a: { // st Y+, r17
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[17]);
        return 4700;
    }
    case 0x125c: { // movw r20, r16
        uint16_t pair = pm_pointer(s, 16);
        pm_setpointer(s, 20, pair);
        return 4702;
    }
    case 0x125e: { // mov r22, r18
        s->r[22] = s->r[18];
        return 4704;
    }
    case 0x1260: { // andi r22, 0x3F
        s->r[22] &= 63;
        pm_nzv(s, s->r[22], false);
        return 4706;
    }
    case 0x1262: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 4708;
    }
    case 0x1264: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 4710;
    }
    case 0x1266: { // rcall .+3740
        s->calls[s->call_depth++] = 4712;
        return 8452;
    }
    case 0x1268: { // rjmp .+108
        return 4822;
    }
    case 0x126a: { // ldi r24, 0x4E
        s->r[24] = 78;
        return 4716;
    }
    case 0x126c: { // cpi r16, 0x21
        pm_sub(s, s->r[16], 33, 0, false);
        return 4718;
    }
    case 0x126e: { // cpc r17, r24
        pm_sub(s, s->r[17], s->r[24], pm_getflag(s, CARRY), true);
        return 4720;
    }
    case 0x1270: { // brcs .+4
        return (pm_getflag(s, 0) == 1) ? 4726 : 4722;
    }
    case 0x1272: { // ldi r16, 0x20
        s->r[16] = 32;
        return 4724;
    }
    case 0x1274: { // ldi r17, 0x4E
        s->r[17] = 78;
        return 4726;
    }
    case 0x1276: { // cli
        pm_irq(s, false);
        return 4728;
    }
    case 0x1278: { // lds r20, 0x0689
        uint16_t address = 1673;
        s->r[20] = pm_read(s, address);
        return 4732;
    }
    case 0x127c: { // sbrs r20, 3
        return (!!(s->r[20] & (1u << 3)) == 1) ? 4736 : 4734;
    }
    case 0x127e: { // rjmp .-276
        return 4460;
    }
    case 0x1280: { // call 0x230e
        s->calls[s->call_depth++] = 4740;
        return 8974;
    }
    case 0x1284: { // sei
        pm_irq(s, true);
        return 4742;
    }
    case 0x1286: { // st Y+, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[16]);
        return 4744;
    }
    case 0x1288: { // st Y+, r17
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[17]);
        return 4746;
    }
    case 0x128a: { // movw r20, r16
        uint16_t pair = pm_pointer(s, 16);
        pm_setpointer(s, 20, pair);
        return 4748;
    }
    case 0x128c: { // mov r22, r18
        s->r[22] = s->r[18];
        return 4750;
    }
    case 0x128e: { // andi r22, 0x3F
        s->r[22] &= 63;
        pm_nzv(s, s->r[22], false);
        return 4752;
    }
    case 0x1290: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 4754;
    }
    case 0x1292: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 4756;
    }
    case 0x1294: { // rcall .+3642
        s->calls[s->call_depth++] = 4758;
        return 8400;
    }
    case 0x1296: { // rjmp .+62
        return 4822;
    }
    case 0x1298: { // ldi r24, 0x0F
        s->r[24] = 15;
        return 4762;
    }
    case 0x129a: { // cpi r16, 0xA1
        pm_sub(s, s->r[16], 161, 0, false);
        return 4764;
    }
    case 0x129c: { // cpc r17, r24
        pm_sub(s, s->r[17], s->r[24], pm_getflag(s, CARRY), true);
        return 4766;
    }
    case 0x129e: { // brcs .+20
        return (pm_getflag(s, 0) == 1) ? 4788 : 4768;
    }
    case 0x12a0: { // ldi r16, 0xA0
        s->r[16] = 160;
        return 4770;
    }
    case 0x12a2: { // ldi r17, 0x0F
        s->r[17] = 15;
        return 4772;
    }
    case 0x12a4: { // cli
        pm_irq(s, false);
        return 4774;
    }
    case 0x12a6: { // lds r20, 0x0689
        uint16_t address = 1673;
        s->r[20] = pm_read(s, address);
        return 4778;
    }
    case 0x12aa: { // sbrs r20, 3
        return (!!(s->r[20] & (1u << 3)) == 1) ? 4782 : 4780;
    }
    case 0x12ac: { // rjmp .-322
        return 4460;
    }
    case 0x12ae: { // call 0x230e
        s->calls[s->call_depth++] = 4786;
        return 8974;
    }
    case 0x12b2: { // sei
        pm_irq(s, true);
        return 4788;
    }
    case 0x12b4: { // st Y+, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[16]);
        return 4790;
    }
    case 0x12b6: { // st Y+, r17
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[17]);
        return 4792;
    }
    case 0x12b8: { // mov r22, r18
        s->r[22] = s->r[18];
        return 4794;
    }
    case 0x12ba: { // andi r22, 0x0F
        s->r[22] &= 15;
        pm_nzv(s, s->r[22], false);
        return 4796;
    }
    case 0x12bc: { // mov r23, r22
        s->r[23] = s->r[22];
        return 4798;
    }
    case 0x12be: { // ldi r30, 0xCF
        s->r[30] = 207;
        return 4800;
    }
    case 0x12c0: { // ldi r31, 0x21
        s->r[31] = 33;
        return 4802;
    }
    case 0x12c2: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 4804;
    }
    case 0x12c4: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 4806;
    }
    case 0x12c6: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 4808;
    }
    case 0x12c8: { // eor r24, r24
        s->r[24] ^= s->r[24];
        pm_nzv(s, s->r[24], false);
        return 4810;
    }
    case 0x12ca: { // add r30, r23
        s->r[30] = pm_add(s, s->r[30], s->r[23], 0);
        return 4812;
    }
    case 0x12cc: { // adc r31, r24
        s->r[31] = pm_add(s, s->r[31], s->r[24], pm_getflag(s, CARRY));
        return 4814;
    }
    case 0x12ce: { // ld r20, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[20] = pm_read(s, address);
        return 4816;
    }
    case 0x12d0: { // ld r21, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[21] = pm_read(s, address);
        return 4818;
    }
    case 0x12d2: { // rcall .+3538
        s->calls[s->call_depth++] = 4820;
        return 8358;
    }
    case 0x12d4: { // rjmp .+0
        return 4822;
    }
    case 0x12d6: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 4824;
    }
    case 0x12d8: { // cpi r18, 0xB0
        pm_sub(s, s->r[18], 176, 0, false);
        return 4826;
    }
    case 0x12da: { // brne .+6
        return (pm_getflag(s, 1) == 0) ? 4834 : 4828;
    }
    case 0x12dc: { // ldi r28, 0x63
        s->r[28] = 99;
        return 4830;
    }
    case 0x12de: { // ldi r29, 0x21
        s->r[29] = 33;
        return 4832;
    }
    case 0x12e0: { // rjmp .-358
        return 4476;
    }
    case 0x12e2: { // cpi r18, 0xBC
        pm_sub(s, s->r[18], 188, 0, false);
        return 4836;
    }
    case 0x12e4: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 4840 : 4838;
    }
    case 0x12e6: { // rjmp .-364
        return 4476;
    }
    case 0x12e8: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
