/**
 * @file    clock_integration.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Integrates external clock selection and PLL lock monitoring in PM.
 * Polls lock status, generates optional sync pulses and caches state.
 * Updates clock-related LEDs/flags and exposes summarized clock status.
 * Bridges low-level PLL programming with system FSM requirements.
 */

#include "clock_integration.h"

/* Cached state */
static volatile bool g_clk_selected_present = false;
static volatile bool g_pll_locked = false;

/* Small pacing to mimic original busy-waits in the binary */
static inline void tiny_guard_delay(void)
{
    __asm__ __volatile__("nop\n\tnop\n\tnop\n\t");
}

void clock_integr_init(void)
{
    /* Initialize low-level PLL porting (idle levels and directions) */
    pll_port_init();

    /* Conservative LED state at boot:
     * - show error until we know we have a valid clock and lock
     * - LED_CLK_IN off, LED_CLK_ERR on
     */
    led_status_set_clk_in(false);
    led_status_set_clk_err(true);

    g_clk_selected_present = false;
    g_pll_locked = pll_is_locked();
    clock_integr_render_leds();
}

void clock_integr_set_selected_present(bool present)
{
    g_clk_selected_present = present;
    clock_integr_render_leds();
}

bool clock_integr_read_lock(void)
{
    g_pll_locked = pll_is_locked();
    return g_pll_locked;
}

bool clock_integr_wait_lock(uint16_t max_loops)
{
    /* Delegate to low-level wait (busy-wait) */
    bool ok = pll_wait_lock(max_loops);
    g_pll_locked = ok;
    clock_integr_render_leds();
    return ok;
}

void clock_integr_pulse_sync(void)
{
    /* SYNC pulse is harmless even if not locked yet,
     * but usually called right after lock is observed. */
    pll_pulse_sync();
    tiny_guard_delay();
}

void clock_integr_render_leds(void)
{
    /* Policy:
     *   IN  = selected_present && pll_locked
     *   ERR = !selected_present || !pll_locked
     */
    const bool in_on  = (g_clk_selected_present && g_pll_locked);
    const bool err_on = (!g_clk_selected_present || !g_pll_locked);

    led_status_set_clk_in(in_on);
    led_status_set_clk_err(err_on);
}

void clock_integr_get_status(clock_status_t* out)
{
    if (!out) return;
    out->clk_selected_present = g_clk_selected_present;
    out->pll_locked           = g_pll_locked;
}

