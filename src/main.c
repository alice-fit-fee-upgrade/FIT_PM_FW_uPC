/**
 * @file    main.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Application entry point for the PM firmware.
 * Initializes clocks, GPIO, peripherals and subsystem modules.
 * Starts the PM control/state-machine and runs the main service loop.
 * Performs periodic housekeeping and dispatches background tasks.
 */

#include <avr/io.h>

/* must come BEFORE any delay headers */
#ifndef F_CPU
# define F_CPU 32000000UL
#endif

#include <util/delay_basic.h>   // not <util/delay.h>
#include <stdbool.h>

#include "system_fsm.h"
#include "uart_console.h"
#include "led_status.h"

/* 4 CPU cycles per iteration → count = F_CPU / (1000 * 4) = F_CPU/4000  */
static inline void delay_ms(uint16_t ms)
{
    // For F_CPU=32 MHz: F_CPU/4000 = 8000 (fits in uint16_t)
    const uint16_t loops_per_ms = (uint16_t)(F_CPU / 4000UL);
    while (ms--) {
        _delay_loop_2(loops_per_ms);
    }
}

int main(void)
{
    system_hw_preinit();

    bool ok = system_startup_sequence();

    uartc_tx_str("\r\n--- XMEGA PM System ---\r\n");
    uartc_tx_str(ok ? "Startup OK\r\n" : "Startup FAILSAFE\r\n");

    for (;;)
    {
        system_mainloop_tick();
        console_service();

        static uint32_t hb = 0;
        if (++hb >= 50000) {
            hb = 0;
            uartc_tx_byte('.');
        }

        delay_ms(10);
    }
}

