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

/* Original CLI dispatch and programming sequence, byte range 0x12ea..0x1579.
 * C owns masks, constants, SRAM/MMIO access and internal jumps. Exact ASM
 * retains comparisons/flags, private calls, tails and CCP write encodings.
 * No independent functional validation of a rejected plain-C dispatcher
 * is recorded; acceptance is complete canonical FLASH equality. */
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
    asm volatile("cpi         r16,'C'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000984" : : : "memory", "cc" : L_000984);
    cli_get_next_char();
    asm volatile("cpi         r16,'A'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000981" : : : "memory", "cc" : L_000981);
    asm volatile("rjmp        alarms_clear" : : : "memory", "cc");
    __builtin_unreachable();
L_000981:
    asm volatile(".Lcli_L_000981:" : : : "memory");
    asm volatile("cpi         r16,'P'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009b1" : : : "memory", "cc" : L_0009b1);
    goto L_000a3c;
L_000984:
    asm volatile(".Lcli_L_000984:" : : : "memory");
    asm volatile("cpi         r16,'O'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_00098e" : : : "memory", "cc" : L_00098e);
    cli_get_next_char();
    asm volatile("cpi         r16,'N'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_00098b" : : : "memory", "cc" : L_00098b);
    goto L_0009eb;
L_00098b:
    asm volatile(".Lcli_L_00098b:" : : : "memory");
    asm volatile("cpi         r16,'F'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009b1" : : : "memory", "cc" : L_0009b1);
    goto L_000a39;
L_00098e:
    asm volatile(".Lcli_L_00098e:" : : : "memory");
    asm volatile("cpi         r16,'P'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_00099b" : : : "memory", "cc" : L_00099b);
    cli_get_next_char();
    asm volatile("cpi         r16,'A'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000995" : : : "memory", "cc" : L_000995);
    goto L_000a82;
L_000995:
    asm volatile(".Lcli_L_000995:" : : : "memory");
    asm volatile("cpi         r16,'C'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000998" : : : "memory", "cc" : L_000998);
    asm volatile("rjmp        cdce62005_rst" : : : "memory", "cc");
    __builtin_unreachable();
L_000998:
    asm volatile(".Lcli_L_000998:" : : : "memory");
    asm volatile("cpi         r16,'F'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009b1" : : : "memory", "cc" : L_0009b1);
    asm volatile("rjmp        fpga_firmware_update" : : : "memory", "cc");
    __builtin_unreachable();
L_00099b:
    asm volatile(".Lcli_L_00099b:" : : : "memory");
    asm volatile("cpi         r16,'R'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009b2" : : : "memory", "cc" : L_0009b2);
    cli_get_next_char();
    asm volatile("cpi         r16,'A'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009a2" : : : "memory", "cc" : L_0009a2);
    asm volatile("rjmp        cli_send_ch_mean_amplitude" : : : "memory", "cc");
    __builtin_unreachable();
L_0009a2:
    asm volatile(".Lcli_L_0009a2:" : : : "memory");
    asm volatile("cpi         r16,'C'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009a5" : : : "memory", "cc" : L_0009a5);
    asm volatile("rjmp        channels_read" : : : "memory", "cc");
    __builtin_unreachable();
L_0009a5:
    asm volatile(".Lcli_L_0009a5:" : : : "memory");
    asm volatile("cpi         r16,'S'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009a8" : : : "memory", "cc" : L_0009a8);
    asm volatile("rjmp        cli_send_system_status" : : : "memory", "cc");
    __builtin_unreachable();
L_0009a8:
    asm volatile(".Lcli_L_0009a8:" : : : "memory");
    asm volatile("cpi         r16,'F'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009ab" : : : "memory", "cc" : L_0009ab);
    asm volatile("rjmp        cli_send_channel_cdf_adc" : : : "memory", "cc");
    __builtin_unreachable();
