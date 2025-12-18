/**
 * @file    led_status.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares status LED control API for PM.
 * Provides functions to initialize LED GPIO and update indications.
 * Exposes helpers to set state/flags affecting rendered LED patterns.
 * Used by FSM and supervision modules for operator-visible status.
 */

#ifndef LED_STATUS_H
#define LED_STATUS_H

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

/* --- Default port/bit mapping (can be overridden) --- */
#ifndef LED_PORT
#  define LED_PORT          PORTA
#endif
#ifndef LED_SYS_FAIL_bm
#  define LED_SYS_FAIL_bm   PIN0_bm   /* PA0 */
#endif
#ifndef LED_CLK_IN_bm
#  define LED_CLK_IN_bm     PIN5_bm   /* PA5 */
#endif
#ifndef LED_CLK_ERR_bm
#  define LED_CLK_ERR_bm    PIN6_bm   /* PA6 */
#endif
#ifndef LED_READY_bm
#  define LED_READY_bm      PIN7_bm   /* PA7 */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* High-level LED input flags, provided by upper layers */
typedef struct {
  bool power_ok;             /* power rails good */
  bool fpga_init_inactive;   /* INIT_B high → FPGA not in reset */
  bool fpga_done;            /* DONE = 1 → FPGA configured */
  bool clk_selected_present; /* clock signal detected */
} led_status_inputs_t;

/* Initialize LED GPIO directions and default states.
 * In ASM: PORTA.DIRSET sets outputs, and OUTSET = 0xC0 → PA6,PA7 = ON. */
void led_status_init(void);

/* Direct control (atomic writes) */
void led_status_set_sys_fail(bool on);
void led_status_set_clk_in(bool on);
void led_status_set_clk_err(bool on);
void led_status_set_ready(bool on);

/* Apply all four at once */
void led_status_apply(bool sys_fail, bool clk_in, bool clk_err, bool ready);

/* Set internal flag copy */
void led_status_set_flags(const led_status_inputs_t* in);

/* Render LED states from current flag set */
void led_status_apply_from_flags(void);

/* Adapter-style helpers corresponding to the original call sites:
 *  - gpi_or_flag_pa0_ctrl() → updates only SYS_FAIL (PA0)
 *  - portd5_status_update() → updates CLK_IN (PA5), CLK_ERR (PA6), READY (PA7)
 */
void gpi_or_flag_pa0_ctrl(void);
void portd5_status_update(void);

#ifdef __cplusplus
}
#endif

#endif /* LED_STATUS_H */

