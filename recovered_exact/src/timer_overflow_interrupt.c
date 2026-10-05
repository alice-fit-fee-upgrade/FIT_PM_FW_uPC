#include <avr/io.h>
/* Private entry calls below intentionally have no GNU argument/result ABI.
 * Adjacent fixed-register setup/capture preserves each historical contract;
 * these declarations emit the original wide CALL, not a new C API. The whole
 * image check verifies CALL width, register moves and every saved frame. */
extern void adt7311_16bit_rw(void);
extern void dac_set_value(void);
extern void fpga_msg_read_t1(void);
extern void fpga_msg_send_t2(void);
extern void fpga_send_mcu_ts(void);
extern void ths788_read(void);
extern void ths788_write(void);
#include "legacy_cpu.h"
#include <stdint.h>
register uint16_t pm_timer_settings_cursor asm("r28");

/* Original timer overflow interrupt and original byte range 0x049e..0x08e3.
 * C owns constants, SRAM/MMIO access and internal jumps. Exact ASM
 * retains the ISR frame, flags and private peripheral/call contracts.
 * No independent functional validation of a rejected plain-C ISR
 * is recorded; acceptance is complete canonical FLASH equality.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
void TCC0_OVF_vect_isr(void)
{
    register uint8_t r0 asm("r0");
    register uint8_t r1 asm("r1");
    register uint8_t r16 asm("r16");
    register uint8_t r17 asm("r17");
    register uint8_t r18 asm("r18");
    register uint8_t r19 asm("r19");
    register uint8_t r20 asm("r20");
    register uint8_t r21 asm("r21");
    register uint8_t r22 asm("r22");
    register uint8_t r23 asm("r23");
    register uint8_t r24 asm("r24");
    register uint8_t r25 asm("r25");
    register uint8_t r28 asm("r28");
    register uint8_t r29 asm("r29");
    register uint8_t r30 asm("r30");
    register uint8_t r31 asm("r31");
    asm volatile("push        r31" : : : "memory", "cc");
    {
        register uint8_t saved_status asm("r31") = *(volatile uint8_t *)0x3f;
        asm volatile("" : "+r" (saved_status) : : "memory");
    }
    asm volatile("push        r31" : : : "memory", "cc");
    asm volatile("push        r30" : : : "memory", "cc");
    asm volatile("push        r29" : : : "memory", "cc");
    asm volatile("push        r28" : : : "memory", "cc");
    asm volatile("push        r25" : : : "memory", "cc");
    asm volatile("push        r24" : : : "memory", "cc");
    asm volatile("push        r23" : : : "memory", "cc");
    asm volatile("push        r22" : : : "memory", "cc");
    asm volatile("push        r21" : : : "memory", "cc");
    asm volatile("push        r20" : : : "memory", "cc");
    asm volatile("push        r19" : : : "memory", "cc");
    asm volatile("push        r18" : : : "memory", "cc");
    asm volatile("push        r17" : : : "memory", "cc");
    asm volatile("push        r16" : : : "memory", "cc");
    asm volatile("push        r1" : : : "memory", "cc");
    asm volatile("push        r0" : : : "memory", "cc");
    r30 = 0x59;
    asm volatile("" : "+r" (r30));
    r31 = 0x21;
    asm volatile("" : "+r" (r31));
    {
        register uint8_t *source asm("r30");
        asm volatile("" : "=z" (source) : : "memory");
        r16 = *source;
        asm volatile("" : "+r" (r16) : : "memory");
    }
    asm volatile("and         r16,r16" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_000267" : : : "memory", "cc" : L_000267);
    goto L_00031b;
L_000267:
    asm volatile(".Ltimer_L_000267:" : : : "memory");
    asm volatile("dec         r16" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_00026a" : : : "memory", "cc" : L_00026a);
    goto L_0002d5;
L_00026a:
    asm volatile(".Ltimer_L_00026a:" : : : "memory");
    asm volatile("dec         r16" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_0002ab" : : : "memory", "cc" : L_0002ab);
    asm volatile("dec         r16" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_000298" : : : "memory", "cc" : L_000298);
    asm volatile("dec         r16" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_00028c" : : : "memory", "cc" : L_00028c);
    r18 = 0x7f;
    asm volatile("" : "+r" (r18));
    pm_cpu_disable_irq();
    fpga_msg_read_t1();
    pm_cpu_enable_irq();
    asm volatile("" : "=r" (r16));
    r16 &= 0xf;
    asm volatile("" : "+r" (r16));
    asm volatile("cpi         r16,0xf" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_00027c" : : : "memory", "cc" : L_00027c);
    asm volatile("ldd         r17,Z+0x1" : : : "memory", "cc");
    asm volatile("dec         r17" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_0002b8" : : : "memory", "cc" : L_0002b8);
    goto L_00031a;
L_00027c:
    asm volatile(".Ltimer_L_00027c:" : : : "memory");
    asm volatile("rcall       fpga_settings_reset" : : : "memory", "cc");
    r16 = 0x2;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x68c = r16;
    r16 = 0x9;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x689 = r16;
    r18 = 0x7f;
    asm volatile("" : "+r" (r18));
    r16 = 0x0;
    asm volatile("" : "+r" (r16));
    r17 = 0x8;
    asm volatile("" : "+r" (r17));
    pm_cpu_disable_irq();
    fpga_msg_send_t2();
    pm_cpu_enable_irq();
    r16 = 0x80;
    asm volatile("" : "+r" (r16));
    goto L_0002c0;
L_00028c:
    asm volatile(".Ltimer_L_00028c:" : : : "memory");
    r16 = *(volatile uint8_t *)0x668;
    asm volatile("" : "+r" (r16));
    asm goto("sbrs        r16,0x3\n\trjmp .Ltimer_L_00031b" : : : "memory", "cc" : L_00031b);
    r16 = 0x80;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x625 = r16;
    r16 = 0x5;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    r16 = 0xfa;
    asm volatile("" : "+r" (r16));
    asm volatile("std         Z+0x1,r16" : : : "memory", "cc");
    goto L_00031b;
L_000298:
    asm volatile(".Ltimer_L_000298:" : : : "memory");
    asm volatile("eor         r20,r20" : : : "memory", "cc");
    asm volatile("eor         r19,r19" : : : "memory", "cc");
    pm_cpu_disable_irq();
L_00029b:
    asm volatile(".Ltimer_L_00029b:" : : : "memory");
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
L_00029c:
    asm volatile(".Ltimer_L_00029c:" : : : "memory");
    ths788_read();
    asm volatile("" : "=r" (r17), "=r" (r20));
    r20 |= r17;
    asm volatile("" : "+r" (r20));
    {
        register uint8_t scratch_r16 asm("r16");
        asm volatile("" : "=r" (scratch_r16) : : "memory");
        scratch_r16 -= 224;
        asm volatile("" : "+r" (scratch_r16) : : "memory");
    }
    asm volatile("cpi         r16,0xc4" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_00029c" : : : "memory", "cc" : L_00029c);
    asm volatile("inc         r19" : : : "memory", "cc");
    asm volatile("cpi         r19,0x3" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_00029b" : : : "memory", "cc" : L_00029b);
    pm_cpu_enable_irq();
    asm volatile("" : "=r" (r20));
    r20 &= 0x1;
    asm volatile("" : "+r" (r20));
    asm goto("brbc 1, .Ltimer_L_0002b8" : : : "memory", "cc" : L_0002b8);
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    goto L_00031b;
L_0002ab:
    asm volatile(".Ltimer_L_0002ab:" : : : "memory");
    r16 = 0x82;
    asm volatile("" : "+r" (r16));
    asm volatile("eor         r19,r19" : : : "memory", "cc");
L_0002ad:
    asm volatile(".Ltimer_L_0002ad:" : : : "memory");
    pm_cpu_disable_irq();
    ths788_read();
    pm_cpu_enable_irq();
    asm volatile("" : "=r" (r17));
    r17 &= 0xf;
    asm volatile("" : "+r" (r17));
    asm volatile("cpi         r17,0x1" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_0002b8" : : : "memory", "cc" : L_0002b8);
    asm volatile("inc         r19" : : : "memory", "cc");
    asm volatile("cpi         r19,0x3" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_0002ad" : : : "memory", "cc" : L_0002ad);
    goto L_0002c5;
L_0002b8:
    asm volatile(".Ltimer_L_0002b8:" : : : "memory");
    pm_cpu_disable_irq();
    r16 = *(volatile uint8_t *)0x2157;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    r16 |= 0x80;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x2157 = r16;
    pm_cpu_enable_irq();
    r16 = 0x40;
    asm volatile("" : "+r" (r16));
L_0002c0:
    asm volatile(".Ltimer_L_0002c0:" : : : "memory");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x606 = r16;
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    goto L_00031b;
L_0002c5:
    asm volatile(".Ltimer_L_0002c5:" : : : "memory");
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    r17 = 0x3;
    asm volatile("" : "+r" (r17));
    asm volatile("eor         r18,r18" : : : "memory", "cc");
    r19 = 0x7;
    asm volatile("" : "+r" (r19));
    pm_cpu_disable_irq();
    ths788_write();
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    asm volatile("eor         r17,r17" : : : "memory", "cc");
    asm volatile("eor         r18,r18" : : : "memory", "cc");
    ths788_write();
    pm_cpu_enable_irq();
    r16 = 0x3;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    goto L_00031b;
L_0002d5:
    asm volatile(".Ltimer_L_0002d5:" : : : "memory");
    asm volatile("ldd         r17,Z+0x1" : : : "memory", "cc");
    asm volatile("dec         r17" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_0002d9" : : : "memory", "cc" : L_0002d9);
    goto L_00031a;
L_0002d9:
    asm volatile(".Ltimer_L_0002d9:" : : : "memory");
    r16 = 0x2;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    r16 = 0x20;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x6a6 = r16;
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x6a5 = r16;
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x625 = r16;
    r16 = 0x8;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x645 = r16;
    r16 = *(volatile uint8_t *)0x628;
    asm volatile("" : "+r" (r16));
    asm goto("sbrc        r16,0x6\n\trjmp .Ltimer_L_0002f7" : : : "memory", "cc" : L_0002f7);
    pm_cpu_disable_irq();
    r16 = *(volatile uint8_t *)0x2157;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    r16 |= 0x40;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x2157 = r16;
    r16 = 0x40;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x606 = r16;
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x686 = r16;
    pm_cpu_enable_irq();
    goto L_00031b;
L_0002f7:
    asm volatile(".Ltimer_L_0002f7:" : : : "memory");
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x62c = r16;
    r16 = 0x2;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x629 = r16;
    r16 = 0x80;
    asm volatile("" : "+r" (r16));
    r17 = 0x0;
    asm volatile("" : "+r" (r17));
    r18 = 0xa6;
    asm volatile("" : "+r" (r18));
    r19 = 0x3;
    asm volatile("" : "+r" (r19));
    pm_cpu_disable_irq();
    ths788_write();
    r16 = 0x81;
    asm volatile("" : "+r" (r16));
    r17 = 0x2;
    asm volatile("" : "+r" (r17));
    asm volatile("eor         r18,r18" : : : "memory", "cc");
    ths788_write();
    r16 = 0xc;
    asm volatile("" : "+r" (r16));
    r28 = 0x7b;
    asm volatile("" : "+r" (r28));
    r29 = 0x21;
    asm volatile("" : "+r" (r29));
    asm volatile("eor         r19,r19" : : : "memory", "cc");
L_00030d:
    asm volatile(".Ltimer_L_00030d:" : : : "memory");
    asm volatile("ld          r17,Y+" : : : "memory", "cc");
    asm volatile("eor         r18,r18" : : : "memory", "cc");
    ths788_write();
    {
        register uint8_t scratch_r16 asm("r16");
        asm volatile("" : "=r" (scratch_r16) : : "memory");
        scratch_r16 -= 224;
        asm volatile("" : "+r" (scratch_r16) : : "memory");
    }
    asm volatile("cpi         r16,0x8c" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_00030d" : : : "memory", "cc" : L_00030d);
    r16 = 0xc;
    asm volatile("" : "+r" (r16));
    asm volatile("inc         r19" : : : "memory", "cc");
    asm volatile("cpi         r19,0x3" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_00030d" : : : "memory", "cc" : L_00030d);
    pm_cpu_enable_irq();
    goto L_00031b;
L_00031a:
    asm volatile(".Ltimer_L_00031a:" : : : "memory");
    asm volatile("std         Z+0x1,r17" : : : "memory", "cc");
L_00031b:
    asm volatile(".Ltimer_L_00031b:" : : : "memory");
    {
        register uint16_t word_counter asm("r30");
        asm volatile("" : "=r" (word_counter) : : "memory");
        word_counter += 2;
        asm volatile("" : "+r" (word_counter) : : "memory");
    }
    {
        register uint8_t *source asm("r30");
        asm volatile("" : "=z" (source) : : "memory");
        r16 = *source;
        asm volatile("" : "+r" (r16) : : "memory");
    }
    asm volatile("ldd         r24,Z+0x1" : : : "memory", "cc");
    asm volatile("ldd         r25,Z+0x2" : : : "memory", "cc");
    asm volatile("and         r16,r16" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_000322" : : : "memory", "cc" : L_000322);
    goto L_000398;
L_000322:
    asm volatile(".Ltimer_L_000322:" : : : "memory");
    asm volatile("cpi         r16,0x1" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_00033d" : : : "memory", "cc" : L_00033d);
    {
        register uint16_t word_counter asm("r24");
        asm volatile("" : "=r" (word_counter) : : "memory");
        word_counter -= 1;
        asm volatile("" : "+r" (word_counter) : : "memory");
    }
    asm goto("brbc 1, .Ltimer_L_000357" : : : "memory", "cc" : L_000357);
    r16 = *(volatile uint8_t *)0x2234;
    asm volatile("" : "+r" (r16));
    asm volatile("and         r16,r16" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_000359" : : : "memory", "cc" : L_000359);
    pm_cpu_disable_irq();
    r16 = *(volatile uint8_t *)0x2157;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    r16 &= 0x7;
    asm volatile("" : "+r" (r16));
    asm volatile("cpi         r16,0x1" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_000339" : : : "memory", "cc" : L_000339);
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x685 = r16;
    pm_cpu_enable_irq();
    r24 = 0xe8;
    asm volatile("" : "+r" (r24));
    r25 = 0x3;
    asm volatile("" : "+r" (r25));
    r16 = 0x2;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    goto L_000357;
L_000339:
    asm volatile(".Ltimer_L_000339:" : : : "memory");
    pm_cpu_enable_irq();
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    goto L_000357;
L_00033d:
    asm volatile(".Ltimer_L_00033d:" : : : "memory");
    asm volatile("cpi         r16,0x2" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_00035a" : : : "memory", "cc" : L_00035a);
    {
        register uint16_t word_counter asm("r24");
        asm volatile("" : "=r" (word_counter) : : "memory");
        word_counter -= 1;
        asm volatile("" : "+r" (word_counter) : : "memory");
    }
    asm goto("brbc 1, .Ltimer_L_000357" : : : "memory", "cc" : L_000357);
    r16 = *(volatile uint8_t *)0x2157;
    asm volatile("" : "+r" (r16));
    asm goto("sbrc        r16,0x3\n\trjmp .Ltimer_L_00034f" : : : "memory", "cc" : L_00034f);
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    pm_cpu_disable_irq();
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x686 = r16;
L_000349:
    asm volatile(".Ltimer_L_000349:" : : : "memory");
    GPIOR0 |= (1u << 1);
    asm volatile("rcall       set_status_and_vd8_led" : : : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    goto L_000357;
L_00034f:
    asm volatile(".Ltimer_L_00034f:" : : : "memory");
    r24 = 0xe8;
    asm volatile("" : "+r" (r24));
    r25 = 0x3;
    asm volatile("" : "+r" (r25));
    r16 = 0x3;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x2442 = r16;
    asm volatile("rcall       system_init" : : : "memory", "cc");
    r16 = 0x3;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
L_000357:
    asm volatile(".Ltimer_L_000357:" : : : "memory");
    asm volatile("std         Z+0x1,r24" : : : "memory", "cc");
    asm volatile("std         Z+0x2,r25" : : : "memory", "cc");
L_000359:
    asm volatile(".Ltimer_L_000359:" : : : "memory");
    goto L_000398;
L_00035a:
    asm volatile(".Ltimer_L_00035a:" : : : "memory");
    asm volatile("cpi         r16,0x3" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_00036c" : : : "memory", "cc" : L_00036c);
    {
        register uint16_t word_counter asm("r24");
        asm volatile("" : "=r" (word_counter) : : "memory");
        word_counter -= 1;
        asm volatile("" : "+r" (word_counter) : : "memory");
    }
    asm goto("brbc 1, .Ltimer_L_000357" : : : "memory", "cc" : L_000357);
    asm volatile("rcall       CDCE62005_control_rst" : : : "memory", "cc");
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x665 = r16;
    r16 = 0x2;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x666 = r16;
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x665 = r16;
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    r24 = 0x88;
    asm volatile("" : "+r" (r24));
    r25 = 0x13;
    asm volatile("" : "+r" (r25));
    goto L_000357;
L_00036c:
    asm volatile(".Ltimer_L_00036c:" : : : "memory");
    asm volatile("cpi         r16,0x4" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_000381" : : : "memory", "cc" : L_000381);
    r16 = *(volatile uint8_t *)0x668;
    asm volatile("" : "+r" (r16));
    asm goto("sbrs        r16,0x3\n\trjmp .Ltimer_L_00037d" : : : "memory", "cc" : L_00037d);
    r16 = 0x5;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    pm_cpu_disable_irq();
    r16 = *(volatile uint8_t *)0x2157;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    r16 |= 0x10;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x2157 = r16;
    pm_cpu_enable_irq();
    asm volatile("rcall       FUN_code_0004ef" : : : "memory", "cc");
    goto L_000383;
L_00037d:
    asm volatile(".Ltimer_L_00037d:" : : : "memory");
    {
        register uint16_t word_counter asm("r24");
        asm volatile("" : "=r" (word_counter) : : "memory");
        word_counter -= 1;
        asm volatile("" : "+r" (word_counter) : : "memory");
    }
    asm goto("brbc 1, .Ltimer_L_000357" : : : "memory", "cc" : L_000357);
    pm_cpu_disable_irq();
    goto L_000349;
L_000381:
    asm volatile(".Ltimer_L_000381:" : : : "memory");
    asm volatile("cpi         r16,0x5" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_000398" : : : "memory", "cc" : L_000398);
L_000383:
    asm volatile(".Ltimer_L_000383:" : : : "memory");
    r16 = *(volatile uint8_t *)0x624;
    asm volatile("" : "+r" (r16));
    asm goto("sbrs        r16,0x7\n\trjmp .Ltimer_L_000398" : : : "memory", "cc" : L_000398);
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x66c = r16;
    r16 = 0x2;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x669 = r16;
    r16 = *(volatile uint8_t *)0x2159;
    asm volatile("" : "+r" (r16));
    asm volatile("and         r16,r16" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_000398" : : : "memory", "cc" : L_000398);
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    {
        register uint8_t *destination asm("r30");
        asm volatile("" : "=z" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    pm_cpu_disable_irq();
    fpga_send_mcu_ts();
    pm_cpu_enable_irq();
    asm volatile("rcall       fpga_settings_init" : : : "memory", "cc");
L_000398:
    asm volatile(".Ltimer_L_000398:" : : : "memory");
    asm volatile("ldd         r24,Z+0x3" : : : "memory", "cc");
    asm volatile("ldd         r25,Z+0x4" : : : "memory", "cc");
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    asm volatile("cpi         r24,0xf4" : : : "memory", "cc");
    asm volatile("cpc         r25,r16" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_0003aa" : : : "memory", "cc" : L_0003aa);
    pm_cpu_disable_irq();
    asm goto("sbis        0,0x0\n\trjmp .Ltimer_L_0003a4" : : : "memory", "cc" : L_0003a4);
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x607 = r16;
L_0003a4:
    asm volatile(".Ltimer_L_0003a4:" : : : "memory");
    asm goto("sbis        0,0x2\n\trjmp .Ltimer_L_0003a9" : : : "memory", "cc" : L_0003a9);
    r16 = 0x20;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x607 = r16;
L_0003a9:
    asm volatile(".Ltimer_L_0003a9:" : : : "memory");
    pm_cpu_enable_irq();
L_0003aa:
    asm volatile(".Ltimer_L_0003aa:" : : : "memory");
    {
        register uint16_t word_counter asm("r24");
        asm volatile("" : "=r" (word_counter) : : "memory");
        word_counter -= 1;
        asm volatile("" : "+r" (word_counter) : : "memory");
    }
    asm goto("brbs 1, .Ltimer_L_0003af" : : : "memory", "cc" : L_0003af);
    asm volatile("std         Z+0x3,r24" : : : "memory", "cc");
    asm volatile("std         Z+0x4,r25" : : : "memory", "cc");
    goto L_00045f;
L_0003af:
    asm volatile(".Ltimer_L_0003af:" : : : "memory");
    pm_cpu_disable_irq();
    asm goto("sbis        0,0x0\n\trjmp .Ltimer_L_0003b5" : : : "memory", "cc" : L_0003b5);
    r16 = 0x1;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x607 = r16;
L_0003b5:
    asm volatile(".Ltimer_L_0003b5:" : : : "memory");
    asm goto("sbis        0,0x2\n\trjmp .Ltimer_L_0003ba" : : : "memory", "cc" : L_0003ba);
    r16 = 0x20;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x607 = r16;
L_0003ba:
    asm volatile(".Ltimer_L_0003ba:" : : : "memory");
    pm_cpu_enable_irq();
    r24 = 0xe8;
    asm volatile("" : "+r" (r24));
    r25 = 0x3;
    asm volatile("" : "+r" (r25));
    asm volatile("std         Z+0x3,r24" : : : "memory", "cc");
    asm volatile("std         Z+0x4,r25" : : : "memory", "cc");
    r16 = 0x50;
    asm volatile("" : "+r" (r16));
    asm volatile("ser         r17" : : : "memory", "cc");
    asm volatile("ser         r18" : : : "memory", "cc");
    adt7311_16bit_rw();
    asm volatile("eor         r17,r17" : : : "memory", "cc");
    asm goto("sbrs        r20,0x2\n\trjmp .Ltimer_L_0003c9" : : : "memory", "cc" : L_0003c9);
    r17 = 0x6;
    asm volatile("" : "+r" (r17));
    goto L_0003d0;
L_0003c9:
    asm volatile(".Ltimer_L_0003c9:" : : : "memory");
    asm goto("sbrs        r20,0x1\n\trjmp .Ltimer_L_0003cd" : : : "memory", "cc" : L_0003cd);
    r17 = 0x4;
    asm volatile("" : "+r" (r17));
    goto L_0003d0;
L_0003cd:
    asm volatile(".Ltimer_L_0003cd:" : : : "memory");
    asm goto("sbrs        r20,0x0\n\trjmp .Ltimer_L_0003d0" : : : "memory", "cc" : L_0003d0);
    r17 = 0x2;
    asm volatile("" : "+r" (r17));
L_0003d0:
    asm volatile(".Ltimer_L_0003d0:" : : : "memory");
    pm_cpu_disable_irq();
    r16 = *(volatile uint8_t *)0x2157;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    r18 = r16;
    asm volatile("" : "+r" (r18));
    asm volatile("" : "=r" (r18));
    r18 &= 0x6;
    asm volatile("" : "+r" (r18));
    asm volatile("" : "=r" (r17), "=r" (r18));
    r18 ^= r17;
    asm volatile("" : "+r" (r18));
    asm goto("brbs 1, .Ltimer_L_0003e7" : : : "memory", "cc" : L_0003e7);
    asm volatile("" : "=r" (r16));
    r16 &= 0xf9;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16), "=r" (r17));
    r16 |= r17;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x2157 = r16;
    asm goto("sbic        0,0x1\n\trjmp .Ltimer_L_0003e6" : : : "memory", "cc" : L_0003e6);
    asm volatile("cpi         r17,0x6" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_0003e1" : : : "memory", "cc" : L_0003e1);
    GPIOR0 |= (1u << 1);
    goto L_0003e5;
L_0003e1:
    asm volatile(".Ltimer_L_0003e1:" : : : "memory");
    asm volatile("and         r17,r17" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_0003e5" : : : "memory", "cc" : L_0003e5);
    GPIOR0 |= (1u << 0);
    goto L_0003e6;
L_0003e5:
    asm volatile(".Ltimer_L_0003e5:" : : : "memory");
    asm volatile("rcall       set_status_and_vd8_led" : : : "memory", "cc");
L_0003e6:
    asm volatile(".Ltimer_L_0003e6:" : : : "memory");
    pm_cpu_enable_irq();
L_0003e7:
    asm volatile(".Ltimer_L_0003e7:" : : : "memory");
    asm volatile("" : "=r" (r20));
    r20 &= 0xf8;
    asm volatile("" : "+r" (r20));
    r18 = 0x14;
    asm volatile("" : "+r" (r18));
    asm volatile("mul         r20,r18" : : : "memory", "cc");
    asm volatile("" : "=r" (r1));
    r16 = r1;
    asm volatile("" : "+r" (r16));
    asm volatile("eor         r17,r17" : : : "memory", "cc");
    asm volatile("mulsu       r21,r18" : : : "memory", "cc");
    asm volatile("" : "=r" (r0), "=r" (r16));
    r16 += r0;
    asm volatile("" : "+r" (r16));
    asm volatile("adc         r17,r1" : : : "memory", "cc");
    asm volatile("ldd         r18,Z+0x5" : : : "memory", "cc");
    asm volatile("ldd         r19,Z+0x6" : : : "memory", "cc");
    asm volatile("std         Z+0x5,r16" : : : "memory", "cc");
    asm volatile("std         Z+0x6,r17" : : : "memory", "cc");
    r20 = *(volatile uint8_t *)0x689;
    asm volatile("" : "+r" (r20));
    asm goto("sbrs        r20,0x3\n\trjmp .Ltimer_L_00045f" : : : "memory", "cc" : L_00045f);
    asm volatile("cp          r16,r18" : : : "memory", "cc");
    asm volatile("cpc         r17,r19" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_0003ff" : : : "memory", "cc" : L_0003ff);
    r18 = 0xbc;
    asm volatile("" : "+r" (r18));
    pm_cpu_disable_irq();
    fpga_msg_send_t2();
    pm_cpu_enable_irq();
L_0003ff:
    asm volatile(".Ltimer_L_0003ff:" : : : "memory");
    asm goto("sbic        0,0x3\n\trjmp .Ltimer_L_00045f" : : : "memory", "cc" : L_00045f);
    r18 = 0x7d;
    asm volatile("" : "+r" (r18));
    pm_cpu_disable_irq();
    fpga_msg_read_t1();
    pm_cpu_enable_irq();
    {
        register uint16_t copied_word asm("r16");
        register uint16_t destination_word asm("r30");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    asm volatile("" : "=r" (r16), "=r" (r17));
    r16 |= r17;
    asm volatile("" : "+r" (r16));
    asm goto("brbc 1, .Ltimer_L_00040a" : : : "memory", "cc" : L_00040a);
    goto L_00045f;
L_00040a:
    asm volatile(".Ltimer_L_00040a:" : : : "memory");
    asm volatile("eor         r23,r23" : : : "memory", "cc");
L_00040b:
    asm volatile(".Ltimer_L_00040b:" : : : "memory");
    asm volatile("" : "=r" (r31));
    r31 >>= 1;
    asm volatile("" : "+r" (r31));
    asm volatile("ror         r30" : : : "memory", "cc");
    asm goto("brbs 0, .Ltimer_L_00040f" : : : "memory", "cc" : L_00040f);
    goto L_00045b;
L_00040f:
    asm volatile(".Ltimer_L_00040f:" : : : "memory");
    asm volatile("" : "=r" (r23));
    r18 = r23;
    asm volatile("" : "+r" (r18));
    asm volatile("" : "=r" (r18));
    r18 += r18;
    asm volatile("" : "+r" (r18));
    {
        register uint8_t scratch_r18 asm("r18");
        asm volatile("" : "=r" (scratch_r18) : : "memory");
        scratch_r18 -= 243;
        asm volatile("" : "+r" (scratch_r18) : : "memory");
    }
    pm_cpu_disable_irq();
    fpga_msg_read_t1();
    pm_cpu_enable_irq();
    {
        register uint16_t copied_word asm("r16");
        register uint16_t destination_word asm("r20");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    asm volatile("inc         r18" : : : "memory", "cc");
    pm_cpu_disable_irq();
    fpga_msg_read_t1();
    pm_cpu_enable_irq();
    asm volatile("cp          r16,r20" : : : "memory", "cc");
    asm volatile("cpc       \tr17,r21" : : : "memory", "cc");
    asm goto("brbc 0, .Ltimer_L_000422" : : : "memory", "cc" : L_000422);
    {
        register uint16_t copied_word asm("r16");
        register uint16_t destination_word asm("r24");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    {
        register uint16_t copied_word asm("r20");
        register uint16_t destination_word asm("r16");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    {
        register uint16_t copied_word asm("r24");
        register uint16_t destination_word asm("r20");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
L_000422:
    asm volatile(".Ltimer_L_000422:" : : : "memory");
    asm volatile("and         r17,r17" : : : "memory", "cc");
    asm goto("brbc 1, .Ltimer_L_00042b" : : : "memory", "cc" : L_00042b);
    asm volatile("cpi         r16,0x97" : : : "memory", "cc");
    asm goto("brbc 0, .Ltimer_L_00042b" : : : "memory", "cc" : L_00042b);
    asm volatile("cpi         r20,0x1e" : : : "memory", "cc");
    asm goto("brbc 0, .Ltimer_L_00045b" : : : "memory", "cc" : L_00045b);
    r16 = 0x1e;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t destination asm("r16");
        register uint8_t operand asm("r20");
        asm volatile("" : "=r" (destination), "=r" (operand) : : "memory");
        destination -= operand;
        asm volatile("" : "+r" (destination) : "r" (operand) : "memory");
    }
    goto L_000434;
L_00042b:
    asm volatile(".Ltimer_L_00042b:" : : : "memory");
    asm volatile("and         r21,r21" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_00042f" : : : "memory", "cc" : L_00042f);
    asm volatile("cpi         r20,0x1e" : : : "memory", "cc");
    asm goto("brbs 0, .Ltimer_L_00045b" : : : "memory", "cc" : L_00045b);
L_00042f:
    asm volatile(".Ltimer_L_00042f:" : : : "memory");
    {
        register uint16_t copied_word asm("r16");
        register uint16_t destination_word asm("r20");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    r16 = 0x96;
    asm volatile("" : "+r" (r16));
    asm volatile("eor         r17,r17" : : : "memory", "cc");
    {
        register uint8_t destination asm("r16");
        register uint8_t operand asm("r20");
        asm volatile("" : "=r" (destination), "=r" (operand) : : "memory");
        destination -= operand;
        asm volatile("" : "+r" (destination) : "r" (operand) : "memory");
    }
    asm volatile("sbc         r17,r21" : : : "memory", "cc");
L_000434:
    asm volatile(".Ltimer_L_000434:" : : : "memory");
    asm volatile("" : "=r" (r23));
    r20 = r23;
    asm volatile("" : "+r" (r20));
    r28 = 0xcf;
    asm volatile("" : "+r" (r28));
    r29 = 0x21;
    asm volatile("" : "+r" (r29));
    asm volatile("" : "=r" (r20));
    r20 += r20;
    asm volatile("" : "+r" (r20));
    asm volatile("" : "=r" (r20));
    r20 += r20;
    asm volatile("" : "+r" (r20));
    asm volatile("" : "=r" (r20));
    r18 = r20;
    asm volatile("" : "+r" (r18));
    asm volatile("" : "=r" (r20));
    r20 += r20;
    asm volatile("" : "+r" (r20));
    {
        register uint8_t scratch_r20 asm("r20");
        asm volatile("" : "=r" (scratch_r20) : : "memory");
        scratch_r20 -= 252;
        asm volatile("" : "+r" (scratch_r20) : : "memory");
    }
    asm volatile("eor         r21,r21" : : : "memory", "cc");
    asm volatile("" : "=r" (r20), "=r" (r28));
    r28 += r20;
    asm volatile("" : "+r" (r28));
    asm volatile("adc         r29,r21" : : : "memory", "cc");
    asm volatile("" : "=r" (pm_timer_settings_cursor) : : "memory");
    r20 = *(volatile uint8_t *)(uintptr_t)pm_timer_settings_cursor;
    asm volatile("" : "+r" (r20) : : "memory");
    asm volatile("ldd         r21,Y+0x1" : : : "memory", "cc");
    asm volatile("" : "=r" (r16), "=r" (r20));
    r20 += r16;
    asm volatile("" : "+r" (r20));
    asm volatile("adc         r21,r17" : : : "memory", "cc");
    r24 = 0x1;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r20,0xf5" : : : "memory", "cc");
    asm volatile("cpc         r21,r24" : : : "memory", "cc");
    asm goto("brbc 4, .Ltimer_L_00044e" : : : "memory", "cc" : L_00044e);
    r24 = 0xfe;
    asm volatile("" : "+r" (r24));
    asm volatile("cpi         r21,0xc" : : : "memory", "cc");
    asm volatile("cpc         r21,r24" : : : "memory", "cc");
    asm goto("brbc 4, .Ltimer_L_000450" : : : "memory", "cc" : L_000450);
    r20 = 0xc;
    asm volatile("" : "+r" (r20));
    r21 = 0xfe;
    asm volatile("" : "+r" (r21));
    goto L_000450;
L_00044e:
    asm volatile(".Ltimer_L_00044e:" : : : "memory");
    r20 = 0xf4;
    asm volatile("" : "+r" (r20));
    r21 = 0x1;
    asm volatile("" : "+r" (r21));
L_000450:
    asm volatile(".Ltimer_L_000450:" : : : "memory");
    asm volatile("" : "=r" (pm_timer_settings_cursor), "=r" (r20) : : "memory");
    *(volatile uint8_t *)(uintptr_t)pm_timer_settings_cursor = r20;
    asm volatile("std         Y+0x1,r21" : : : "memory", "cc");
    {
        register uint16_t copied_word asm("r20");
        register uint16_t destination_word asm("r16");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    {
        register uint8_t scratch_r18 asm("r18");
        asm volatile("" : "=r" (scratch_r18) : : "memory");
        scratch_r18 -= 126;
        asm volatile("" : "+r" (scratch_r18) : : "memory");
    }
    pm_cpu_disable_irq();
    fpga_msg_send_t2();
    pm_cpu_enable_irq();
    asm volatile("" : "=r" (r23));
    r22 = r23;
    asm volatile("" : "+r" (r22));
    dac_set_value();
L_00045b:
    asm volatile(".Ltimer_L_00045b:" : : : "memory");
    asm volatile("inc         r23" : : : "memory", "cc");
    asm volatile("cpi         r23,0xc" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_00045f" : : : "memory", "cc" : L_00045f);
    goto L_00040b;
L_00045f:
    asm volatile(".Ltimer_L_00045f:" : : : "memory");
    asm volatile("pop         r0" : : : "memory", "cc");
    asm volatile("pop         r1" : : : "memory", "cc");
    asm volatile("pop         r16" : : : "memory", "cc");
    asm volatile("pop         r17" : : : "memory", "cc");
    asm volatile("pop         r18" : : : "memory", "cc");
    asm volatile("pop         r19" : : : "memory", "cc");
    asm volatile("pop         r20" : : : "memory", "cc");
    asm volatile("pop         r21" : : : "memory", "cc");
    asm volatile("pop         r22" : : : "memory", "cc");
    asm volatile("pop         r23" : : : "memory", "cc");
    asm volatile("pop         r24" : : : "memory", "cc");
    asm volatile("pop         r25" : : : "memory", "cc");
    asm volatile("pop         r28" : : : "memory", "cc");
    asm volatile("pop         r29" : : : "memory", "cc");
    asm volatile("pop         r30" : : : "memory", "cc");
    asm volatile("pop         r31" : : : "memory", "cc");
    {
        register uint8_t saved_status asm("r31");
        asm volatile("" : "=r" (saved_status) : : "memory");
        *(volatile uint8_t *)0x3f = saved_status;
    }
    asm volatile("pop         r31" : : : "memory", "cc");
    asm volatile("reti" : : : "memory", "cc");
    __builtin_unreachable();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 7824 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_timer_overflow_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x049e: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 1184;
    }
    case 0x04a0: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 1186;
    }
    case 0x04a2: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 1188;
    }
    case 0x04a4: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 1190;
    }
    case 0x04a6: { // push r29
        s->stack[s->depth++] = s->r[29];
        return 1192;
    }
    case 0x04a8: { // push r28
        s->stack[s->depth++] = s->r[28];
        return 1194;
    }
    case 0x04aa: { // push r25
        s->stack[s->depth++] = s->r[25];
        return 1196;
    }
    case 0x04ac: { // push r24
        s->stack[s->depth++] = s->r[24];
        return 1198;
    }
    case 0x04ae: { // push r23
        s->stack[s->depth++] = s->r[23];
        return 1200;
    }
    case 0x04b0: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 1202;
    }
    case 0x04b2: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 1204;
    }
    case 0x04b4: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 1206;
    }
    case 0x04b6: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 1208;
    }
    case 0x04b8: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 1210;
    }
    case 0x04ba: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 1212;
    }
    case 0x04bc: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 1214;
    }
    case 0x04be: { // push r1
        s->stack[s->depth++] = s->r[1];
        return 1216;
    }
    case 0x04c0: { // push r0
        s->stack[s->depth++] = s->r[0];
        return 1218;
    }
    case 0x04c2: { // ldi r30, 0x59
        s->r[30] = 89;
        return 1220;
    }
    case 0x04c4: { // ldi r31, 0x21
        s->r[31] = 33;
        return 1222;
    }
    case 0x04c6: { // ld r16, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[16] = pm_read(s, address);
        return 1224;
    }
    case 0x04c8: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1226;
    }
    case 0x04ca: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 1230 : 1228;
    }
    case 0x04cc: { // rjmp .+360
        return 1590;
    }
    case 0x04ce: { // dec r16
        s->r[16]--;
        pm_nzv(s, s->r[16], s->r[16] == 127);
        return 1232;
    }
    case 0x04d0: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 1236 : 1234;
    }
    case 0x04d2: { // rjmp .+214
        return 1450;
    }
    case 0x04d4: { // dec r16
        s->r[16]--;
        pm_nzv(s, s->r[16], s->r[16] == 127);
        return 1238;
    }
    case 0x04d6: { // breq .+126
        return (pm_getflag(s, 1) == 1) ? 1366 : 1240;
    }
    case 0x04d8: { // dec r16
        s->r[16]--;
        pm_nzv(s, s->r[16], s->r[16] == 127);
        return 1242;
    }
    case 0x04da: { // breq .+84
        return (pm_getflag(s, 1) == 1) ? 1328 : 1244;
    }
    case 0x04dc: { // dec r16
        s->r[16]--;
        pm_nzv(s, s->r[16], s->r[16] == 127);
        return 1246;
    }
    case 0x04de: { // breq .+56
        return (pm_getflag(s, 1) == 1) ? 1304 : 1248;
    }
    case 0x04e0: { // ldi r18, 0x7F
        s->r[18] = 127;
        return 1250;
    }
    case 0x04e2: { // cli
        pm_irq(s, false);
        return 1252;
    }
    case 0x04e4: { // call 0x2368
        s->calls[s->call_depth++] = 1256;
        return 9064;
    }
    case 0x04e8: { // sei
        pm_irq(s, true);
        return 1258;
    }
    case 0x04ea: { // andi r16, 0x0F
        s->r[16] &= 15;
        pm_nzv(s, s->r[16], false);
        return 1260;
    }
    case 0x04ec: { // cpi r16, 0x0F
        pm_sub(s, s->r[16], 15, 0, false);
        return 1262;
    }
    case 0x04ee: { // breq .+8
        return (pm_getflag(s, 1) == 1) ? 1272 : 1264;
    }
    case 0x04f0: { // ldd r17, Z+1
        uint16_t address = pm_pointer(s, 30) + 1;
        s->r[17] = pm_read(s, address);
        return 1266;
    }
    case 0x04f2: { // dec r17
        s->r[17]--;
        pm_nzv(s, s->r[17], s->r[17] == 127);
        return 1268;
    }
    case 0x04f4: { // breq .+122
        return (pm_getflag(s, 1) == 1) ? 1392 : 1270;
    }
    case 0x04f6: { // rjmp .+316
        return 1588;
    }
    case 0x04f8: { // rcall .+1168
        s->calls[s->call_depth++] = 1274;
        return 2442;
    }
    case 0x04fa: { // ldi r16, 0x02
        s->r[16] = 2;
        return 1276;
    }
    case 0x04fc: { // sts 0x068C, r16
        uint16_t address = 1676;
        pm_write(s, address, s->r[16]);
        return 1280;
    }
    case 0x0500: { // ldi r16, 0x09
        s->r[16] = 9;
        return 1282;
    }
    case 0x0502: { // sts 0x0689, r16
        uint16_t address = 1673;
        pm_write(s, address, s->r[16]);
        return 1286;
    }
    case 0x0506: { // ldi r18, 0x7F
        s->r[18] = 127;
        return 1288;
    }
    case 0x0508: { // ldi r16, 0x00
        s->r[16] = 0;
        return 1290;
    }
    case 0x050a: { // ldi r17, 0x08
        s->r[17] = 8;
        return 1292;
    }
    case 0x050c: { // cli
        pm_irq(s, false);
        return 1294;
    }
    case 0x050e: { // call 0x230e
        s->calls[s->call_depth++] = 1298;
        return 8974;
    }
    case 0x0512: { // sei
        pm_irq(s, true);
        return 1300;
    }
    case 0x0514: { // ldi r16, 0x80
        s->r[16] = 128;
        return 1302;
    }
    case 0x0516: { // rjmp .+104
        return 1408;
    }
    case 0x0518: { // lds r16, 0x0668
        uint16_t address = 1640;
        s->r[16] = pm_read(s, address);
        return 1308;
    }
    case 0x051c: { // sbrs r16, 3
        return (!!(s->r[16] & (1u << 3)) == 1) ? 1312 : 1310;
    }
    case 0x051e: { // rjmp .+278
        return 1590;
    }
    case 0x0520: { // ldi r16, 0x80
        s->r[16] = 128;
        return 1314;
    }
    case 0x0522: { // sts 0x0625, r16
        uint16_t address = 1573;
        pm_write(s, address, s->r[16]);
        return 1318;
    }
    case 0x0526: { // ldi r16, 0x05
        s->r[16] = 5;
        return 1320;
    }
    case 0x0528: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1322;
    }
    case 0x052a: { // ldi r16, 0xFA
        s->r[16] = 250;
        return 1324;
    }
    case 0x052c: { // std Z+1, r16
        uint16_t address = pm_pointer(s, 30) + 1;
        pm_write(s, address, s->r[16]);
        return 1326;
    }
    case 0x052e: { // rjmp .+262
        return 1590;
    }
    case 0x0530: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 1330;
    }
    case 0x0532: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 1332;
    }
    case 0x0534: { // cli
        pm_irq(s, false);
        return 1334;
    }
    case 0x0536: { // ldi r16, 0x04
        s->r[16] = 4;
        return 1336;
    }
    case 0x0538: { // call 0x2208
        s->calls[s->call_depth++] = 1340;
        return 8712;
    }
    case 0x053c: { // or r20, r17
        s->r[20] |= s->r[17];
        pm_nzv(s, s->r[20], false);
        return 1342;
    }
    case 0x053e: { // subi r16, 0xE0
        s->r[16] = pm_sub(s, s->r[16], 224, 0, false);
        return 1344;
    }
    case 0x0540: { // cpi r16, 0xC4
        pm_sub(s, s->r[16], 196, 0, false);
        return 1346;
    }
    case 0x0542: { // brne .-12
        return (pm_getflag(s, 1) == 0) ? 1336 : 1348;
    }
    case 0x0544: { // inc r19
        s->r[19]++;
        pm_nzv(s, s->r[19], s->r[19] == 128);
        return 1350;
    }
    case 0x0546: { // cpi r19, 0x03
        pm_sub(s, s->r[19], 3, 0, false);
        return 1352;
    }
    case 0x0548: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 1334 : 1354;
    }
    case 0x054a: { // sei
        pm_irq(s, true);
        return 1356;
    }
    case 0x054c: { // andi r20, 0x01
        s->r[20] &= 1;
        pm_nzv(s, s->r[20], false);
        return 1358;
    }
    case 0x054e: { // brne .+32
        return (pm_getflag(s, 1) == 0) ? 1392 : 1360;
    }
    case 0x0550: { // ldi r16, 0x04
        s->r[16] = 4;
        return 1362;
    }
    case 0x0552: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1364;
    }
    case 0x0554: { // rjmp .+224
        return 1590;
    }
    case 0x0556: { // ldi r16, 0x82
        s->r[16] = 130;
        return 1368;
    }
    case 0x0558: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 1370;
    }
    case 0x055a: { // cli
        pm_irq(s, false);
        return 1372;
    }
    case 0x055c: { // call 0x2208
        s->calls[s->call_depth++] = 1376;
        return 8712;
    }
    case 0x0560: { // sei
        pm_irq(s, true);
        return 1378;
    }
    case 0x0562: { // andi r17, 0x0F
        s->r[17] &= 15;
        pm_nzv(s, s->r[17], false);
        return 1380;
    }
    case 0x0564: { // cpi r17, 0x01
        pm_sub(s, s->r[17], 1, 0, false);
        return 1382;
    }
    case 0x0566: { // brne .+8
        return (pm_getflag(s, 1) == 0) ? 1392 : 1384;
    }
    case 0x0568: { // inc r19
        s->r[19]++;
        pm_nzv(s, s->r[19], s->r[19] == 128);
        return 1386;
    }
    case 0x056a: { // cpi r19, 0x03
        pm_sub(s, s->r[19], 3, 0, false);
        return 1388;
    }
    case 0x056c: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 1370 : 1390;
    }
    case 0x056e: { // rjmp .+26
        return 1418;
    }
    case 0x0570: { // cli
        pm_irq(s, false);
        return 1394;
    }
    case 0x0572: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 1398;
    }
    case 0x0576: { // ori r16, 0x80
        s->r[16] |= 128;
        pm_nzv(s, s->r[16], false);
        return 1400;
    }
    case 0x0578: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 1404;
    }
    case 0x057c: { // sei
        pm_irq(s, true);
        return 1406;
    }
    case 0x057e: { // ldi r16, 0x40
        s->r[16] = 64;
        return 1408;
    }
    case 0x0580: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 1412;
    }
    case 0x0584: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1414;
    }
    case 0x0586: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1416;
    }
    case 0x0588: { // rjmp .+172
        return 1590;
    }
    case 0x058a: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1420;
    }
    case 0x058c: { // ldi r17, 0x03
        s->r[17] = 3;
        return 1422;
    }
    case 0x058e: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 1424;
    }
    case 0x0590: { // ldi r19, 0x07
        s->r[19] = 7;
        return 1426;
    }
    case 0x0592: { // cli
        pm_irq(s, false);
        return 1428;
    }
    case 0x0594: { // call 0x2174
        s->calls[s->call_depth++] = 1432;
        return 8564;
    }
    case 0x0598: { // ldi r16, 0x01
        s->r[16] = 1;
        return 1434;
    }
    case 0x059a: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 1436;
    }
    case 0x059c: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 1438;
    }
    case 0x059e: { // call 0x2174
        s->calls[s->call_depth++] = 1442;
        return 8564;
    }
    case 0x05a2: { // sei
        pm_irq(s, true);
        return 1444;
    }
    case 0x05a4: { // ldi r16, 0x03
        s->r[16] = 3;
        return 1446;
    }
    case 0x05a6: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1448;
    }
    case 0x05a8: { // rjmp .+140
        return 1590;
    }
    case 0x05aa: { // ldd r17, Z+1
        uint16_t address = pm_pointer(s, 30) + 1;
        s->r[17] = pm_read(s, address);
        return 1452;
    }
    case 0x05ac: { // dec r17
        s->r[17]--;
        pm_nzv(s, s->r[17], s->r[17] == 127);
        return 1454;
    }
    case 0x05ae: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 1458 : 1456;
    }
    case 0x05b0: { // rjmp .+130
        return 1588;
    }
    case 0x05b2: { // ldi r16, 0x02
        s->r[16] = 2;
        return 1460;
    }
    case 0x05b4: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1462;
    }
    case 0x05b6: { // ldi r16, 0x20
        s->r[16] = 32;
        return 1464;
    }
    case 0x05b8: { // sts 0x06A6, r16
        uint16_t address = 1702;
        pm_write(s, address, s->r[16]);
        return 1468;
    }
    case 0x05bc: { // sts 0x06A5, r16
        uint16_t address = 1701;
        pm_write(s, address, s->r[16]);
        return 1472;
    }
    case 0x05c0: { // sts 0x0625, r16
        uint16_t address = 1573;
        pm_write(s, address, s->r[16]);
        return 1476;
    }
    case 0x05c4: { // ldi r16, 0x08
        s->r[16] = 8;
        return 1478;
    }
    case 0x05c6: { // sts 0x0645, r16
        uint16_t address = 1605;
        pm_write(s, address, s->r[16]);
        return 1482;
    }
    case 0x05ca: { // lds r16, 0x0628
        uint16_t address = 1576;
        s->r[16] = pm_read(s, address);
        return 1486;
    }
    case 0x05ce: { // sbrc r16, 6
        return (!!(s->r[16] & (1u << 6)) == 0) ? 1490 : 1488;
    }
    case 0x05d0: { // rjmp .+28
        return 1518;
    }
    case 0x05d2: { // cli
        pm_irq(s, false);
        return 1492;
    }
    case 0x05d4: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 1496;
    }
    case 0x05d8: { // ori r16, 0x40
        s->r[16] |= 64;
        pm_nzv(s, s->r[16], false);
        return 1498;
    }
    case 0x05da: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 1502;
    }
    case 0x05de: { // ldi r16, 0x40
        s->r[16] = 64;
        return 1504;
    }
    case 0x05e0: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 1508;
    }
    case 0x05e4: { // ldi r16, 0x04
        s->r[16] = 4;
        return 1510;
    }
    case 0x05e6: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 1514;
    }
    case 0x05ea: { // sei
        pm_irq(s, true);
        return 1516;
    }
    case 0x05ec: { // rjmp .+72
        return 1590;
    }
    case 0x05ee: { // ldi r16, 0x01
        s->r[16] = 1;
        return 1520;
    }
    case 0x05f0: { // sts 0x062C, r16
        uint16_t address = 1580;
        pm_write(s, address, s->r[16]);
        return 1524;
    }
    case 0x05f4: { // ldi r16, 0x02
        s->r[16] = 2;
        return 1526;
    }
    case 0x05f6: { // sts 0x0629, r16
        uint16_t address = 1577;
        pm_write(s, address, s->r[16]);
        return 1530;
    }
    case 0x05fa: { // ldi r16, 0x80
        s->r[16] = 128;
        return 1532;
    }
    case 0x05fc: { // ldi r17, 0x00
        s->r[17] = 0;
        return 1534;
    }
    case 0x05fe: { // ldi r18, 0xA6
        s->r[18] = 166;
        return 1536;
    }
    case 0x0600: { // ldi r19, 0x03
        s->r[19] = 3;
        return 1538;
    }
    case 0x0602: { // cli
        pm_irq(s, false);
        return 1540;
    }
    case 0x0604: { // call 0x2174
        s->calls[s->call_depth++] = 1544;
        return 8564;
    }
    case 0x0608: { // ldi r16, 0x81
        s->r[16] = 129;
        return 1546;
    }
    case 0x060a: { // ldi r17, 0x02
        s->r[17] = 2;
        return 1548;
    }
    case 0x060c: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 1550;
    }
    case 0x060e: { // call 0x2174
        s->calls[s->call_depth++] = 1554;
        return 8564;
    }
    case 0x0612: { // ldi r16, 0x0C
        s->r[16] = 12;
        return 1556;
    }
    case 0x0614: { // ldi r28, 0x7B
        s->r[28] = 123;
        return 1558;
    }
    case 0x0616: { // ldi r29, 0x21
        s->r[29] = 33;
        return 1560;
    }
    case 0x0618: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 1562;
    }
    case 0x061a: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 1564;
    }
    case 0x061c: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 1566;
    }
    case 0x061e: { // call 0x2174
        s->calls[s->call_depth++] = 1570;
        return 8564;
    }
    case 0x0622: { // subi r16, 0xE0
        s->r[16] = pm_sub(s, s->r[16], 224, 0, false);
        return 1572;
    }
    case 0x0624: { // cpi r16, 0x8C
        pm_sub(s, s->r[16], 140, 0, false);
        return 1574;
    }
    case 0x0626: { // brne .-14
        return (pm_getflag(s, 1) == 0) ? 1562 : 1576;
    }
    case 0x0628: { // ldi r16, 0x0C
        s->r[16] = 12;
        return 1578;
    }
    case 0x062a: { // inc r19
        s->r[19]++;
        pm_nzv(s, s->r[19], s->r[19] == 128);
        return 1580;
    }
    case 0x062c: { // cpi r19, 0x03
        pm_sub(s, s->r[19], 3, 0, false);
        return 1582;
    }
    case 0x062e: { // brne .-22
        return (pm_getflag(s, 1) == 0) ? 1562 : 1584;
    }
    case 0x0630: { // sei
        pm_irq(s, true);
        return 1586;
    }
    case 0x0632: { // rjmp .+2
        return 1590;
    }
    case 0x0634: { // std Z+1, r17
        uint16_t address = pm_pointer(s, 30) + 1;
        pm_write(s, address, s->r[17]);
        return 1590;
    }
    case 0x0636: { // adiw r30, 0x02
        uint16_t old = pm_pointer(s, 30);
        uint16_t value = old + 2;
        pm_setpointer(s, 30, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 1592;
    }
    case 0x0638: { // ld r16, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[16] = pm_read(s, address);
        return 1594;
    }
    case 0x063a: { // ldd r24, Z+1
        uint16_t address = pm_pointer(s, 30) + 1;
        s->r[24] = pm_read(s, address);
        return 1596;
    }
    case 0x063c: { // ldd r25, Z+2
        uint16_t address = pm_pointer(s, 30) + 2;
        s->r[25] = pm_read(s, address);
        return 1598;
    }
    case 0x063e: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1600;
    }
    case 0x0640: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 1604 : 1602;
    }
    case 0x0642: { // rjmp .+236
        return 1840;
    }
    case 0x0644: { // cpi r16, 0x01
        pm_sub(s, s->r[16], 1, 0, false);
        return 1606;
    }
    case 0x0646: { // brne .+50
        return (pm_getflag(s, 1) == 0) ? 1658 : 1608;
    }
    case 0x0648: { // sbiw r24, 0x01
        uint16_t old = pm_pointer(s, 24);
        uint16_t value = old - 1;
        pm_setpointer(s, 24, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 1610;
    }
    case 0x064a: { // brne .+98
        return (pm_getflag(s, 1) == 0) ? 1710 : 1612;
    }
    case 0x064c: { // lds r16, 0x2234
        uint16_t address = 8756;
        s->r[16] = pm_read(s, address);
        return 1616;
    }
    case 0x0650: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1618;
    }
    case 0x0652: { // breq .+94
        return (pm_getflag(s, 1) == 1) ? 1714 : 1620;
    }
    case 0x0654: { // cli
        pm_irq(s, false);
        return 1622;
    }
    case 0x0656: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 1626;
    }
    case 0x065a: { // andi r16, 0x07
        s->r[16] &= 7;
        pm_nzv(s, s->r[16], false);
        return 1628;
    }
    case 0x065c: { // cpi r16, 0x01
        pm_sub(s, s->r[16], 1, 0, false);
        return 1630;
    }
    case 0x065e: { // brne .+18
        return (pm_getflag(s, 1) == 0) ? 1650 : 1632;
    }
    case 0x0660: { // ldi r16, 0x04
        s->r[16] = 4;
        return 1634;
    }
    case 0x0662: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 1638;
    }
    case 0x0666: { // sei
        pm_irq(s, true);
        return 1640;
    }
    case 0x0668: { // ldi r24, 0xE8
        s->r[24] = 232;
        return 1642;
    }
    case 0x066a: { // ldi r25, 0x03
        s->r[25] = 3;
        return 1644;
    }
    case 0x066c: { // ldi r16, 0x02
        s->r[16] = 2;
        return 1646;
    }
    case 0x066e: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1648;
    }
    case 0x0670: { // rjmp .+60
        return 1710;
    }
    case 0x0672: { // sei
        pm_irq(s, true);
        return 1652;
    }
    case 0x0674: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1654;
    }
    case 0x0676: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1656;
    }
    case 0x0678: { // rjmp .+52
        return 1710;
    }
    case 0x067a: { // cpi r16, 0x02
        pm_sub(s, s->r[16], 2, 0, false);
        return 1660;
    }
    case 0x067c: { // brne .+54
        return (pm_getflag(s, 1) == 0) ? 1716 : 1662;
    }
    case 0x067e: { // sbiw r24, 0x01
        uint16_t old = pm_pointer(s, 24);
        uint16_t value = old - 1;
        pm_setpointer(s, 24, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 1664;
    }
    case 0x0680: { // brne .+44
        return (pm_getflag(s, 1) == 0) ? 1710 : 1666;
    }
    case 0x0682: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 1670;
    }
    case 0x0686: { // sbrc r16, 3
        return (!!(s->r[16] & (1u << 3)) == 0) ? 1674 : 1672;
    }
    case 0x0688: { // rjmp .+20
        return 1694;
    }
    case 0x068a: { // ldi r16, 0x04
        s->r[16] = 4;
        return 1676;
    }
    case 0x068c: { // cli
        pm_irq(s, false);
        return 1678;
    }
    case 0x068e: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 1682;
    }
    case 0x0692: { // sbi 0x00, 1
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 1));
        return 1684;
    }
    case 0x0694: { // rcall .+1512
        s->calls[s->call_depth++] = 1686;
        return 3198;
    }
    case 0x0696: { // sei
        pm_irq(s, true);
        return 1688;
    }
    case 0x0698: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1690;
    }
    case 0x069a: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1692;
    }
    case 0x069c: { // rjmp .+16
        return 1710;
    }
    case 0x069e: { // ldi r24, 0xE8
        s->r[24] = 232;
        return 1696;
    }
    case 0x06a0: { // ldi r25, 0x03
        s->r[25] = 3;
        return 1698;
    }
    case 0x06a2: { // ldi r16, 0x03
        s->r[16] = 3;
        return 1700;
    }
    case 0x06a4: { // sts 0x2442, r16
        uint16_t address = 9282;
        pm_write(s, address, s->r[16]);
        return 1704;
    }
    case 0x06a8: { // rcall .+1638
        s->calls[s->call_depth++] = 1706;
        return 3344;
    }
    case 0x06aa: { // ldi r16, 0x03
        s->r[16] = 3;
        return 1708;
    }
    case 0x06ac: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1710;
    }
    case 0x06ae: { // std Z+1, r24
        uint16_t address = pm_pointer(s, 30) + 1;
        pm_write(s, address, s->r[24]);
        return 1712;
    }
    case 0x06b0: { // std Z+2, r25
        uint16_t address = pm_pointer(s, 30) + 2;
        pm_write(s, address, s->r[25]);
        return 1714;
    }
    case 0x06b2: { // rjmp .+124
        return 1840;
    }
    case 0x06b4: { // cpi r16, 0x03
        pm_sub(s, s->r[16], 3, 0, false);
        return 1718;
    }
    case 0x06b6: { // brne .+32
        return (pm_getflag(s, 1) == 0) ? 1752 : 1720;
    }
    case 0x06b8: { // sbiw r24, 0x01
        uint16_t old = pm_pointer(s, 24);
        uint16_t value = old - 1;
        pm_setpointer(s, 24, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 1722;
    }
    case 0x06ba: { // brne .-14
        return (pm_getflag(s, 1) == 0) ? 1710 : 1724;
    }
    case 0x06bc: { // rcall .+1822
        s->calls[s->call_depth++] = 1726;
        return 3548;
    }
    case 0x06be: { // ldi r16, 0x04
        s->r[16] = 4;
        return 1728;
    }
    case 0x06c0: { // sts 0x0665, r16
        uint16_t address = 1637;
        pm_write(s, address, s->r[16]);
        return 1732;
    }
    case 0x06c4: { // ldi r16, 0x02
        s->r[16] = 2;
        return 1734;
    }
    case 0x06c6: { // sts 0x0666, r16
        uint16_t address = 1638;
        pm_write(s, address, s->r[16]);
        return 1738;
    }
    case 0x06ca: { // sts 0x0665, r16
        uint16_t address = 1637;
        pm_write(s, address, s->r[16]);
        return 1742;
    }
    case 0x06ce: { // ldi r16, 0x04
        s->r[16] = 4;
        return 1744;
    }
    case 0x06d0: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1746;
    }
    case 0x06d2: { // ldi r24, 0x88
        s->r[24] = 136;
        return 1748;
    }
    case 0x06d4: { // ldi r25, 0x13
        s->r[25] = 19;
        return 1750;
    }
    case 0x06d6: { // rjmp .-42
        return 1710;
    }
    case 0x06d8: { // cpi r16, 0x04
        pm_sub(s, s->r[16], 4, 0, false);
        return 1754;
    }
    case 0x06da: { // brne .+38
        return (pm_getflag(s, 1) == 0) ? 1794 : 1756;
    }
    case 0x06dc: { // lds r16, 0x0668
        uint16_t address = 1640;
        s->r[16] = pm_read(s, address);
        return 1760;
    }
    case 0x06e0: { // sbrs r16, 3
        return (!!(s->r[16] & (1u << 3)) == 1) ? 1764 : 1762;
    }
    case 0x06e2: { // rjmp .+22
        return 1786;
    }
    case 0x06e4: { // ldi r16, 0x05
        s->r[16] = 5;
        return 1766;
    }
    case 0x06e6: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1768;
    }
    case 0x06e8: { // cli
        pm_irq(s, false);
        return 1770;
    }
    case 0x06ea: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 1774;
    }
    case 0x06ee: { // ori r16, 0x10
        s->r[16] |= 16;
        pm_nzv(s, s->r[16], false);
        return 1776;
    }
    case 0x06f0: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 1780;
    }
    case 0x06f4: { // sei
        pm_irq(s, true);
        return 1782;
    }
    case 0x06f6: { // rcall .+742
        s->calls[s->call_depth++] = 1784;
        return 2526;
    }
    case 0x06f8: { // rjmp .+12
        return 1798;
    }
    case 0x06fa: { // sbiw r24, 0x01
        uint16_t old = pm_pointer(s, 24);
        uint16_t value = old - 1;
        pm_setpointer(s, 24, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 1788;
    }
    case 0x06fc: { // brne .-80
        return (pm_getflag(s, 1) == 0) ? 1710 : 1790;
    }
    case 0x06fe: { // cli
        pm_irq(s, false);
        return 1792;
    }
    case 0x0700: { // rjmp .-112
        return 1682;
    }
    case 0x0702: { // cpi r16, 0x05
        pm_sub(s, s->r[16], 5, 0, false);
        return 1796;
    }
    case 0x0704: { // brne .+42
        return (pm_getflag(s, 1) == 0) ? 1840 : 1798;
    }
    case 0x0706: { // lds r16, 0x0624
        uint16_t address = 1572;
        s->r[16] = pm_read(s, address);
        return 1802;
    }
    case 0x070a: { // sbrs r16, 7
        return (!!(s->r[16] & (1u << 7)) == 1) ? 1806 : 1804;
    }
    case 0x070c: { // rjmp .+34
        return 1840;
    }
    case 0x070e: { // ldi r16, 0x01
        s->r[16] = 1;
        return 1808;
    }
    case 0x0710: { // sts 0x066C, r16
        uint16_t address = 1644;
        pm_write(s, address, s->r[16]);
        return 1812;
    }
    case 0x0714: { // ldi r16, 0x02
        s->r[16] = 2;
        return 1814;
    }
    case 0x0716: { // sts 0x0669, r16
        uint16_t address = 1641;
        pm_write(s, address, s->r[16]);
        return 1818;
    }
    case 0x071a: { // lds r16, 0x2159
        uint16_t address = 8537;
        s->r[16] = pm_read(s, address);
        return 1822;
    }
    case 0x071e: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1824;
    }
    case 0x0720: { // brne .+14
        return (pm_getflag(s, 1) == 0) ? 1840 : 1826;
    }
    case 0x0722: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1828;
    }
    case 0x0724: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 1830;
    }
    case 0x0726: { // cli
        pm_irq(s, false);
        return 1832;
    }
    case 0x0728: { // call 0x2530
        s->calls[s->call_depth++] = 1836;
        return 9520;
    }
    case 0x072c: { // sei
        pm_irq(s, true);
        return 1838;
    }
    case 0x072e: { // rcall .+436
        s->calls[s->call_depth++] = 1840;
        return 2276;
    }
    case 0x0730: { // ldd r24, Z+3
        uint16_t address = pm_pointer(s, 30) + 3;
        s->r[24] = pm_read(s, address);
        return 1842;
    }
    case 0x0732: { // ldd r25, Z+4
        uint16_t address = pm_pointer(s, 30) + 4;
        s->r[25] = pm_read(s, address);
        return 1844;
    }
    case 0x0734: { // ldi r16, 0x01
        s->r[16] = 1;
        return 1846;
    }
    case 0x0736: { // cpi r24, 0xF4
        pm_sub(s, s->r[24], 244, 0, false);
        return 1848;
    }
    case 0x0738: { // cpc r25, r16
        pm_sub(s, s->r[25], s->r[16], pm_getflag(s, CARRY), true);
        return 1850;
    }
    case 0x073a: { // brne .+24
        return (pm_getflag(s, 1) == 0) ? 1876 : 1852;
    }
    case 0x073c: { // cli
        pm_irq(s, false);
        return 1854;
    }
    case 0x073e: { // sbis 0x00, 0
        return (!!(pm_io_read(s, 0) & (1u << 0)) == 1) ? 1858 : 1856;
    }
    case 0x0740: { // rjmp .+6
        return 1864;
    }
    case 0x0742: { // ldi r16, 0x01
        s->r[16] = 1;
        return 1860;
    }
    case 0x0744: { // sts 0x0607, r16
        uint16_t address = 1543;
        pm_write(s, address, s->r[16]);
        return 1864;
    }
    case 0x0748: { // sbis 0x00, 2
        return (!!(pm_io_read(s, 0) & (1u << 2)) == 1) ? 1868 : 1866;
    }
    case 0x074a: { // rjmp .+6
        return 1874;
    }
    case 0x074c: { // ldi r16, 0x20
        s->r[16] = 32;
        return 1870;
    }
    case 0x074e: { // sts 0x0607, r16
        uint16_t address = 1543;
        pm_write(s, address, s->r[16]);
        return 1874;
    }
    case 0x0752: { // sei
        pm_irq(s, true);
        return 1876;
    }
    case 0x0754: { // sbiw r24, 0x01
        uint16_t old = pm_pointer(s, 24);
        uint16_t value = old - 1;
        pm_setpointer(s, 24, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 1878;
    }
    case 0x0756: { // breq .+6
        return (pm_getflag(s, 1) == 1) ? 1886 : 1880;
    }
    case 0x0758: { // std Z+3, r24
        uint16_t address = pm_pointer(s, 30) + 3;
        pm_write(s, address, s->r[24]);
        return 1882;
    }
    case 0x075a: { // std Z+4, r25
        uint16_t address = pm_pointer(s, 30) + 4;
        pm_write(s, address, s->r[25]);
        return 1884;
    }
    case 0x075c: { // rjmp .+352
        return 2238;
    }
    case 0x075e: { // cli
        pm_irq(s, false);
        return 1888;
    }
    case 0x0760: { // sbis 0x00, 0
        return (!!(pm_io_read(s, 0) & (1u << 0)) == 1) ? 1892 : 1890;
    }
    case 0x0762: { // rjmp .+6
        return 1898;
    }
    case 0x0764: { // ldi r16, 0x01
        s->r[16] = 1;
        return 1894;
    }
    case 0x0766: { // sts 0x0607, r16
        uint16_t address = 1543;
        pm_write(s, address, s->r[16]);
        return 1898;
    }
    case 0x076a: { // sbis 0x00, 2
        return (!!(pm_io_read(s, 0) & (1u << 2)) == 1) ? 1902 : 1900;
    }
    case 0x076c: { // rjmp .+6
        return 1908;
    }
    case 0x076e: { // ldi r16, 0x20
        s->r[16] = 32;
        return 1904;
    }
    case 0x0770: { // sts 0x0607, r16
        uint16_t address = 1543;
        pm_write(s, address, s->r[16]);
        return 1908;
    }
    case 0x0774: { // sei
        pm_irq(s, true);
        return 1910;
    }
    case 0x0776: { // ldi r24, 0xE8
        s->r[24] = 232;
        return 1912;
    }
    case 0x0778: { // ldi r25, 0x03
        s->r[25] = 3;
        return 1914;
    }
    case 0x077a: { // std Z+3, r24
        uint16_t address = pm_pointer(s, 30) + 3;
        pm_write(s, address, s->r[24]);
        return 1916;
    }
    case 0x077c: { // std Z+4, r25
        uint16_t address = pm_pointer(s, 30) + 4;
        pm_write(s, address, s->r[25]);
        return 1918;
    }
    case 0x077e: { // ldi r16, 0x50
        s->r[16] = 80;
        return 1920;
    }
    case 0x0780: { // ldi r17, 0xFF
        s->r[17] = 255;
        return 1922;
    }
    case 0x0782: { // ldi r18, 0xFF
        s->r[18] = 255;
        return 1924;
    }
    case 0x0784: { // call 0x25be
        s->calls[s->call_depth++] = 1928;
        return 9662;
    }
    case 0x0788: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 1930;
    }
    case 0x078a: { // sbrs r20, 2
        return (!!(s->r[20] & (1u << 2)) == 1) ? 1934 : 1932;
    }
    case 0x078c: { // rjmp .+4
        return 1938;
    }
    case 0x078e: { // ldi r17, 0x06
        s->r[17] = 6;
        return 1936;
    }
    case 0x0790: { // rjmp .+14
        return 1952;
    }
    case 0x0792: { // sbrs r20, 1
        return (!!(s->r[20] & (1u << 1)) == 1) ? 1942 : 1940;
    }
    case 0x0794: { // rjmp .+4
        return 1946;
    }
    case 0x0796: { // ldi r17, 0x04
        s->r[17] = 4;
        return 1944;
    }
    case 0x0798: { // rjmp .+6
        return 1952;
    }
    case 0x079a: { // sbrs r20, 0
        return (!!(s->r[20] & (1u << 0)) == 1) ? 1950 : 1948;
    }
    case 0x079c: { // rjmp .+2
        return 1952;
    }
    case 0x079e: { // ldi r17, 0x02
        s->r[17] = 2;
        return 1952;
    }
    case 0x07a0: { // cli
        pm_irq(s, false);
        return 1954;
    }
    case 0x07a2: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 1958;
    }
    case 0x07a6: { // mov r18, r16
        s->r[18] = s->r[16];
        return 1960;
    }
    case 0x07a8: { // andi r18, 0x06
        s->r[18] &= 6;
        pm_nzv(s, s->r[18], false);
        return 1962;
    }
    case 0x07aa: { // eor r18, r17
        s->r[18] ^= s->r[17];
        pm_nzv(s, s->r[18], false);
        return 1964;
    }
    case 0x07ac: { // breq .+32
        return (pm_getflag(s, 1) == 1) ? 1998 : 1966;
    }
    case 0x07ae: { // andi r16, 0xF9
        s->r[16] &= 249;
        pm_nzv(s, s->r[16], false);
        return 1968;
    }
    case 0x07b0: { // or r16, r17
        s->r[16] |= s->r[17];
        pm_nzv(s, s->r[16], false);
        return 1970;
    }
    case 0x07b2: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 1974;
    }
    case 0x07b6: { // sbic 0x00, 1
        return (!!(pm_io_read(s, 0) & (1u << 1)) == 0) ? 1978 : 1976;
    }
    case 0x07b8: { // rjmp .+18
        return 1996;
    }
    case 0x07ba: { // cpi r17, 0x06
        pm_sub(s, s->r[17], 6, 0, false);
        return 1980;
    }
    case 0x07bc: { // brne .+4
        return (pm_getflag(s, 1) == 0) ? 1986 : 1982;
    }
    case 0x07be: { // sbi 0x00, 1
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 1));
        return 1984;
    }
    case 0x07c0: { // rjmp .+8
        return 1994;
    }
    case 0x07c2: { // and r17, r17
        s->r[17] &= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 1988;
    }
    case 0x07c4: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 1994 : 1990;
    }
    case 0x07c6: { // sbi 0x00, 0
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 0));
        return 1992;
    }
    case 0x07c8: { // rjmp .+2
        return 1996;
    }
    case 0x07ca: { // rcall .+1202
        s->calls[s->call_depth++] = 1996;
        return 3198;
    }
    case 0x07cc: { // sei
        pm_irq(s, true);
        return 1998;
    }
    case 0x07ce: { // andi r20, 0xF8
        s->r[20] &= 248;
        pm_nzv(s, s->r[20], false);
        return 2000;
    }
    case 0x07d0: { // ldi r18, 0x14
        s->r[18] = 20;
        return 2002;
    }
    case 0x07d2: { // mul r20, r18
        uint16_t product = (s->r[20]) * (int)s->r[18];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 2004;
    }
    case 0x07d4: { // mov r16, r1
        s->r[16] = s->r[1];
        return 2006;
    }
    case 0x07d6: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 2008;
    }
    case 0x07d8: { // mulsu r21, r18
        uint16_t product = ((int8_t)s->r[21]) * (int)s->r[18];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 2010;
    }
    case 0x07da: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 2012;
    }
    case 0x07dc: { // adc r17, r1
        s->r[17] = pm_add(s, s->r[17], s->r[1], pm_getflag(s, CARRY));
        return 2014;
    }
    case 0x07de: { // ldd r18, Z+5
        uint16_t address = pm_pointer(s, 30) + 5;
        s->r[18] = pm_read(s, address);
        return 2016;
    }
    case 0x07e0: { // ldd r19, Z+6
        uint16_t address = pm_pointer(s, 30) + 6;
        s->r[19] = pm_read(s, address);
        return 2018;
    }
    case 0x07e2: { // std Z+5, r16
        uint16_t address = pm_pointer(s, 30) + 5;
        pm_write(s, address, s->r[16]);
        return 2020;
    }
    case 0x07e4: { // std Z+6, r17
        uint16_t address = pm_pointer(s, 30) + 6;
        pm_write(s, address, s->r[17]);
        return 2022;
    }
    case 0x07e6: { // lds r20, 0x0689
        uint16_t address = 1673;
        s->r[20] = pm_read(s, address);
        return 2026;
    }
    case 0x07ea: { // sbrs r20, 3
        return (!!(s->r[20] & (1u << 3)) == 1) ? 2030 : 2028;
    }
    case 0x07ec: { // rjmp .+208
        return 2238;
    }
    case 0x07ee: { // cp r16, r18
        pm_sub(s, s->r[16], s->r[18], 0, false);
        return 2032;
    }
    case 0x07f0: { // cpc r17, r19
        pm_sub(s, s->r[17], s->r[19], pm_getflag(s, CARRY), true);
        return 2034;
    }
    case 0x07f2: { // breq .+10
        return (pm_getflag(s, 1) == 1) ? 2046 : 2036;
    }
    case 0x07f4: { // ldi r18, 0xBC
        s->r[18] = 188;
        return 2038;
    }
    case 0x07f6: { // cli
        pm_irq(s, false);
        return 2040;
    }
    case 0x07f8: { // call 0x230e
        s->calls[s->call_depth++] = 2044;
        return 8974;
    }
    case 0x07fc: { // sei
        pm_irq(s, true);
        return 2046;
    }
    case 0x07fe: { // sbic 0x00, 3
        return (!!(pm_io_read(s, 0) & (1u << 3)) == 0) ? 2050 : 2048;
    }
    case 0x0800: { // rjmp .+188
        return 2238;
    }
    case 0x0802: { // ldi r18, 0x7D
        s->r[18] = 125;
        return 2052;
    }
    case 0x0804: { // cli
        pm_irq(s, false);
        return 2054;
    }
    case 0x0806: { // call 0x2368
        s->calls[s->call_depth++] = 2058;
        return 9064;
    }
    case 0x080a: { // sei
        pm_irq(s, true);
        return 2060;
    }
    case 0x080c: { // movw r30, r16
        uint16_t pair = pm_pointer(s, 16);
        pm_setpointer(s, 30, pair);
        return 2062;
    }
    case 0x080e: { // or r16, r17
        s->r[16] |= s->r[17];
        pm_nzv(s, s->r[16], false);
        return 2064;
    }
    case 0x0810: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 2068 : 2066;
    }
    case 0x0812: { // rjmp .+170
        return 2238;
    }
    case 0x0814: { // eor r23, r23
        s->r[23] ^= s->r[23];
        pm_nzv(s, s->r[23], false);
        return 2070;
    }
    case 0x0816: { // lsr r31
        bool carry = s->r[31] & 1;
        s->r[31] = (s->r[31] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[31], !!(s->r[31] & 128) ^ carry);
        return 2072;
    }
    case 0x0818: { // ror r30
        bool carry = s->r[30] & 1;
        s->r[30] = (s->r[30] >> 1) | (pm_getflag(s, CARRY) << 7);
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[30], !!(s->r[30] & 128) ^ carry);
        return 2074;
    }
    case 0x081a: { // brcs .+2
        return (pm_getflag(s, 0) == 1) ? 2078 : 2076;
    }
    case 0x081c: { // rjmp .+152
        return 2230;
    }
    case 0x081e: { // mov r18, r23
        s->r[18] = s->r[23];
        return 2080;
    }
    case 0x0820: { // add r18, r18
        s->r[18] = pm_add(s, s->r[18], s->r[18], 0);
        return 2082;
    }
    case 0x0822: { // subi r18, 0xF3
        s->r[18] = pm_sub(s, s->r[18], 243, 0, false);
        return 2084;
    }
    case 0x0824: { // cli
        pm_irq(s, false);
        return 2086;
    }
    case 0x0826: { // call 0x2368
        s->calls[s->call_depth++] = 2090;
        return 9064;
    }
    case 0x082a: { // sei
        pm_irq(s, true);
        return 2092;
    }
    case 0x082c: { // movw r20, r16
        uint16_t pair = pm_pointer(s, 16);
        pm_setpointer(s, 20, pair);
        return 2094;
    }
    case 0x082e: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 2096;
    }
    case 0x0830: { // cli
        pm_irq(s, false);
        return 2098;
    }
    case 0x0832: { // call 0x2368
        s->calls[s->call_depth++] = 2102;
        return 9064;
    }
    case 0x0836: { // sei
        pm_irq(s, true);
        return 2104;
    }
    case 0x0838: { // cp r16, r20
        pm_sub(s, s->r[16], s->r[20], 0, false);
        return 2106;
    }
    case 0x083a: { // cpc r17, r21
        pm_sub(s, s->r[17], s->r[21], pm_getflag(s, CARRY), true);
        return 2108;
    }
    case 0x083c: { // brcc .+6
        return (pm_getflag(s, 0) == 0) ? 2116 : 2110;
    }
    case 0x083e: { // movw r24, r16
        uint16_t pair = pm_pointer(s, 16);
        pm_setpointer(s, 24, pair);
        return 2112;
    }
    case 0x0840: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 2114;
    }
    case 0x0842: { // movw r20, r24
        uint16_t pair = pm_pointer(s, 24);
        pm_setpointer(s, 20, pair);
        return 2116;
    }
    case 0x0844: { // and r17, r17
        s->r[17] &= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 2118;
    }
    case 0x0846: { // brne .+14
        return (pm_getflag(s, 1) == 0) ? 2134 : 2120;
    }
    case 0x0848: { // cpi r16, 0x97
        pm_sub(s, s->r[16], 151, 0, false);
        return 2122;
    }
    case 0x084a: { // brcc .+10
        return (pm_getflag(s, 0) == 0) ? 2134 : 2124;
    }
    case 0x084c: { // cpi r20, 0x1E
        pm_sub(s, s->r[20], 30, 0, false);
        return 2126;
    }
    case 0x084e: { // brcc .+102
        return (pm_getflag(s, 0) == 0) ? 2230 : 2128;
    }
    case 0x0850: { // ldi r16, 0x1E
        s->r[16] = 30;
        return 2130;
    }
    case 0x0852: { // sub r16, r20
        s->r[16] = pm_sub(s, s->r[16], s->r[20], 0, false);
        return 2132;
    }
    case 0x0854: { // rjmp .+18
        return 2152;
    }
    case 0x0856: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 2136;
    }
    case 0x0858: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 2142 : 2138;
    }
    case 0x085a: { // cpi r20, 0x1E
        pm_sub(s, s->r[20], 30, 0, false);
        return 2140;
    }
    case 0x085c: { // brcs .+88
        return (pm_getflag(s, 0) == 1) ? 2230 : 2142;
    }
    case 0x085e: { // movw r20, r16
        uint16_t pair = pm_pointer(s, 16);
        pm_setpointer(s, 20, pair);
        return 2144;
    }
    case 0x0860: { // ldi r16, 0x96
        s->r[16] = 150;
        return 2146;
    }
    case 0x0862: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 2148;
    }
    case 0x0864: { // sub r16, r20
        s->r[16] = pm_sub(s, s->r[16], s->r[20], 0, false);
        return 2150;
    }
    case 0x0866: { // sbc r17, r21
        s->r[17] = pm_sub(s, s->r[17], s->r[21], pm_getflag(s, CARRY), true);
        return 2152;
    }
    case 0x0868: { // mov r20, r23
        s->r[20] = s->r[23];
        return 2154;
    }
    case 0x086a: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 2156;
    }
    case 0x086c: { // ldi r29, 0x21
        s->r[29] = 33;
        return 2158;
    }
    case 0x086e: { // add r20, r20
        s->r[20] = pm_add(s, s->r[20], s->r[20], 0);
        return 2160;
    }
    case 0x0870: { // add r20, r20
        s->r[20] = pm_add(s, s->r[20], s->r[20], 0);
        return 2162;
    }
    case 0x0872: { // mov r18, r20
        s->r[18] = s->r[20];
        return 2164;
    }
    case 0x0874: { // add r20, r20
        s->r[20] = pm_add(s, s->r[20], s->r[20], 0);
        return 2166;
    }
    case 0x0876: { // subi r20, 0xFC
        s->r[20] = pm_sub(s, s->r[20], 252, 0, false);
        return 2168;
    }
    case 0x0878: { // eor r21, r21
        s->r[21] ^= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 2170;
    }
    case 0x087a: { // add r28, r20
        s->r[28] = pm_add(s, s->r[28], s->r[20], 0);
        return 2172;
    }
    case 0x087c: { // adc r29, r21
        s->r[29] = pm_add(s, s->r[29], s->r[21], pm_getflag(s, CARRY));
        return 2174;
    }
    case 0x087e: { // ld r20, Y
        uint16_t address = pm_pointer(s, 28) + 0;
        s->r[20] = pm_read(s, address);
        return 2176;
    }
    case 0x0880: { // ldd r21, Y+1
        uint16_t address = pm_pointer(s, 28) + 1;
        s->r[21] = pm_read(s, address);
        return 2178;
    }
    case 0x0882: { // add r20, r16
        s->r[20] = pm_add(s, s->r[20], s->r[16], 0);
        return 2180;
    }
    case 0x0884: { // adc r21, r17
        s->r[21] = pm_add(s, s->r[21], s->r[17], pm_getflag(s, CARRY));
        return 2182;
    }
    case 0x0886: { // ldi r24, 0x01
        s->r[24] = 1;
        return 2184;
    }
    case 0x0888: { // cpi r20, 0xF5
        pm_sub(s, s->r[20], 245, 0, false);
        return 2186;
    }
    case 0x088a: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 2188;
    }
    case 0x088c: { // brge .+14
        return (pm_getflag(s, 4) == 0) ? 2204 : 2190;
    }
    case 0x088e: { // ldi r24, 0xFE
        s->r[24] = 254;
        return 2192;
    }
    case 0x0890: { // cpi r21, 0x0C
        pm_sub(s, s->r[21], 12, 0, false);
        return 2194;
    }
    case 0x0892: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 2196;
    }
    case 0x0894: { // brge .+10
        return (pm_getflag(s, 4) == 0) ? 2208 : 2198;
    }
    case 0x0896: { // ldi r20, 0x0C
        s->r[20] = 12;
        return 2200;
    }
    case 0x0898: { // ldi r21, 0xFE
        s->r[21] = 254;
        return 2202;
    }
    case 0x089a: { // rjmp .+4
        return 2208;
    }
    case 0x089c: { // ldi r20, 0xF4
        s->r[20] = 244;
        return 2206;
    }
    case 0x089e: { // ldi r21, 0x01
        s->r[21] = 1;
        return 2208;
    }
    case 0x08a0: { // st Y, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[20]);
        return 2210;
    }
    case 0x08a2: { // std Y+1, r21
        uint16_t address = pm_pointer(s, 28) + 1;
        pm_write(s, address, s->r[21]);
        return 2212;
    }
    case 0x08a4: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 2214;
    }
    case 0x08a6: { // subi r18, 0x7E
        s->r[18] = pm_sub(s, s->r[18], 126, 0, false);
        return 2216;
    }
    case 0x08a8: { // cli
        pm_irq(s, false);
        return 2218;
    }
    case 0x08aa: { // call 0x230e
        s->calls[s->call_depth++] = 2222;
        return 8974;
    }
    case 0x08ae: { // sei
        pm_irq(s, true);
        return 2224;
    }
    case 0x08b0: { // mov r22, r23
        s->r[22] = s->r[23];
        return 2226;
    }
    case 0x08b2: { // call 0x2104
        s->calls[s->call_depth++] = 2230;
        return 8452;
    }
    case 0x08b6: { // inc r23
        s->r[23]++;
        pm_nzv(s, s->r[23], s->r[23] == 128);
        return 2232;
    }
    case 0x08b8: { // cpi r23, 0x0C
        pm_sub(s, s->r[23], 12, 0, false);
        return 2234;
    }
    case 0x08ba: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 2238 : 2236;
    }
    case 0x08bc: { // rjmp .-168
        return 2070;
    }
    case 0x08be: { // pop r0
        s->r[0] = s->stack[--s->depth];
        return 2240;
    }
    case 0x08c0: { // pop r1
        s->r[1] = s->stack[--s->depth];
        return 2242;
    }
    case 0x08c2: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 2244;
    }
    case 0x08c4: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 2246;
    }
    case 0x08c6: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 2248;
    }
    case 0x08c8: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 2250;
    }
    case 0x08ca: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 2252;
    }
    case 0x08cc: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 2254;
    }
    case 0x08ce: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 2256;
    }
    case 0x08d0: { // pop r23
        s->r[23] = s->stack[--s->depth];
        return 2258;
    }
    case 0x08d2: { // pop r24
        s->r[24] = s->stack[--s->depth];
        return 2260;
    }
    case 0x08d4: { // pop r25
        s->r[25] = s->stack[--s->depth];
        return 2262;
    }
    case 0x08d6: { // pop r28
        s->r[28] = s->stack[--s->depth];
        return 2264;
    }
    case 0x08d8: { // pop r29
        s->r[29] = s->stack[--s->depth];
        return 2266;
    }
    case 0x08da: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 2268;
    }
    case 0x08dc: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 2270;
    }
    case 0x08de: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 2272;
    }
    case 0x08e0: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 2274;
    }
    case 0x08e2: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
