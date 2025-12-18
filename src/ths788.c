/**
 * @file    ths788.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements GPIO bit-banged driver for the THS788 timing unit in PM.
 * Provides reset, port initialization and serial write sequencing.
 * Uses deterministic delays and bit masks compatible with legacy firmware.
 * Used during bring-up and for configuring timing-related functions.
 */

#include "ths788.h"

/* tiny pacing to mimic original busy-wait */
static inline void tiny_delay(void)
{
    __asm__ __volatile__("nop\n\tnop\n\tnop\n\t");
}

void ths788_port_init(void)
{
    /* PB0..PB5 outputs */
    THS_PORT_B.DIRSET = (THS_HCLK_bm | THS_HDATA_bm | THS_HSTB1_bm |
                         THS_HSTB2_bm | THS_HSTB3_bm | THS_RESETN_bm);

    /* Idle: CLK=0, DATA=0, STROBES=high (inactive), RESETN=1 (released) */
    THS_PORT_B.OUTCLR = (THS_HCLK_bm | THS_HDATA_bm);
    THS_PORT_B.OUTSET = (THS_HSTB1_bm | THS_HSTB2_bm | THS_HSTB3_bm | THS_RESETN_bm);
}

void ths788_reset(bool assert_low)
{
    if (assert_low)
        THS_PORT_B.OUTCLR = THS_RESETN_bm;  /* active-low reset */
    else
        THS_PORT_B.OUTSET = THS_RESETN_bm;  /* release reset */
}

void ths788_reset_pulse(uint16_t cycles)
{
    THS_PORT_B.OUTCLR = THS_RESETN_bm;
    while (cycles--) tiny_delay();
    THS_PORT_B.OUTSET = THS_RESETN_bm;
}

/* select exactly one strobe line low (1..3), release others high */
static inline void strobe_drive(uint8_t idx)
{
    /* release all first */
    THS_PORT_B.OUTSET = (THS_HSTB1_bm | THS_HSTB2_bm | THS_HSTB3_bm);

    switch (idx) {
        case 1: THS_PORT_B.OUTCLR = THS_HSTB1_bm; break;
        case 2: THS_PORT_B.OUTCLR = THS_HSTB2_bm; break;
        case 3: THS_PORT_B.OUTCLR = THS_HSTB3_bm; break;
        default: /* none */ break;
    }
}

void ths788_select_strobe(uint8_t idx) { strobe_drive(idx); }

void ths788_deselect_all(void)
{
    THS_PORT_B.OUTSET = (THS_HSTB1_bm | THS_HSTB2_bm | THS_HSTB3_bm);
}

/* send one bit: set DATA to b, then clock rising edge, then clear clock;
 * for '1' we also observed DATA cleared after clock in some paths, keep symmetry.
 */
static inline void tx_bit(uint8_t b)
{
    if (b) THS_PORT_B.OUTSET = THS_HDATA_bm; else THS_PORT_B.OUTCLR = THS_HDATA_bm;
    tiny_delay();
    THS_PORT_B.OUTSET = THS_HCLK_bm;  /* CLK↑ */
    tiny_delay();
    THS_PORT_B.OUTCLR = THS_HCLK_bm;  /* CLK↓ */
    if (b) THS_PORT_B.OUTCLR = THS_HDATA_bm; /* optional data drop mirrors ASM snippets */
    tiny_delay();
}

void ths788_tx24(uint32_t word)
{
    /* MSB-first: bits 23..0 */
    for (int i = 23; i >= 0; --i) {
        uint8_t bit = (uint8_t)((word >> i) & 1u);
        tx_bit(bit);
    }
}

void ths788_write_reg(uint8_t strobe_idx, uint32_t word24)
{
    strobe_drive(strobe_idx);
    tiny_delay();
    ths788_tx24(word24);
    ths788_deselect_all();
}

