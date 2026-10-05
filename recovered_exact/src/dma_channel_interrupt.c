/* Private entry calls below intentionally have no GNU argument/result ABI.
 * Adjacent fixed-register setup/capture preserves each historical contract;
 * these declarations emit the original wide CALL, not a new C API. The whole
 * image check verifies CALL width, register moves and every saved frame. */
extern void FUN_code_000b32(void);
extern void FUN_code_000b3f(void);
extern void FUN_code_000b59(void);
extern void FUN_code_000b88(void);
extern void FUN_code_000b9d(void);
extern void FUN_code_000bb9(void);
extern void FUN_code_000c20(void);
#include "legacy_cpu.h"
#include <stdint.h>
/* Exact private word pairs; no SRAM objects or GNU save/restore frames. */
register uint16_t pm_dma_address asm("r28");
register uint16_t pm_dma_current asm("r20");
register uint16_t pm_dma_buffer_cursor asm("r26");

/* Original DMA channel interrupt and original byte range 0x01e2..0x046b.
 * C owns constants, SRAM/MMIO access and internal jumps. Exact ASM
 * retains the ISR frame, flags and private peripheral/call contracts.
 * No independent functional validation of a rejected plain-C ISR
 * is recorded; acceptance is complete canonical FLASH equality.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
void DMA_CH1_vect_isr(void)
{
    register uint8_t r2 asm("r2");
    register uint8_t r16 asm("r16");
    register uint8_t r17 asm("r17");
    register uint8_t r18 asm("r18");
    register uint8_t r19 asm("r19");
    register uint8_t r20 asm("r20");
    register uint8_t r22 asm("r22");
    register uint8_t r26 asm("r26");
    register uint8_t r27 asm("r27");
    register uint8_t r28 asm("r28");
    register uint8_t r29 asm("r29");
    register uint8_t r30 asm("r30");
    register uint8_t r31 asm("r31");
    asm volatile("push        r31" : : : "memory", "cc");
    {
        register uint8_t saved_status asm("r31") = *(volatile uint8_t *)0x3f;
        asm volatile("" : "+r" (saved_status) : : "memory");
    }
    asm volatile("push        r16" : : : "memory", "cc");
    asm volatile("push        r17" : : : "memory", "cc");
    asm volatile("push        r18" : : : "memory", "cc");
    asm volatile("push        r28" : : : "memory", "cc");
    asm volatile("push        r29" : : : "memory", "cc");
    asm volatile("push        r30" : : : "memory", "cc");
    asm volatile("push        r31" : : : "memory", "cc");
    r17 = 0xa;
    asm volatile("" : "+r" (r17));
L_0000fb:
    asm volatile(".Ldma_L_0000fb:" : : : "memory");
    r16 = *(volatile uint8_t *)0x668;
    asm volatile("" : "+r" (r16));
    asm goto("sbrc        r16,0x4\n\trjmp .Ldma_L_000101" : : : "memory", "cc" : L_000101);
    asm volatile("dec         r17" : : : "memory", "cc");
    asm goto("brbc 1, .Ldma_L_0000fb" : : : "memory", "cc" : L_0000fb);
L_000101:
    asm volatile(".Ldma_L_000101:" : : : "memory");
    r17 = *(volatile uint8_t *)0x124;
    asm volatile("" : "+r" (r17));
    r16 = 0x11;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x121 = r16;
    r30 = 0x76;
    asm volatile("" : "+r" (r30));
    r31 = 0x2b;
    asm volatile("" : "+r" (r31));
    r28 = 0x39;
    asm volatile("" : "+r" (r28));
    r29 = 0x24;
    asm volatile("" : "+r" (r29));
    r16 = 0x8;
    asm volatile("" : "+r" (r16));
    {
        register uint8_t destination asm("r16");
        register uint8_t operand asm("r17");
        asm volatile("" : "=r" (destination), "=r" (operand) : : "memory");
        destination -= operand;
        asm volatile("" : "+r" (destination) : "r" (operand) : "memory");
    }
    asm volatile("eor         r17,r17" : : : "memory", "cc");
    asm volatile("" : "=r" (r16), "=r" (r28));
    r28 += r16;
    asm volatile("" : "+r" (r28));
    asm volatile("adc         r29,r17" : : : "memory", "cc");
    asm volatile("ld          r16,Y+" : : : "memory", "cc");
    asm volatile("cpi         r16,0xaa" : : : "memory", "cc");
    asm goto("brbs 1, .Ldma_L_000116" : : : "memory", "cc" : L_000116);
L_000112:
    asm volatile(".Ldma_L_000112:" : : : "memory");
    asm volatile("lpm         r17,Z+" : : : "memory", "cc");
    asm volatile("cp          r16,r17" : : : "memory", "cc");
    asm goto("brbs 1, .Ldma_L_000130" : : : "memory", "cc" : L_000130);
    goto L_00022c;
L_000116:
    asm volatile(".Ldma_L_000116:" : : : "memory");
    asm volatile("cpi         r17,0x7" : : : "memory", "cc");
    asm goto("brbs 1, .Ldma_L_000120" : : : "memory", "cc" : L_000120);
    asm volatile("inc         r17" : : : "memory", "cc");
    asm volatile("cpi         r28,0x41" : : : "memory", "cc");
    asm goto("brbc 1, .Ldma_L_00011c" : : : "memory", "cc" : L_00011c);
    asm volatile("" : "=r" (pm_dma_address) : : "memory");
    pm_dma_address -= 8;
    asm volatile("" : "+r" (pm_dma_address) : : "memory");
L_00011c:
    asm volatile(".Ldma_L_00011c:" : : : "memory");
    asm volatile("ld          r16,Y+" : : : "memory", "cc");
    asm volatile("cpi         r16,0xaa" : : : "memory", "cc");
    asm goto("brbs 1, .Ldma_L_000116" : : : "memory", "cc" : L_000116);
    goto L_00022c;
L_000120:
    asm volatile(".Ldma_L_000120:" : : : "memory");
    pm_cpu_disable_irq();
    asm volatile("rcall       FUN_code_0005b4" : : : "memory", "cc");
    pm_cpu_enable_irq();
    r16 = 0x3;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x2441 = r16;
    asm volatile("pop         r31" : : : "memory", "cc");
    asm volatile("pop         r30" : : : "memory", "cc");
    asm volatile("pop         r29" : : : "memory", "cc");
    asm volatile("pop         r28" : : : "memory", "cc");
    asm volatile("pop         r18" : : : "memory", "cc");
    asm volatile("pop         r17" : : : "memory", "cc");
    asm volatile("pop         r16" : : : "memory", "cc");
    {
        register uint8_t saved_status asm("r31");
        asm volatile("" : "=r" (saved_status) : : "memory");
        *(volatile uint8_t *)0x3f = saved_status;
    }
    asm volatile("pop         r31" : : : "memory", "cc");
    asm volatile("rjmp        PORTF_INT1_vect_isr" : : : "memory", "cc");
    __builtin_unreachable();
L_000130:
    asm volatile(".Ldma_L_000130:" : : : "memory");
    asm volatile("cpi         r28,0x41" : : : "memory", "cc");
    asm goto("brbc 1, .Ldma_L_000133" : : : "memory", "cc" : L_000133);
    asm volatile("" : "=r" (pm_dma_address) : : "memory");
    pm_dma_address -= 8;
    asm volatile("" : "+r" (pm_dma_address) : : "memory");
L_000133:
    asm volatile(".Ldma_L_000133:" : : : "memory");
    asm volatile("ld          r16,Y+" : : : "memory", "cc");
    asm volatile("cpi         r30,0x7e" : : : "memory", "cc");
    asm goto("brbc 1, .Ldma_L_000112" : : : "memory", "cc" : L_000112);
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x120 = r16;
    pm_cpu_enable_irq();
    asm volatile("push        r0" : : : "memory", "cc");
    asm volatile("push        r1" : : : "memory", "cc");
    asm volatile("push        r2" : : : "memory", "cc");
    asm volatile("push        r10" : : : "memory", "cc");
    asm volatile("push        r19" : : : "memory", "cc");
    asm volatile("push        r20" : : : "memory", "cc");
    asm volatile("push        r21" : : : "memory", "cc");
    asm volatile("push        r22" : : : "memory", "cc");
    asm volatile("push        r23" : : : "memory", "cc");
    asm volatile("push        r24" : : : "memory", "cc");
    asm volatile("push        r25" : : : "memory", "cc");
    asm volatile("push        r26" : : : "memory", "cc");
    asm volatile("push        r27" : : : "memory", "cc");
    FUN_code_000b32();
    FUN_code_000c20();
    r28 = 0x39;
    asm volatile("" : "+r" (r28));
    r29 = 0x24;
    asm volatile("" : "+r" (r29));
    asm volatile("" : "=r" (r28));
    *(volatile uint8_t *)0x118 = r28;
    asm volatile("" : "=r" (r29));
    *(volatile uint8_t *)0x119 = r29;
    asm volatile("eor         r20,r20" : : : "memory", "cc");
    asm volatile("" : "=r" (r20));
    *(volatile uint8_t *)0x11a = r20;
    asm volatile("st          Y+,r18" : : : "memory", "cc");
    asm volatile("st          Y+,r19" : : : "memory", "cc");
    asm volatile("st          Y+,r16" : : : "memory", "cc");
    {
        register uint8_t *destination asm("r28");
        asm volatile("" : "=y" (destination), "=r" (r20) : : "memory");
        *destination = r20;
        asm volatile("" : : : "memory");
    }
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x114 = r16;
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x115 = r16;
    r16 = 0x94;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x110 = r16;
L_000161:
    asm volatile(".Ldma_L_000161:" : : : "memory");
    r16 = *(volatile uint8_t *)0x111;
    asm volatile("" : "+r" (r16));
    asm goto("sbrs        r16,0x4\n\trjmp .Ldma_L_000161" : : : "memory", "cc" : L_000161);
    r16 = 0x10;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x111 = r16;
    r26 = 0x39;
    asm volatile("" : "+r" (r26));
    r27 = 0x24;
    asm volatile("" : "+r" (r27));
    asm volatile("" : "=r" (r26));
    *(volatile uint8_t *)0x12c = r26;
    asm volatile("" : "=r" (r27));
    *(volatile uint8_t *)0x12d = r27;
    asm volatile("eor         r20,r20" : : : "memory", "cc");
    asm volatile("" : "=r" (r20));
    *(volatile uint8_t *)0x12e = r20;
    r16 = 0x8;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x124 = r16;
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x125 = r16;
L_000177:
    asm volatile(".Ldma_L_000177:" : : : "memory");
    r16 = *(volatile uint8_t *)0x668;
    asm volatile("" : "+r" (r16));
    asm goto("sbrs        r16,0x4\n\trjmp .Ldma_L_000177" : : : "memory", "cc" : L_000177);
    r16 = *(volatile uint8_t *)0x9c2;
    asm volatile("" : "+r" (r16));
    r16 = *(volatile uint8_t *)0x9c3;
    asm volatile("" : "+r" (r16));
    r16 = 0x84;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x120 = r16;
L_000182:
    asm volatile(".Ldma_L_000182:" : : : "memory");
    r16 = *(volatile uint8_t *)0x121;
    asm volatile("" : "+r" (r16));
    asm goto("sbrs        r16,0x4\n\trjmp .Ldma_L_000182" : : : "memory", "cc" : L_000182);
    r16 = 0x10;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x121 = r16;
    asm volatile("ld          r28,X+" : : : "memory", "cc");
    asm volatile("ld          r29,X+" : : : "memory", "cc");
    asm volatile("ld          r30,X+" : : : "memory", "cc");
    asm volatile("" : "=r" (pm_dma_buffer_cursor) : : "memory");
    pm_dma_buffer_cursor += 1;
    asm volatile("" : "+r" (pm_dma_buffer_cursor) : : "memory");
    asm volatile("ld          r0,X+" : : : "memory", "cc");
    asm volatile("ld          r1,X+" : : : "memory", "cc");
    {
        register uint8_t *source asm("r26");
        asm volatile("" : "=x" (source) : : "memory");
        r2 = *source;
        asm volatile("" : "+r" (r2) : : "memory");
    }
    asm volatile("" : "=r" (pm_dma_address));
    pm_dma_current = pm_dma_address;
    asm volatile("" : "+r" (pm_dma_current));
    asm volatile("" : "=r" (r30));
    r22 = r30;
    asm volatile("" : "+r" (r22));
    r26 = 0x39;
    asm volatile("" : "+r" (r26));
    r27 = 0x24;
    asm volatile("" : "+r" (r27));
    asm volatile("" : "=r" (r26));
    *(volatile uint8_t *)0x118 = r26;
    asm volatile("" : "=r" (r27));
    *(volatile uint8_t *)0x119 = r27;
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("st          X+,r16" : : : "memory", "cc");
    asm volatile("st          X+,r16" : : : "memory", "cc");
    asm volatile("st          X+,r16" : : : "memory", "cc");
    {
        register uint8_t *destination asm("r26");
        asm volatile("" : "=x" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x11a = r16;
    r16 = 0x4;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x114 = r16;
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x115 = r16;
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x116 = r16;
L_0001a7:
    asm volatile(".Ldma_L_0001a7:" : : : "memory");
    r16 = *(volatile uint8_t *)0x668;
    asm volatile("" : "+r" (r16));
    asm goto("sbrs        r16,0x4\n\trjmp .Ldma_L_0001a7" : : : "memory", "cc" : L_0001a7);
    r16 = 0xb4;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x110 = r16;
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    r26 = 0x35;
    asm volatile("" : "+r" (r26));
    r27 = 0x24;
    asm volatile("" : "+r" (r27));
    asm volatile("st          X+,r16" : : : "memory", "cc");
    asm volatile("st          X+,r16" : : : "memory", "cc");
    asm volatile("st          X+,r16" : : : "memory", "cc");
    {
        register uint8_t *destination asm("r26");
        asm volatile("" : "=x" (destination), "=r" (r16) : : "memory");
        *destination = r16;
        asm volatile("" : : : "memory");
    }
L_0001b5:
    asm volatile(".Ldma_L_0001b5:" : : : "memory");
    FUN_code_000b88();
L_0001b7:
    asm volatile(".Ldma_L_0001b7:" : : : "memory");
    {
        register uint16_t copied_word asm("r0");
        register uint16_t destination_word asm("r16");
        asm volatile("" : "=r" (copied_word) : : "memory");
        destination_word = copied_word;
        asm volatile("" : "+r" (destination_word) : : "memory");
    }
    asm volatile("" : "=r" (r2));
    r18 = r2;
    asm volatile("" : "+r" (r18));
    {
        register uint8_t destination asm("r16");
        register uint8_t operand asm("r20");
        asm volatile("" : "=r" (destination), "=r" (operand) : : "memory");
        destination -= operand;
        asm volatile("" : "+r" (destination) : "r" (operand) : "memory");
    }
    asm volatile("sbc         r17,r21" : : : "memory", "cc");
    asm volatile("sbc         r18,r22" : : : "memory", "cc");
    {
        register __uint24 remaining_bytes asm("r16");
        asm volatile("" : "=r" (remaining_bytes) : : "memory");
        ++remaining_bytes;
        asm volatile("" : "+r" (remaining_bytes) : : "memory");
    }
    asm volatile("and         r18,r18" : : : "memory", "cc");
    asm goto("brbc 1, .Ldma_L_0001c7" : : : "memory", "cc" : L_0001c7);
    r18 = 0x1;
    asm volatile("" : "+r" (r18));
    asm volatile("cp          r16,r18" : : : "memory", "cc");
    asm volatile("cpc         r17,r18" : : : "memory", "cc");
    asm goto("brbc 0, .Ldma_L_0001c7" : : : "memory", "cc" : L_0001c7);
    r31 = 0x1;
    asm volatile("" : "+r" (r31));
    goto L_0001ca;
L_0001c7:
    asm volatile(".Ldma_L_0001c7:" : : : "memory");
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    r17 = 0x1;
    asm volatile("" : "+r" (r17));
    asm volatile("eor         r31,r31" : : : "memory", "cc");
L_0001ca:
    asm volatile(".Ldma_L_0001ca:" : : : "memory");
    r26 = 0x35;
    asm volatile("" : "+r" (r26));
    r27 = 0x22;
    asm volatile("" : "+r" (r27));
    r18 = *(volatile uint8_t *)0x2435;
    asm volatile("" : "+r" (r18));
    asm volatile("" : "=r" (r18), "=r" (r27));
    r27 += r18;
    asm volatile("" : "+r" (r27));
    r19 = 0x1;
    asm volatile("" : "+r" (r19));
    asm volatile("" : "=r" (r18), "=r" (r19));
    r18 ^= r19;
    asm volatile("" : "+r" (r18));
    asm volatile("" : "=r" (r18));
    *(volatile uint8_t *)0x2435 = r18;
    asm volatile("" : "=r" (r26));
    *(volatile uint8_t *)0x12c = r26;
    asm volatile("" : "=r" (r27));
    *(volatile uint8_t *)0x12d = r27;
    asm volatile("eor         r20,r20" : : : "memory", "cc");
    asm volatile("" : "=r" (r20));
    *(volatile uint8_t *)0x12e = r20;
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x124 = r16;
    asm volatile("" : "=r" (r17));
    *(volatile uint8_t *)0x125 = r17;
    asm volatile("rcall       FUN_code_000236" : : : "memory", "cc");
    r16 = 0x84;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x120 = r16;
L_0001e2:
    asm volatile(".Ldma_L_0001e2:" : : : "memory");
    r16 = *(volatile uint8_t *)0x121;
    asm volatile("" : "+r" (r16));
    asm goto("sbrs        r16,0x4\n\trjmp .Ldma_L_0001e2" : : : "memory", "cc" : L_0001e2);
    r16 = 0x10;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x121 = r16;
    asm volatile("and         r31,r31" : : : "memory", "cc");
    asm goto("brbs 1, .Ldma_L_000204" : : : "memory", "cc" : L_000204);
L_0001eb:
    asm volatile(".Ldma_L_0001eb:" : : : "memory");
    r18 = *(volatile uint8_t *)0x668;
    asm volatile("" : "+r" (r18));
    asm goto("sbrs        r18,0x4\n\trjmp .Ldma_L_0001eb" : : : "memory", "cc" : L_0001eb);
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x110 = r16;
    r26 = 0x39;
    asm volatile("" : "+r" (r26));
    r27 = 0x24;
    asm volatile("" : "+r" (r27));
    asm volatile("" : "=r" (r26));
    *(volatile uint8_t *)0x118 = r26;
    asm volatile("" : "=r" (r27));
    *(volatile uint8_t *)0x119 = r27;
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x11a = r16;
    r16 = 0x8;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x114 = r16;
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x115 = r16;
    r16 = 0xb4;
    asm volatile("" : "+r" (r16));
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x110 = r16;
L_000204:
    asm volatile(".Ldma_L_000204:" : : : "memory");
    FUN_code_000b9d();
    FUN_code_000b59();
    asm goto("brbs 0, .Ldma_L_00020f" : : : "memory", "cc" : L_00020f);
    asm volatile("and         r21,r21" : : : "memory", "cc");
    asm goto("brbs 1, .Ldma_L_00020c" : : : "memory", "cc" : L_00020c);
    goto L_0001b7;
L_00020c:
    asm volatile(".Ldma_L_00020c:" : : : "memory");
    FUN_code_000b9d();
    goto L_0001b5;
L_00020f:
    asm volatile(".Ldma_L_00020f:" : : : "memory");
    FUN_code_000b9d();
    FUN_code_000bb9();
    r26 = 0x3d;
    asm volatile("" : "+r" (r26));
    r27 = 0x24;
    asm volatile("" : "+r" (r27));
    asm volatile("st          X+,r18" : : : "memory", "cc");
    asm volatile("st          X+,r19" : : : "memory", "cc");
    asm volatile("st          X+,r16" : : : "memory", "cc");
    asm volatile("st          X+,r17" : : : "memory", "cc");
    asm volatile("rcall       FUN_code_000236" : : : "memory", "cc");
    asm volatile("eor         r16,r16" : : : "memory", "cc");
    asm volatile("" : "=r" (r16));
    *(volatile uint8_t *)0x100 = r16;
    FUN_code_000b3f();
    asm volatile("pop         r27" : : : "memory", "cc");
    asm volatile("pop         r26" : : : "memory", "cc");
    asm volatile("pop         r25" : : : "memory", "cc");
    asm volatile("pop         r24" : : : "memory", "cc");
    asm volatile("pop         r23" : : : "memory", "cc");
    asm volatile("pop         r22" : : : "memory", "cc");
    asm volatile("pop         r21" : : : "memory", "cc");
    asm volatile("pop         r20" : : : "memory", "cc");
    asm volatile("pop         r19" : : : "memory", "cc");
    asm volatile("pop         r10" : : : "memory", "cc");
    asm volatile("pop         r2" : : : "memory", "cc");
    asm volatile("pop         r1" : : : "memory", "cc");
    asm volatile("pop         r0" : : : "memory", "cc");
L_00022c:
    asm volatile(".Ldma_L_00022c:" : : : "memory");
    asm volatile("pop         r31" : : : "memory", "cc");
    asm volatile("pop         r30" : : : "memory", "cc");
    asm volatile("pop         r29" : : : "memory", "cc");
    asm volatile("pop         r28" : : : "memory", "cc");
    asm volatile("pop         r18" : : : "memory", "cc");
    asm volatile("pop         r17" : : : "memory", "cc");
    asm volatile("pop         r16" : : : "memory", "cc");
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
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 4240 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_dma_channel_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x01e2: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 484;
    }
    case 0x01e4: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 486;
    }
    case 0x01e6: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 488;
    }
    case 0x01e8: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 490;
    }
    case 0x01ea: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 492;
    }
    case 0x01ec: { // push r28
        s->stack[s->depth++] = s->r[28];
        return 494;
    }
    case 0x01ee: { // push r29
        s->stack[s->depth++] = s->r[29];
        return 496;
    }
    case 0x01f0: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 498;
    }
    case 0x01f2: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 500;
    }
    case 0x01f4: { // ldi r17, 0x0A
        s->r[17] = 10;
        return 502;
    }
    case 0x01f6: { // lds r16, 0x0668
        uint16_t address = 1640;
        s->r[16] = pm_read(s, address);
        return 506;
    }
    case 0x01fa: { // sbrc r16, 4
        return (!!(s->r[16] & (1u << 4)) == 0) ? 510 : 508;
    }
    case 0x01fc: { // rjmp .+4
        return 514;
    }
    case 0x01fe: { // dec r17
        s->r[17]--;
        pm_nzv(s, s->r[17], s->r[17] == 127);
        return 512;
    }
    case 0x0200: { // brne .-12
        return (pm_getflag(s, 1) == 0) ? 502 : 514;
    }
    case 0x0202: { // lds r17, 0x0124
        uint16_t address = 292;
        s->r[17] = pm_read(s, address);
        return 518;
    }
    case 0x0206: { // ldi r16, 0x11
        s->r[16] = 17;
        return 520;
    }
    case 0x0208: { // sts 0x0121, r16
        uint16_t address = 289;
        pm_write(s, address, s->r[16]);
        return 524;
    }
    case 0x020c: { // ldi r30, 0x76
        s->r[30] = 118;
        return 526;
    }
    case 0x020e: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 528;
    }
    case 0x0210: { // ldi r28, 0x39
        s->r[28] = 57;
        return 530;
    }
    case 0x0212: { // ldi r29, 0x24
        s->r[29] = 36;
        return 532;
    }
    case 0x0214: { // ldi r16, 0x08
        s->r[16] = 8;
        return 534;
    }
    case 0x0216: { // sub r16, r17
        s->r[16] = pm_sub(s, s->r[16], s->r[17], 0, false);
        return 536;
    }
    case 0x0218: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 538;
    }
    case 0x021a: { // add r28, r16
        s->r[28] = pm_add(s, s->r[28], s->r[16], 0);
        return 540;
    }
    case 0x021c: { // adc r29, r17
        s->r[29] = pm_add(s, s->r[29], s->r[17], pm_getflag(s, CARRY));
        return 542;
    }
    case 0x021e: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 544;
    }
    case 0x0220: { // cpi r16, 0xAA
        pm_sub(s, s->r[16], 170, 0, false);
        return 546;
    }
    case 0x0222: { // breq .+8
        return (pm_getflag(s, 1) == 1) ? 556 : 548;
    }
    case 0x0224: { // lpm r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_golden_flash[address];
        return 550;
    }
    case 0x0226: { // cp r16, r17
        pm_sub(s, s->r[16], s->r[17], 0, false);
        return 552;
    }
    case 0x0228: { // breq .+54
        return (pm_getflag(s, 1) == 1) ? 608 : 554;
    }
    case 0x022a: { // rjmp .+556
        return 1112;
    }
    case 0x022c: { // cpi r17, 0x07
        pm_sub(s, s->r[17], 7, 0, false);
        return 558;
    }
    case 0x022e: { // breq .+16
        return (pm_getflag(s, 1) == 1) ? 576 : 560;
    }
    case 0x0230: { // inc r17
        s->r[17]++;
        pm_nzv(s, s->r[17], s->r[17] == 128);
        return 562;
    }
    case 0x0232: { // cpi r28, 0x41
        pm_sub(s, s->r[28], 65, 0, false);
        return 564;
    }
    case 0x0234: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 568 : 566;
    }
    case 0x0236: { // sbiw r28, 0x08
        uint16_t old = pm_pointer(s, 28);
        uint16_t value = old - 8;
        pm_setpointer(s, 28, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 568;
    }
    case 0x0238: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 570;
    }
    case 0x023a: { // cpi r16, 0xAA
        pm_sub(s, s->r[16], 170, 0, false);
        return 572;
    }
    case 0x023c: { // breq .-18
        return (pm_getflag(s, 1) == 1) ? 556 : 574;
    }
    case 0x023e: { // rjmp .+536
        return 1112;
    }
    case 0x0240: { // cli
        pm_irq(s, false);
        return 578;
    }
    case 0x0242: { // rcall .+2340
        s->calls[s->call_depth++] = 580;
        return 2920;
    }
    case 0x0244: { // sei
        pm_irq(s, true);
        return 582;
    }
    case 0x0246: { // ldi r16, 0x03
        s->r[16] = 3;
        return 584;
    }
    case 0x0248: { // sts 0x2441, r16
        uint16_t address = 9281;
        pm_write(s, address, s->r[16]);
        return 588;
    }
    case 0x024c: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 590;
    }
    case 0x024e: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 592;
    }
    case 0x0250: { // pop r29
        s->r[29] = s->stack[--s->depth];
        return 594;
    }
    case 0x0252: { // pop r28
        s->r[28] = s->stack[--s->depth];
        return 596;
    }
    case 0x0254: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 598;
    }
    case 0x0256: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 600;
    }
    case 0x0258: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 602;
    }
    case 0x025a: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 604;
    }
    case 0x025c: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 606;
    }
    case 0x025e: { // rjmp .+2176
        return 2784;
    }
    case 0x0260: { // cpi r28, 0x41
        pm_sub(s, s->r[28], 65, 0, false);
        return 610;
    }
    case 0x0262: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 614 : 612;
    }
    case 0x0264: { // sbiw r28, 0x08
        uint16_t old = pm_pointer(s, 28);
        uint16_t value = old - 8;
        pm_setpointer(s, 28, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 614;
    }
    case 0x0266: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 616;
    }
    case 0x0268: { // cpi r30, 0x7E
        pm_sub(s, s->r[30], 126, 0, false);
        return 618;
    }
    case 0x026a: { // brne .-72
        return (pm_getflag(s, 1) == 0) ? 548 : 620;
    }
    case 0x026c: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 622;
    }
    case 0x026e: { // sts 0x0120, r16
        uint16_t address = 288;
        pm_write(s, address, s->r[16]);
        return 626;
    }
    case 0x0272: { // sei
        pm_irq(s, true);
        return 628;
    }
    case 0x0274: { // push r0
        s->stack[s->depth++] = s->r[0];
        return 630;
    }
    case 0x0276: { // push r1
        s->stack[s->depth++] = s->r[1];
        return 632;
    }
    case 0x0278: { // push r2
        s->stack[s->depth++] = s->r[2];
        return 634;
    }
    case 0x027a: { // push r10
        s->stack[s->depth++] = s->r[10];
        return 636;
    }
    case 0x027c: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 638;
    }
    case 0x027e: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 640;
    }
    case 0x0280: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 642;
    }
    case 0x0282: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 644;
    }
    case 0x0284: { // push r23
        s->stack[s->depth++] = s->r[23];
        return 646;
    }
    case 0x0286: { // push r24
        s->stack[s->depth++] = s->r[24];
        return 648;
    }
    case 0x0288: { // push r25
        s->stack[s->depth++] = s->r[25];
        return 650;
    }
    case 0x028a: { // push r26
        s->stack[s->depth++] = s->r[26];
        return 652;
    }
    case 0x028c: { // push r27
        s->stack[s->depth++] = s->r[27];
        return 654;
    }
    case 0x028e: { // call 0x1664
        s->calls[s->call_depth++] = 658;
        return 5732;
    }
    case 0x0292: { // call 0x1840
        s->calls[s->call_depth++] = 662;
        return 6208;
    }
    case 0x0296: { // ldi r28, 0x39
        s->r[28] = 57;
        return 664;
    }
    case 0x0298: { // ldi r29, 0x24
        s->r[29] = 36;
        return 666;
    }
    case 0x029a: { // sts 0x0118, r28
        uint16_t address = 280;
        pm_write(s, address, s->r[28]);
        return 670;
    }
    case 0x029e: { // sts 0x0119, r29
        uint16_t address = 281;
        pm_write(s, address, s->r[29]);
        return 674;
    }
    case 0x02a2: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 676;
    }
    case 0x02a4: { // sts 0x011A, r20
        uint16_t address = 282;
        pm_write(s, address, s->r[20]);
        return 680;
    }
    case 0x02a8: { // st Y+, r18
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[18]);
        return 682;
    }
    case 0x02aa: { // st Y+, r19
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[19]);
        return 684;
    }
    case 0x02ac: { // st Y+, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[16]);
        return 686;
    }
    case 0x02ae: { // st Y, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[20]);
        return 688;
    }
    case 0x02b0: { // ldi r16, 0x04
        s->r[16] = 4;
        return 690;
    }
    case 0x02b2: { // sts 0x0114, r16
        uint16_t address = 276;
        pm_write(s, address, s->r[16]);
        return 694;
    }
    case 0x02b6: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 696;
    }
    case 0x02b8: { // sts 0x0115, r16
        uint16_t address = 277;
        pm_write(s, address, s->r[16]);
        return 700;
    }
    case 0x02bc: { // ldi r16, 0x94
        s->r[16] = 148;
        return 702;
    }
    case 0x02be: { // sts 0x0110, r16
        uint16_t address = 272;
        pm_write(s, address, s->r[16]);
        return 706;
    }
    case 0x02c2: { // lds r16, 0x0111
        uint16_t address = 273;
        s->r[16] = pm_read(s, address);
        return 710;
    }
    case 0x02c6: { // sbrs r16, 4
        return (!!(s->r[16] & (1u << 4)) == 1) ? 714 : 712;
    }
    case 0x02c8: { // rjmp .-8
        return 706;
    }
    case 0x02ca: { // ldi r16, 0x10
        s->r[16] = 16;
        return 716;
    }
    case 0x02cc: { // sts 0x0111, r16
        uint16_t address = 273;
        pm_write(s, address, s->r[16]);
        return 720;
    }
    case 0x02d0: { // ldi r26, 0x39
        s->r[26] = 57;
        return 722;
    }
    case 0x02d2: { // ldi r27, 0x24
        s->r[27] = 36;
        return 724;
    }
    case 0x02d4: { // sts 0x012C, r26
        uint16_t address = 300;
        pm_write(s, address, s->r[26]);
        return 728;
    }
    case 0x02d8: { // sts 0x012D, r27
        uint16_t address = 301;
        pm_write(s, address, s->r[27]);
        return 732;
    }
    case 0x02dc: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 734;
    }
    case 0x02de: { // sts 0x012E, r20
        uint16_t address = 302;
        pm_write(s, address, s->r[20]);
        return 738;
    }
    case 0x02e2: { // ldi r16, 0x08
        s->r[16] = 8;
        return 740;
    }
    case 0x02e4: { // sts 0x0124, r16
        uint16_t address = 292;
        pm_write(s, address, s->r[16]);
        return 744;
    }
    case 0x02e8: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 746;
    }
    case 0x02ea: { // sts 0x0125, r16
        uint16_t address = 293;
        pm_write(s, address, s->r[16]);
        return 750;
    }
    case 0x02ee: { // lds r16, 0x0668
        uint16_t address = 1640;
        s->r[16] = pm_read(s, address);
        return 754;
    }
    case 0x02f2: { // sbrs r16, 4
        return (!!(s->r[16] & (1u << 4)) == 1) ? 758 : 756;
    }
    case 0x02f4: { // rjmp .-8
        return 750;
    }
    case 0x02f6: { // lds r16, 0x09C2
        uint16_t address = 2498;
        s->r[16] = pm_read(s, address);
        return 762;
    }
    case 0x02fa: { // lds r16, 0x09C3
        uint16_t address = 2499;
        s->r[16] = pm_read(s, address);
        return 766;
    }
    case 0x02fe: { // ldi r16, 0x84
        s->r[16] = 132;
        return 768;
    }
    case 0x0300: { // sts 0x0120, r16
        uint16_t address = 288;
        pm_write(s, address, s->r[16]);
        return 772;
    }
    case 0x0304: { // lds r16, 0x0121
        uint16_t address = 289;
        s->r[16] = pm_read(s, address);
        return 776;
    }
    case 0x0308: { // sbrs r16, 4
        return (!!(s->r[16] & (1u << 4)) == 1) ? 780 : 778;
    }
    case 0x030a: { // rjmp .-8
        return 772;
    }
    case 0x030c: { // ldi r16, 0x10
        s->r[16] = 16;
        return 782;
    }
    case 0x030e: { // sts 0x0121, r16
        uint16_t address = 289;
        pm_write(s, address, s->r[16]);
        return 786;
    }
    case 0x0312: { // ld r28, X+
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        s->r[28] = pm_read(s, address);
        return 788;
    }
    case 0x0314: { // ld r29, X+
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        s->r[29] = pm_read(s, address);
        return 790;
    }
    case 0x0316: { // ld r30, X+
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        s->r[30] = pm_read(s, address);
        return 792;
    }
    case 0x0318: { // adiw r26, 0x01
        uint16_t old = pm_pointer(s, 26);
        uint16_t value = old + 1;
        pm_setpointer(s, 26, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 794;
    }
    case 0x031a: { // ld r0, X+
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        s->r[0] = pm_read(s, address);
        return 796;
    }
    case 0x031c: { // ld r1, X+
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        s->r[1] = pm_read(s, address);
        return 798;
    }
    case 0x031e: { // ld r2, X
        uint16_t address = pm_pointer(s, 26) + 0;
        s->r[2] = pm_read(s, address);
        return 800;
    }
    case 0x0320: { // movw r20, r28
        uint16_t pair = pm_pointer(s, 28);
        pm_setpointer(s, 20, pair);
        return 802;
    }
    case 0x0322: { // mov r22, r30
        s->r[22] = s->r[30];
        return 804;
    }
    case 0x0324: { // ldi r26, 0x39
        s->r[26] = 57;
        return 806;
    }
    case 0x0326: { // ldi r27, 0x24
        s->r[27] = 36;
        return 808;
    }
    case 0x0328: { // sts 0x0118, r26
        uint16_t address = 280;
        pm_write(s, address, s->r[26]);
        return 812;
    }
    case 0x032c: { // sts 0x0119, r27
        uint16_t address = 281;
        pm_write(s, address, s->r[27]);
        return 816;
    }
    case 0x0330: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 818;
    }
    case 0x0332: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 820;
    }
    case 0x0334: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 822;
    }
    case 0x0336: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 824;
    }
    case 0x0338: { // st X, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_write(s, address, s->r[16]);
        return 826;
    }
    case 0x033a: { // sts 0x011A, r16
        uint16_t address = 282;
        pm_write(s, address, s->r[16]);
        return 830;
    }
    case 0x033e: { // ldi r16, 0x04
        s->r[16] = 4;
        return 832;
    }
    case 0x0340: { // sts 0x0114, r16
        uint16_t address = 276;
        pm_write(s, address, s->r[16]);
        return 836;
    }
    case 0x0344: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 838;
    }
    case 0x0346: { // sts 0x0115, r16
        uint16_t address = 277;
        pm_write(s, address, s->r[16]);
        return 842;
    }
    case 0x034a: { // sts 0x0116, r16
        uint16_t address = 278;
        pm_write(s, address, s->r[16]);
        return 846;
    }
    case 0x034e: { // lds r16, 0x0668
        uint16_t address = 1640;
        s->r[16] = pm_read(s, address);
        return 850;
    }
    case 0x0352: { // sbrs r16, 4
        return (!!(s->r[16] & (1u << 4)) == 1) ? 854 : 852;
    }
    case 0x0354: { // rjmp .-8
        return 846;
    }
    case 0x0356: { // ldi r16, 0xB4
        s->r[16] = 180;
        return 856;
    }
    case 0x0358: { // sts 0x0110, r16
        uint16_t address = 272;
        pm_write(s, address, s->r[16]);
        return 860;
    }
    case 0x035c: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 862;
    }
    case 0x035e: { // ldi r26, 0x35
        s->r[26] = 53;
        return 864;
    }
    case 0x0360: { // ldi r27, 0x24
        s->r[27] = 36;
        return 866;
    }
    case 0x0362: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 868;
    }
    case 0x0364: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 870;
    }
    case 0x0366: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 872;
    }
    case 0x0368: { // st X, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_write(s, address, s->r[16]);
        return 874;
    }
    case 0x036a: { // call 0x1710
        s->calls[s->call_depth++] = 878;
        return 5904;
    }
    case 0x036e: { // movw r16, r0
        uint16_t pair = pm_pointer(s, 0);
        pm_setpointer(s, 16, pair);
        return 880;
    }
    case 0x0370: { // mov r18, r2
        s->r[18] = s->r[2];
        return 882;
    }
    case 0x0372: { // sub r16, r20
        s->r[16] = pm_sub(s, s->r[16], s->r[20], 0, false);
        return 884;
    }
    case 0x0374: { // sbc r17, r21
        s->r[17] = pm_sub(s, s->r[17], s->r[21], pm_getflag(s, CARRY), true);
        return 886;
    }
    case 0x0376: { // sbc r18, r22
        s->r[18] = pm_sub(s, s->r[18], s->r[22], pm_getflag(s, CARRY), true);
        return 888;
    }
    case 0x0378: { // subi r16, 0xFF
        s->r[16] = pm_sub(s, s->r[16], 255, 0, false);
        return 890;
    }
    case 0x037a: { // sbci r17, 0xFF
        s->r[17] = pm_sub(s, s->r[17], 255, pm_getflag(s, CARRY), true);
        return 892;
    }
    case 0x037c: { // sbci r18, 0xFF
        s->r[18] = pm_sub(s, s->r[18], 255, pm_getflag(s, CARRY), true);
        return 894;
    }
    case 0x037e: { // and r18, r18
        s->r[18] &= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 896;
    }
    case 0x0380: { // brne .+12
        return (pm_getflag(s, 1) == 0) ? 910 : 898;
    }
    case 0x0382: { // ldi r18, 0x01
        s->r[18] = 1;
        return 900;
    }
    case 0x0384: { // cp r16, r18
        pm_sub(s, s->r[16], s->r[18], 0, false);
        return 902;
    }
    case 0x0386: { // cpc r17, r18
        pm_sub(s, s->r[17], s->r[18], pm_getflag(s, CARRY), true);
        return 904;
    }
    case 0x0388: { // brcc .+4
        return (pm_getflag(s, 0) == 0) ? 910 : 906;
    }
    case 0x038a: { // ldi r31, 0x01
        s->r[31] = 1;
        return 908;
    }
    case 0x038c: { // rjmp .+6
        return 916;
    }
    case 0x038e: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 912;
    }
    case 0x0390: { // ldi r17, 0x01
        s->r[17] = 1;
        return 914;
    }
    case 0x0392: { // eor r31, r31
        s->r[31] ^= s->r[31];
        pm_nzv(s, s->r[31], false);
        return 916;
    }
    case 0x0394: { // ldi r26, 0x35
        s->r[26] = 53;
        return 918;
    }
    case 0x0396: { // ldi r27, 0x22
        s->r[27] = 34;
        return 920;
    }
    case 0x0398: { // lds r18, 0x2435
        uint16_t address = 9269;
        s->r[18] = pm_read(s, address);
        return 924;
    }
    case 0x039c: { // add r27, r18
        s->r[27] = pm_add(s, s->r[27], s->r[18], 0);
        return 926;
    }
    case 0x039e: { // ldi r19, 0x01
        s->r[19] = 1;
        return 928;
    }
    case 0x03a0: { // eor r18, r19
        s->r[18] ^= s->r[19];
        pm_nzv(s, s->r[18], false);
        return 930;
    }
    case 0x03a2: { // sts 0x2435, r18
        uint16_t address = 9269;
        pm_write(s, address, s->r[18]);
        return 934;
    }
    case 0x03a6: { // sts 0x012C, r26
        uint16_t address = 300;
        pm_write(s, address, s->r[26]);
        return 938;
    }
    case 0x03aa: { // sts 0x012D, r27
        uint16_t address = 301;
        pm_write(s, address, s->r[27]);
        return 942;
    }
    case 0x03ae: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 944;
    }
    case 0x03b0: { // sts 0x012E, r20
        uint16_t address = 302;
        pm_write(s, address, s->r[20]);
        return 948;
    }
    case 0x03b4: { // sts 0x0124, r16
        uint16_t address = 292;
        pm_write(s, address, s->r[16]);
        return 952;
    }
    case 0x03b8: { // sts 0x0125, r17
        uint16_t address = 293;
        pm_write(s, address, s->r[17]);
        return 956;
    }
    case 0x03bc: { // rcall .+174
        s->calls[s->call_depth++] = 958;
        return 1132;
    }
    case 0x03be: { // ldi r16, 0x84
        s->r[16] = 132;
        return 960;
    }
    case 0x03c0: { // sts 0x0120, r16
        uint16_t address = 288;
        pm_write(s, address, s->r[16]);
        return 964;
    }
    case 0x03c4: { // lds r16, 0x0121
        uint16_t address = 289;
        s->r[16] = pm_read(s, address);
        return 968;
    }
    case 0x03c8: { // sbrs r16, 4
        return (!!(s->r[16] & (1u << 4)) == 1) ? 972 : 970;
    }
    case 0x03ca: { // rjmp .-8
        return 964;
    }
    case 0x03cc: { // ldi r16, 0x10
        s->r[16] = 16;
        return 974;
    }
    case 0x03ce: { // sts 0x0121, r16
        uint16_t address = 289;
        pm_write(s, address, s->r[16]);
        return 978;
    }
    case 0x03d2: { // and r31, r31
        s->r[31] &= s->r[31];
        pm_nzv(s, s->r[31], false);
        return 980;
    }
    case 0x03d4: { // breq .+50
        return (pm_getflag(s, 1) == 1) ? 1032 : 982;
    }
    case 0x03d6: { // lds r18, 0x0668
        uint16_t address = 1640;
        s->r[18] = pm_read(s, address);
        return 986;
    }
    case 0x03da: { // sbrs r18, 4
        return (!!(s->r[18] & (1u << 4)) == 1) ? 990 : 988;
    }
    case 0x03dc: { // rjmp .-8
        return 982;
    }
    case 0x03de: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 992;
    }
    case 0x03e0: { // sts 0x0110, r16
        uint16_t address = 272;
        pm_write(s, address, s->r[16]);
        return 996;
    }
    case 0x03e4: { // ldi r26, 0x39
        s->r[26] = 57;
        return 998;
    }
    case 0x03e6: { // ldi r27, 0x24
        s->r[27] = 36;
        return 1000;
    }
    case 0x03e8: { // sts 0x0118, r26
        uint16_t address = 280;
        pm_write(s, address, s->r[26]);
        return 1004;
    }
    case 0x03ec: { // sts 0x0119, r27
        uint16_t address = 281;
        pm_write(s, address, s->r[27]);
        return 1008;
    }
    case 0x03f0: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1010;
    }
    case 0x03f2: { // sts 0x011A, r16
        uint16_t address = 282;
        pm_write(s, address, s->r[16]);
        return 1014;
    }
    case 0x03f6: { // ldi r16, 0x08
        s->r[16] = 8;
        return 1016;
    }
    case 0x03f8: { // sts 0x0114, r16
        uint16_t address = 276;
        pm_write(s, address, s->r[16]);
        return 1020;
    }
    case 0x03fc: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1022;
    }
    case 0x03fe: { // sts 0x0115, r16
        uint16_t address = 277;
        pm_write(s, address, s->r[16]);
        return 1026;
    }
    case 0x0402: { // ldi r16, 0xB4
        s->r[16] = 180;
        return 1028;
    }
    case 0x0404: { // sts 0x0110, r16
        uint16_t address = 272;
        pm_write(s, address, s->r[16]);
        return 1032;
    }
    case 0x0408: { // call 0x173a
        s->calls[s->call_depth++] = 1036;
        return 5946;
    }
    case 0x040c: { // call 0x16b2
        s->calls[s->call_depth++] = 1040;
        return 5810;
    }
    case 0x0410: { // brcs .+12
        return (pm_getflag(s, 0) == 1) ? 1054 : 1042;
    }
    case 0x0412: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 1044;
    }
    case 0x0414: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 1048 : 1046;
    }
    case 0x0416: { // rjmp .-170
        return 878;
    }
    case 0x0418: { // call 0x173a
        s->calls[s->call_depth++] = 1052;
        return 5946;
    }
    case 0x041c: { // rjmp .-180
        return 874;
    }
    case 0x041e: { // call 0x173a
        s->calls[s->call_depth++] = 1058;
        return 5946;
    }
    case 0x0422: { // call 0x1772
        s->calls[s->call_depth++] = 1062;
        return 6002;
    }
    case 0x0426: { // ldi r26, 0x3D
        s->r[26] = 61;
        return 1064;
    }
    case 0x0428: { // ldi r27, 0x24
        s->r[27] = 36;
        return 1066;
    }
    case 0x042a: { // st X+, r18
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[18]);
        return 1068;
    }
    case 0x042c: { // st X+, r19
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[19]);
        return 1070;
    }
    case 0x042e: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 1072;
    }
    case 0x0430: { // st X+, r17
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[17]);
        return 1074;
    }
    case 0x0432: { // rcall .+56
        s->calls[s->call_depth++] = 1076;
        return 1132;
    }
    case 0x0434: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1078;
    }
    case 0x0436: { // sts 0x0100, r16
        uint16_t address = 256;
        pm_write(s, address, s->r[16]);
        return 1082;
    }
    case 0x043a: { // call 0x167e
        s->calls[s->call_depth++] = 1086;
        return 5758;
    }
    case 0x043e: { // pop r27
        s->r[27] = s->stack[--s->depth];
        return 1088;
    }
    case 0x0440: { // pop r26
        s->r[26] = s->stack[--s->depth];
        return 1090;
    }
    case 0x0442: { // pop r25
        s->r[25] = s->stack[--s->depth];
        return 1092;
    }
    case 0x0444: { // pop r24
        s->r[24] = s->stack[--s->depth];
        return 1094;
    }
    case 0x0446: { // pop r23
        s->r[23] = s->stack[--s->depth];
        return 1096;
    }
    case 0x0448: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 1098;
    }
    case 0x044a: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 1100;
    }
    case 0x044c: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 1102;
    }
    case 0x044e: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 1104;
    }
    case 0x0450: { // pop r10
        s->r[10] = s->stack[--s->depth];
        return 1106;
    }
    case 0x0452: { // pop r2
        s->r[2] = s->stack[--s->depth];
        return 1108;
    }
    case 0x0454: { // pop r1
        s->r[1] = s->stack[--s->depth];
        return 1110;
    }
    case 0x0456: { // pop r0
        s->r[0] = s->stack[--s->depth];
        return 1112;
    }
    case 0x0458: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 1114;
    }
    case 0x045a: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 1116;
    }
    case 0x045c: { // pop r29
        s->r[29] = s->stack[--s->depth];
        return 1118;
    }
    case 0x045e: { // pop r28
        s->r[28] = s->stack[--s->depth];
        return 1120;
    }
    case 0x0460: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 1122;
    }
    case 0x0462: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 1124;
    }
    case 0x0464: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 1126;
    }
    case 0x0466: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 1128;
    }
    case 0x0468: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 1130;
    }
    case 0x046a: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
