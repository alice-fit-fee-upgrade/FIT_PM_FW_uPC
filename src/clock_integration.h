/**
 * @file    clock_integration.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares clock integration API for PM.
 * Provides functions to select clock sources and poll PLL lock state.
 * Exposes cached clock status used by LED indication and system FSM.
 * Used during bring-up and for diagnostic/status reporting.
 */

#ifndef CLOCK_INTEGRATION_H
#define CLOCK_INTEGRATION_H

#include <stdbool.h>
#include <stdint.h>

/* External deps */
#include "pll_clock.h"   /* PF4/PF5/PF6 + SPIC helpers */
#include "led_status.h"  /* PA5/PA6 LED control */

/* Snapshot of the clock domain status */
typedef struct {
  bool clk_selected_present;  /* upper-layer indication that the selected source is present */
  bool pll_locked;            /* PF6 lock */
} clock_status_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize integration layer:
 *   - ensures PLL ports are initialized (LE/SYNC low, SPI idle)
 *   - sets initial LED state (CLK_ERR asserted until proved otherwise)
 */
void clock_integr_init(void);

/* Upper layer informs whether the currently selected clock is present.
 * This does not change hardware; it just updates the cached flag. */
void clock_integr_set_selected_present(bool present);

/* Poll PLL lock (PF6). Non-blocking and cached to status. */
bool clock_integr_read_lock(void);

/* Wait for lock with bounded loops (mirrors decompiled busy-wait pattern).
 * Returns true if lock observed; also updates LEDs accordingly. */
bool clock_integr_wait_lock(uint16_t max_loops);

/* Optionally pulse PLL SYNC after lock is achieved. */
void clock_integr_pulse_sync(void);

/* Render LED_CLK_IN / LED_CLK_ERR according to current status and rules.
 * (Does not touch READY / SYS_FAIL — those are handled elsewhere.) */
void clock_integr_render_leds(void);

/* Read current status snapshot. */
void clock_integr_get_status(clock_status_t* out);

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_INTEGRATION_H */

