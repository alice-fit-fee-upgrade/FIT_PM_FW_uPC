/**
 * @file    link_handshake.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares link handshake API for PM.
 * Provides functions to initialize the link and service handshake steps.
 * Exposes link state and status flags to the PM FSM and diagnostics.
 * Used to coordinate communication readiness with other FIT components.
 */

#ifndef LINK_HANDSHAKE_H
#define LINK_HANDSHAKE_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/* Port aliases */
#ifndef LINK_PORT_D
#  define LINK_PORT_D  PORTD
#endif
#ifndef LINK_PORT_E
#  define LINK_PORT_E  PORTE
#endif

/* Bit masks */
#define LINK_PD4_bm   PIN4_bm  /* 0x10 */
#define LINK_PD5_bm   PIN5_bm  /* 0x20 */
#define LINK_PD6_bm   PIN6_bm  /* 0x40 */
#define LINK_PD7_bm   PIN7_bm  /* 0x80 */
#define LINK_PE0_bm   PIN0_bm  /* 0x01 */

/* DMA kick callback type (called after handshake succeeds) */
typedef void (*link_dma_kick_fn_t)(void);

#ifdef __cplusplus
extern "C" {
#endif

/* Configure directions:
 *  - PD4, PD5, PD7 as outputs (drive to FPGA)
 *  - PD6 as input (readback from FPGA)
 *  - PE0 as output (auxiliary)
 * Set default idle levels: PD4/PD5/PD7 low, PE0 low.
 */
void link_port_init(void);

/* Set/clear the primary handshake line on PD4 */
void link_set_pd4(bool high);

/* Optional helpers for additional strobes to FPGA */
void link_set_pd5(bool high);
void link_set_pd7(bool high);
void link_set_pe0(bool high);

/* Wait for FPGA ready/ack on PD6 (read = 1) with bounded retries.
 * Returns true if PD6 observed high within the loop budget. */
bool link_wait_pd6_ready(uint16_t max_loops);

/* Full handshake sequence:
 *   1) assert PD4 (signal from MCU to FPGA)
 *   2) wait for PD6=1 (FPGA ack)
 *   3) call dma_kick() if provided
 *   4) deassert PD4
 * Returns true on success (ack observed), false on timeout.
 */
bool link_handshake_pd4_then_kick(uint16_t max_loops, link_dma_kick_fn_t dma_kick);

/* Short "burst-4" style strobe sequence used alongside SPIC transfers.
 * This mirrors the observed spic_pd_tx_sync_on_ff_and_lt_then_burst4() intent:
 * - quickly toggle PD4/PD5/PD7 in a 4-phase burst window to align data.
 * The exact phase timing was busy-waited in the original; here we do
 * deterministic, minimal NOP delays.
 */
void link_burst4_phased_strobes(void);

#ifdef __cplusplus
}
#endif

#endif /* LINK_HANDSHAKE_H */

