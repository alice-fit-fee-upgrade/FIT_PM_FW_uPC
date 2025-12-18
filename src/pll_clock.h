/**
 * @file    pll_clock.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares low-level PLL/CDCE clock programming API for PM.
 * Provides functions to initialize SPI/GPIO and program device registers.
 * Defines helper types/constants for register write sequences.
 * Used by clock integration and console/diagnostic commands.
 */

#ifndef PLL_CLOCK_H
#define PLL_CLOCK_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/* Port aliases */
#ifndef PLL_PORT_C
#  define PLL_PORT_C  PORTC
#endif
#ifndef PLL_PORT_F
#  define PLL_PORT_F  PORTF
#endif

/* Bit masks (XMEGA naming) */
#define PLL_SPI_MOSI_bm   PIN5_bm   /* PC5 */
#define PLL_SPI_MISO_bm   PIN6_bm   /* PC6 */
#define PLL_SPI_SCK_bm    PIN7_bm   /* PC7 */

#define PLL_LE_bm         PIN4_bm   /* PF4 */
#define PLL_SYNC_bm       PIN5_bm   /* PF5 */
#define PLL_LOCK_bm       PIN6_bm   /* PF6 */

/* Public API */
#ifdef __cplusplus
extern "C" {
#endif

/* Configure SPIC pins (PC5/PC7 as outputs, PC6 input) and PF4/PF5 outputs, PF6 input.
 * Sets idle states: SCK=0, MOSI=0, LE=0, SYNC=0. */
void pll_port_init(void);

/* Pulse LE (low→high→low), used to latch a written register word into the PLL. */
void pll_pulse_le(void);

/* Pulse SYNC (low→high→low), used to synchronize PLL outputs as per CDCE62005. */
void pll_pulse_sync(void);

/* Send one 24/32-bit configuration word to the PLL over SPIC.
 * The exact word width depends on device; we provide both helpers. */
void pll_write_word24(uint32_t word24);
void pll_write_word32(uint32_t word32);

/* Convenience: write then latch (LE pulse). */
static inline void pll_write_word24_latched(uint32_t word24) { pll_write_word24(word24); pll_pulse_le(); }
static inline void pll_write_word32_latched(uint32_t word32) { pll_write_word32(word32); pll_pulse_le(); }

/* Poll LOCK (PF6). Returns true if locked. */
bool pll_is_locked(void);

/* Wait for LOCK with timeout (in loops). Returns true if lock observed. */
bool pll_wait_lock(uint16_t max_loops);

/* Optional high-level init stub (kept minimal — sequence depends on board config). */
void pll_basic_init_sequence(void);

#ifdef __cplusplus
}
#endif

#endif /* PLL_CLOCK_H */

