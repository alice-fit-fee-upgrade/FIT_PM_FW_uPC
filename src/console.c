/**
 * @file    console.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements lightweight console framework for PM.
 * Routes complete input lines to registered command handlers.
 * Provides common helpers for parsing, acknowledgements and responses.
 * Used by UART console transport and PM command implementations.
 */

#include "console.h"
#include "uart_console.h"
#include "uart_f0_parser.h"

#ifndef CONSOLE_RX_LINE_MAX
#define CONSOLE_RX_LINE_MAX  96u
#endif

static char     s_line[CONSOLE_RX_LINE_MAX];
static uint8_t  s_len = 0;

void console_init(void)
{
    /* UART is already initialized in system_hw_preinit().
     * If not, you may call uartc_init() before using console.
     */
    s_len = 0;
}

void console_send_bytes(const void* data, size_t len)
{
    uartc_tx_buf(data, len);
}

void console_send_cr_ack(void)
{
    /* match original helper style: CR then ACK */
    uartc_tx_byte('\r');
    uartc_tx_byte(0x06);
}

void console_send_cr_nak(void)
{
    uartc_tx_byte('\r');
    uartc_tx_byte(0x15); /* NAK */
}

void console_service(void)
{
    uint8_t b;
    while (uartc_try_rx_byte(&b))
    {
        char c = (char)b;

        /* accept printable ASCII + '\r' (commands are short) */
        if (c == '\r') {
            s_line[s_len] = 0;
            uart_f0_parser_handle_line(s_line, s_len);
            s_len = 0;
            continue;
        }

        /* Ignore LF if host sends CRLF */
        if (c == '\n') {
            continue;
        }

        if (s_len < (CONSOLE_RX_LINE_MAX - 1u)) {
            s_line[s_len++] = c;
        } else {
            /* overflow: reset buffer and NAK */
            s_len = 0;
            console_send_cr_nak();
        }
    }
}
