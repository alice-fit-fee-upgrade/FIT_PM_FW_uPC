/**
 * @file    power_supervision.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares power supervision and control API for PM.
 * Provides initialization, periodic monitoring and alarm query routines.
 * Defines interfaces to enable/disable supplies and reset lines safely.
 * Used by FSM, console commands and bring-up diagnostics.
 */

#ifndef POWER_SUPERVISION_H
#define POWER_SUPERVISION_H

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

/* Default port definitions */
#ifndef PWR_PORT_E
#  define PWR_PORT_E  PORTE
#endif
#ifndef PWR_PORT_B
#  define PWR_PORT_B  PORTB
#endif

/* Bit masks from confirmed ASM:
 * PE1=0x02, PE2=0x04, PE3=0x08, PB6=0x40
 */
#define PWR_RST_N_bm     PIN1_bm  /* PE1 */
#define PWR_ENABLE_bm     PIN2_bm  /* PE2 */
#define PWR_PGOOD_bm      PIN3_bm  /* PE3 */
#define PWR_OVERTEMP_bm   PIN6_bm  /* PB6 */

/* Input snapshot */
typedef struct {
  bool power_good_1v2;  /* PE3 = 1 → 1.2V good */
  bool overtemp_alarm;  /* PB6 = 1 → Overtemperature alarm active */
} power_inputs_t;

#ifdef __cplusplus
extern "C" {
#endif

void power_supervision_init(void);
void power_enable(bool on);
void power_reset_ltc(bool assert_low);

void power_poll_inputs(power_inputs_t* out);
void power_poll_and_route(const power_inputs_t* in);

bool power_read_pgood(void);
bool power_read_overtemp(void);

#ifdef __cplusplus
}
#endif

#endif /* POWER_SUPERVISION_H */

