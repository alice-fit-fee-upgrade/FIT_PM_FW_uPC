#include <avr/io.h>
#include "legacy_r16.h"
#define RAM(a) (*(volatile uint8_t *)(a))
static inline void wait_dma_reset(void)
{
    asm volatile ("1: lds r16, %[ctrl]\n\tsbrc r16, 6\n\trjmp 1b"
        : : [ctrl] "n" (_SFR_MEM_ADDR(DMA_CTRL)) : "r16", "memory");
}
static inline void initialize_pll(void)
{
    asm volatile (
        "ldi r16, 8\n\tldi r17, 0x10\n\tclr r18\n\tclr r19\n\t"
        "cli\n\tcall CDCE62005_send_control_settings\n\tsei"
        : : : "r16", "r17", "r18", "r19", "memory", "cc");
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
