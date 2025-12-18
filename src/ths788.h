/**
 * @file    ths788.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares THS788 timing unit driver API for PM.
 * Provides functions to initialize ports, reset device and write settings.
 * Defines GPIO masks/pins and command formatting helpers as needed.
 * Used by FSM and console commands during hardware bring-up.
 */

#ifndef THS788_H
#define THS788_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef THS_PORT_B
#  define THS_PORT_B  PORTB
#endif

#define THS_HCLK_bm      PIN0_bm   /* PB0 */
#define THS_HDATA_bm     PIN1_bm   /* PB1 */
#define THS_HSTB1_bm     PIN2_bm   /* PB2 */
#define THS_HSTB2_bm     PIN3_bm   /* PB3 */
#define THS_HSTB3_bm     PIN4_bm   /* PB4 */
#define THS_RESETN_bm    PIN5_bm   /* PB5 */

/* Initialize directions and idle levels.
 * - PB0..PB5 as outputs
 * - Idle: CLK=0, DATA=0, STROBE1..3 high (inactive), RESETN=1 (released)
 */
void ths788_port_init(void);

/* Assert/deassert hardware reset (active-low). */
void ths788_reset(bool assert_low);

/* Pulse reset low for a short, deterministic cycle count. */
void ths788_reset_pulse(uint16_t cycles);

/* Select a strobe line (1..3) active-low; deselect releases all strobes high. */
void ths788_select_strobe(uint8_t idx);
void ths788_deselect_all(void);

/* Transmit a 24-bit word MSB-first on (DATA, CLK) pins. */
void ths788_tx24(uint32_t word);

/* Convenience: select strobe, send word, deselect. */
void ths788_write_reg(uint8_t strobe_idx, uint32_t word24);

#endif /* THS788_H */

