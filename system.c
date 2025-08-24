#include <avr/eeprom.h>

#include "drivers/port_driver.h"
#include "drivers/spi_driver.h"

#include "fpga.h"
#include "ths788.h"
#include "system.h"

struct system_eeprom *p_ee_params = (struct system_eeprom *)MAPPED_EEPROM_START;
struct system_status system_status = {0};
struct system_timers system_timers = {0};

void system_init()
{
    /* PORTB */
    fpga_rst_set();
    PORT_SetPinsAsOutput(&PORTB, 0b10000000);
    
    /* PORTC */
    PORT_SetPinsAsOutput(&PORTC, 0b10110000);

    /* PORTD */
    fpga_cs_deselect();
    PORT_SetPinsAsOutput(&PORTD, 0b00001001);
    /* TODO: Attenuator settings*/
    /*
    USARTD0_CTRLB = 0x18;
    USARTD0_CTRLA = 0x10;
    */

    /* Configure FPGA SPI */
    fpga_init();

    // PORTC
    /* TODO: Si5338 settings */
    
    TWIC_MASTER_STATUS = 1;
    //si5338_state_get()->case_pri_state = 0;
    //si5338_state_get()->case_sec_state = 0;
    //si5338_state_get()->msg = 0x06;

    /*DAT_mem_2163 = 0;
    DAT_mem_2164 = 0;
    DAT_mem_2165 = 0xfe;
    DAT_mem_2166 = 0x25;*/

    /* TODO: DAT_mem_2168 = 0xe0;*/

    system_status_get()->b_ths788_fail = false;
    /*
    bVar1 = GPIO_GPIOR0;
    GPIO_GPIOR0 = bVar1 & 0b11000111;
    */

    //TWIC_MASTER_ADDR = 0xE0;
    //TWIC_MASTER_CTRLA = 0x58;
    //si5338_twic_master_ctrla_setup();
    
    return;
}

void system_deinit()
{
    /*
    DAT_mem_215c = 0;
  DAT_mem_215d = 0;
  DAT_mem_215b = 0;
  DAT_mem_2159 = 0;
  DAT_mem_2441 = 0;
  DAT_mem_2006 = 0;
  DAT_mem_2162 = 0;
  PORTE_INTCTRL = 1;
  PORTF_INTCTRL = 2;
  PORTB_INTCTRL = 0;
  PORTD_INTCTRL = 0;
  PORTB_DIRCLR = 0xbf;
  SPIC = 0;
  PORTC_DIRCLR = 0xff;
  PORTD_DIRCLR = 0x41;
  PORTD_OUTCLR = 4;
  PORTF_OUTCLR = 0x20;
  PORTF_DIRCLR = 0x20;
  DAT_mem_2157 = DAT_mem_2157 & 0xef;
  PORTA_OUTSET = 0xa0;
  SPID = 0;
  DMA = 0;
  */

    return;
}

void system_reset()
{
    system_status.b_ths788_fail = true;
    PORTC_INTCTRL = 0;
    PORTE_INTCTRL = 1;
    PORTA_OUTSET = 0x80;
    PORTB_OUTCLR = 0x80;

    //DAT_clk_frs = DAT_clk_frs & 0b00001110;

    system_timers.ts_ths788.state = 1;
    system_timers.ts_ths788.counter = 100;

    return;
}

void system_clock_init(void)
{
    CLKSYS_XOSC_Config(OSC_FRQRANGE_12TO16_gc, false, OSC_XOSCSEL_XTAL_16KCLK_gc);
    CLKSYS_Enable(OSC_RC2MEN_bm | OSC_XOSCEN_bm);
    do
    {
    } while (CLKSYS_IsReady(OSC_XOSCRDY_bm) == 0);

    /* PLL: 16MHz x 2 */
    CLKSYS_PLL_Config(OSC_PLLSRC_XOSC_gc, 2);
    CLKSYS_Enable(OSC_PLLEN_bm);
    do
    {
    } while (CLKSYS_IsReady(OSC_PLLRDY_bm) == 0);

    CPU_CCP = CCP_IOREG_gc;
    /* Set PLL as sysclk */
    CLK_CTRL = CLK_SCLKSEL_PLL_gc;
    CLKSYS_Disable(OSC_RC2MEN_bm);

    return;
}

struct system_eeprom *system_eeprom_get(void)
{
    return p_ee_params;
}

struct system_status *system_status_get(void)
{
    return &system_status;
}

struct system_timers *system_timers_get(void)
{
    return &system_timers;
}