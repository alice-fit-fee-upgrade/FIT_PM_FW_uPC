/**
 * @file    console_cmds.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements PM command handlers for console and UART F0 commands.
 * Maps textual commands to actions across clock, power, FPGA and EEPROM.
 * Handles argument parsing, validation and formatted response output.
 * Acts as the integration point for service/diagnostic command set.
 */

#include "console_cmds.h"
#include "console.h"
#include "power_supervision.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Recognize the required UART F0 commands and ACK them.
 * NOTE: This is an integration scaffold only.
 * Real handlers + payload formatting will be implemented next.
 */
static int is_known_cmd(const char* cmd)
{
    /* 2-letter commands */
    static const char* k2[] = {
        "CA","CP","ON","OF","PA","PC","PF","RA","RC","RF","RS","RT","RZ","WR",
        "SD","SL","SO","SS","ST","SV","SZ"
    };
    for (unsigned i=0;i<sizeof(k2)/sizeof(k2[0]);++i) {
        if (strcmp(cmd, k2[i]) == 0) return 1;
    }
    /* 3-letter commands */
    static const char* k3[] = { "SCL","SCS","SCT","SCR" };
    for (unsigned i=0;i<sizeof(k3)/sizeof(k3[0]);++i) {
        if (strcmp(cmd, k3[i]) == 0) return 1;
    }
    return 0;
}

static const char* skip_ws(const char* s)
{
    while (s && (*s == ' ' || *s == '\t')) ++s;
    return s;
}

static void handle_cmd_onof(const char* cmd)
{
    /* all_c/all.asm: ON/OF toggles a persisted enable flag and manipulates PE2.
     * In pm3 we map this directly to DA11_EN (power_enable()).
     */
    bool on = (cmd[1] == 'N');
    power_enable(on);
    console_send_cr_ack();
}

static void handle_cmd_scs(const char* args)
{
    /* all_c/all.asm (cmd_parse_optional_second_then_ack):
     * Parse first decimal int 0..11, then optionally a second int.
     * Values are accepted/ignored; command always ends with ACK.
     */
    const char* p = skip_ws(args);
    if (!p || *p == 0) {
        console_send_cr_ack();
        return;
    }

    char* endp = NULL;
    long v0 = strtol(p, &endp, 10);
    if (endp && endp != p && v0 >= 0 && v0 < 12) {
        /* Optional second value */
        p = skip_ws(endp);
        if (p && *p) {
            (void)strtol(p, &endp, 10);
        }
    }
    console_send_cr_ack();
}

void console_cmds_dispatch_line(const char* line)
{
    if (line == NULL) {
        console_send_cr_nak();
        return;
    }

    line = skip_ws(line);
    if (line[0] == 0) {
        console_send_cr_nak();
        return;
    }

    /* Extract command token (2 or 3 letters) */
    char cmd[4] = {0,0,0,0};
    size_t i = 0;
    while (line[i] && !isspace((unsigned char)line[i]) && i < 3) {
        cmd[i] = (char)toupper((unsigned char)line[i]);
        ++i;
    }
    cmd[i] = 0;

    const char* args = line + i;

    if (!is_known_cmd(cmd)) {
        console_send_cr_nak();
        return;
    }

    /* Implement the previously-missing/ambiguous commands using all_c/all.asm semantics. */
    if ((cmd[0] == 'O') && ((cmd[1] == 'N') || (cmd[1] == 'F')) && cmd[2] == 0) {
        handle_cmd_onof(cmd);
        return;
    }
    if (strcmp(cmd, "SCS") == 0) {
        handle_cmd_scs(args);
        return;
    }

    /* Default scaffold behavior for the remaining commands: ACK only. */
    console_send_cr_ack();
}
