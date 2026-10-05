#include <avr/io.h>
#include "legacy_cpu.h"
extern void CDCE62005_send_control_settings(void);
#include "legacy_r16_c.h"
/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_recovery_system_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/system_control.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * void pm_system_init_c(void)
 * {
 *     PORTB_OUT=0xbc; PORTB_DIRSET=0xbf;
 *     PORTC_OUT=7; PORTC_DIRSET=0xbf;
 *     PORTD_OUTSET=1; PORTD_DIRSET=0x41;
 *     PORTF_OUTSET=0x10; PORTF_DIRSET=0x30;
 *     SPIC_CTRL=0xd1;
 *     DMA_CTRL=0x40;
 *     while (DMA_CTRL & 0x40u) { }
 *     SPID_CTRL=0x44; DMA_CTRL=0x83;
 *     DMA_CH0_TRIGSRC=0x6a;
 *     DMA_CH0_DESTADDR0=0xc3; DMA_CH0_DESTADDR1=9; DMA_CH0_DESTADDR2=0;
 *     DMA_CH0_ADDRCTRL=0x50;
 *     DMA_CH1_TRIGSRC=0x6a;
 *     DMA_CH1_DESTADDR0=0x39; DMA_CH1_DESTADDR1=0x24; DMA_CH1_DESTADDR2=0;
 *     DMA_CH1_SRCADDR0=0xc3; DMA_CH1_SRCADDR1=9; DMA_CH1_SRCADDR2=0;
 *     DMA_CH1_REPCNT=0;
 *     // Original writes low and high transfer-count bytes separately.
 *     REG8(0x0124)=8; REG8(0x0125)=0;
 *     DMA_CH1_ADDRCTRL=5; DMA_CH1_CTRLA=0xa4; DMA_CH1_CTRLB=1;
 *     pm_pll_write_c(0x00001008UL);
 * }
 */

