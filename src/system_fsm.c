/**
 * @file    system_fsm.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements the Processing Module (PM) high-level state machine.
 * Coordinates initialization, configuration, run-time operation and faults.
 * Calls into clock, power, FPGA and link modules based on system state.
 * Centralizes transitions, timeouts and error recovery policies.
 */

#include "system_fsm.h"

/* Subsystems (our refactored modules) */
#include "led_status.h"
#include "power_supervision.h"
#include "spie_norflash.h"
#include "pll_clock.h"
#include "clock_integration.h"
#include "link_handshake.h"
#include "ths788.h"
#include "fpga_config.h"
#include "uart_console.h"

/* ---------------- Internal state ---------------------------------------- */
static volatile system_state_t g_state = SYS_STATE_INIT;
static volatile bool g_fpga_done = false;
static volatile bool g_pll_locked = false;
static volatile bool g_pgood = false;
static volatile bool g_overtemp = false;

/* tiny pacing to preserve deterministic ordering (like original busy-waits) */
static inline void tiny_guard_delay(void)
{
    __asm__ __volatile__("nop\n\tnop\n\tnop\n\t");
}

/* ---------------- Hardware “defaults” phase ------------------------------ */
void system_hw_preinit(void)
{
    /* LED directions + default states
     * Original board_io_reset_defaults(): PA6/PA7 set (CLK_ERR, READY on),
     * PA0/PA5 off. We do the same, and subsequent modules will adjust LEDs. */
    led_status_init();

    /* Power supervision lines: PE1/PE2 output, PE3 input, PB6 input.
     * Initial: PE1=1 (release LTC reset), PE2=0 (power disabled). */
    power_supervision_init();

    /* UART console for logs/ACKs (115200 8N1, RX optional=false) */
    uartc_config_t ucfg = { .baud = 115200, .enable_rx = true };
    uartc_init(&ucfg);
    console_init();

    /* SPIE (NOR Flash) and SPIC/PLL ports: set idle levels and directions */
    spie_port_init();
    pll_port_init();

    /* LINK handshake GPIOs default (PD4/5/7 out low, PD6 in; PE0 out low) */
    link_port_init();

    /* THS788 lines default: all outputs, CLK/DATA low, strobes high, RESETN=1 */
    ths788_port_init();

    /* FPGA config pins defaults: PROGRAM_B high, INIT_B/DONE inputs */
    fpga_port_init();

    /* Clock integration layer (ties PF6 LOCK to LED_CLK_IN/ERR rendering) */
    clock_integr_init();

    g_state = SYS_STATE_WAIT_CLOCK;
}

/* ---------------- Bring-up sequence ------------------------------------- */
bool system_startup_sequence(void)
{
    /* 1) Enable main power rails (DA11_EN) after preinit */
    power_enable(true);

    /* 2) Optional: read JEDEC ID to sanity-check SPIE flash presence */
    spie_read_jedec_id();

    /* 3) Minimal PLL “basic sequence” (table-driven config would go here) */
    pll_basic_init_sequence();

    /* 4) Wait for PLL LOCK (bounded) and update clock LEDs accordingly */
    if (!clock_integr_wait_lock(50000)) {
        /* Stay in FAILSAFE; LEDs will indicate error */
        g_state = SYS_STATE_FAILSAFE;
        return false;
    }

    /* 5) Reconfigure FPGA via PROGRAM_B/INIT_B/DONE monitoring */
    if (!fpga_reconfigure_pipeline(50000, 50000, 200000)) {
        g_state = SYS_STATE_FAILSAFE;
        return false;
    }

    /* 6) Optionally sync PLL after configuration to align domains */
    clock_integr_pulse_sync();

    /* 7) THS788 basic reset & deselect strobes (safe idle) */
    ths788_reset(true);
    tiny_guard_delay();
    ths788_reset(false);
    ths788_deselect_all();

    /* 8) Final LED rendering through the high-level LED system.
     * Mark ready if power good + fpga done + locked clock. */
    power_inputs_t p; power_poll_inputs(&p);
    g_pgood = p.power_good_1v2;
    g_overtemp = p.overtemp_alarm;
    g_pll_locked = clock_integr_read_lock();
    g_fpga_done = fpga_read_done();

    /* Update LED unified flags (same relations as in led_status module) */
    led_status_inputs_t li = {
        .power_ok = g_pgood,
        .fpga_init_inactive = true,   /* at this point INIT_B should be high */
        .fpga_done = g_fpga_done,
        .clk_selected_present = g_pll_locked /* used as “present & locked” */
    };
    led_status_set_flags(&li);
    led_status_apply_from_flags();

    /* Console ACK (mirrors _flush_to_cr_then_ack()) */
    uartc_flush_cr_then_ack();

    g_state = SYS_STATE_READY;
    return true;
}

/* ---------------- Periodic polling (fail-safe) -------------------------- */
void system_mainloop_tick(void)
{
    /* Poll inputs */
    power_inputs_t p; power_poll_inputs(&p);
    g_pgood = p.power_good_1v2;
    g_overtemp = p.overtemp_alarm;
    g_pll_locked = clock_integr_read_lock();
    g_fpga_done = fpga_read_done();

    /* Fail-safe policy:
     *  - If overtemp or power not good → force SYS_FAIL on, keep READY off.
     *  - Otherwise keep LED policy from clock/ready logic. */
    if (!g_pgood || g_overtemp) {
        /* Force SYS_FAIL */
        led_status_set_sys_fail(true);
        /* Optional: drop enable to protect hardware (commented for now)
         * power_enable(false);
         */
        g_state = SYS_STATE_FAILSAFE;
    } else if (g_state == SYS_STATE_FAILSAFE) {
        /* try to recover if conditions improved */
        if (g_pll_locked && g_fpga_done) {
            g_state = SYS_STATE_READY;
        }
    }

    /* Maintain clock LEDs consistently (IN/ERR) */
    clock_integr_render_leds();

    /* READY/SYSFAIL rendering via high-level flags */
    led_status_inputs_t li = {
        .power_ok = g_pgood,
        .fpga_init_inactive = true,
        .fpga_done = g_fpga_done,
        .clk_selected_present = g_pll_locked
    };
    led_status_set_flags(&li);
    led_status_apply_from_flags();
}

/* ---------------- Status accessors -------------------------------------- */
void system_get_status(system_status_t* out)
{
    if (!out) return;
    out->state = g_state;
    out->power_good_1v2 = g_pgood;
    out->overtemp_alarm = g_overtemp;
    out->pll_locked = g_pll_locked;
    out->fpga_done = g_fpga_done;
}

