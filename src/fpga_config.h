/**
 * @file    fpga_config.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares FPGA configuration and register access API for PM.
 * Provides functions to configure the FPGA and access register map.
 * Defines register ranges/identifiers used by PM settings and diagnostics.
 * Used by FSM, console command handlers and UART F0 protocol support.
 */

#ifndef FPGA_CONFIG_H
#define FPGA_CONFIG_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/* Port alias */
#ifndef FPGA_PORT_D
#  define FPGA_PORT_D  PORTD
#endif

/* Bit masks (XMEGA PINx_bm) */
#define FPGA_PROGRAM_B_bm   PIN1_bm /* PD1 */
#define FPGA_INIT_B_bm      PIN2_bm /* PD2 */
#define FPGA_DONE_bm        PIN3_bm /* PD3 */

/* Optional hooks (weak) to integrate checks around the pipeline.
 * Provide your own definitions elsewhere if desired. */
void __attribute__((weak)) fpga_precheck_nor_flash(void);  /* e.g., read JEDEC ID */
void __attribute__((weak)) fpga_post_config_hook(void);    /* e.g., enable links/clock */

/* Compact status snapshot */
typedef struct {
  bool program_b_high;  /* true when PROGRAM_B is released (high) */
  bool init_b_high;     /* true when INIT_B is high (ready) */
  bool done_high;       /* true when DONE is high (configured) */
} fpga_status_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Configure directions and idle levels:
 * - PD1 as output (PROGRAM_B), default high (released)
 * - PD2 as input  (INIT_B)
 * - PD3 as input  (DONE)
 */
void fpga_port_init(void);

/* Drive PROGRAM_B (active-low) */
void fpga_program_assert(bool assert_low);

/* Read individual status pins */
bool fpga_read_init_b(void);  /* true if INIT_B is high */
bool fpga_read_done(void);    /* true if DONE is high  */

/* Capture full status snapshot */
void fpga_read_status(fpga_status_t* st);

/* Wait helpers with bounded polling loops.
 * Return true if the condition is observed within the provided budget. */
bool fpga_wait_init_low(uint32_t max_loops);
bool fpga_wait_init_high(uint32_t max_loops);
bool fpga_wait_done_high(uint32_t max_loops);

/* Full reconfiguration pipeline:
 *  1) (optional) fpga_precheck_nor_flash()
 *  2) PROGRAM_B low (assert), small delay
 *  3) PROGRAM_B high (release)
 *  4) wait INIT_B low, then INIT_B high
 *  5) wait DONE high
 *  6) (optional) fpga_post_config_hook()
 *
 * Returns true on success (DONE observed high), false on timeout.
 */
bool fpga_reconfigure_pipeline(uint32_t t_init_low, uint32_t t_init_high, uint32_t t_done);

#ifdef __cplusplus
}
#endif

#endif /* FPGA_CONFIG_H */

