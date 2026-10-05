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
    asm goto("sbrc        r16,0x7\n\trjmp %l[L_0008b8]" : : : "memory", "cc" : L_0008b8);
L_0008b6:
    pm_cpu_enable_irq();
    asm volatile("ret");
    __builtin_unreachable();
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
     * no independent functional-test claim for this explanatory alternative. */
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
    asm goto("sbrs        r20,0x3\n\trjmp %l[L_0008b6]" : : : "memory", "cc" : L_0008b6);
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
    asm volatile("" : "+r" (r19));
    asm goto("brbc 1, %l[L_0008f5]" : : : "memory", "cc" : L_0008f5);
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
    asm goto("sbrs        r20,0x3\n\trjmp %l[L_0008b6]" : : : "memory", "cc" : L_0008b6);
    fpga_msg_send_t2();
    pm_cpu_enable_irq();
L_0008ec:
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
    asm goto("sbrs        r20,0x3\n\trjmp %l[L_0008b6]" : : : "memory", "cc" : L_0008b6);
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
    asm goto("sbrs        r20,0x3\n\trjmp %l[L_0008b6]" : : : "memory", "cc" : L_0008b6);
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
    asm goto("sbrs        r20,0x3\n\trjmp %l[L_0008b6]" : : : "memory", "cc" : L_0008b6);
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
    asm goto("sbrs        r20,0x3\n\trjmp %l[L_0008b6]" : : : "memory", "cc" : L_0008b6);
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
    asm volatile("ld          r21,Z" : : : "memory", "cc");
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
    asm volatile("ret");
    __builtin_unreachable();
}
