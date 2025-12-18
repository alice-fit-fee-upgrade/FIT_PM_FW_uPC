/**
 * @file    uart_console.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements UART transport for PM console and legacy UART F0 protocol.
 * Provides RX/TX buffering, line collection and basic framing utilities.
 * Feeds received lines into the console/command dispatcher.
 * Used as the primary human/service interface during bring-up and tests.
 */

#include "uart_console.h"

/* --- tiny delay for symmetry with original busy-waits (optional) -------- */
static inline void tiny_guard_delay(void)
{
    __asm__ __volatile__("nop\n\tnop\n\tnop\n\t");
}

/* --- baud calculation (simple 16x oversampling, BSCALE=0) ----------------
 * XMEGA USART baud registers use:
 *   BAUDCTRLA = BSEL[7:0]
 *   BAUDCTRLB = (BSCALE[3:0]<<4) | BSEL[11:8]
 * Here we choose BSCALE=0 and BSEL ≈ (F_CPU / (16*baud)) - 1 for common rates.
 * For corner cases you may override by writing BAUDCTRLA/B directly before init.
 */
static void uartc_set_baud_simple(uint32_t f_cpu, uint32_t baud)
{
    if (baud == 0) baud = 115200;
    uint32_t bsel = (f_cpu / (16UL * baud));
    if (bsel) bsel -= 1;

    UARTC_USART.BAUDCTRLA = (uint8_t)(bsel & 0xFF);
    UARTC_USART.BAUDCTRLB = (uint8_t)((0 << 4) | ((bsel >> 8) & 0x0F)); /* BSCALE=0 */
}

/* --- public API ---------------------------------------------------------- */
void uartc_init(const uartc_config_t* cfg)
{
    /* 1) Direction: PF3 as output (TXD), PF2 as input (RXD) */
    UARTC_PORT.DIRSET = UARTC_TXD_bm;
    UARTC_PORT.DIRCLR = UARTC_RXD_bm;

    /* 2) Disable transmitter/receiver before reconfig */
    UARTC_USART.CTRLB = 0x00;

    /* 3) Frame format: 8N1 (CHSIZE=8bit, no parity, 1 stop) */
    UARTC_USART.CTRLC =
        (0 << USART_PMODE_gp) |  /* PMODE = 00 (no parity) */
        (0 << USART_SBMODE_bp) | /* SBMODE = 0 (1 stop)    */
        (3 << USART_CHSIZE_gp);  /* CHSIZE = 11 (8-bit)    */

    /* 4) Baud setting (simple) */
    #ifdef F_CPU
      uartc_set_baud_simple((uint32_t)F_CPU, cfg ? cfg->baud : 115200UL);
    #else
      /* If F_CPU unknown at compile time, assume 32 MHz default */
      uartc_set_baud_simple(32000000UL, cfg ? cfg->baud : 115200UL);
    #endif

    /* 5) Enable TX (always) and RX (optional) */
    uint8_t en = USART_TXEN_bm;
    if (cfg && cfg->enable_rx) en |= USART_RXEN_bm;
    UARTC_USART.CTRLB = en;

    /* Optional tiny guard to mirror original pacing */
    tiny_guard_delay();
}

static inline void uartc_wait_dre(void)
{
    while ( (UARTC_USART.STATUS & USART_DREIF_bm) == 0 ) { /* wait */ }
}

void uartc_tx_byte(uint8_t b)
{
    uartc_wait_dre();
    UARTC_USART.DATA = b;
}

void uartc_tx_buf(const void* data, size_t len)
{
    const uint8_t* p = (const uint8_t*)data;
    for (size_t i=0; i<len; ++i) {
        uartc_tx_byte(p[i]);
    }
}

void uartc_tx_str(const char* s)
{
    if (!s) return;
    while (*s) uartc_tx_byte((uint8_t)*s++);
}

bool uartc_rx_available(void)
{
    return (UARTC_USART.STATUS & USART_RXCIF_bm) != 0;
}

uint8_t uartc_rx_byte(void)
{
    while (!uartc_rx_available()) { /* blocking */ }
    return UARTC_USART.DATA;
}

bool uartc_try_rx_byte(uint8_t* out)
{
    if (!out) return false;
    if (!uartc_rx_available()) return false;
    *out = UARTC_USART.DATA;
    return true;
}

/* Matches the decompiled helper: send CR then ACK (0x06).
 * This is used to confirm command/line processing on the console. */
void uartc_flush_cr_then_ack(void)
{
    uartc_tx_byte((uint8_t)'\r');  /* 0x0D */
    uartc_tx_byte(0x06);           /* ACK  */
}

/* --- small hex helpers --------------------------------------------------- */
static inline uint8_t hexn(uint8_t v)
{
    v &= 0x0F;
    return (v < 10) ? (uint8_t)('0' + v) : (uint8_t)('A' + (v-10));
}

void uartc_tx_hex8(uint8_t v)
{
    uartc_tx_byte(hexn(v >> 4));
    uartc_tx_byte(hexn(v));
}

void uartc_tx_hex16(uint16_t v)
{
    uartc_tx_hex8((uint8_t)(v >> 8));
    uartc_tx_hex8((uint8_t)(v & 0xFF));
}

