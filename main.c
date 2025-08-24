#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

#include "avr_compiler.h"
#include "drivers/clksys_driver.h"
#include "drivers/eeprom_driver.h"
#include "drivers/pmic_driver.h"

#include "console.h"

#include "fpga.h"
#include "io.h"
#include "ths788.h"
#include "system.h"

extern bool fpga_comm_request;

extern volatile bool b_report_rdy;
extern volatile uint16_t report_cnt;

/*! \brief Main loop of the PM ATxmega MCU code
 *
 * This function:
 * 1. Configures the MCU (io, usart, twi, etc.),
 * 2. Sets up configuration structs
 * 3. Sets up ADT7311
 * 4. Loads configuration from EEPROM
 * 5. Sends interface ready message via USART
 * 6. Starts main loop consisting of:
 *   a. communicating with FPGA (on FPGA request)
 *   b. parsing incoming USART msgs
 */
int main(void)
{
  system_clock_init();

  io_init();

  console_init();

  timer_init();

  EEPROM_EnableMapping();

  //TODO DAT_mem_2442 = 3;

  /* Check 5V supply status */
  if (io_is_12v_present())
  {
    system_status_get()->b_sys_fail = false;
    timer_tc_set_state(&system_timers_get()->ts_fpga, 1);
    timer_tc_set_value_ms(&system_timers_get()->ts_fpga, 2000);
  }
  else
  {
    system_status_get()->b_sys_fail = true;
    io_led_system_fail_update();
  }

  /* Set attenuator timer */
  timer_tc_set_value_ms(&system_timers_get()->ts_attenuator, 1000);

  /* Enable FPGA PORTE.0 INT1 level LOW */
  fpga_req_intr_enable();

  /* Enable PWR status INT0 level LOW */
  io_enable_pwr_interrupt();

  /* Enable console CTS interrupt level MED */
  console_cts_enable();

  /* Enable PMIC interrupt level low & med. */
  PMIC_EnableLowLevel();
  PMIC_EnableMediumLevel();
  sei();

  /* TODO: Init ADT7311 */

  /* Send welcome message */
  console_print("\r\nINR PM12 control interface ready\r\n");
  console_rts_clr();

  do
  {
    do
    {
      if (b_report_rdy)
      {
        // TODO
        b_report_rdy = false;
        //console_print("FPGA: %d, %d\r\n", system_timers_get()->ts_fpga.state, system_timers_get()->ts_fpga.counter);
        //uint16_t data;
        //fpga_exchange_data(0xF2, &data);
        //console_print("MSG: %x \r\n", data);
        report_cnt = 250;
      }
    } while (true);
    // TODO usart_f0_parse_prompt();

  } while (true);

  /* Should never get here */
  return 0;
}
