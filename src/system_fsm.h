/**
 * @file    system_fsm.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares the Processing Module (PM) state machine API and states.
 * Provides entry points for initialization and periodic FSM servicing.
 * Defines state identifiers, events and shared state query helpers.
 * Used by main and subsystem modules to coordinate PM operation.
 */

#ifndef SYSTEM_FSM_H
#define SYSTEM_FSM_H

#include <stdbool.h>
#include <stdint.h>

/* === Public boot entry points =========================================== */

/* Performs hardware pin defaulting and low-level submodules init.
 * This is the “pre-FSM” phase that mirrors board_io_reset_defaults() et al. */
void system_hw_preinit(void);

/* Executes the canonical bring-up sequence:
 *   Power → PLL ports → SPIE port → FPGA reconfigure → LINK/THS → Clock LEDs
 * Returns true on success (FPGA DONE observed; clock integrated).
 */
bool system_startup_sequence(void);

/* Main loop: polls power/overtemp/lock and updates LEDs and fault state.
 * Non-blocking “tick” step: call periodically from your scheduler. */
void system_mainloop_tick(void);

/* Aggregate status for telemetry / debug prints */
typedef enum {
  SYS_STATE_INIT = 0,
  SYS_STATE_WAIT_CLOCK,
  SYS_STATE_FPGA_CONFIG,
  SYS_STATE_READY,
  SYS_STATE_FAILSAFE
} system_state_t;

typedef struct {
  system_state_t state;
  bool power_good_1v2;
  bool overtemp_alarm;
  bool pll_locked;
  bool fpga_done;
} system_status_t;

void system_get_status(system_status_t* out);

#endif /* SYSTEM_FSM_H */