L_0009ab:
    asm volatile(".Lcli_L_0009ab:" : : : "memory");
    asm volatile("cpi         r16,'T'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009ae" : : : "memory", "cc" : L_0009ae);
    asm volatile("rjmp        cli_send_tdc_data" : : : "memory", "cc");
    __builtin_unreachable();
L_0009ae:
    asm volatile(".Lcli_L_0009ae:" : : : "memory");
    asm volatile("cpi         r16,'Z'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009b1" : : : "memory", "cc" : L_0009b1);
    asm volatile("rjmp        cli_send_adc_baseline_dispersion" : : : "memory", "cc");
    __builtin_unreachable();
L_0009b1:
    asm volatile(".Lcli_L_0009b1:" : : : "memory");
    goto L_0009e4;
L_0009b2:
    asm volatile(".Lcli_L_0009b2:" : : : "memory");
    asm volatile("cpi         r16,'S'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009db" : : : "memory", "cc" : L_0009db);
    cli_get_next_char();
    asm volatile("cpi         r16,'C'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009c6" : : : "memory", "cc" : L_0009c6);
    cli_get_next_char();
    asm volatile("cpi         r16,'L'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009bd" : : : "memory", "cc" : L_0009bd);
    asm volatile("rjmp        fpga_set_threshold_calibration" : : : "memory", "cc");
    __builtin_unreachable();
L_0009bd:
    asm volatile(".Lcli_L_0009bd:" : : : "memory");
    asm volatile("cpi         r16,'S'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009c0" : : : "memory", "cc" : L_0009c0);
    asm volatile("rjmp        FUN_code_000f70" : : : "memory", "cc");
    __builtin_unreachable();
L_0009c0:
    asm volatile(".Lcli_L_0009c0:" : : : "memory");
    asm volatile("cpi         r16,'T'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009c3" : : : "memory", "cc" : L_0009c3);
    asm volatile("rjmp        fpga_set_tdc_values" : : : "memory", "cc");
    __builtin_unreachable();
L_0009c3:
    asm volatile(".Lcli_L_0009c3:" : : : "memory");
    asm volatile("cpi         r16,'R'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009e4" : : : "memory", "cc" : L_0009e4);
    asm volatile("rjmp        fpga_set_adc_range_corr" : : : "memory", "cc");
    __builtin_unreachable();
L_0009c6:
    asm volatile(".Lcli_L_0009c6:" : : : "memory");
    asm volatile("cpi         r16,'D'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009c9" : : : "memory", "cc" : L_0009c9);
    asm volatile("rjmp        fpga_set_ch_adc_delay" : : : "memory", "cc");
    __builtin_unreachable();
L_0009c9:
    asm volatile(".Lcli_L_0009c9:" : : : "memory");
    asm volatile("cpi         r16,'L'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009cc" : : : "memory", "cc" : L_0009cc);
    asm volatile("rjmp        fpga_set_ch_cfd_threshold" : : : "memory", "cc");
    __builtin_unreachable();
L_0009cc:
    asm volatile(".Lcli_L_0009cc:" : : : "memory");
    asm volatile("cpi         r16,'O'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009cf" : : : "memory", "cc" : L_0009cf);
    asm volatile("rjmp        fpga_set_adc_zero" : : : "memory", "cc");
    __builtin_unreachable();
L_0009cf:
    asm volatile(".Lcli_L_0009cf:" : : : "memory");
    asm volatile("cpi         r16,'S'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009d2" : : : "memory", "cc" : L_0009d2);
    asm volatile("rjmp        fpga_set_trg_charge_lvls" : : : "memory", "cc");
    __builtin_unreachable();
L_0009d2:
    asm volatile(".Lcli_L_0009d2:" : : : "memory");
    asm volatile("cpi         r16,'T'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009d5" : : : "memory", "cc" : L_0009d5);
    asm volatile("rjmp        fpga_set_trg_settings" : : : "memory", "cc");
    __builtin_unreachable();
