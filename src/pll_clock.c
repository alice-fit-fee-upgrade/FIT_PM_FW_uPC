/**
 * @file    pll_clock.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements low-level PLL/CDCE clock device programming over SPI.
 * Handles latch-enable and timing required for register transactions.
 * Provides routines to write default settings and verify lock behavior.
 * Used by clock integration and system bring-up sequences.
 */

#include "pll_clock.h"

/* --------------------- low-level SPI helper (blocking) ------------------- */
static inline void spic_tx(uint8_t b)
{
    SPIC.DATA = b;
    while ((SPIC.STATUS & 0x80) == 0) { /* wait transfer complete */ }
    (void)SPIC.DATA; /* read to clear */
}

/* --------------------- port init / defaults ------------------------------ */
void pll_port_init(void)
{
    /* PC5 (MOSI), PC7 (SCK) as outputs; PC6 (MISO) input */
    PLL_PORT_C.DIRSET = (PLL_SPI_MOSI_bm | PLL_SPI_SCK_bm);
    PLL_PORT_C.DIRCLR = (PLL_SPI_MISO_bm);

    /* PF4 (LE), PF5 (SYNC) outputs; PF6 (LOCK) input */
    PLL_PORT_F.DIRSET = (PLL_LE_bm | PLL_SYNC_bm);
    PLL_PORT_F.DIRCLR = (PLL_LOCK_bm);

    /* Idle states: SCK=0, MOSI=0; LE=0, SYNC=0  */
    PLL_PORT_C.OUTCLR = (PLL_SPI_MOSI_bm | PLL_SPI_SCK_bm);
    PLL_PORT_F.OUTCLR = (PLL_LE_bm | PLL_SYNC_bm);
}

/* --------------------- strobes ------------------------------------------- */
void pll_pulse_le(void)
{
    /* LE: 0 → 1 → 0 */
    PLL_PORT_F.OUTSET = PLL_LE_bm;
    /* small guard gap mirrors original busy-waits */
    __asm__ __volatile__ ("nop\n\t""nop\n\t");
    PLL_PORT_F.OUTCLR = PLL_LE_bm;
}

void pll_pulse_sync(void)
{
    /* SYNC: 0 → 1 → 0 */
    PLL_PORT_F.OUTSET = PLL_SYNC_bm;
    __asm__ __volatile__ ("nop\n\t""nop\n\t");
    PLL_PORT_F.OUTCLR = PLL_SYNC_bm;
}

/* --------------------- write words over SPIC ----------------------------- */
/* Many TI PLLs (e.g., CDCE62005) accept 24 or 32-bit shift words latched by LE. */

void pll_write_word24(uint32_t word24)
{
    /* Shift out MSB..LSB, 3 bytes */
    spic_tx((uint8_t)((word24 >> 16) & 0xFF));
    spic_tx((uint8_t)((word24 >>  8) & 0xFF));
    spic_tx((uint8_t)((word24 >>  0) & 0xFF));
}

void pll_write_word32(uint32_t word32)
{
    /* Shift out MSB..LSB, 4 bytes */
    spic_tx((uint8_t)((word32 >> 24) & 0xFF));
    spic_tx((uint8_t)((word32 >> 16) & 0xFF));
    spic_tx((uint8_t)((word32 >>  8) & 0xFF));
    spic_tx((uint8_t)((word32 >>  0) & 0xFF));
}

/* --------------------- lock monitoring ----------------------------------- */
bool pll_is_locked(void)
{
    /* PF6 high → LOCK asserted */
    return (PLL_PORT_F.IN & PLL_LOCK_bm) != 0;
}

bool pll_wait_lock(uint16_t max_loops)
{
    /* Busy-wait pattern mirrors original STATUS polling style */
    while (max_loops--)
    {
        if (pll_is_locked())
            return true;
        /* tiny back-off matching decompiled spin delays */
        __asm__ __volatile__ ("nop\n\t""nop\n\t""nop\n\t");
    }
    return false;
}

/* --------------------- minimal init stub --------------------------------- */
void pll_basic_init_sequence(void)
{
    /* Typical minimal bring-up:
     *  1) ensure idle levels and SPI ready
     *  2) write device-specific registers
     *  3) pulse LE after each register write
     *  4) optionally pulse SYNC
     *  5) wait for LOCK (PF6)
     *
     * Exact register values depend on board requirements (not encoded in binary symbols).
     * Keep this as a placeholder or provide a table-driven config elsewhere.
     */

    /* Example skeleton (commented; fill with real words):
     *
     * pll_write_word32_latched(0xXXXXXXXX);
     * pll_write_word32_latched(0xYYYYYYYY);
     * pll_pulse_sync();
     * (void)pll_wait_lock(5000);
     */
}

