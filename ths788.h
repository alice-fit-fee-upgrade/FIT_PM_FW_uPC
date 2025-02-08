#pragma once

#include "drivers/port_driver.h"

#define PORTB_OUT_THS788_HCLK_PIN_bp        (0)
#define PORTB_OUT_THS788_HCLK_PIN_bm        (1 << PORTB_OUT_THS788_HCLK_PIN_bp)
#define PORTB_IO_THS788_DATA_PIN_bp         (1)
#define PORTB_IO_THS788_DATA_PIN_bm         (1 << PORTB_IO_THS788_DATA_PIN_bp)
#define PORTB_OUT_THS788_HSTROBE1_PIN_bp    (2)
#define PORTB_OUT_THS788_HSTROBE1_PIN_bm    (1 << PORTB_OUT_THS788_HSTROBE1_PIN_bp)
#define PORTB_OUT_THS788_HSTROBE2_PIN_bp    (3)
#define PORTB_OUT_THS788_HSTROBE2_PIN_bm    (1 << PORTB_OUT_THS788_HSTROBE2_PIN_bp)
#define PORTB_OUT_THS788_HSTROBE3_PIN_bp    (4)
#define PORTB_OUT_THS788_HSTROBE3_PIN_bm    (1 << PORTB_OUT_THS788_HSTROBE3_PIN_bp)
#define PORTB_OUT_THS788_RST_PIN_bp         (5)
#define PORTB_OUT_THS788_RST_PIN_bm         (1 << PORTB_OUT_THS788_RST_PIN_bp)
#define PORTB_IN_THS788_OTAL_PIN_bp         (6)
#define PORTB_IN_THS788_OTAL_PIN_bm         (1 << PORTB_IN_THS788_OTAL_PIN_bp)

static inline void ths788_ot_intr_enable(void)
{
    PORT_ClearInterruptFlags(&PORTB, PORT_INT0IF_bm);
    PORTB_INTCTRL = ( PORTD_INTCTRL & ~PORT_INT0LVL_gm ) | PORT_INT0LVL_MED_gc;
    return;
}

static inline bool ths788_ot_alarm_get()
{
    return PORT_GetPinValue(&PORTB, PORTB_IN_THS788_OTAL_PIN_bp);
}

void ths788_init(void);