L_0009d5:
    asm volatile(".Lcli_L_0009d5:" : : : "memory");
    asm volatile("cpi         r16,'V'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009d8" : : : "memory", "cc" : L_0009d8);
    goto L_000a6c;
L_0009d8:
    asm volatile(".Lcli_L_0009d8:" : : : "memory");
    asm volatile("cpi         r16,'Z'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009e4" : : : "memory", "cc" : L_0009e4);
    asm volatile("rjmp        fpga_set_ch_cfd_zero" : : : "memory", "cc");
    __builtin_unreachable();
L_0009db:
    asm volatile(".Lcli_L_0009db:" : : : "memory");
    asm volatile("cpi         r16,'W'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009e4" : : : "memory", "cc" : L_0009e4);
    cli_get_next_char();
    asm volatile("cpi         r16,'R'" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009e4" : : : "memory", "cc" : L_0009e4);
    asm volatile("rjmp        eeprom_settings_save" : : : "memory", "cc");
    __builtin_unreachable();
L_0009e2:
    asm volatile(".Lcli_L_0009e2:" : : : "memory");
    cli_get_next_char();
L_0009e4:
    asm volatile(".Lcli_L_0009e4:" : : : "memory");
    asm volatile("cpi         r16,0xd" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009e2" : : : "memory", "cc" : L_0009e2);
    r30 = 0x88;
    asm volatile("" : "+r" (r30));
    r31 = 0x29;
    asm volatile("" : "+r" (r31));
    cli_send_msg();
    asm volatile("ret" : : : "memory", "cc");
    __builtin_unreachable();
L_0009eb:
    asm volatile(".Lcli_L_0009eb:" : : : "memory");
    r20 = 0x1;
    asm volatile("" : "+r" (r20));
L_0009ec:
    asm volatile(".Lcli_L_0009ec:" : : : "memory");
    cli_get_next_char();
    asm volatile("cpi         r16,0xd" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_0009e4" : : : "memory", "cc" : L_0009e4);
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
    asm goto("sbrc        r16,0x7\n\trjmp .Lcli_L_0009f5" : : : "memory", "cc" : L_0009f5);
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
    asm goto("sbrc        r16,0x7\n\trjmp .Lcli_L_000a06" : : : "memory", "cc" : L_000a06);
    r28 = 0xd1;
    asm volatile("" : "+r" (r28));
    r29 = 0x10;
    asm volatile("" : "+r" (r29));
    asm volatile("st          Y,r20" : : : "memory", "cc");
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
    asm goto("sbrc        r16,0x7\n\trjmp .Lcli_L_000a1c" : : : "memory", "cc" : L_000a1c);
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
    asm goto("sbrc        r16,0x3\n\trjmp .Lcli_L_000a25" : : : "memory", "cc" : L_000a25);
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
    asm volatile("cpi         r16,0xd" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000a3b" : : : "memory", "cc" : L_000a3b);
    asm volatile("rcall       FUN_code_00108e" : : : "memory", "cc");
    asm goto("brbc 0, .Lcli_L_000a43" : : : "memory", "cc" : L_000a43);
    asm volatile("ret" : : : "memory", "cc");
    __builtin_unreachable();
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
    asm volatile("cpi         r16,0x20" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000a6b" : : : "memory", "cc" : L_000a6b);
    cli_get_hex();
    asm goto("brbs 0, .Lcli_L_000a6b" : : : "memory", "cc" : L_000a6b);
    asm volatile("cpi         r16,0xd" : : : "memory", "cc");
    asm goto("brbc 1, .Lcli_L_000a6b" : : : "memory", "cc" : L_000a6b);
    asm volatile("rcall       FUN_code_00108e" : : : "memory", "cc");
    asm goto("brbc 0, .Lcli_L_000a78" : : : "memory", "cc" : L_000a78);
    asm volatile("ret" : : : "memory", "cc");
    __builtin_unreachable();
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
