/**
 * @file    console_cmds.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares PM console/UART command handler registration.
 * Provides prototypes for command handlers and registration function(s).
 * Defines command identifiers and shared parsing helpers if applicable.
 * Used by console dispatcher to bind commands to implementation.
 */

#ifndef CONSOLE_CMDS_H
#define CONSOLE_CMDS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Dispatch a full UART line (without trailing CR). The line may include
 * arguments separated by spaces, e.g. "SV 0001" or "SCS 3 7".
 */
void console_cmds_dispatch_line(const char* line);

#ifdef __cplusplus
}
#endif

#endif /* CONSOLE_CMDS_H */
