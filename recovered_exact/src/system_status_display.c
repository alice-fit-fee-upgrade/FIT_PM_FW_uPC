#include "legacy_cpu.h"
#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))
#define SET_MESSAGE(address) do { message = (const uint8_t *)(address); asm volatile("" : "+z" (message)); } while (0)
#define SEND_MESSAGE() asm volatile("rcall cli_send_msg" : "+z" (message) : : "memory", "cc")

/* Original status message order and early returns are intentionally retained.
 * Message addresses name existing FLASH strings, never linker-created copies. */
void cli_send_system_status(void)
{
    register uint16_t word asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000d1e" : "=r" (word) : : "memory", "cc");
    register const uint8_t *message asm("r30");
    SET_MESSAGE(0x2b6a); SEND_MESSAGE();
    word = *(volatile uint16_t *)0x2232;
    asm volatile("rcall cli_send_32bit_hex" : "+r" (word) : : "memory", "cc");
    SET_MESSAGE(0x2b7e); SEND_MESSAGE();
    SET_MESSAGE(0x2b92);
    register uint16_t second asm("r18");
    asm volatile("lpm r18, Z+\n\tlpm r19, Z+\n\tlpm r16, Z+\n\tlpm r17, Z+\n\trcall cli_send_32bit_hex"
                 : "=r" (second), "=r" (word), "+z" (message) : : "memory", "cc");
    word = second;
    asm volatile("rcall cli_send_32bit_hex" : "+r" (word) : : "memory", "cc");
    SET_MESSAGE(0x29a6); SEND_MESSAGE();
    pm_cpu_disable_irq();
    register uint8_t status asm("r18") = RAM8(0x2157);
    asm volatile("" : "+r" (status));
    word = *(volatile uint16_t *)0x2160;
    asm volatile("" : "+r" (word));
    register uint8_t power asm("r20") = RAM8(0x0688);
    asm volatile("" : : "r" (power) : "memory");
    pm_cpu_enable_irq();
    SET_MESSAGE(0x2998);
    if ((status & (1u << 0))) goto power_state;
    SET_MESSAGE(0x299e);
power_state:
    SEND_MESSAGE();
    register uint8_t thermal asm("r19") = status;
    asm volatile("" : "+r" (thermal), "+r" (status));
    SET_MESSAGE(0x29c0); SEND_MESSAGE();
    asm volatile("rcall cli_send_temperature" : "+r" (word) : : "memory", "cc");
    SET_MESSAGE(0x29ce);
    thermal &= 6; asm volatile("" : "+r" (thermal));
    asm goto("breq %l[thermal_state]" : : : : thermal_state);
    SET_MESSAGE(0x29e2);
    asm goto("cpi r19, 6\n\tbreq %l[thermal_state]" : : "r" (thermal) : "cc" : thermal_state);
    SET_MESSAGE(0x29dc);
    thermal &= 4; asm volatile("" : "+r" (thermal));
    asm goto("brne %l[thermal_state]" : : : : thermal_state);
    SET_MESSAGE(0x29d6);
thermal_state:
    SEND_MESSAGE();
    SET_MESSAGE(0x29ec); SEND_MESSAGE();
    /* C alternative: if ((power & (1u << 2))) goto pll_powered;
     * Trial changed the exact layout/encoding; retained ASM. See
     * exact_status_branch_results.json. No functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("sbrc r20, 2\n\trjmp %l[pll_powered]" : : "r" (power) : : pll_powered);
    SET_MESSAGE(0x2a02);
message_and_return:
    SEND_MESSAGE();
    return;
pll_powered:
    SET_MESSAGE(0x29fc); SEND_MESSAGE();
    SET_MESSAGE(0x2998);
    if ((status & (1u << 3))) goto pll_locked;
    SET_MESSAGE(0x299e); SEND_MESSAGE();
    asm volatile("sbrs r18, 6\n\tret" : : "r" (status));
    SET_MESSAGE(0x2af2);
    goto message_and_return;
pll_locked:
    SEND_MESSAGE();
    SET_MESSAGE(0x2a28); SEND_MESSAGE();
    thermal = RAM8(0x2162);
    asm volatile("" : "+r" (thermal));
    SET_MESSAGE(0x2a36);
    if ((thermal & (1u << 1))) goto pll_configuration;
    SET_MESSAGE(0x2a3e);
    if ((thermal & (1u << 2))) goto pll_configuration;
    SET_MESSAGE(0x2a48);
pll_configuration:
    SEND_MESSAGE();
    SET_MESSAGE(0x2a52); SEND_MESSAGE();
    SET_MESSAGE(0x2a5a);
    if (!(thermal & (1u << 0))) goto pll_control;
    SET_MESSAGE(0x2a5e);
pll_control:
    SEND_MESSAGE();
    SET_MESSAGE(0x2b04);
    if ((status & (1u << 4))) goto fpga_ready;
    SET_MESSAGE(0x2ab4);
    goto message_and_return;
fpga_ready:
    SEND_MESSAGE();
    SET_MESSAGE(0x2b12); SEND_MESSAGE();
    SET_MESSAGE(0x2998);
    if (!(status & (1u << 7))) goto tdc_inactive;
    SET_MESSAGE(0x2b20); SEND_MESSAGE();
    power = RAM8(0x2158);
    register uint8_t device asm("r17");
    asm volatile("clr r17" : "=r" (device) : : "cc");
tdc_alarm:
    if (!(power & (1u << 2))) goto next_tdc;
    SET_MESSAGE(0x2b28); SEND_MESSAGE();
    register uint8_t character asm("r16") = '0';
    asm volatile("" : "+r" (character));
    character += device;
    asm volatile("rcall cli_send_buf" : "+r" (character) : : "memory", "cc");
    SET_MESSAGE(0x2b2e); SEND_MESSAGE();
next_tdc:
    asm goto("cpi r17, 2\n\tbrcc %l[finished]" : : "r" (device) : "cc" : finished);
    /* C value equivalent: ++device; INC keeps the original carry flag. */
    asm volatile("inc %0" : "+r" (device) : : "cc");
    power >>= 1;
    asm volatile("" : "+r" (power));
    goto tdc_alarm;
tdc_inactive:
    SEND_MESSAGE();
    SET_MESSAGE(0x2b96); SEND_MESSAGE();
    power = RAM8(0x2441);
    power += power; asm volatile("" : "+r" (power));
    SET_MESSAGE(0x2ba8);
    register uint8_t zero asm("r21");
    asm volatile("clr r21" : "=r" (zero) : : "cc");
    { register uint8_t address_low asm("r30");
      asm volatile("" : "=r" (address_low) : "z" (message));
      address_low += power; asm volatile("" : "+r" (address_low)); }
    asm volatile("adc r31, r21\n\tlpm r20, Z+"
                 : "=r" (zero), "=z" (message), "+r" (power) : : "memory", "cc");
    { register const __flash uint8_t *read_cursor asm("r30");
      asm volatile("" : "=z" (read_cursor) : "z" (message));
      zero = *read_cursor; asm volatile("" : "+r" (zero) : : "memory"); }
    register uint16_t selected_message asm("r20");
    asm volatile("" : "=r" (selected_message));
    message = (const uint8_t *)selected_message;
    SEND_MESSAGE();
    SET_MESSAGE(0x2984); SEND_MESSAGE();
    power = RAM8(0x2158);
    SET_MESSAGE(0x2b44); SEND_MESSAGE();
    SET_MESSAGE(0x2a02);
    /* C alternative: if ((power & (1u << 0))) goto tdc_powered;
     * Trial changed the exact layout/encoding; retained ASM. See
     * exact_status_branch_results.json. No functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("sbrc r20, 0\n\trjmp %l[tdc_powered]" : : "r" (power) : : tdc_powered);
    SEND_MESSAGE();
    return;
tdc_powered:
    SET_MESSAGE(0x29fc); SEND_MESSAGE();
    SET_MESSAGE(0x2b4e);
    /* C alternative: if (!(power & (1u << 1))) goto tdc_configuration;
     * Trial changed the exact layout/encoding; retained ASM. See
     * exact_status_branch_results.json. No functional-test claim.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm goto("sbrs r20, 1\n\trjmp %l[tdc_configuration]" : : "r" (power) : : tdc_configuration);
    SET_MESSAGE(0x2b5c);
tdc_configuration:
    SEND_MESSAGE();
finished:
    return;
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 2912 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_system_status_display(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1a64: { // rcall .+3542
        s->calls[s->call_depth++] = 6758;
        return 10300;
    }
    case 0x1a66: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6760;
    }
    case 0x1a68: { // brne .-46
        return (pm_getflag(s, 1) == 0) ? 6716 : 6762;
    }
    case 0x1a6a: { // ldi r30, 0x6A
        s->r[30] = 106;
        return 6764;
    }
    case 0x1a6c: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 6766;
    }
    case 0x1a6e: { // rcall .+3510
        s->calls[s->call_depth++] = 6768;
        return 10278;
    }
    case 0x1a70: { // lds r16, 0x2232
        uint16_t address = 8754;
        s->r[16] = pm_read(s, address);
        return 6772;
    }
    case 0x1a74: { // lds r17, 0x2233
        uint16_t address = 8755;
        s->r[17] = pm_read(s, address);
        return 6776;
    }
    case 0x1a78: { // rcall .+3198
        s->calls[s->call_depth++] = 6778;
        return 9976;
    }
    case 0x1a7a: { // ldi r30, 0x7E
        s->r[30] = 126;
        return 6780;
    }
    case 0x1a7c: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 6782;
    }
    case 0x1a7e: { // rcall .+3494
        s->calls[s->call_depth++] = 6784;
        return 10278;
    }
    case 0x1a80: { // ldi r30, 0x92
        s->r[30] = 146;
        return 6786;
    }
    case 0x1a82: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 6788;
    }
    case 0x1a84: { // lpm r18, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[18] = pm_golden_flash[address];
        return 6790;
    }
    case 0x1a86: { // lpm r19, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[19] = pm_golden_flash[address];
        return 6792;
    }
    case 0x1a88: { // lpm r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_golden_flash[address];
        return 6794;
    }
    case 0x1a8a: { // lpm r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_golden_flash[address];
        return 6796;
    }
    case 0x1a8c: { // rcall .+3178
        s->calls[s->call_depth++] = 6798;
        return 9976;
    }
    case 0x1a8e: { // movw r16, r18
        uint16_t pair = pm_pointer(s, 18);
        pm_setpointer(s, 16, pair);
        return 6800;
    }
    case 0x1a90: { // rcall .+3174
        s->calls[s->call_depth++] = 6802;
        return 9976;
    }
    case 0x1a92: { // ldi r30, 0xA6
        s->r[30] = 166;
        return 6804;
    }
    case 0x1a94: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6806;
    }
    case 0x1a96: { // rcall .+3470
        s->calls[s->call_depth++] = 6808;
        return 10278;
    }
    case 0x1a98: { // cli
        pm_irq(s, false);
        return 6810;
    }
    case 0x1a9a: { // lds r18, 0x2157
        uint16_t address = 8535;
        s->r[18] = pm_read(s, address);
        return 6814;
    }
    case 0x1a9e: { // lds r16, 0x2160
        uint16_t address = 8544;
        s->r[16] = pm_read(s, address);
        return 6818;
    }
    case 0x1aa2: { // lds r17, 0x2161
        uint16_t address = 8545;
        s->r[17] = pm_read(s, address);
        return 6822;
    }
    case 0x1aa6: { // lds r20, 0x0688
        uint16_t address = 1672;
        s->r[20] = pm_read(s, address);
        return 6826;
    }
    case 0x1aaa: { // sei
        pm_irq(s, true);
        return 6828;
    }
    case 0x1aac: { // ldi r30, 0x98
        s->r[30] = 152;
        return 6830;
    }
    case 0x1aae: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6832;
    }
    case 0x1ab0: { // sbrc r18, 0
        return (!!(s->r[18] & (1u << 0)) == 0) ? 6836 : 6834;
    }
    case 0x1ab2: { // rjmp .+4
        return 6840;
    }
    case 0x1ab4: { // ldi r30, 0x9E
        s->r[30] = 158;
        return 6838;
    }
    case 0x1ab6: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6840;
    }
    case 0x1ab8: { // rcall .+3436
        s->calls[s->call_depth++] = 6842;
        return 10278;
    }
    case 0x1aba: { // mov r19, r18
        s->r[19] = s->r[18];
        return 6844;
    }
    case 0x1abc: { // ldi r30, 0xC0
        s->r[30] = 192;
        return 6846;
    }
    case 0x1abe: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6848;
    }
    case 0x1ac0: { // rcall .+3428
        s->calls[s->call_depth++] = 6850;
        return 10278;
    }
    case 0x1ac2: { // rcall .+3186
        s->calls[s->call_depth++] = 6852;
        return 10038;
    }
    case 0x1ac4: { // ldi r30, 0xCE
        s->r[30] = 206;
        return 6854;
    }
    case 0x1ac6: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6856;
    }
    case 0x1ac8: { // andi r19, 0x06
        s->r[19] &= 6;
        pm_nzv(s, s->r[19], false);
        return 6858;
    }
    case 0x1aca: { // breq .+20
        return (pm_getflag(s, 1) == 1) ? 6880 : 6860;
    }
    case 0x1acc: { // ldi r30, 0xE2
        s->r[30] = 226;
        return 6862;
    }
    case 0x1ace: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6864;
    }
    case 0x1ad0: { // cpi r19, 0x06
        pm_sub(s, s->r[19], 6, 0, false);
        return 6866;
    }
    case 0x1ad2: { // breq .+12
        return (pm_getflag(s, 1) == 1) ? 6880 : 6868;
    }
    case 0x1ad4: { // ldi r30, 0xDC
        s->r[30] = 220;
        return 6870;
    }
    case 0x1ad6: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6872;
    }
    case 0x1ad8: { // andi r19, 0x04
        s->r[19] &= 4;
        pm_nzv(s, s->r[19], false);
        return 6874;
    }
    case 0x1ada: { // brne .+4
        return (pm_getflag(s, 1) == 0) ? 6880 : 6876;
    }
    case 0x1adc: { // ldi r30, 0xD6
        s->r[30] = 214;
        return 6878;
    }
    case 0x1ade: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6880;
    }
    case 0x1ae0: { // rcall .+3396
        s->calls[s->call_depth++] = 6882;
        return 10278;
    }
    case 0x1ae2: { // ldi r30, 0xEC
        s->r[30] = 236;
        return 6884;
    }
    case 0x1ae4: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6886;
    }
    case 0x1ae6: { // rcall .+3390
        s->calls[s->call_depth++] = 6888;
        return 10278;
    }
    case 0x1ae8: { // sbrc r20, 2
        return (!!(s->r[20] & (1u << 2)) == 0) ? 6892 : 6890;
    }
    case 0x1aea: { // rjmp .+8
        return 6900;
    }
    case 0x1aec: { // ldi r30, 0x02
        s->r[30] = 2;
        return 6894;
    }
    case 0x1aee: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6896;
    }
    case 0x1af0: { // rcall .+3380
        s->calls[s->call_depth++] = 6898;
        return 10278;
    }
    case 0x1af2: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1af4: { // ldi r30, 0xFC
        s->r[30] = 252;
        return 6902;
    }
    case 0x1af6: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6904;
    }
    case 0x1af8: { // rcall .+3372
        s->calls[s->call_depth++] = 6906;
        return 10278;
    }
    case 0x1afa: { // ldi r30, 0x98
        s->r[30] = 152;
        return 6908;
    }
    case 0x1afc: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6910;
    }
    case 0x1afe: { // sbrc r18, 3
        return (!!(s->r[18] & (1u << 3)) == 0) ? 6914 : 6912;
    }
    case 0x1b00: { // rjmp .+16
        return 6930;
    }
    case 0x1b02: { // ldi r30, 0x9E
        s->r[30] = 158;
        return 6916;
    }
    case 0x1b04: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6918;
    }
    case 0x1b06: { // rcall .+3358
        s->calls[s->call_depth++] = 6920;
        return 10278;
    }
    case 0x1b08: { // sbrs r18, 6
        return (!!(s->r[18] & (1u << 6)) == 1) ? 6924 : 6922;
    }
    case 0x1b0a: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1b0c: { // ldi r30, 0xF2
        s->r[30] = 242;
        return 6926;
    }
    case 0x1b0e: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6928;
    }
    case 0x1b10: { // rjmp .-34
        return 6896;
    }
    case 0x1b12: { // rcall .+3346
        s->calls[s->call_depth++] = 6932;
        return 10278;
    }
    case 0x1b14: { // ldi r30, 0x28
        s->r[30] = 40;
        return 6934;
    }
    case 0x1b16: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6936;
    }
    case 0x1b18: { // rcall .+3340
        s->calls[s->call_depth++] = 6938;
        return 10278;
    }
    case 0x1b1a: { // lds r19, 0x2162
        uint16_t address = 8546;
        s->r[19] = pm_read(s, address);
        return 6942;
    }
    case 0x1b1e: { // ldi r30, 0x36
        s->r[30] = 54;
        return 6944;
    }
    case 0x1b20: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6946;
    }
    case 0x1b22: { // sbrc r19, 1
        return (!!(s->r[19] & (1u << 1)) == 0) ? 6950 : 6948;
    }
    case 0x1b24: { // rjmp .+12
        return 6962;
    }
    case 0x1b26: { // ldi r30, 0x3E
        s->r[30] = 62;
        return 6952;
    }
    case 0x1b28: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6954;
    }
    case 0x1b2a: { // sbrc r19, 2
        return (!!(s->r[19] & (1u << 2)) == 0) ? 6958 : 6956;
    }
    case 0x1b2c: { // rjmp .+4
        return 6962;
    }
    case 0x1b2e: { // ldi r30, 0x48
        s->r[30] = 72;
        return 6960;
    }
    case 0x1b30: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6962;
    }
    case 0x1b32: { // rcall .+3314
        s->calls[s->call_depth++] = 6964;
        return 10278;
    }
    case 0x1b34: { // ldi r30, 0x52
        s->r[30] = 82;
        return 6966;
    }
    case 0x1b36: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6968;
    }
    case 0x1b38: { // rcall .+3308
        s->calls[s->call_depth++] = 6970;
        return 10278;
    }
    case 0x1b3a: { // ldi r30, 0x5A
        s->r[30] = 90;
        return 6972;
    }
    case 0x1b3c: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6974;
    }
    case 0x1b3e: { // sbrs r19, 0
        return (!!(s->r[19] & (1u << 0)) == 1) ? 6978 : 6976;
    }
    case 0x1b40: { // rjmp .+4
        return 6982;
    }
    case 0x1b42: { // ldi r30, 0x5E
        s->r[30] = 94;
        return 6980;
    }
    case 0x1b44: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6982;
    }
    case 0x1b46: { // rcall .+3294
        s->calls[s->call_depth++] = 6984;
        return 10278;
    }
    case 0x1b48: { // ldi r30, 0x04
        s->r[30] = 4;
        return 6986;
    }
    case 0x1b4a: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 6988;
    }
    case 0x1b4c: { // sbrc r18, 4
        return (!!(s->r[18] & (1u << 4)) == 0) ? 6992 : 6990;
    }
    case 0x1b4e: { // rjmp .+6
        return 6998;
    }
    case 0x1b50: { // ldi r30, 0xB4
        s->r[30] = 180;
        return 6994;
    }
    case 0x1b52: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 6996;
    }
    case 0x1b54: { // rjmp .-102
        return 6896;
    }
    case 0x1b56: { // rcall .+3278
        s->calls[s->call_depth++] = 7000;
        return 10278;
    }
    case 0x1b58: { // ldi r30, 0x12
        s->r[30] = 18;
        return 7002;
    }
    case 0x1b5a: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 7004;
    }
    case 0x1b5c: { // rcall .+3272
        s->calls[s->call_depth++] = 7006;
        return 10278;
    }
    case 0x1b5e: { // ldi r30, 0x98
        s->r[30] = 152;
        return 7008;
    }
    case 0x1b60: { // ldi r31, 0x29
        s->r[31] = 41;
        return 7010;
    }
    case 0x1b62: { // sbrs r18, 7
        return (!!(s->r[18] & (1u << 7)) == 1) ? 7014 : 7012;
    }
    case 0x1b64: { // rjmp .+44
        return 7058;
    }
    case 0x1b66: { // ldi r30, 0x20
        s->r[30] = 32;
        return 7016;
    }
    case 0x1b68: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 7018;
    }
    case 0x1b6a: { // rcall .+3258
        s->calls[s->call_depth++] = 7020;
        return 10278;
    }
    case 0x1b6c: { // lds r20, 0x2158
        uint16_t address = 8536;
        s->r[20] = pm_read(s, address);
        return 7024;
    }
    case 0x1b70: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 7026;
    }
    case 0x1b72: { // sbrs r20, 2
        return (!!(s->r[20] & (1u << 2)) == 1) ? 7030 : 7028;
    }
    case 0x1b74: { // rjmp .+18
        return 7048;
    }
    case 0x1b76: { // ldi r30, 0x28
        s->r[30] = 40;
        return 7032;
    }
    case 0x1b78: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 7034;
    }
    case 0x1b7a: { // rcall .+3242
        s->calls[s->call_depth++] = 7036;
        return 10278;
    }
    case 0x1b7c: { // ldi r16, 0x30
        s->r[16] = 48;
        return 7038;
    }
    case 0x1b7e: { // add r16, r17
        s->r[16] = pm_add(s, s->r[16], s->r[17], 0);
        return 7040;
    }
    case 0x1b80: { // rcall .+3370
        s->calls[s->call_depth++] = 7042;
        return 10412;
    }
    case 0x1b82: { // ldi r30, 0x2E
        s->r[30] = 46;
        return 7044;
    }
    case 0x1b84: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 7046;
    }
    case 0x1b86: { // rcall .+3230
        s->calls[s->call_depth++] = 7048;
        return 10278;
    }
    case 0x1b88: { // cpi r17, 0x02
        pm_sub(s, s->r[17], 2, 0, false);
        return 7050;
    }
    case 0x1b8a: { // brcc .+86
        return (pm_getflag(s, 0) == 0) ? 7138 : 7052;
    }
    case 0x1b8c: { // inc r17
        s->r[17]++;
        pm_nzv(s, s->r[17], s->r[17] == 128);
        return 7054;
    }
    case 0x1b8e: { // lsr r20
        bool carry = s->r[20] & 1;
        s->r[20] = (s->r[20] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[20], !!(s->r[20] & 128) ^ carry);
        return 7056;
    }
    case 0x1b90: { // rjmp .-32
        return 7026;
    }
    case 0x1b92: { // rcall .+3218
        s->calls[s->call_depth++] = 7060;
        return 10278;
    }
    case 0x1b94: { // ldi r30, 0x96
        s->r[30] = 150;
        return 7062;
    }
    case 0x1b96: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 7064;
    }
    case 0x1b98: { // rcall .+3212
        s->calls[s->call_depth++] = 7066;
        return 10278;
    }
    case 0x1b9a: { // lds r20, 0x2441
        uint16_t address = 9281;
        s->r[20] = pm_read(s, address);
        return 7070;
    }
    case 0x1b9e: { // add r20, r20
        s->r[20] = pm_add(s, s->r[20], s->r[20], 0);
        return 7072;
    }
    case 0x1ba0: { // ldi r30, 0xA8
        s->r[30] = 168;
        return 7074;
    }
    case 0x1ba2: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 7076;
    }
    case 0x1ba4: { // eor r21, r21
        s->r[21] ^= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7078;
    }
    case 0x1ba6: { // add r30, r20
        s->r[30] = pm_add(s, s->r[30], s->r[20], 0);
        return 7080;
    }
    case 0x1ba8: { // adc r31, r21
        s->r[31] = pm_add(s, s->r[31], s->r[21], pm_getflag(s, CARRY));
        return 7082;
    }
    case 0x1baa: { // lpm r20, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[20] = pm_golden_flash[address];
        return 7084;
    }
    case 0x1bac: { // lpm r21, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[21] = pm_golden_flash[address];
        return 7086;
    }
    case 0x1bae: { // movw r30, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 30, pair);
        return 7088;
    }
    case 0x1bb0: { // rcall .+3188
        s->calls[s->call_depth++] = 7090;
        return 10278;
    }
    case 0x1bb2: { // ldi r30, 0x84
        s->r[30] = 132;
        return 7092;
    }
    case 0x1bb4: { // ldi r31, 0x29
        s->r[31] = 41;
        return 7094;
    }
    case 0x1bb6: { // rcall .+3182
        s->calls[s->call_depth++] = 7096;
        return 10278;
    }
    case 0x1bb8: { // lds r20, 0x2158
        uint16_t address = 8536;
        s->r[20] = pm_read(s, address);
        return 7100;
    }
    case 0x1bbc: { // ldi r30, 0x44
        s->r[30] = 68;
        return 7102;
    }
    case 0x1bbe: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 7104;
    }
    case 0x1bc0: { // rcall .+3172
        s->calls[s->call_depth++] = 7106;
        return 10278;
    }
    case 0x1bc2: { // ldi r30, 0x02
        s->r[30] = 2;
        return 7108;
    }
    case 0x1bc4: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 7110;
    }
    case 0x1bc6: { // sbrc r20, 0
        return (!!(s->r[20] & (1u << 0)) == 0) ? 7114 : 7112;
    }
    case 0x1bc8: { // rjmp .+4
        return 7118;
    }
    case 0x1bca: { // rcall .+3162
        s->calls[s->call_depth++] = 7116;
        return 10278;
    }
    case 0x1bcc: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1bce: { // ldi r30, 0xFC
        s->r[30] = 252;
        return 7120;
    }
    case 0x1bd0: { // ldi r31, 0x29
        s->r[31] = 41;
        return 7122;
    }
    case 0x1bd2: { // rcall .+3154
        s->calls[s->call_depth++] = 7124;
        return 10278;
    }
    case 0x1bd4: { // ldi r30, 0x4E
        s->r[30] = 78;
        return 7126;
    }
    case 0x1bd6: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 7128;
    }
    case 0x1bd8: { // sbrs r20, 1
        return (!!(s->r[20] & (1u << 1)) == 1) ? 7132 : 7130;
    }
    case 0x1bda: { // rjmp .+4
        return 7136;
    }
    case 0x1bdc: { // ldi r30, 0x5C
        s->r[30] = 92;
        return 7134;
    }
    case 0x1bde: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 7136;
    }
    case 0x1be0: { // rcall .+3140
        s->calls[s->call_depth++] = 7138;
        return 10278;
    }
    case 0x1be2: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
