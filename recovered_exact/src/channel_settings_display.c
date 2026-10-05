#include "legacy_cpu.h"
#include <stdint.h>
/* Private Y cursor: register reservation, no SRAM object or GNU frame. */
register uint8_t *settings asm("r28");
#define MESSAGE(cursor) asm volatile("rcall cli_send_msg" : "+z" (cursor) : : "memory", "cc")
#define OFFSET_POINTER(pointer, offset, zero) \
 do { \
 register uint8_t address_low asm("r28"); \
 asm volatile("" : "=r" (address_low) : "y" (pointer)); \
 address_low += (offset); asm volatile("" : "+r" (address_low)); \
 asm volatile("adc r29, %1" : "=y" (pointer) : "r" (zero) : "cc"); \
 } while (0)
#define READ_PAIR(last_load, formatter, tail) \
 asm volatile("ld r16, Y+\n\t" last_load "\n\trcall " formatter "\n\trcall " tail \
 : "=r" (word), "+y" (settings), "+z" (message) : : "memory", "cc")

#define READ_PAIR_PLAIN(formatter, tail) do { \
 asm volatile("ld r16, Y+" : "=r" (word), "+y" (settings) : : "memory"); \
 register uint8_t high asm("r17") = *settings; \
 asm volatile("" : "+r" (high) : : "memory"); \
 asm volatile("" : "=r" (word)); \
 asm volatile("rcall " formatter "\n\trcall " tail : "+r" (word), "+y" (settings), "+z" (message) : : "memory", "cc"); \
 } while (0)

