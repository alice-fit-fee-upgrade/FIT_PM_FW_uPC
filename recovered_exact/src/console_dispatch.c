#include <avr/io.h>
/* Private entry calls below intentionally have no GNU argument/result ABI.
 * Adjacent fixed-register setup/capture preserves each historical contract;
 * these declarations emit the original wide CALL, not a new C API. The whole
 * image check verifies CALL width, register moves and every saved frame. */
extern void cli_get_hex(void);
extern void cli_get_next_byte(void);
extern void cli_get_next_char(void);
extern void cli_send_msg(void);
extern void fpga_msg_read_t1(void);
#include "legacy_cpu.h"
#include <stdint.h>
register uint8_t *pm_dispatch_destination asm("r28");

/* Original CLI dispatch and programming sequence, byte range 0x12ea..0x1579.
 * C owns masks, constants, SRAM/MMIO access, internal jumps and35 of37
 * command-character predicates. Exact ASM retains the remaining comparisons,
 * live flags, private calls, tails and protected CCP write encodings.
 * No independent functional validation of a rejected plain-C dispatcher
 * is recorded; acceptance is complete canonical FLASH equality.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
void cli_prompt_parse(void)
{
    register uint8_t r3 asm("r3");
    register uint8_t r4 asm("r4");
    register uint8_t r5 asm("r5");
    register uint8_t r16 asm("r16");
    register uint8_t r17 asm("r17");
    register uint8_t r18 asm("r18");
    register uint8_t r20 asm("r20");
    register uint8_t r21 asm("r21");
    register uint8_t r22 asm("r22");
    register uint8_t r28 asm("r28");
    register uint8_t r29 asm("r29");
    register uint8_t r30 asm("r30");
    register uint8_t r31 asm("r31");
    asm volatile("" : "=r" (r16));
    r16 &= 0x7f;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x2005 = r16;
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'C') goto L_000984;
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'A') goto L_000981;
    asm volatile("rjmp        alarms_clear" : : : "memory", "cc");
    __builtin_unreachable();
L_000981:
    asm volatile(".Lcli_L_000981:" : : : "memory");
    /* Unvalidated C equivalent after capturing the original R16 byte:
     * if (r16 != 'P') goto L_0009b1;
     * Trials550/585/588 did not match the fixed binary/layout.
     * Preserve this comparison, its exact flags and original target.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("cpi         r16,'P'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009b1" : : : "memory", "cc" : L_0009b1);
    goto L_000a3c;
L_000984:
    asm volatile(".Lcli_L_000984:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'O') goto L_00098e;
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'N') goto L_00098b;
    goto L_0009eb;
L_00098b:
    asm volatile(".Lcli_L_00098b:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'F') goto L_0009b1;
    goto L_000a39;
L_00098e:
    asm volatile(".Lcli_L_00098e:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'P') goto L_00099b;
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'A') goto L_000995;
    goto L_000a82;
L_000995:
    asm volatile(".Lcli_L_000995:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'C') goto L_000998;
    asm volatile("rjmp        cdce62005_rst" : : : "memory", "cc");
    __builtin_unreachable();
L_000998:
    asm volatile(".Lcli_L_000998:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'F') goto L_0009b1;
    asm volatile("rjmp        fpga_firmware_update" : : : "memory", "cc");
    __builtin_unreachable();
L_00099b:
    asm volatile(".Lcli_L_00099b:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'R') goto L_0009b2;
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'A') goto L_0009a2;
    asm volatile("rjmp        cli_send_ch_mean_amplitude" : : : "memory", "cc");
    __builtin_unreachable();
L_0009a2:
    asm volatile(".Lcli_L_0009a2:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'C') goto L_0009a5;
    asm volatile("rjmp        channels_read" : : : "memory", "cc");
    __builtin_unreachable();
L_0009a5:
    asm volatile(".Lcli_L_0009a5:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'S') goto L_0009a8;
    asm volatile("rjmp        cli_send_system_status" : : : "memory", "cc");
    __builtin_unreachable();
L_0009a8:
    asm volatile(".Lcli_L_0009a8:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'F') goto L_0009ab;
    asm volatile("rjmp        cli_send_channel_cdf_adc" : : : "memory", "cc");
    __builtin_unreachable();
L_0009ab:
    asm volatile(".Lcli_L_0009ab:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'T') goto L_0009ae;
    asm volatile("rjmp        cli_send_tdc_data" : : : "memory", "cc");
    __builtin_unreachable();
L_0009ae:
    asm volatile(".Lcli_L_0009ae:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'Z') goto L_0009b1;
    asm volatile("rjmp        cli_send_adc_baseline_dispersion" : : : "memory", "cc");
    __builtin_unreachable();
L_0009b1:
    asm volatile(".Lcli_L_0009b1:" : : : "memory");
    goto L_0009e4;
L_0009b2:
    asm volatile(".Lcli_L_0009b2:" : : : "memory");
    /* Unvalidated C equivalent after capturing the original R16 byte:
     * if (r16 != 'S') goto L_0009db;
     * Trials565/587/589 did not match the fixed binary/layout.
     * Preserve this comparison, its exact flags and original target.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile("cpi         r16,'S'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009db" : : : "memory", "cc" : L_0009db);
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'C') goto L_0009c6;
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'L') goto L_0009bd;
    asm volatile("rjmp        fpga_set_threshold_calibration" : : : "memory", "cc");
    __builtin_unreachable();
L_0009bd:
    asm volatile(".Lcli_L_0009bd:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'S') goto L_0009c0;
    asm volatile("rjmp        FUN_code_000f70" : : : "memory", "cc");
    __builtin_unreachable();
L_0009c0:
    asm volatile(".Lcli_L_0009c0:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'T') goto L_0009c3;
    asm volatile("rjmp        fpga_set_tdc_values" : : : "memory", "cc");
    __builtin_unreachable();
L_0009c3:
    asm volatile(".Lcli_L_0009c3:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'R') goto L_0009e4;
    asm volatile("rjmp        fpga_set_adc_range_corr" : : : "memory", "cc");
    __builtin_unreachable();
L_0009c6:
    asm volatile(".Lcli_L_0009c6:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'D') goto L_0009c9;
    asm volatile("rjmp        fpga_set_ch_adc_delay" : : : "memory", "cc");
    __builtin_unreachable();
L_0009c9:
    asm volatile(".Lcli_L_0009c9:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'L') goto L_0009cc;
    asm volatile("rjmp        fpga_set_ch_cfd_threshold" : : : "memory", "cc");
    __builtin_unreachable();
L_0009cc:
    asm volatile(".Lcli_L_0009cc:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'O') goto L_0009cf;
    asm volatile("rjmp        fpga_set_adc_zero" : : : "memory", "cc");
    __builtin_unreachable();
L_0009cf:
    asm volatile(".Lcli_L_0009cf:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'S') goto L_0009d2;
    asm volatile("rjmp        fpga_set_trg_charge_lvls" : : : "memory", "cc");
    __builtin_unreachable();
L_0009d2:
    asm volatile(".Lcli_L_0009d2:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'T') goto L_0009d5;
    asm volatile("rjmp        fpga_set_trg_settings" : : : "memory", "cc");
    __builtin_unreachable();
L_0009d5:
    asm volatile(".Lcli_L_0009d5:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'V') goto L_0009d8;
    goto L_000a6c;
L_0009d8:
    asm volatile(".Lcli_L_0009d8:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'Z') goto L_0009e4;
    asm volatile("rjmp        fpga_set_ch_cfd_zero" : : : "memory", "cc");
    __builtin_unreachable();
L_0009db:
    asm volatile(".Lcli_L_0009db:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'W') goto L_0009e4;
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 'R') goto L_0009e4;
    asm volatile("rjmp        eeprom_settings_save" : : : "memory", "cc");
    __builtin_unreachable();
L_0009e2:
    asm volatile(".Lcli_L_0009e2:" : : : "memory");
    cli_get_next_char();
L_0009e4:
    asm volatile(".Lcli_L_0009e4:" : : : "memory");
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 0xd) goto L_0009e2;
    r30 = 0x88;
    asm volatile("" : "+r" (r30));
    r31 = 0x29;
    asm volatile("" : "+r" (r31));
    cli_send_msg();
    /* Combined trial429 changed layout; individually matched C returns
     * with the compiler boundary in steps504-506 preserve the exact RET. */
    asm volatile("" : : : "memory", "cc");
    return;
