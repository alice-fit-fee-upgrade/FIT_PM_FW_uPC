/**
 * @file    power_supervision.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements power rail supervision and safety handling for PM.
 * Monitors supply status inputs and raises/clears alarm conditions.
 * Controls enables/resets and applies shutdown policy on faults.
 * Provides query functions used by FSM and diagnostic commands.
 */

#include "power_supervision.h"
#include "led_status.h"  /* optional LED link */

/* --- Initialization ------------------------------------------------------ */
void power_supervision_init(void)
{
  /* Configure PE1 and PE2 as outputs, PE3 remains input. */
  PWR_PORT_E.DIRSET = (PWR_RST_N_bm | PWR_ENABLE_bm);

  /* Initial state:
   *   - PE1 = 1 → release LTC reset (active-low)
   *   - PE2 = 0 → power disabled
   */
  PWR_PORT_E.OUTSET = PWR_RST_N_bm;   /* set PE1 high */
  PWR_PORT_E.OUTCLR = PWR_ENABLE_bm;  /* clear PE2 low */
}

/* --- Control functions --------------------------------------------------- */

/* Controls DA11_EN (PE2) */
void power_enable(bool on)
{
  if (on)
    PWR_PORT_E.OUTSET = PWR_ENABLE_bm;
  else
    PWR_PORT_E.OUTCLR = PWR_ENABLE_bm;
}

/* Controls DA21_RST_N (PE1, active-low) */
void power_reset_ltc(bool assert_low)
{
  if (assert_low)
    PWR_PORT_E.OUTCLR = PWR_RST_N_bm;  /* drive low = reset active */
  else
    PWR_PORT_E.OUTSET = PWR_RST_N_bm;  /* release reset */
}

/* --- Input reads --------------------------------------------------------- */

/* Poll both power-good and overtemp lines */
void power_poll_inputs(power_inputs_t* out)
{
  if (!out) return;
  uint8_t inE = PWR_PORT_E.IN;
  uint8_t inB = PWR_PORT_B.IN;
  out->power_good_1v2 = (inE & PWR_PGOOD_bm);
  out->overtemp_alarm = (inB & PWR_OVERTEMP_bm);
}

/* Return single flags */
bool power_read_pgood(void)   { return (PWR_PORT_E.IN & PWR_PGOOD_bm) != 0; }
bool power_read_overtemp(void){ return (PWR_PORT_B.IN & PWR_OVERTEMP_bm) != 0; }

/* --- System integration (pf_status_poll_and_route equivalent) ------------ */
/* Mirrors behavior from pf_status_poll_and_route():
 *   - If overtemp active → LED_SYS_FAIL ON
 *   - If power_good missing → LED_SYS_FAIL ON
 *   - Otherwise → LED_SYS_FAIL OFF
 */
void power_poll_and_route(const power_inputs_t* in)
{
  if (!in) return;

  bool fault = false;
  if (in->overtemp_alarm) fault = true;
  if (!in->power_good_1v2) fault = true;

  led_status_set_sys_fail(fault);
}

