/**
 * @file    fpga_config.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements FPGA configuration flows and register access for PM.
 * Loads FPGA images (e.g. from SPI NOR) and manages config sequencing.
 * Provides helpers to read/write FPGA registers used by UART F0 commands.
 * Coordinates with power/clock modules to ensure safe configuration.
 */

#include "fpga_config.h"

/* --- weak hooks default (no-op) ----------------------------------------- */
void __attribute__((weak)) fpga_precheck_nor_flash(void) { /* optional */ }
void __attribute__((weak)) fpga_post_config_hook(void)   { /* optional */ }

/* Small pacing similar to original busy-waits */
static inline void tiny_guard_delay(void)
{
    __asm__ __volatile__("nop\n\tnop\n\tnop\n\t");
}

/* ------------------- port init & basic control -------------------------- */
void fpga_port_init(void)
{
    /* PD1 as output; PD2/PD3 as inputs */
    FPGA_PORT_D.DIRSET = FPGA_PROGRAM_B_bm;
    FPGA_PORT_D.DIRCLR = (FPGA_INIT_B_bm | FPGA_DONE_bm);

    /* Idle: PROGRAM_B high (not asserted) */
    FPGA_PORT_D.OUTSET = FPGA_PROGRAM_B_bm;
}

void fpga_program_assert(bool assert_low)
{
    if (assert_low)
        FPGA_PORT_D.OUTCLR = FPGA_PROGRAM_B_bm;  /* drive low = reset config logic */
    else
        FPGA_PORT_D.OUTSET = FPGA_PROGRAM_B_bm;  /* release (high) */
}

/* ------------------- status reads --------------------------------------- */
bool fpga_read_init_b(void)
{
    /* INIT_B high means device ready to accept configuration */
    return (FPGA_PORT_D.IN & FPGA_INIT_B_bm) != 0;
}

bool fpga_read_done(void)
{
    /* DONE high means configuration completed */
    return (FPGA_PORT_D.IN & FPGA_DONE_bm) != 0;
}

void fpga_read_status(fpga_status_t* st)
{
    if (!st) return;
    uint8_t in  = FPGA_PORT_D.IN;
    uint8_t out = FPGA_PORT_D.OUT;
    st->program_b_high = (out & FPGA_PROGRAM_B_bm) != 0;  /* we control this pin */
    st->init_b_high    = (in  & FPGA_INIT_B_bm)    != 0;
    st->done_high      = (in  & FPGA_DONE_bm)      != 0;
}

/* ------------------- bounded waits -------------------------------------- */
bool fpga_wait_init_low(uint32_t max_loops)
{
    while (max_loops--)
    {
        if (!fpga_read_init_b()) return true; /* observed low */
        tiny_guard_delay();
    }
    return false;
}

bool fpga_wait_init_high(uint32_t max_loops)
{
    while (max_loops--)
    {
        if (fpga_read_init_b()) return true; /* observed high */
        tiny_guard_delay();
    }
    return false;
}

bool fpga_wait_done_high(uint32_t max_loops)
{
    while (max_loops--)
    {
        if (fpga_read_done()) return true; /* observed high */
        tiny_guard_delay();
    }
    return false;
}

/* ------------------- full pipeline -------------------------------------- */
bool fpga_reconfigure_pipeline(uint32_t t_init_low, uint32_t t_init_high, uint32_t t_done)
{
    /* Optional pre-check: NOR flash presence / sanity */
    fpga_precheck_nor_flash();

    /* PROGRAM_B low pulse */
    fpga_program_assert(true);
    tiny_guard_delay();        /* small guard; original used short spin-waits */
    fpga_program_assert(false);

    /* INIT_B should go low, then high */
    if (!fpga_wait_init_low(t_init_low))   return false;
    if (!fpga_wait_init_high(t_init_high)) return false;

    /* DONE should go high to indicate success */
    if (!fpga_wait_done_high(t_done))      return false;

    /* Optional post-configuration hook (e.g., enable links/clock domains) */
    fpga_post_config_hook();

    return true;
}

