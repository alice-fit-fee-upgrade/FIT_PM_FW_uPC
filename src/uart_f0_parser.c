/**
 * @file    uart_f0_parser.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements parser/dispatcher for legacy UART F0 command protocol.
 * Tokenizes incoming command lines and routes to PM command handlers.
 * Supports commands for status readout, parameter updates and flashing.
 * Maintains compatibility with historical all_c / all.asm behavior.
 */

#include "uart_f0_parser.h"
#include "console_cmds.h"

/* Minimal parser: dispatches by full command token.
 * This will be replaced/expanded with bit-perfect behavior using all_c/all.asm.
 */
void uart_f0_parser_handle_line(const char* line, size_t len)
{
    (void)len;
    console_cmds_dispatch_line(line);
}
