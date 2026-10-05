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

/* Original timer overflow interrupt and original byte range 0x049e..0x08e3.
 * C owns constants, SRAM/MMIO access and internal jumps. Exact ASM
 * retains the ISR frame, flags and private peripheral/call contracts.
 * No independent functional validation of a rejected plain-C ISR
 * is recorded; acceptance is complete canonical FLASH equality. */
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
    asm volatile("ld          r16,Z" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("ld          r16,Z" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
    goto L_000357;
L_000339:
    asm volatile(".Ltimer_L_000339:" : : : "memory");
    pm_cpu_enable_irq();
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("sbi         0,0x1" : : : "memory", "cc");
    asm volatile("rcall       set_status_and_vd8_led" : : : "memory", "cc");
    pm_cpu_enable_irq();
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("st          Z,r16" : : : "memory", "cc");
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
    asm volatile("sbi         0,0x1" : : : "memory", "cc");
    goto L_0003e5;
L_0003e1:
    asm volatile(".Ltimer_L_0003e1:" : : : "memory");
    asm volatile("and         r17,r17" : : : "memory", "cc");
    asm goto("brbs 1, .Ltimer_L_0003e5" : : : "memory", "cc" : L_0003e5);
    asm volatile("sbi         0,0x0" : : : "memory", "cc");
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
    asm volatile("ld          r20,Y" : : : "memory", "cc");
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
    asm volatile("st          Y,r20" : : : "memory", "cc");
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
