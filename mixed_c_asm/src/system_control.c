#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "pm_addresses.h"
#define REG8(a) (*(volatile uint8_t *)(a))
/* 0x0C96: preserve ordered RAM/peripheral writes, including status RMW. */
void pm_system_deinit_c(void)
{
    REG8(PM_FPGA_TIMER_LOW)=0; REG8(PM_FPGA_TIMER_HIGH)=0;
    REG8(PM_FPGA_STATE)=0; REG8(PM_THS_STATE)=0;
    REG8(PM_RESTART_REASON)=0; REG8(PM_FPGA_REQUEST)=0; REG8(PM_CLOCK_STATE)=0;
    REG8(PM_PORTE_INTCTRL)=1; REG8(PM_PORTF_INTCTRL)=2;
    REG8(PM_PORTB_INTCTRL)=0; REG8(PM_PORTD_INTCTRL)=0;
    REG8(PM_PORTB_DIRCLR)=0xbf; REG8(PM_SPIC_CTRL)=0;
    REG8(PM_PORTC_DIRCLR)=0xff; REG8(PM_PORTD_DIRCLR)=0x41;
    REG8(PM_PORTD_OUTCLR)=4; REG8(PM_PORTF_OUTCLR)=0x20;
    REG8(PM_PORTF_DIRCLR)=0x20;
    REG8(PM_STATUS_FLAGS) &= 0xefu;
    REG8(PM_PORTA_OUTSET)=0xa0; REG8(PM_SPID_CTRL)=0; REG8(PM_DMA_CTRL)=0;
}

extern void pm_pll_write_c(uint32_t value);
/* 0x0D10: exact GPIO/DMA startup order, no added reset timeout. */
void pm_system_init_c(void)
{
    PORTB_OUT=0xbc; PORTB_DIRSET=0xbf;
    PORTC_OUT=7; PORTC_DIRSET=0xbf;
    PORTD_OUTSET=1; PORTD_DIRSET=0x41;
    PORTF_OUTSET=0x10; PORTF_DIRSET=0x30;
    SPIC_CTRL=0xd1;
    DMA_CTRL=0x40;
    while (DMA_CTRL & 0x40u) { }
    SPID_CTRL=0x44; DMA_CTRL=0x83;
    DMA_CH0_TRIGSRC=0x6a;
    DMA_CH0_DESTADDR0=0xc3; DMA_CH0_DESTADDR1=9; DMA_CH0_DESTADDR2=0;
    DMA_CH0_ADDRCTRL=0x50;
    DMA_CH1_TRIGSRC=0x6a;
    DMA_CH1_DESTADDR0=0x39; DMA_CH1_DESTADDR1=0x24; DMA_CH1_DESTADDR2=0;
    DMA_CH1_SRCADDR0=0xc3; DMA_CH1_SRCADDR1=9; DMA_CH1_SRCADDR2=0;
    DMA_CH1_REPCNT=0;
    /* Original writes low and high transfer-count bytes separately. */
    REG8(0x0124)=8; REG8(0x0125)=0;
    DMA_CH1_ADDRCTRL=5; DMA_CH1_CTRLA=0xa4; DMA_CH1_CTRLB=1;
    pm_pll_write_c(0x00001008UL);
}

/* 0x0DDC: original ten 32-bit control words at FLASH byte address 0x2916.
 * Read bytes explicitly, retaining original low-to-high LPM order. */
void pm_pll_control_reset_c(void)
{
    uint16_t address=0x2916;
    for (uint8_t remaining=10; remaining!=0; --remaining) {
        uint8_t b0=pgm_read_byte(address++);
        uint8_t b1=pgm_read_byte(address++);
        uint8_t b2=pgm_read_byte(address++);
        uint8_t b3=pgm_read_byte(address++);
        uint32_t value=(uint32_t)b0 | ((uint32_t)b1<<8)
                     | ((uint32_t)b2<<16) | ((uint32_t)b3<<24);
        pm_pll_write_c(value);
    }
    PORTF_INTCTRL=0x0a;
    PORTB_OUTCLR=0x20;
}