L_0009eb:
    asm volatile(".Lcli_L_0009eb:" : : : "memory");
    r20 = 0x1;
    asm volatile("" : "+r" (r20));
L_0009ec:
    asm volatile(".Lcli_L_0009ec:" : : : "memory");
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 0xd) goto L_0009e4;
    r16 = *(volatile uint8_t *)0x2234;
    asm volatile("" : "+r" (r16));
    asm volatile("cp          r16,r20" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009f5" : : : "memory", "cc" : L_0009f5);
    asm volatile("rjmp        LAB_code_000ff1" : : : "memory", "cc");
    __builtin_unreachable();
L_0009f5:
    asm volatile(".Lcli_L_0009f5:" : : : "memory");
    asm volatile("" : "=r" (r20));
    *(volatile uint8_t *)0x2234 = r20;
    r16 = *(volatile uint8_t *)0x1cf;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    if ((r16 & (1u << 7))) goto L_0009f5;
    r16 = 0x36;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x1ca = r16;
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    r17 = 0xd8;
    asm volatile("" : "+r" (r17));
    pm_cpu_disable_irq();
    asm volatile("sts         52,r17" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x1cb = r16;
    pm_cpu_enable_irq();
L_000a06:
    asm volatile(".Lcli_L_000a06:" : : : "memory");
    r16 = *(volatile uint8_t *)0x1cf;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    if ((r16 & (1u << 7))) goto L_000a06;
    r28 = 0xd1;
    asm volatile("" : "+r" (r28));
    r29 = 0x10;
    asm volatile("" : "+r" (r29));
    {
      register uint8_t byte asm("r20");
      asm volatile("" : "=r" (pm_dispatch_destination), "=r" (byte) : : "memory");
      *pm_dispatch_destination = byte; asm volatile("" : : : "memory"); }
    r16 = 0x35;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x1ca = r16;
    asm volatile("" : "=r" (r28));
    *(volatile uint8_t *)0x1c0 = r28;
    asm volatile("" : "=r" (r29));
    *(volatile uint8_t *)0x1c1 = r29;
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    r17 = 0xd8;
    asm volatile("" : "+r" (r17));
    pm_cpu_disable_irq();
    asm volatile("sts         52,r17" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x1cb = r16;
    pm_cpu_enable_irq();
L_000a1c:
    asm volatile(".Lcli_L_000a1c:" : : : "memory");
    r16 = *(volatile uint8_t *)0x1cf;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    if ((r16 & (1u << 7))) goto L_000a1c;
    asm volatile("and         r20,r20" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000a38" : : : "memory", "cc" : L_000a38);
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x686 = r16;
L_000a25:
    asm volatile(".Lcli_L_000a25:" : : : "memory");
    r16 = *(volatile uint8_t *)0x2157;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    if ((r16 & (1u << 3))) goto L_000a25;
    pm_cpu_disable_irq();
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x215b = r16;
    r16 = 0xd0;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x215c = r16;
    r16 = 0x7;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x215d = r16;
    GPIOR0 &= (uint8_t)~(1u << 1);
    r16 = 0x41;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x605 = r16;
    pm_cpu_enable_irq();
L_000a38:
    asm volatile(".Lcli_L_000a38:" : : : "memory");
    asm volatile("rjmp        LAB_code_000ff1" : : : "memory", "cc");
    __builtin_unreachable();
L_000a39:
    asm volatile(".Lcli_L_000a39:" : : : "memory");
    asm volatile("eor         r20,r20" : : : "memory", "cc");
    goto L_0009ec;
L_000a3b:
    asm volatile(".Lcli_L_000a3b:" : : : "memory");
    goto L_0009e4;
L_000a3c:
    asm volatile(".Lcli_L_000a3c:" : : : "memory");
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 0xd) goto L_000a3b;
    asm volatile("rcall       FUN_code_00108e" : : : "memory", "cc");
    asm goto("brbc 0, .Lcli_L_000a43" : : : "memory", "cc" : L_000a43);
    /* Combined trial429 changed layout; individually matched C returns
     * with the compiler boundary in steps504-506 preserve the exact RET. */
    asm volatile("" : : : "memory", "cc");
    return;
L_000a43:
    asm volatile(".Lcli_L_000a43:" : : : "memory");
    asm volatile("eor         r18,r18" : : : "memory", "cc");
    pm_cpu_disable_irq();
    fpga_msg_read_t1();
    pm_cpu_enable_irq();
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x222f = r16;
    r28 = 0xb7;
    asm volatile("" : "+r" (r28));
    r29 = 0x21;
    asm volatile("" : "+r" (r29));
L_000a4c:
    asm volatile(".Lcli_L_000a4c:" : : : "memory");
    asm volatile("inc         r18" : : : "memory", "cc");
    pm_cpu_disable_irq();
    fpga_msg_read_t1();
    pm_cpu_enable_irq();
    asm volatile("st          Y+,r16" : : : "memory", "cc");
    asm volatile("st          Y+,r17" : : : "memory", "cc");
    asm volatile("cpi         r18,0xc" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000a4c" : : : "memory", "cc" : L_000a4c);
    r18 = 0x24;
    asm volatile("" : "+r" (r18));
    r28 = 0x87;
    asm volatile("" : "+r" (r28));
    r29 = 0x21;
    asm volatile("" : "+r" (r29));
L_000a58:
    asm volatile(".Lcli_L_000a58:" : : : "memory");
    asm volatile("inc         r18" : : : "memory", "cc");
    pm_cpu_disable_irq();
    fpga_msg_read_t1();
    pm_cpu_enable_irq();
    asm volatile("st          Y+,r16" : : : "memory", "cc");
    asm volatile("st          Y+,r17" : : : "memory", "cc");
    asm volatile("cpi         r18,0x3c" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000a58" : : : "memory", "cc" : L_000a58);
    asm volatile("inc         r18" : : : "memory", "cc");
    pm_cpu_disable_irq();
    fpga_msg_read_t1();
    pm_cpu_enable_irq();
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x2230 = r16;
    asm volatile("" : "=r" (r17));
    *(volatile uint8_t *)0x2231 = r17;
    asm volatile("rjmp        LAB_code_000ff1" : : : "memory", "cc");
    __builtin_unreachable();
L_000a6b:
    asm volatile(".Lcli_L_000a6b:" : : : "memory");
    goto L_0009e4;
L_000a6c:
    asm volatile(".Lcli_L_000a6c:" : : : "memory");
    cli_get_next_char();
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 0x20) goto L_000a6b;
    cli_get_hex();
    asm goto("brbs 0, .Lcli_L_000a6b" : : : "memory", "cc" : L_000a6b);
    asm volatile("" : "=r" (r16) : : "memory");
    if (r16 != 0xd) goto L_000a6b;
    asm volatile("rcall       FUN_code_00108e" : : : "memory", "cc");
    asm goto("brbc 0, .Lcli_L_000a78" : : : "memory", "cc" : L_000a78);
    /* Combined trial429 changed layout; individually matched C returns
     * with the compiler boundary in steps504-506 preserve the exact RET. */
    asm volatile("" : : : "memory", "cc");
    return;
L_000a78:
    asm volatile(".Lcli_L_000a78:" : : : "memory");
    asm volatile("" : "=r" (r20));
    *(volatile uint8_t *)0x2232 = r20;
    asm volatile("" : "=r" (r21));
    *(volatile uint8_t *)0x2233 = r21;
    r18 = 0xbd;
    asm volatile("" : "+r" (r18));
    {
        register uint16_t copied_word asm("r20");
        register uint16_t destination_word asm("r16");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    pm_cpu_disable_irq();
    asm volatile("rcall       fpga_msg_send_t2" : : : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("rjmp        LAB_code_000ff1" : : : "memory", "cc");
    __builtin_unreachable();
L_000a82:
    asm volatile(".Lcli_L_000a82:" : : : "memory");
    asm volatile("rcall       unlock_programming" : : : "memory", "cc");
    asm goto("brbs 0, .Lcli_L_000a6b" : : : "memory", "cc" : L_000a6b);
    cli_get_next_byte();
    asm volatile("" : "=r" (r16));
    r30 = r16;
    asm volatile("" : "+r" (r30));
    asm volatile("" : "=r" (r16));
    r20 = r16;
    asm volatile("" : "+r" (r20));
    cli_get_next_byte();
    asm volatile("" : "=r" (r16));
    r31 = r16;
    asm volatile("" : "+r" (r31));
    asm volatile("" : "=r" (r16));
    r21 = r16;
    asm volatile("" : "+r" (r21));
    cli_get_next_byte();
    asm volatile("" : "=r" (r16));
    r22 = r16;
    asm volatile("" : "+r" (r22));
    cli_get_next_byte();
    asm volatile("" : "=r" (r16));
    r3 = r16;
    asm volatile("" : "+r" (r3));
    cli_get_next_byte();
    asm volatile("" : "=r" (r16));
    r4 = r16;
    asm volatile("" : "+r" (r4));
    cli_get_next_byte();
    asm volatile("" : "=r" (r16));
    r5 = r16;
    asm volatile("" : "+r" (r5));
    asm volatile("cpi         r22,0x2" : : : "memory", "cc");
    asm goto("brbc 4, .Lcli_L_000a6b" : : : "memory", "cc" : L_000a6b);
    asm volatile("cp          r3,r30" : : : "memory", "cc");
    asm volatile("cpc         r4,r31" : : : "memory", "cc");
    asm volatile("cpc         r5,r22" : : : "memory", "cc");
    asm goto("brbs 0, .Lcli_L_000a6b" : : : "memory", "cc" : L_000a6b);
    pm_cpu_disable_irq();
    asm volatile("sts         59,r22" : : : "memory", "cc");
    asm volatile("rcall       system_deinit" : : : "memory", "cc");
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x689 = r16;
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x806 = r16;
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x686 = r16;
    r16 = 0x43;
    asm volatile("" : "+r" (r16));
    r17 = 0xd8;
    asm volatile("" : "+r" (r17));
    asm volatile("sts         52,r17" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0xa2 = r16;
    r16 = 0x26;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x1ca = r16;
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    r17 = 0xd8;
    asm volatile("" : "+r" (r17));
    pm_cpu_disable_irq();
    asm volatile("sts         52,r17" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x1cb = r16;
    pm_cpu_enable_irq();
    asm volatile("jmp         boot_020308" : : : "memory", "cc");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 4304 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_console_dispatch(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x12ea: { // andi r16, 0x7F
        s->r[16] &= 127;
        pm_nzv(s, s->r[16], false);
        return 4844;
    }
    case 0x12ec: { // sts 0x2005, r16
        uint16_t address = 8197;
        pm_write(s, address, s->r[16]);
        return 4848;
    }
    case 0x12f0: { // call 0x283c
        s->calls[s->call_depth++] = 4852;
        return 10300;
    }
    case 0x12f4: { // cpi r16, 0x43
        pm_sub(s, s->r[16], 67, 0, false);
        return 4854;
    }
    case 0x12f6: { // brne .+16
        return (pm_getflag(s, 1) == 0) ? 4872 : 4856;
    }
    case 0x12f8: { // call 0x283c
        s->calls[s->call_depth++] = 4860;
        return 10300;
    }
    case 0x12fc: { // cpi r16, 0x41
        pm_sub(s, s->r[16], 65, 0, false);
        return 4862;
    }
    case 0x12fe: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4866 : 4864;
    }
    case 0x1300: { // rjmp .+2274
        return 7140;
    }
    case 0x1302: { // cpi r16, 0x50
        pm_sub(s, s->r[16], 80, 0, false);
        return 4868;
    }
    case 0x1304: { // brne .+92
        return (pm_getflag(s, 1) == 0) ? 4962 : 4870;
    }
    case 0x1306: { // rjmp .+368
        return 5240;
    }
    case 0x1308: { // cpi r16, 0x4F
        pm_sub(s, s->r[16], 79, 0, false);
        return 4874;
    }
    case 0x130a: { // brne .+16
        return (pm_getflag(s, 1) == 0) ? 4892 : 4876;
    }
    case 0x130c: { // call 0x283c
        s->calls[s->call_depth++] = 4880;
        return 10300;
    }
    case 0x1310: { // cpi r16, 0x4E
        pm_sub(s, s->r[16], 78, 0, false);
        return 4882;
    }
    case 0x1312: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4886 : 4884;
    }
    case 0x1314: { // rjmp .+192
        return 5078;
    }
    case 0x1316: { // cpi r16, 0x46
        pm_sub(s, s->r[16], 70, 0, false);
        return 4888;
    }
    case 0x1318: { // brne .+72
        return (pm_getflag(s, 1) == 0) ? 4962 : 4890;
    }
    case 0x131a: { // rjmp .+342
        return 5234;
    }
    case 0x131c: { // cpi r16, 0x50
        pm_sub(s, s->r[16], 80, 0, false);
        return 4894;
    }
    case 0x131e: { // brne .+22
        return (pm_getflag(s, 1) == 0) ? 4918 : 4896;
    }
    case 0x1320: { // call 0x283c
        s->calls[s->call_depth++] = 4900;
        return 10300;
    }
    case 0x1324: { // cpi r16, 0x41
        pm_sub(s, s->r[16], 65, 0, false);
        return 4902;
    }
    case 0x1326: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4906 : 4904;
    }
    case 0x1328: { // rjmp .+474
        return 5380;
    }
    case 0x132a: { // cpi r16, 0x43
        pm_sub(s, s->r[16], 67, 0, false);
        return 4908;
    }
    case 0x132c: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4912 : 4910;
    }
    case 0x132e: { // rjmp .+1806
        return 6718;
    }
    case 0x1330: { // cpi r16, 0x46
        pm_sub(s, s->r[16], 70, 0, false);
        return 4914;
    }
    case 0x1332: { // brne .+46
        return (pm_getflag(s, 1) == 0) ? 4962 : 4916;
    }
    case 0x1334: { // rjmp .+624
        return 5542;
    }
    case 0x1336: { // cpi r16, 0x52
        pm_sub(s, s->r[16], 82, 0, false);
        return 4920;
    }
    case 0x1338: { // brne .+42
        return (pm_getflag(s, 1) == 0) ? 4964 : 4922;
    }
    case 0x133a: { // call 0x283c
        s->calls[s->call_depth++] = 4926;
        return 10300;
    }
    case 0x133e: { // cpi r16, 0x41
        pm_sub(s, s->r[16], 65, 0, false);
        return 4928;
    }
    case 0x1340: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4932 : 4930;
    }
    case 0x1342: { // rjmp .+1354
        return 6286;
    }
    case 0x1344: { // cpi r16, 0x43
        pm_sub(s, s->r[16], 67, 0, false);
        return 4934;
    }
    case 0x1346: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4938 : 4936;
    }
    case 0x1348: { // rjmp .+2328
        return 7266;
    }
    case 0x134a: { // cpi r16, 0x53
        pm_sub(s, s->r[16], 83, 0, false);
        return 4940;
    }
    case 0x134c: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4944 : 4942;
    }
    case 0x134e: { // rjmp .+1812
        return 6756;
    }
    case 0x1350: { // cpi r16, 0x46
        pm_sub(s, s->r[16], 70, 0, false);
        return 4946;
    }
    case 0x1352: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4950 : 4948;
    }
    case 0x1354: { // rjmp .+2460
        return 7410;
    }
    case 0x1356: { // cpi r16, 0x54
        pm_sub(s, s->r[16], 84, 0, false);
        return 4952;
    }
    case 0x1358: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4956 : 4954;
    }
    case 0x135a: { // rjmp .+1460
        return 6416;
    }
    case 0x135c: { // cpi r16, 0x5A
        pm_sub(s, s->r[16], 90, 0, false);
        return 4958;
    }
    case 0x135e: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4962 : 4960;
    }
    case 0x1360: { // rjmp .+1372
        return 6334;
    }
    case 0x1362: { // rjmp .+100
        return 5064;
    }
    case 0x1364: { // cpi r16, 0x53
        pm_sub(s, s->r[16], 83, 0, false);
        return 4966;
    }
    case 0x1366: { // brne .+78
        return (pm_getflag(s, 1) == 0) ? 5046 : 4968;
    }
    case 0x1368: { // call 0x283c
        s->calls[s->call_depth++] = 4972;
        return 10300;
    }
    case 0x136c: { // cpi r16, 0x43
        pm_sub(s, s->r[16], 67, 0, false);
        return 4974;
    }
    case 0x136e: { // brne .+28
        return (pm_getflag(s, 1) == 0) ? 5004 : 4976;
    }
    case 0x1370: { // call 0x283c
        s->calls[s->call_depth++] = 4980;
        return 10300;
    }
    case 0x1374: { // cpi r16, 0x4C
        pm_sub(s, s->r[16], 76, 0, false);
        return 4982;
    }
    case 0x1376: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4986 : 4984;
    }
    case 0x1378: { // rjmp .+2818
        return 7804;
    }
    case 0x137a: { // cpi r16, 0x53
        pm_sub(s, s->r[16], 83, 0, false);
        return 4988;
    }
    case 0x137c: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4992 : 4990;
    }
    case 0x137e: { // rjmp .+2912
        return 7904;
    }
    case 0x1380: { // cpi r16, 0x54
        pm_sub(s, s->r[16], 84, 0, false);
        return 4994;
    }
    case 0x1382: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4998 : 4996;
    }
    case 0x1384: { // rjmp .+2728
        return 7726;
    }
    case 0x1386: { // cpi r16, 0x52
        pm_sub(s, s->r[16], 82, 0, false);
        return 5000;
    }
    case 0x1388: { // brne .+62
        return (pm_getflag(s, 1) == 0) ? 5064 : 5002;
    }
    case 0x138a: { // rjmp .+2602
        return 7606;
    }
    case 0x138c: { // cpi r16, 0x44
        pm_sub(s, s->r[16], 68, 0, false);
        return 5006;
    }
    case 0x138e: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 5010 : 5008;
    }
    case 0x1390: { // rjmp .+2984
        return 7994;
    }
    case 0x1392: { // cpi r16, 0x4C
        pm_sub(s, s->r[16], 76, 0, false);
        return 5012;
    }
    case 0x1394: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 5016 : 5014;
    }
    case 0x1396: { // rjmp .+3062
        return 8078;
    }
    case 0x1398: { // cpi r16, 0x4F
        pm_sub(s, s->r[16], 79, 0, false);
        return 5018;
    }
    case 0x139a: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 5022 : 5020;
    }
    case 0x139c: { // rjmp .+3150
        return 8172;
    }
    case 0x139e: { // cpi r16, 0x53
        pm_sub(s, s->r[16], 83, 0, false);
        return 5024;
    }
    case 0x13a0: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 5028 : 5026;
    }
    case 0x13a2: { // rjmp .+2494
        return 7522;
    }
    case 0x13a4: { // cpi r16, 0x54
        pm_sub(s, s->r[16], 84, 0, false);
        return 5030;
    }
    case 0x13a6: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 5034 : 5032;
    }
    case 0x13a8: { // rjmp .+2536
        return 7570;
    }
    case 0x13aa: { // cpi r16, 0x56
        pm_sub(s, s->r[16], 86, 0, false);
        return 5036;
    }
    case 0x13ac: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 5040 : 5038;
    }
    case 0x13ae: { // rjmp .+296
        return 5336;
    }
    case 0x13b0: { // cpi r16, 0x5A
        pm_sub(s, s->r[16], 90, 0, false);
        return 5042;
    }
    case 0x13b2: { // brne .+20
        return (pm_getflag(s, 1) == 0) ? 5064 : 5044;
    }
    case 0x13b4: { // rjmp .+3224
        return 8270;
    }
    case 0x13b6: { // cpi r16, 0x57
        pm_sub(s, s->r[16], 87, 0, false);
        return 5048;
    }
    case 0x13b8: { // brne .+14
        return (pm_getflag(s, 1) == 0) ? 5064 : 5050;
    }
    case 0x13ba: { // call 0x283c
        s->calls[s->call_depth++] = 5054;
        return 10300;
    }
    case 0x13be: { // cpi r16, 0x52
        pm_sub(s, s->r[16], 82, 0, false);
        return 5056;
    }
    case 0x13c0: { // brne .+6
        return (pm_getflag(s, 1) == 0) ? 5064 : 5058;
    }
    case 0x13c2: { // rjmp .+1514
        return 6574;
    }
    case 0x13c4: { // call 0x283c
        s->calls[s->call_depth++] = 5064;
        return 10300;
    }
    case 0x13c8: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 5066;
    }
    case 0x13ca: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 5060 : 5068;
    }
    case 0x13cc: { // ldi r30, 0x88
        s->r[30] = 136;
        return 5070;
    }
    case 0x13ce: { // ldi r31, 0x29
        s->r[31] = 41;
        return 5072;
    }
    case 0x13d0: { // call 0x2826
        s->calls[s->call_depth++] = 5076;
        return 10278;
    }
    case 0x13d4: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x13d6: { // ldi r20, 0x01
        s->r[20] = 1;
        return 5080;
    }
    case 0x13d8: { // call 0x283c
        s->calls[s->call_depth++] = 5084;
        return 10300;
    }
    case 0x13dc: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 5086;
    }
    case 0x13de: { // brne .-24
        return (pm_getflag(s, 1) == 0) ? 5064 : 5088;
    }
    case 0x13e0: { // lds r16, 0x2234
        uint16_t address = 8756;
        s->r[16] = pm_read(s, address);
        return 5092;
    }
    case 0x13e4: { // cp r16, r20
        pm_sub(s, s->r[16], s->r[20], 0, false);
        return 5094;
    }
    case 0x13e6: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 5098 : 5096;
    }
    case 0x13e8: { // rjmp .+3064
        return 8162;
    }
    case 0x13ea: { // sts 0x2234, r20
        uint16_t address = 8756;
        pm_write(s, address, s->r[20]);
        return 5102;
    }
    case 0x13ee: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 5106;
    }
    case 0x13f2: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 5110 : 5108;
    }
    case 0x13f4: { // rjmp .-12
        return 5098;
    }
    case 0x13f6: { // ldi r16, 0x36
        s->r[16] = 54;
        return 5112;
    }
    case 0x13f8: { // sts 0x01CA, r16
        uint16_t address = 458;
        pm_write(s, address, s->r[16]);
        return 5116;
    }
    case 0x13fc: { // ldi r16, 0x01
        s->r[16] = 1;
        return 5118;
    }
    case 0x13fe: { // ldi r17, 0xD8
        s->r[17] = 216;
        return 5120;
    }
    case 0x1400: { // cli
        pm_irq(s, false);
        return 5122;
    }
    case 0x1402: { // sts 0x0034, r17
        uint16_t address = 52;
        pm_write(s, address, s->r[17]);
        return 5126;
    }
    case 0x1406: { // sts 0x01CB, r16
        uint16_t address = 459;
        pm_write(s, address, s->r[16]);
        return 5130;
    }
    case 0x140a: { // sei
        pm_irq(s, true);
        return 5132;
    }
    case 0x140c: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 5136;
    }
    case 0x1410: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 5140 : 5138;
    }
    case 0x1412: { // rjmp .-8
        return 5132;
    }
    case 0x1414: { // ldi r28, 0xD1
        s->r[28] = 209;
        return 5142;
    }
    case 0x1416: { // ldi r29, 0x10
        s->r[29] = 16;
        return 5144;
    }
    case 0x1418: { // st Y, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[20]);
        return 5146;
    }
    case 0x141a: { // ldi r16, 0x35
        s->r[16] = 53;
        return 5148;
    }
    case 0x141c: { // sts 0x01CA, r16
        uint16_t address = 458;
        pm_write(s, address, s->r[16]);
        return 5152;
    }
    case 0x1420: { // sts 0x01C0, r28
        uint16_t address = 448;
        pm_write(s, address, s->r[28]);
        return 5156;
    }
    case 0x1424: { // sts 0x01C1, r29
        uint16_t address = 449;
        pm_write(s, address, s->r[29]);
        return 5160;
    }
    case 0x1428: { // ldi r16, 0x01
        s->r[16] = 1;
        return 5162;
    }
    case 0x142a: { // ldi r17, 0xD8
        s->r[17] = 216;
        return 5164;
    }
    case 0x142c: { // cli
        pm_irq(s, false);
        return 5166;
    }
    case 0x142e: { // sts 0x0034, r17
        uint16_t address = 52;
        pm_write(s, address, s->r[17]);
        return 5170;
    }
    case 0x1432: { // sts 0x01CB, r16
        uint16_t address = 459;
        pm_write(s, address, s->r[16]);
        return 5174;
    }
    case 0x1436: { // sei
        pm_irq(s, true);
        return 5176;
    }
    case 0x1438: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 5180;
    }
    case 0x143c: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 5184 : 5182;
    }
    case 0x143e: { // rjmp .-8
        return 5176;
    }
    case 0x1440: { // and r20, r20
        s->r[20] &= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 5186;
    }
    case 0x1442: { // brne .+44
        return (pm_getflag(s, 1) == 0) ? 5232 : 5188;
    }
    case 0x1444: { // ldi r16, 0x04
        s->r[16] = 4;
        return 5190;
    }
    case 0x1446: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 5194;
    }
    case 0x144a: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 5198;
    }
    case 0x144e: { // sbrc r16, 3
        return (!!(s->r[16] & (1u << 3)) == 0) ? 5202 : 5200;
    }
    case 0x1450: { // rjmp .-8
        return 5194;
    }
    case 0x1452: { // cli
        pm_irq(s, false);
        return 5204;
    }
    case 0x1454: { // ldi r16, 0x01
        s->r[16] = 1;
        return 5206;
    }
    case 0x1456: { // sts 0x215B, r16
        uint16_t address = 8539;
        pm_write(s, address, s->r[16]);
        return 5210;
    }
    case 0x145a: { // ldi r16, 0xD0
        s->r[16] = 208;
        return 5212;
    }
    case 0x145c: { // sts 0x215C, r16
        uint16_t address = 8540;
        pm_write(s, address, s->r[16]);
        return 5216;
    }
    case 0x1460: { // ldi r16, 0x07
        s->r[16] = 7;
        return 5218;
    }
    case 0x1462: { // sts 0x215D, r16
        uint16_t address = 8541;
        pm_write(s, address, s->r[16]);
        return 5222;
    }
    case 0x1466: { // cbi 0x00, 1
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 1));
        return 5224;
    }
    case 0x1468: { // ldi r16, 0x41
        s->r[16] = 65;
        return 5226;
    }
    case 0x146a: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 5230;
    }
    case 0x146e: { // sei
        pm_irq(s, true);
        return 5232;
    }
    case 0x1470: { // rjmp .+2928
        return 8162;
    }
    case 0x1472: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 5236;
    }
    case 0x1474: { // rjmp .-158
        return 5080;
    }
    case 0x1476: { // rjmp .-176
        return 5064;
    }
    case 0x1478: { // call 0x283c
        s->calls[s->call_depth++] = 5244;
        return 10300;
    }
    case 0x147c: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 5246;
    }
    case 0x147e: { // brne .-10
        return (pm_getflag(s, 1) == 0) ? 5238 : 5248;
    }
    case 0x1480: { // rcall .+3226
        s->calls[s->call_depth++] = 5250;
        return 8476;
    }
    case 0x1482: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 5254 : 5252;
    }
    case 0x1484: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1486: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 5256;
    }
    case 0x1488: { // cli
        pm_irq(s, false);
        return 5258;
    }
    case 0x148a: { // call 0x2368
        s->calls[s->call_depth++] = 5262;
        return 9064;
    }
    case 0x148e: { // sei
        pm_irq(s, true);
        return 5264;
    }
    case 0x1490: { // sts 0x222F, r16
        uint16_t address = 8751;
        pm_write(s, address, s->r[16]);
        return 5268;
    }
    case 0x1494: { // ldi r28, 0xB7
        s->r[28] = 183;
        return 5270;
    }
    case 0x1496: { // ldi r29, 0x21
        s->r[29] = 33;
        return 5272;
    }
    case 0x1498: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 5274;
    }
    case 0x149a: { // cli
        pm_irq(s, false);
        return 5276;
    }
    case 0x149c: { // call 0x2368
        s->calls[s->call_depth++] = 5280;
        return 9064;
    }
    case 0x14a0: { // sei
        pm_irq(s, true);
        return 5282;
    }
    case 0x14a2: { // st Y+, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[16]);
        return 5284;
    }
    case 0x14a4: { // st Y+, r17
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[17]);
        return 5286;
    }
    case 0x14a6: { // cpi r18, 0x0C
        pm_sub(s, s->r[18], 12, 0, false);
        return 5288;
    }
    case 0x14a8: { // brne .-18
        return (pm_getflag(s, 1) == 0) ? 5272 : 5290;
    }
    case 0x14aa: { // ldi r18, 0x24
        s->r[18] = 36;
        return 5292;
    }
    case 0x14ac: { // ldi r28, 0x87
        s->r[28] = 135;
        return 5294;
    }
    case 0x14ae: { // ldi r29, 0x21
        s->r[29] = 33;
        return 5296;
    }
    case 0x14b0: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 5298;
    }
    case 0x14b2: { // cli
        pm_irq(s, false);
        return 5300;
    }
    case 0x14b4: { // call 0x2368
        s->calls[s->call_depth++] = 5304;
        return 9064;
    }
    case 0x14b8: { // sei
        pm_irq(s, true);
        return 5306;
    }
    case 0x14ba: { // st Y+, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[16]);
        return 5308;
    }
    case 0x14bc: { // st Y+, r17
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[17]);
        return 5310;
    }
    case 0x14be: { // cpi r18, 0x3C
        pm_sub(s, s->r[18], 60, 0, false);
        return 5312;
    }
    case 0x14c0: { // brne .-18
        return (pm_getflag(s, 1) == 0) ? 5296 : 5314;
    }
    case 0x14c2: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 5316;
    }
    case 0x14c4: { // cli
        pm_irq(s, false);
        return 5318;
    }
    case 0x14c6: { // call 0x2368
        s->calls[s->call_depth++] = 5322;
        return 9064;
    }
    case 0x14ca: { // sei
        pm_irq(s, true);
        return 5324;
    }
    case 0x14cc: { // sts 0x2230, r16
        uint16_t address = 8752;
        pm_write(s, address, s->r[16]);
        return 5328;
    }
    case 0x14d0: { // sts 0x2231, r17
        uint16_t address = 8753;
        pm_write(s, address, s->r[17]);
        return 5332;
    }
    case 0x14d4: { // rjmp .+2828
        return 8162;
    }
    case 0x14d6: { // rjmp .-272
        return 5064;
    }
    case 0x14d8: { // call 0x283c
        s->calls[s->call_depth++] = 5340;
        return 10300;
    }
    case 0x14dc: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 5342;
    }
    case 0x14de: { // brne .-10
        return (pm_getflag(s, 1) == 0) ? 5334 : 5344;
    }
    case 0x14e0: { // call 0x26ac
        s->calls[s->call_depth++] = 5348;
        return 9900;
    }
    case 0x14e4: { // brcs .-16
        return (pm_getflag(s, 0) == 1) ? 5334 : 5350;
    }
    case 0x14e6: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 5352;
    }
    case 0x14e8: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 5334 : 5354;
    }
    case 0x14ea: { // rcall .+3120
        s->calls[s->call_depth++] = 5356;
        return 8476;
    }
    case 0x14ec: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 5360 : 5358;
    }
    case 0x14ee: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x14f0: { // sts 0x2232, r20
        uint16_t address = 8754;
        pm_write(s, address, s->r[20]);
        return 5364;
    }
    case 0x14f4: { // sts 0x2233, r21
        uint16_t address = 8755;
        pm_write(s, address, s->r[21]);
        return 5368;
    }
    case 0x14f8: { // ldi r18, 0xBD
        s->r[18] = 189;
        return 5370;
    }
    case 0x14fa: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 5372;
    }
    case 0x14fc: { // cli
        pm_irq(s, false);
        return 5374;
    }
    case 0x14fe: { // rcall .+3598
        s->calls[s->call_depth++] = 5376;
        return 8974;
    }
    case 0x1500: { // sei
        pm_irq(s, true);
        return 5378;
    }
    case 0x1502: { // rjmp .+2782
        return 8162;
    }
    case 0x1504: { // rcall .+116
        s->calls[s->call_depth++] = 5382;
        return 5498;
    }
    case 0x1506: { // brcs .-50
        return (pm_getflag(s, 0) == 1) ? 5334 : 5384;
    }
    case 0x1508: { // call 0x2836
        s->calls[s->call_depth++] = 5388;
        return 10294;
    }
    case 0x150c: { // mov r30, r16
        s->r[30] = s->r[16];
        return 5390;
    }
    case 0x150e: { // mov r20, r16
        s->r[20] = s->r[16];
        return 5392;
    }
    case 0x1510: { // call 0x2836
        s->calls[s->call_depth++] = 5396;
        return 10294;
    }
    case 0x1514: { // mov r31, r16
        s->r[31] = s->r[16];
        return 5398;
    }
    case 0x1516: { // mov r21, r16
        s->r[21] = s->r[16];
        return 5400;
    }
    case 0x1518: { // call 0x2836
        s->calls[s->call_depth++] = 5404;
        return 10294;
    }
    case 0x151c: { // mov r22, r16
        s->r[22] = s->r[16];
        return 5406;
    }
    case 0x151e: { // call 0x2836
        s->calls[s->call_depth++] = 5410;
        return 10294;
    }
    case 0x1522: { // mov r3, r16
        s->r[3] = s->r[16];
        return 5412;
    }
    case 0x1524: { // call 0x2836
        s->calls[s->call_depth++] = 5416;
        return 10294;
    }
    case 0x1528: { // mov r4, r16
        s->r[4] = s->r[16];
        return 5418;
    }
    case 0x152a: { // call 0x2836
        s->calls[s->call_depth++] = 5422;
        return 10294;
    }
    case 0x152e: { // mov r5, r16
        s->r[5] = s->r[16];
        return 5424;
    }
    case 0x1530: { // cpi r22, 0x02
        pm_sub(s, s->r[22], 2, 0, false);
        return 5426;
    }
    case 0x1532: { // brge .-94
        return (pm_getflag(s, 4) == 0) ? 5334 : 5428;
    }
    case 0x1534: { // cp r3, r30
        pm_sub(s, s->r[3], s->r[30], 0, false);
        return 5430;
    }
    case 0x1536: { // cpc r4, r31
        pm_sub(s, s->r[4], s->r[31], pm_getflag(s, CARRY), true);
        return 5432;
    }
    case 0x1538: { // cpc r5, r22
        pm_sub(s, s->r[5], s->r[22], pm_getflag(s, CARRY), true);
        return 5434;
    }
    case 0x153a: { // brcs .-102
        return (pm_getflag(s, 0) == 1) ? 5334 : 5436;
    }
    case 0x153c: { // cli
        pm_irq(s, false);
        return 5438;
    }
    case 0x153e: { // sts 0x003B, r22
        uint16_t address = 59;
        pm_write(s, address, s->r[22]);
        return 5442;
    }
    case 0x1542: { // rcall .-2222
        s->calls[s->call_depth++] = 5444;
        return 3222;
    }
    case 0x1544: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 5446;
    }
    case 0x1546: { // sts 0x0689, r16
        uint16_t address = 1673;
        pm_write(s, address, s->r[16]);
        return 5450;
    }
    case 0x154a: { // sts 0x0806, r16
        uint16_t address = 2054;
        pm_write(s, address, s->r[16]);
        return 5454;
    }
    case 0x154e: { // ldi r16, 0x04
        s->r[16] = 4;
        return 5456;
    }
    case 0x1550: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 5460;
    }
    case 0x1554: { // ldi r16, 0x43
        s->r[16] = 67;
        return 5462;
    }
    case 0x1556: { // ldi r17, 0xD8
        s->r[17] = 216;
        return 5464;
    }
    case 0x1558: { // sts 0x0034, r17
        uint16_t address = 52;
        pm_write(s, address, s->r[17]);
        return 5468;
    }
    case 0x155c: { // sts 0x00A2, r16
        uint16_t address = 162;
        pm_write(s, address, s->r[16]);
        return 5472;
    }
    case 0x1560: { // ldi r16, 0x26
        s->r[16] = 38;
        return 5474;
    }
    case 0x1562: { // sts 0x01CA, r16
        uint16_t address = 458;
        pm_write(s, address, s->r[16]);
        return 5478;
    }
    case 0x1566: { // ldi r16, 0x01
        s->r[16] = 1;
        return 5480;
    }
    case 0x1568: { // ldi r17, 0xD8
        s->r[17] = 216;
        return 5482;
    }
    case 0x156a: { // cli
        pm_irq(s, false);
        return 5484;
    }
    case 0x156c: { // sts 0x0034, r17
        uint16_t address = 52;
        pm_write(s, address, s->r[17]);
        return 5488;
    }
    case 0x1570: { // sts 0x01CB, r16
        uint16_t address = 459;
        pm_write(s, address, s->r[16]);
        return 5492;
    }
    case 0x1574: { // sei
        pm_irq(s, true);
        return 5494;
    }
    case 0x1576: { // jmp 0x20308
        return 131848;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
