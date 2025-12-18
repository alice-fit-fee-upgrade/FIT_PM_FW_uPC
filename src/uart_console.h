/**
 * @file    uart_console.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares UART console transport API for PM.
 * Provides initialization and non-blocking RX/TX service functions.
 * Exposes line/byte send helpers used by console and command handlers.
 * Used for diagnostic output and UART F0 command handling.
 */

#ifndef UART_CONSOLE_H
#define UART_CONSOLE_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Default peripheral aliases */
#ifndef UARTC_USART
#  define UARTC_USART  USARTF0
#endif
#ifndef UARTC_PORT
#  define UARTC_PORT   PORTF
#endif

/* Bit masks for direction config */
#define UARTC_TXD_bm   PIN3_bm   /* PF3 TXD0 */
#define UARTC_RXD_bm   PIN2_bm   /* PF2 RXD0 */

/* Typical frame: 8 data, no parity, 1 stop (8N1) */
typedef struct {
  uint32_t baud;   /* desired baud rate (e.g., 115200) */
  bool     enable_rx; /* enable RX path (true/false) */
} uartc_config_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize USARTF0 for console logging.
 * If F_CPU is known, a simple BSEL/BSCALE pair is calculated for 8N1.
 * TX is mandatory; RX is optional (set by config.enable_rx). */
void uartc_init(const uartc_config_t* cfg);

/* Low-level TX/RX primitives (blocking/non-blocking) */
void     uartc_tx_byte(uint8_t b);             /* blocking until DRE is ready */
void     uartc_tx_buf(const void* data, size_t len);
void     uartc_tx_str(const char* s);          /* zero-terminated string */

bool     uartc_rx_available(void);             /* true if RX has a byte */
uint8_t  uartc_rx_byte(void);                  /* blocking read one byte  */
bool     uartc_try_rx_byte(uint8_t* out);      /* non-blocking read */

/* Matches the original helper: transmit CR (0x0D) then ACK (0x06) */
void     uartc_flush_cr_then_ack(void);

/* Optional helpers for hex logging (handy for low-level debug) */
void     uartc_tx_hex8(uint8_t v);
void     uartc_tx_hex16(uint16_t v);

#ifdef __cplusplus
}
#endif

#endif /* UART_CONSOLE_H */