void channels_read(void)
{
    register uint16_t word asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000e30" : "=r" (word) : : "memory", "cc");
    register uint8_t channel asm("r20"), zero asm("r21");
    asm volatile("clr %0\n\tclr %1" : "=r" (channel), "=r" (zero) : : "cc");
next_channel:;
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2ac6;
    MESSAGE(message);
    register uint8_t low asm("r16") = channel;
    asm volatile("" : "+r" (low), "+r" (channel));
    asm volatile("clr r17\n\trcall cli_send_uint16\n\trcall cli_send_msg" : "+r" (low), "+z" (message) : : "r17", "memory", "cc");
    settings = (uint8_t *)0x2163;
    asm volatile("" : "+y" (settings));
    register uint8_t offset asm("r22") = channel;
    asm volatile("" : "+r" (offset), "+r" (channel));
    offset += offset; asm volatile("" : "+r" (offset), "+r" (channel));
    OFFSET_POINTER(settings, offset, zero);
    READ_PAIR_PLAIN("cli_send_uint16", "cli_send_msg");
    register uint8_t tdc_channel asm("r23") = channel;
    asm volatile("" : "+r" (tdc_channel), "+r" (channel));
    tdc_channel &= 3; asm volatile("" : "+r" (tdc_channel), "+r" (channel));
    register uint8_t spacing asm("r24") = 0x20;
    asm volatile("mul %0, %1" : : "r" (tdc_channel), "r" (spacing) : "r0", "r1", "cc");
    low = 0x0c;
    register uint8_t product_low asm("r0");
    asm volatile("" : "=r" (product_low));
    low += product_low;
    asm volatile("" : "+r" (low));
    register uint8_t device asm("r19") = channel;
    asm volatile("" : "+r" (device), "+r" (channel));
    device >>= 1;
    asm volatile("" : "+r" (device));
    device >>= 1;
    asm volatile("" : "+r" (device) : "r" (low));
    pm_cpu_disable_irq();
    asm volatile("rcall ths788_read" : "+r" (device), "=r" (word) : "r" (low) : "memory", "cc");
    pm_cpu_enable_irq();
    register uint8_t high asm("r17");
    asm volatile("" : "=r" (high));
    low = high;
    asm volatile("clr r17\n\tsbrc r16, 7\n\tldi r17, 0xff\n\trcall cli_send_int16\n\trcall cli_send_msg"
                 : "+r" (low), "+z" (message) : : "r17", "memory", "cc");
    settings = (uint8_t *)0x21b7;
    asm volatile("" : "+y" (settings));
    offset = channel; asm volatile("" : "+r" (offset), "+r" (channel));
    offset += offset; asm volatile("" : "+r" (offset), "+r" (channel));
    OFFSET_POINTER(settings, offset, zero);
    READ_PAIR("ld r17, Y+", "cli_send_int16", "cli_send_msg");
    settings = (uint8_t *)0x2187;
    asm volatile("" : "+y" (settings));
    offset = channel; asm volatile("" : "+r" (offset), "+r" (channel));
    offset += offset; asm volatile("" : "+r" (offset), "+r" (channel));
    offset += offset; asm volatile("" : "+r" (offset), "+r" (channel));
    OFFSET_POINTER(settings, offset, zero);
    asm volatile("ld r16, Y+\n\tld r17, Y+\n\trcall cli_send_uint16" : "=r" (word), "+y" (settings) : : "memory", "cc");
    low = ' ';
    asm volatile("rcall cli_send_buf" : "+r" (low) : : "memory", "cc");
    READ_PAIR_PLAIN("cli_send_uint16", "cli_send_crlf");
    asm volatile("inc %0" : "+r" (channel) : : "cc");
    asm goto("cpi %0, 12\n\tbreq 1f\n\trjmp %l[next_channel]\n1:" : : "r" (channel) : "cc" : next_channel);
}
asm(".pushsection .text.channels_read,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 1152 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_channel_settings_display(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1c62: { // rcall .+3032
        s->calls[s->call_depth++] = 7268;
        return 10300;
    }
    case 0x1c64: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 7270;
    }
    case 0x1c66: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 7264 : 7272;
    }
    case 0x1c68: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 7274;
    }
    case 0x1c6a: { // eor r21, r21
        s->r[21] ^= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7276;
    }
    case 0x1c6c: { // ldi r30, 0xC6
        s->r[30] = 198;
        return 7278;
    }
    case 0x1c6e: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 7280;
    }
    case 0x1c70: { // rcall .+2996
        s->calls[s->call_depth++] = 7282;
        return 10278;
    }
    case 0x1c72: { // mov r16, r20
        s->r[16] = s->r[20];
        return 7284;
    }
    case 0x1c74: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 7286;
    }
    case 0x1c76: { // rcall .+2774
        s->calls[s->call_depth++] = 7288;
        return 10062;
    }
    case 0x1c78: { // rcall .+2988
        s->calls[s->call_depth++] = 7290;
        return 10278;
    }
    case 0x1c7a: { // ldi r28, 0x63
        s->r[28] = 99;
        return 7292;
    }
    case 0x1c7c: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7294;
    }
    case 0x1c7e: { // mov r22, r20
        s->r[22] = s->r[20];
        return 7296;
    }
    case 0x1c80: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 7298;
    }
    case 0x1c82: { // add r28, r22
        s->r[28] = pm_add(s, s->r[28], s->r[22], 0);
        return 7300;
    }
    case 0x1c84: { // adc r29, r21
        s->r[29] = pm_add(s, s->r[29], s->r[21], pm_getflag(s, CARRY));
        return 7302;
    }
    case 0x1c86: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 7304;
    }
    case 0x1c88: { // ld r17, Y
        uint16_t address = pm_pointer(s, 28) + 0;
        s->r[17] = pm_read(s, address);
        return 7306;
    }
    case 0x1c8a: { // rcall .+2754
        s->calls[s->call_depth++] = 7308;
        return 10062;
    }
    case 0x1c8c: { // rcall .+2968
        s->calls[s->call_depth++] = 7310;
        return 10278;
    }
    case 0x1c8e: { // mov r23, r20
        s->r[23] = s->r[20];
        return 7312;
    }
    case 0x1c90: { // andi r23, 0x03
        s->r[23] &= 3;
        pm_nzv(s, s->r[23], false);
        return 7314;
    }
    case 0x1c92: { // ldi r24, 0x20
        s->r[24] = 32;
        return 7316;
    }
    case 0x1c94: { // mul r23, r24
        uint16_t product = (s->r[23]) * (int)s->r[24];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 7318;
    }
    case 0x1c96: { // ldi r16, 0x0C
        s->r[16] = 12;
        return 7320;
    }
    case 0x1c98: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 7322;
    }
    case 0x1c9a: { // mov r19, r20
        s->r[19] = s->r[20];
        return 7324;
    }
    case 0x1c9c: { // lsr r19
        bool carry = s->r[19] & 1;
        s->r[19] = (s->r[19] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[19], !!(s->r[19] & 128) ^ carry);
        return 7326;
    }
    case 0x1c9e: { // lsr r19
        bool carry = s->r[19] & 1;
        s->r[19] = (s->r[19] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[19], !!(s->r[19] & 128) ^ carry);
        return 7328;
    }
    case 0x1ca0: { // cli
        pm_irq(s, false);
        return 7330;
    }
    case 0x1ca2: { // rcall .+1380
        s->calls[s->call_depth++] = 7332;
        return 8712;
    }
    case 0x1ca4: { // sei
        pm_irq(s, true);
        return 7334;
    }
    case 0x1ca6: { // mov r16, r17
        s->r[16] = s->r[17];
        return 7336;
    }
    case 0x1ca8: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 7338;
    }
    case 0x1caa: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 7342 : 7340;
    }
    case 0x1cac: { // ldi r17, 0xFF
        s->r[17] = 255;
        return 7342;
    }
    case 0x1cae: { // rcall .+2710
        s->calls[s->call_depth++] = 7344;
        return 10054;
    }
    case 0x1cb0: { // rcall .+2932
        s->calls[s->call_depth++] = 7346;
        return 10278;
    }
    case 0x1cb2: { // ldi r28, 0xB7
        s->r[28] = 183;
        return 7348;
    }
    case 0x1cb4: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7350;
    }
    case 0x1cb6: { // mov r22, r20
        s->r[22] = s->r[20];
        return 7352;
    }
    case 0x1cb8: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 7354;
    }
    case 0x1cba: { // add r28, r22
        s->r[28] = pm_add(s, s->r[28], s->r[22], 0);
        return 7356;
    }
    case 0x1cbc: { // adc r29, r21
        s->r[29] = pm_add(s, s->r[29], s->r[21], pm_getflag(s, CARRY));
        return 7358;
    }
    case 0x1cbe: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 7360;
    }
    case 0x1cc0: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 7362;
    }
    case 0x1cc2: { // rcall .+2690
        s->calls[s->call_depth++] = 7364;
        return 10054;
    }
    case 0x1cc4: { // rcall .+2912
        s->calls[s->call_depth++] = 7366;
        return 10278;
    }
    case 0x1cc6: { // ldi r28, 0x87
        s->r[28] = 135;
        return 7368;
    }
    case 0x1cc8: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7370;
    }
    case 0x1cca: { // mov r22, r20
        s->r[22] = s->r[20];
        return 7372;
    }
    case 0x1ccc: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 7374;
    }
    case 0x1cce: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 7376;
    }
    case 0x1cd0: { // add r28, r22
        s->r[28] = pm_add(s, s->r[28], s->r[22], 0);
        return 7378;
    }
    case 0x1cd2: { // adc r29, r21
        s->r[29] = pm_add(s, s->r[29], s->r[21], pm_getflag(s, CARRY));
        return 7380;
    }
    case 0x1cd4: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 7382;
    }
    case 0x1cd6: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 7384;
    }
    case 0x1cd8: { // rcall .+2676
        s->calls[s->call_depth++] = 7386;
        return 10062;
    }
    case 0x1cda: { // ldi r16, 0x20
        s->r[16] = 32;
        return 7388;
    }
    case 0x1cdc: { // rcall .+3022
        s->calls[s->call_depth++] = 7390;
        return 10412;
    }
    case 0x1cde: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 7392;
    }
    case 0x1ce0: { // ld r17, Y
        uint16_t address = pm_pointer(s, 28) + 0;
        s->r[17] = pm_read(s, address);
        return 7394;
    }
    case 0x1ce2: { // rcall .+2666
        s->calls[s->call_depth++] = 7396;
        return 10062;
    }
    case 0x1ce4: { // rcall .+2872
        s->calls[s->call_depth++] = 7398;
        return 10270;
    }
    case 0x1ce6: { // inc r20
        s->r[20]++;
        pm_nzv(s, s->r[20], s->r[20] == 128);
        return 7400;
    }
    case 0x1ce8: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 7402;
    }
    case 0x1cea: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 7406 : 7404;
    }
    case 0x1cec: { // rjmp .-130
        return 7276;
    }
    case 0x1cee: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1cf0: { // rjmp .-2346
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
