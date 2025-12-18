/**
 * @file    led_status.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements status LED control and indication policies for PM.
 * Renders system state, alarms and clock lock into LED patterns.
 * Provides lightweight periodic update functions for the main loop.
 * Separates UI indication logic from hardware control and supervision.
 */

#include <avr/io.h>
#include "led_status.h"

/* Local helpers (active-high) */
static inline void led_on(uint8_t bm)  { LED_PORT.OUTSET = bm; }
static inline void led_off(uint8_t bm) { LED_PORT.OUTCLR = bm; }

/* Internal copy of LED control flags */
static volatile led_status_inputs_t g_led_in = {
  .power_ok = false,
  .fpga_init_inactive = false,
  .fpga_done = false,
  .clk_selected_present = false
};

void led_status_init(void)
{
  /* Set LED pins as outputs: PA0/PA5/PA6/PA7 */
  LED_PORT.DIRSET = (LED_SYS_FAIL_bm | LED_CLK_IN_bm |
                     LED_CLK_ERR_bm | LED_READY_bm);

  /* Default LED state per ASM:
   *   PORTA.OUTSET = 0xC0 → PA6 (CLK_ERR) + PA7 (READY) ON
   *   PA0 (SYS_FAIL) and PA5 (CLK_IN) remain OFF
   */
  led_off(LED_SYS_FAIL_bm);
  led_off(LED_CLK_IN_bm);
  led_on (LED_CLK_ERR_bm);
  led_on (LED_READY_bm);
}

/* --- direct control ------------------------------------------------------ */
void led_status_set_sys_fail(bool on) { on ? led_on(LED_SYS_FAIL_bm) : led_off(LED_SYS_FAIL_bm); }
void led_status_set_clk_in(bool on)   { on ? led_on(LED_CLK_IN_bm)   : led_off(LED_CLK_IN_bm);   }
void led_status_set_clk_err(bool on)  { on ? led_on(LED_CLK_ERR_bm)  : led_off(LED_CLK_ERR_bm);  }
void led_status_set_ready(bool on)    { on ? led_on(LED_READY_bm)    : led_off(LED_READY_bm);    }

void led_status_apply(bool sys_fail, bool clk_in, bool clk_err, bool ready)
{
  led_status_set_sys_fail(sys_fail);
  led_status_set_clk_in(clk_in);
  led_status_set_clk_err(clk_err);
  led_status_set_ready(ready);
}

/* --- flag management ----------------------------------------------------- */
void led_status_set_flags(const led_status_inputs_t* in)
{
  if (!in) return;
  g_led_in = *in; /* shallow copy */
}

/* Compute target LED states from input flags.
 * Derived from observed relationships in all_c:
 *  - CLK_IN = clk_selected_present
 *  - CLK_ERR = inverse of CLK_IN
 *  - READY = power_ok && init_inactive && done && !clk_err
 *  - SYS_FAIL = inverse of READY
 */
static inline void compute_render(bool* sys_fail, bool* clk_in, bool* clk_err, bool* ready)
{
  const bool clk_in_on  =  g_led_in.clk_selected_present;
  const bool clk_err_on = !g_led_in.clk_selected_present;

  const bool ready_on =
      g_led_in.power_ok &&
      g_led_in.fpga_init_inactive &&
      g_led_in.fpga_done &&
      (!clk_err_on);

  const bool sys_fail_on = !ready_on;

  *sys_fail = sys_fail_on;
  *clk_in   = clk_in_on;
  *clk_err  = clk_err_on;
  *ready    = ready_on;
}

void led_status_apply_from_flags(void)
{
  bool s,c,e,r;
  compute_render(&s,&c,&e,&r);
  led_status_apply(s,c,e,r);
}

/* --- adapter-style update entry points ----------------------------------- */
/* Updates only SYS_FAIL (PA0) — called from gate_pa0_by_gpior1_and_clear_gpior0bit0() */
void gpi_or_flag_pa0_ctrl(void)
{
  bool s,c,e,r;
  compute_render(&s,&c,&e,&r);
  led_status_set_sys_fail(s);
}

/* Updates CLK_IN (PA5), CLK_ERR (PA6), and READY (PA7) */
void portd5_status_update(void)
{
  bool s,c,e,r;
  compute_render(&s,&c,&e,&r);
  led_status_set_clk_in(c);
  led_status_set_clk_err(e);
  led_status_set_ready(r);
}

