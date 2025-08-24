#include "io.h"

void io_init(void)
{
    /* --- PORT A ---
     * LEDS                 PA0, PA5..7
     * TEMPSENS ADT7311     PA1..4 
     */
    PORT_SetDirection(&PORTA, 0b11111011);
    PORT_SetPins(&PORTA, 0b11110011);
    PORT_ConfigurePins(&PORTA, (PIN1_bm | PIN3_bm | PIN4_bm), true, false, PORT_OPC_TOTEM_gc, 0);
    PORT_ConfigurePins(&PORTA, PIN2_bm, false, false, PORT_OPC_PULLUP_gc, 0);

    /* --- PORT B ---
     * TMU THS788           PB0..6
     * FPGA ()              PB7 
     */
    PORT_ConfigurePins(&PORTB, 0b01000000, false, false, PORT_OPC_TOTEM_gc, PORT_ISC_FALLING_gc);
    PORT_ConfigureInterrupt0(&PORTB, PORT_INT0LVL_OFF_gc, PIN6_bm);

    /* --- PORT C ---
     * DAC8554              PC0..3
     * FPGA (EVENT)         PC4
     * CLKGEN CDC62005 SPI  PC5..7
     */
    PORT_ConfigurePins(&PORTC, PIN3_bm, true, false, PORT_OPC_TOTEM_gc, 0);
    PORT_ConfigurePins(&PORTC, PIN4_bm, false, false, PORT_OPC_WIREDOR_gc, 0);

    /* --- PORT D ---
     * FPGA                 PD0..3
     * HDMI                 PD4..7
     */
    PORT_SetDirection(&PORTD, 0b00000110);
    PORT_SetPins(&PORTD, PIN1_bm);
    PORT_ClearPins(&PORTD, PIN2_bm);
    PORT_ConfigurePins(&PORTD, PIN3_bm, false, false, PORT_OPC_TOTEM_gc, PORT_ISC_BOTHEDGES_gc);
    PORT_ConfigurePins(&PORTD, (PIN1_bm | PIN2_bm), false, false, PORT_OPC_WIREDAND_gc, 0);
    PORT_ConfigureInterrupt0(&PORTD, PORT_INT0LVL_OFF_gc, PIN3_bm);

    /* --- PORT E ---
     * FPGA ()              PE0
     * 12V INPUT            PE1
     * PS ENABLE            PE2
     * PS GOOD              PE3
     * FPGA SPI             PE4..7
     */
    PORT_SetDirection(&PORTE, 0b00000100);
    PORT_ConfigurePins(&PORTE, PIN0_bm, false, false, PORT_OPC_TOTEM_gc, PORT_ISC_RISING_gc);
    PORT_ConfigurePins(&PORTE, (PIN1_bm | PIN3_bm), false, false, PORT_OPC_TOTEM_gc, PORT_ISC_BOTHEDGES_gc);
    PORT_ConfigurePins(&PORTE, PIN2_bm, true, false, PORT_OPC_TOTEM_gc, 0);
    PORT_ConfigureInterrupt0(&PORTE, PORT_INT0LVL_OFF_gc, (PIN3_bm | PIN1_bm));
    PORT_ConfigureInterrupt1(&PORTE, PORT_INT0LVL_OFF_gc, PIN0_bm);

    /* --- PORTF ---
     * CLI                  PF0..3
     * CLKGEN CDC62005      PF4..6
     */
    PORT_SetDirection(&PORTF, 0b00001001);
    PORT_SetPins(&PORTF, PIN0_bm);
    PORT_ConfigurePins(&PORTF, (PIN1_bm | PIN6_bm), false, false, PORT_OPC_TOTEM_gc, PORT_ISC_BOTHEDGES_gc);
    PORT_ConfigurePins(&PORTF, PIN7_bm, false, false, PORT_OPC_PULLDOWN_gc, 0);
    PORT_ConfigureInterrupt0(&PORTF, PORT_INT0LVL_OFF_gc, PIN1_bm);
    PORT_ConfigureInterrupt1(&PORTF, PORT_INT0LVL_OFF_gc, PIN6_bm);

    return;
}

/* Handling input power & LDO presence */
ISR(PORTE_INT0_vect)
{
    uint8_t port_e_val = PORT_GetPortValue(&PORTE);
    uint8_t port_e_tgl = system_status_get()->porte_status ^ port_e_val;
    system_status_get()->porte_status = port_e_val;

    /* 12V unchanged */
    if (0 == (port_e_tgl & PORTE_IN_PWR_OK_PIN_bm))
    {
        /* nothing changed */
        if(0 == (port_e_tgl & PORTE_LDO_OK_PIN_bm))
        {
            return;
        }

        /* LDO turned on */
        if (port_e_val & PORTE_LDO_OK_PIN_bm)
        {
            system_status_get()->b_pwr_ldo_ok =  true;
            return;
        }

        /* LDO turned off */
        system_status_get()->b_pwr_ldo_ok =  false;
        if ((DAT_mem_215b != 2) || (DAT_mem_2442 == 0))  // FPGA not in state 2 or tries == 0
        {
            system_status_get()->b_sys_fail = true;
            system_deinit();
            /* PSU not enabled */
            if (0 == (port_e_val & PORTE_EN_PSU_PIN_bm))
            {
                return;
            }
        }
        /* FPGA in state 2 */
        else
        {
            // DAT_mem_2442 -= 1;
            io_psu_disable();
            timer_tc_set_value_ms(&system_timers_get()->ts_fpga, 5000);
        }
    }
    /* 12V turned off */
    else if (0 == (port_e_val & PORTE_IN_PWR_OK_PIN_bm))
    {
        io_psu_disable();
        system_status_get()->b_sys_fail =  true;
        system_status_get()->b_pwr_in_ok =  false;
        system_deinit();
    }
    /* 12V turned on*/
    else
    {
        timer_tc_set_state(&system_timers_get()->ts_fpga, 1);
        timer_tc_set_value_ms(&system_timers_get()->ts_fpga, 2000);
        system_status_get()->b_sys_fail =  false;
    }

    io_led_system_fail_update();
}
