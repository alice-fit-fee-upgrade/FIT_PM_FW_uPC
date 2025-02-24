#pragma once

#include "drivers/port_driver.h"
#include "system.h"

#define PORTD_OUT_FPGA_PROG_PIN_bp  (1)
#define PORTD_OUT_FPGA_PROG_PIN_bm  (1 << PORT_DOUT_FPGA_PROG_PIN_bp)
#define PORTD_OUT_FPGA_INIT_PIN_bp  (2)
#define PORTD_OUT_FPGA_INIT_PIN_bm  (1 << PORTD_OUT_FPGA_INIT_PIN_bp)
#define PORTD_IN_FPGA_DONE_PIN_bp   (3)
#define PORTD_IN_FPGA_DONE_PIN_bm   (1 << PORTD_IN_FPGA_DONE_PIN_bp)

static inline void fpga_done_intr_enable(void)
{
    PORT_ClearInterruptFlags(&PORTD, PORT_INT0IF_bm);
    PORTD_INTCTRL = ( PORTD_INTCTRL & ~PORT_INT0LVL_gm ) | PORT_INT0LVL_MED_gc;
    return;
}

static inline void fpga_req_intr_enable(void)
{
    PORTE_INTCTRL = ( PORTE_INTCTRL & ~PORT_INT1LVL_gm ) | PORT_INT1LVL_LO_gc;
    return;
}

static inline void fpga_rst_set()
{
    PORT_SetOutputBit(&PORTB, 7);
    return;
}

static inline void fpga_rst_clr()
{
    PORT_ClearOutputBit(&PORTB, 7);
    return;
}

static inline bool fpga_rst_is_set()
{
    return !!(PORTB_OUT & 0b10000000);
}

static inline void fpga_cs_select()
{
    PORT_ClearOutputBit(&PORTD, 0);
    return;
}

static inline void fpga_cs_deselect()
{
    PORT_SetOutputBit(&PORTD, 0);
    return;
}

static inline void fpga_prog_set()
{
    PORT_SetOutputBit(&PORTD, PORTD_OUT_FPGA_PROG_PIN_bp);
    return;
}

static inline void fpga_prog_clr()
{
    PORT_ClearOutputBit(&PORTD, PORTD_OUT_FPGA_PROG_PIN_bp);
    return;
}

static inline void fpga_init_set()
{
    PORT_SetOutputBit(&PORTD, PORTD_OUT_FPGA_INIT_PIN_bp);
    return;
}

static inline void fpga_init_clr()
{
    PORT_ClearOutputBit(&PORTD, PORTD_OUT_FPGA_INIT_PIN_bp);
    return; 
}

static inline bool fpga_init_get()
{
    return PORT_GetPinValue(&PORTD, PORTD_OUT_FPGA_INIT_PIN_bp);
}

static inline bool fpga_done_get()
{
    return PORT_GetPinValue(&PORTD, PORTD_IN_FPGA_DONE_PIN_bp);
}

void fpga_init();
void fpga_exchange_data(uint8_t addr, uint16_t *p_data);