/**
 * @file    link_handshake.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements link bring-up and handshake logic with external controller.
 * Performs initialization exchange, status checks and retry/timeout rules.
 * Maintains link state used by the PM FSM for readiness and operation.
 * Provides diagnostic/status reporting hooks for console commands.
 */

#include "link_handshake.h"

/* Small fixed delays to mimic original busy-wait pacing */
static inline void tiny_delay(void)
{
    __asm__ __volatile__("nop\n\tnop\n\tnop\n\t");
}

/* -------------------- init ------------------------------------------------ */
void link_port_init(void)
{
    /* PD4, PD5, PD7 as outputs; PD6 as input */
    LINK_PORT_D.DIRSET = (LINK_PD4_bm | LINK_PD5_bm | LINK_PD7_bm);
    LINK_PORT_D.DIRCLR = (LINK_PD6_bm);

    /* PE0 as output (used as auxiliary control) */
    LINK_PORT_E.DIRSET = (LINK_PE0_bm);

    /* Idle levels: all driven-low */
    LINK_PORT_D.OUTCLR = (LINK_PD4_bm | LINK_PD5_bm | LINK_PD7_bm);
    LINK_PORT_E.OUTCLR = (LINK_PE0_bm);
}

/* -------------------- single-line setters -------------------------------- */
void link_set_pd4(bool high) { if (high) LINK_PORT_D.OUTSET = LINK_PD4_bm; else LINK_PORT_D.OUTCLR = LINK_PD4_bm; }
void link_set_pd5(bool high) { if (high) LINK_PORT_D.OUTSET = LINK_PD5_bm; else LINK_PORT_D.OUTCLR = LINK_PD5_bm; }
void link_set_pd7(bool high) { if (high) LINK_PORT_D.OUTSET = LINK_PD7_bm; else LINK_PORT_D.OUTCLR = LINK_PD7_bm; }
void link_set_pe0(bool high) { if (high) LINK_PORT_E.OUTSET = LINK_PE0_bm; else LINK_PORT_E.OUTCLR = LINK_PE0_bm; }

/* -------------------- wait for FPGA ack on PD6 --------------------------- */
bool link_wait_pd6_ready(uint16_t max_loops)
{
    while (max_loops--)
    {
        if (LINK_PORT_D.IN & LINK_PD6_bm)
            return true; /* PD6 high = FPGA acknowledges */
        tiny_delay();
    }
    return false; /* timeout */
}

/* -------------------- handshake + DMA kick ------------------------------- */
bool link_handshake_pd4_then_kick(uint16_t max_loops, link_dma_kick_fn_t dma_kick)
{
    /* 1) assert PD4 toward FPGA */
    LINK_PORT_D.OUTSET = LINK_PD4_bm;

    /* 2) wait for PD6=1 (FPGA ready) */
    bool ok = link_wait_pd6_ready(max_loops);

    /* 3) fire DMA (or other start) if provided */
    if (ok && dma_kick) dma_kick();

    /* 4) deassert PD4 */
    LINK_PORT_D.OUTCLR = LINK_PD4_bm;

    return ok;
}

/* -------------------- 4-phase strobes (burst alignment) ------------------ */
void link_burst4_phased_strobes(void)
{
    /* Phase A: PD4 high */
    LINK_PORT_D.OUTSET = LINK_PD4_bm;
    tiny_delay();

    /* Phase B: PD5 high, PD4 stays */
    LINK_PORT_D.OUTSET = LINK_PD5_bm;
    tiny_delay();

    /* Phase C: PD7 high, PD4/PD5 stay */
    LINK_PORT_D.OUTSET = LINK_PD7_bm;
    tiny_delay();

    /* Phase D: drop all (end of burst) */
    LINK_PORT_D.OUTCLR = (LINK_PD4_bm | LINK_PD5_bm | LINK_PD7_bm);
    tiny_delay();
}

