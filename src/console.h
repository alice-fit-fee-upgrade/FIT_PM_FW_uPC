/**
 * @file    console.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares console framework API for PM.
 * Defines command registration structures and dispatch entry points.
 * Provides helper prototypes for common parsing/response formatting.
 * Used by UART transport and command handler modules.
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void console_init(void);
void console_service(void);

/* Response helpers (original behavior uses CR then ACK for many cases) */
void console_send_cr_ack(void);
void console_send_cr_nak(void);
void console_send_bytes(const void* data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* CONSOLE_H */
