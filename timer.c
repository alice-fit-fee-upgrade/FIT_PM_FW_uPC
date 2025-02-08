#include "fpga.h"
#include "io.h"
#include "ths788.h"
#include "timer.h"
#include "drivers/TC_driver.h"
#include "drivers/port_driver.h"

volatile bool b_report_rdy = false;
volatile uint16_t report_cnt = 1000;

void timer_init(void)
{
    // 32MHZ / 256 divider & 125 ticks period => 1ms
    TC_SetPeriod(&TCC0, 125);
    TC0_ConfigWGM(&TCC0, 0);
    TCC0.CTRLE = 0;
    TC0_ConfigClockSource(&TCC0, TC_CLKSEL_DIV256_gc);
    TC0_SetOverflowIntLevel(&TCC0, TC_OVFINTLVL_LO_gc);

    return;
}

void timer_tc_set_state(struct timer_state *p_ts, uint8_t state)
{
    p_ts->state = state;

    return;
}

void timer_tc_set_value_ms(struct timer_state *p_ts, uint16_t timer)
{
    p_ts->counter = timer;

    return;
}

ISR(TCC0_OVF_vect)
{
    /* Temporary counter */
    if (1 == report_cnt)
    {
        b_report_rdy = true;
    }
    else if (0 == report_cnt)
    {
        
    }
    else
    {
        --report_cnt;
    }

    /* Check THS788 state and timer */
    struct timer_state *p_ts_ths788 = &system_timers_get()->ts_ths788;
    switch (p_ts_ths788->state)
    {
        /* Do nothing, state 0 is a "nop" state */
        case 0:
        {
            break;
        }
        /* Check OT alarm, initialize Control Regs and  */
        case 1:
        {
            if (0 == --p_ts_ths788->counter)
            {
                timer_tc_set_state(p_ts_ths788, 2);

                PORT_ClearPins(&PORTF, 0b00100000); 
                PORT_SetPins(&PORTF, 0b00100000); // CDCE62005RGZT PLL_SYNC clr->set
                PORT_SetPins(&PORTB, 0b00100000); // THS788 RESET
                PORT_SetPins(&PORTC, 0b00001000); // CDCF5801ADBQ ADC_PLL_PWRDNB

                /* THS788 Overtemperature alarm */
                if( ths788_ot_alarm_get() == 0)
                {
                    // DAT_mem_2157 = DAT_mem_2157 | 0x40; TODO THS788 set alarm status
                    PORT_ClearPins(&PORTA, 0b01000000); // LED_CLK_ERR
                    io_psu_disable();
                }
                /* Enable interrupts & send configuration data (Positive edge sync and positive edge hit calibration) */
                else 
                {
                    ths788_ot_intr_enable();

                    // 0x80 0x00 0xa6 0x03
                    ths788_write();
                    // 0x81 0x02 0x00 0x03
                    ths788_write();
                    uint8_t reg_num = 0x0C;
                    uint8_t strobe_id = 0x00;
                    // Get data from EEPROM 0x1018 to 0x1023;
                    do 
                    {
                        do 
                        {
                            // 0xX0 0x00 0xDATA 0x0a
                            ths788_write();
                        } 
                        while (reg_num != 140);

                        reg_num = 0xc;
                        strobe_id += 1;
                        //in_R21R20 = extraout_R21R20_03;
                        //in_R23R22 = extraout_R23R22_03;
                    } 
                    while (strobe_id != 3);
                }
            }
            break;
        }
        /* Check for TMU READY status set CH.A control registers (0x00, 0x01) */
        case 2:
        {
            // 0x00 0x03 0x00 0x07
            // 0x01 0x00 0x00 0x07
            break;
        }
        /* Check DLL LOCK status for each channel */
        case 3:
        {
            
            break;
        }
        /* Checkif FPGA DONE flag is ready */
        case 4:
        {
            if(fpga_done_get())
            {
                fpga_rst_set();
                timer_tc_set_state(p_ts_ths788, 5);
                timer_tc_set_value_ms(p_ts_ths788, 250);
            }
            break;
        }
        /*  */
        case 5:
        {
            uint8_t data = 0;
            // fpga_send_msg_t1(0x7f,data);

            break;
        }
        default:
        {
            break;
        }
    }

    /* Check FPGA status and timer */
    struct timer_state *p_ts_fpga = &system_timers_get()->ts_fpga;
    switch (p_ts_fpga->state)
    {
        /* [OK] Do nothing, state 0 is a "nop" state */
        case 0:
        {
            break;
        }
        /* [OK] If 5V is present, then enable the following PS
         * and wait 1 second before going to the next state
         * - DA11: LM21215AMHX-1 
         * - DA7:  LTC3649EFE
         * - DA8:  LTC7149EFE
         * - DA12: LM21212MH-1
         * - DA13: LM21215AMHX-1
         * - DA3:  LD49150PT10R
         * - DA4:  LD49150PT12R
         * - DA5:  LT1963EQ-1.8
         */
        case 1:
        {
            if (0 == --p_ts_fpga->counter)
            {   
                /* TODO: if 21ad == 0 -> goto next timer */
                if(!system_status_get()->b_sys_fail)
                {
                    io_psu_enable();
                    timer_tc_set_state(p_ts_fpga, 2);
                    timer_tc_set_value_ms(p_ts_fpga, 1000);
                }
                else
                {
                    timer_tc_set_state(p_ts_fpga, 0);
                }
            }
            break;
        }
        /* [OK] Check for Power Good flag, init system ports and DMA */
        case 2:
        {
            if (0 == --p_ts_fpga->counter)
            {
                if (system_status_get()->b_pwr_ldo_ok)
                {
                    system_init(); // TODO
                    timer_tc_set_state(p_ts_fpga, 3); 
                    timer_tc_set_value_ms(p_ts_fpga, 1000);
                }
                else
                {
                    io_psu_disable();
                    system_status_get()->b_sys_fail = true;
                    io_led_system_fail_update();
                    timer_tc_set_state(p_ts_fpga, 0);
                }
            }
            break;
        }
        /* [OK] Reset CDCE62005, initiate FPGA programming */
        case 3:
        {
            if (0 == --p_ts_fpga->counter)
            { 
                // CDCE62005_reset_control(); TODO 
                fpga_init_set();
                fpga_prog_clr();
                timer_tc_set_state(p_ts_fpga, 4);
                timer_tc_set_value_ms(p_ts_fpga, 5000);
                fpga_prog_set();
            }
            break;
        }
        /* [OK] Wait for FPGA DONE flag */
        case 4:
        {
            if (fpga_done_get())
            {
                timer_tc_set_state(p_ts_fpga, 5);
                timer_tc_set_value_ms(p_ts_fpga, 2560);
                system_status_get()->b_fpga_done_ok = true;
                // FUN_code_0004ef(iVar17,(char)in_R23R22,in_R21R20,uVar8,uVar7); TODO
            }
            else
            {
                if (0 == --p_ts_fpga->counter)
                {
                    system_status_get()->b_sys_fail = true;
                    io_led_system_fail_update();
                    timer_tc_set_state(p_ts_fpga, 0);
                }
            }
            break;
        }
        /* [OK] Check for FPGA RST flag, send initial settings */
        case 5:
        {
            if (fpga_rst_is_set())
            {
                fpga_req_intr_enable();
                if (0 == --p_ts_fpga->counter)
                {
                    timer_tc_set_state(p_ts_fpga, 0);
                    // fpga_send_mcu_ts(); TODO
                    // fpga_init_settings(); TODO
                }   
            }
            break;
        }
        default:
        {
            break;
        }
    }

    /* Check ... status and timer */
}