#define RAM(a) (*(volatile uint8_t *)(a))
static inline void wait_dma_reset(void)
{
    asm volatile ("1: lds r16, %[ctrl]\n\tsbrc r16, 6\n\trjmp 1b"
        : : [ctrl] "n" (_SFR_MEM_ADDR(DMA_CTRL)) : "r16", "memory");
}
static inline void initialize_pll(void)
{
    register uint8_t low asm("r16") = 8;
    asm volatile("" : "+r" (low));
    register uint8_t high asm("r17") = 0x10;
    asm volatile("" : "+r" (high));
    asm volatile (
        "clr r18\n\tclr r19"
        : "+r" (low), "+r" (high) : : "r18", "r19", "memory", "cc");
    pm_cpu_disable_irq();
    asm volatile("" : "+r" (low), "+r" (high) : : "memory");
    CDCE62005_send_control_settings();
    asm volatile("" : "=r" (low), "=r" (high) : : "memory");
    pm_cpu_enable_irq();
}
void system_init(void)
{
    PORTB_OUT=0xbc; PORTB_DIRSET=0xbf;
    PORTC_OUT=7; PORTC_DIRSET=0xbf;
    PORTD_OUTSET=1; PORTD_DIRSET=0x41;
    PORTF_OUTSET=0x10; PORTF_DIRSET=0x30;
    SPIC_CTRL=0xd1; DMA_CTRL=0x40;
    wait_dma_reset();
    SPID_CTRL=0x44;
    register uint8_t dma_control asm("r16")=0x83;
    /* O1 retry536 also failed full-image matching/allocation. */
    /* Unvalidated C equivalent: DMA_CTRL = dma_control; step458 failed
     * matching; keep exact store and the original R16 value contract.
 * Validated flag-aware version: compiled C model below,
 * PASS_INSTRUCTION_TRANSITIONS (docs/logical_c_validation.json).
 * This does not claim the historical value-only candidate preserved all flags. */
    asm volatile ("sts %[ctrl], %[value]" :
        : [ctrl] "n" (_SFR_MEM_ADDR(DMA_CTRL)), [value] "r" (dma_control) : "memory");
    DMA_CH0_TRIGSRC=0x6a;
    DMA_CH0_DESTADDR0=0xc3; DMA_CH0_DESTADDR1=9;
    DMA_CH0_DESTADDR2=pm_scratch_zero();
    DMA_CH0_ADDRCTRL=0x50;
    DMA_CH1_TRIGSRC=0x6a;
    DMA_CH1_DESTADDR0=0x39; DMA_CH1_DESTADDR1=0x24;
    DMA_CH1_DESTADDR2=pm_scratch_zero();
    DMA_CH1_SRCADDR0=0xc3; DMA_CH1_SRCADDR1=9;
    uint8_t zero=pm_scratch_zero();
    DMA_CH1_SRCADDR2=zero; DMA_CH1_REPCNT=zero;
    RAM(0x0124)=8; RAM(0x0125)=pm_scratch_zero();
    DMA_CH1_ADDRCTRL=5; DMA_CH1_CTRLA=0xa4; DMA_CH1_CTRLB=1;
    initialize_pll();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 1120 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_system_init(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0d10: { // ldi r16, 0xBC
        s->r[16] = 188;
        return 3346;
    }
    case 0x0d12: { // sts 0x0624, r16
        uint16_t address = 1572;
        pm_write(s, address, s->r[16]);
        return 3350;
    }
    case 0x0d16: { // ldi r16, 0xBF
        s->r[16] = 191;
        return 3352;
    }
    case 0x0d18: { // sts 0x0621, r16
        uint16_t address = 1569;
        pm_write(s, address, s->r[16]);
        return 3356;
    }
    case 0x0d1c: { // ldi r16, 0x07
        s->r[16] = 7;
        return 3358;
    }
    case 0x0d1e: { // sts 0x0644, r16
        uint16_t address = 1604;
        pm_write(s, address, s->r[16]);
        return 3362;
    }
    case 0x0d22: { // ldi r16, 0xBF
        s->r[16] = 191;
        return 3364;
    }
    case 0x0d24: { // sts 0x0641, r16
        uint16_t address = 1601;
        pm_write(s, address, s->r[16]);
        return 3368;
    }
    case 0x0d28: { // ldi r16, 0x01
        s->r[16] = 1;
        return 3370;
    }
    case 0x0d2a: { // sts 0x0665, r16
        uint16_t address = 1637;
        pm_write(s, address, s->r[16]);
        return 3374;
    }
    case 0x0d2e: { // ldi r16, 0x41
        s->r[16] = 65;
        return 3376;
    }
    case 0x0d30: { // sts 0x0661, r16
        uint16_t address = 1633;
        pm_write(s, address, s->r[16]);
        return 3380;
    }
    case 0x0d34: { // ldi r16, 0x10
        s->r[16] = 16;
        return 3382;
    }
    case 0x0d36: { // sts 0x06A5, r16
        uint16_t address = 1701;
        pm_write(s, address, s->r[16]);
        return 3386;
    }
    case 0x0d3a: { // ldi r16, 0x30
        s->r[16] = 48;
        return 3388;
    }
    case 0x0d3c: { // sts 0x06A1, r16
        uint16_t address = 1697;
        pm_write(s, address, s->r[16]);
        return 3392;
    }
    case 0x0d40: { // ldi r16, 0xD1
        s->r[16] = 209;
        return 3394;
    }
    case 0x0d42: { // sts 0x08C0, r16
        uint16_t address = 2240;
        pm_write(s, address, s->r[16]);
        return 3398;
    }
    case 0x0d46: { // ldi r16, 0x40
        s->r[16] = 64;
        return 3400;
    }
    case 0x0d48: { // sts 0x0100, r16
        uint16_t address = 256;
        pm_write(s, address, s->r[16]);
        return 3404;
    }
    case 0x0d4c: { // lds r16, 0x0100
        uint16_t address = 256;
        s->r[16] = pm_read(s, address);
        return 3408;
    }
    case 0x0d50: { // sbrc r16, 6
        return (!!(s->r[16] & (1u << 6)) == 0) ? 3412 : 3410;
    }
    case 0x0d52: { // rjmp .-8
        return 3404;
    }
    case 0x0d54: { // ldi r16, 0x44
        s->r[16] = 68;
        return 3414;
    }
    case 0x0d56: { // sts 0x09C0, r16
        uint16_t address = 2496;
        pm_write(s, address, s->r[16]);
        return 3418;
    }
    case 0x0d5a: { // ldi r16, 0x83
        s->r[16] = 131;
        return 3420;
    }
    case 0x0d5c: { // sts 0x0100, r16
        uint16_t address = 256;
        pm_write(s, address, s->r[16]);
        return 3424;
    }
    case 0x0d60: { // ldi r16, 0x6A
        s->r[16] = 106;
        return 3426;
    }
    case 0x0d62: { // sts 0x0113, r16
        uint16_t address = 275;
        pm_write(s, address, s->r[16]);
        return 3430;
    }
    case 0x0d66: { // ldi r16, 0xC3
        s->r[16] = 195;
        return 3432;
    }
    case 0x0d68: { // sts 0x011C, r16
        uint16_t address = 284;
        pm_write(s, address, s->r[16]);
        return 3436;
    }
    case 0x0d6c: { // ldi r16, 0x09
        s->r[16] = 9;
        return 3438;
    }
    case 0x0d6e: { // sts 0x011D, r16
        uint16_t address = 285;
        pm_write(s, address, s->r[16]);
        return 3442;
    }
    case 0x0d72: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3444;
    }
    case 0x0d74: { // sts 0x011E, r16
        uint16_t address = 286;
        pm_write(s, address, s->r[16]);
        return 3448;
    }
    case 0x0d78: { // ldi r16, 0x50
        s->r[16] = 80;
        return 3450;
    }
    case 0x0d7a: { // sts 0x0112, r16
        uint16_t address = 274;
        pm_write(s, address, s->r[16]);
        return 3454;
    }
    case 0x0d7e: { // ldi r16, 0x6A
        s->r[16] = 106;
        return 3456;
    }
    case 0x0d80: { // sts 0x0123, r16
        uint16_t address = 291;
        pm_write(s, address, s->r[16]);
        return 3460;
    }
    case 0x0d84: { // ldi r16, 0x39
        s->r[16] = 57;
        return 3462;
    }
    case 0x0d86: { // sts 0x012C, r16
        uint16_t address = 300;
        pm_write(s, address, s->r[16]);
        return 3466;
    }
    case 0x0d8a: { // ldi r16, 0x24
        s->r[16] = 36;
        return 3468;
    }
    case 0x0d8c: { // sts 0x012D, r16
        uint16_t address = 301;
        pm_write(s, address, s->r[16]);
        return 3472;
    }
    case 0x0d90: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3474;
    }
    case 0x0d92: { // sts 0x012E, r16
        uint16_t address = 302;
        pm_write(s, address, s->r[16]);
        return 3478;
    }
    case 0x0d96: { // ldi r16, 0xC3
        s->r[16] = 195;
        return 3480;
    }
    case 0x0d98: { // sts 0x0128, r16
        uint16_t address = 296;
        pm_write(s, address, s->r[16]);
        return 3484;
    }
    case 0x0d9c: { // ldi r16, 0x09
        s->r[16] = 9;
        return 3486;
    }
    case 0x0d9e: { // sts 0x0129, r16
        uint16_t address = 297;
        pm_write(s, address, s->r[16]);
        return 3490;
    }
    case 0x0da2: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3492;
    }
    case 0x0da4: { // sts 0x012A, r16
        uint16_t address = 298;
        pm_write(s, address, s->r[16]);
        return 3496;
    }
    case 0x0da8: { // sts 0x0126, r16
        uint16_t address = 294;
        pm_write(s, address, s->r[16]);
        return 3500;
    }
    case 0x0dac: { // ldi r16, 0x08
        s->r[16] = 8;
        return 3502;
    }
    case 0x0dae: { // sts 0x0124, r16
        uint16_t address = 292;
        pm_write(s, address, s->r[16]);
        return 3506;
    }
    case 0x0db2: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3508;
    }
    case 0x0db4: { // sts 0x0125, r16
        uint16_t address = 293;
        pm_write(s, address, s->r[16]);
        return 3512;
    }
    case 0x0db8: { // ldi r16, 0x05
        s->r[16] = 5;
        return 3514;
    }
    case 0x0dba: { // sts 0x0122, r16
        uint16_t address = 290;
        pm_write(s, address, s->r[16]);
        return 3518;
    }
    case 0x0dbe: { // ldi r16, 0xA4
        s->r[16] = 164;
        return 3520;
    }
    case 0x0dc0: { // sts 0x0120, r16
        uint16_t address = 288;
        pm_write(s, address, s->r[16]);
        return 3524;
    }
    case 0x0dc4: { // ldi r16, 0x01
        s->r[16] = 1;
        return 3526;
    }
    case 0x0dc6: { // sts 0x0121, r16
        uint16_t address = 289;
        pm_write(s, address, s->r[16]);
        return 3530;
    }
    case 0x0dca: { // ldi r16, 0x08
        s->r[16] = 8;
        return 3532;
    }
    case 0x0dcc: { // ldi r17, 0x10
        s->r[17] = 16;
        return 3534;
    }
    case 0x0dce: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 3536;
    }
    case 0x0dd0: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 3538;
    }
    case 0x0dd2: { // cli
        pm_irq(s, false);
        return 3540;
    }
    case 0x0dd4: { // call 0x2486
        s->calls[s->call_depth++] = 3544;
        return 9350;
    }
    case 0x0dd8: { // sei
        pm_irq(s, true);
        return 3546;
    }
    case 0x0dda: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
