/**
 * @file    uart_f0_parser.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares legacy UART F0 protocol parser API for PM.
 * Provides functions to parse a received line and dispatch commands.
 * Defines constants/limits for command tokens and argument formats.
 * Used by UART console transport and console command layer.
 */

#ifndef UART_F0_PARSER_H
#define UART_F0_PARSER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void uart_f0_parser_handle_line(const char* line, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* UART_F0_PARSER_H */
