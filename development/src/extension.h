#ifndef PM_EXTENSION_H
#define PM_EXTENSION_H
#include <stdint.h>
#ifdef __AVR__
#define PM_FLASH __flash
#else
#define PM_FLASH
#endif
/* Standard GNU ABI, main-loop context. Legacy queue I/O blocks until progress;
 * do not call from an ISR or use it as a timeout-capable sensor API. */
uint8_t pm_console_getc(void);
void pm_console_putc(uint8_t value);
void pm_console_write(const PM_FLASH char *text);
void pm_extension_dispatch(void);
/* Stateless handlers: new commands live here, not in recovered_exact. */
void pm_command_ping(const char *arguments);
void pm_command_help(const char *arguments);
void pm_command_pgtime(const char *arguments);
void pm_command_hw(const char *arguments);
void pm_command_temp(const char *arguments);
void pm_command_tdc(const char *arguments);
void pm_command_pll(const char *arguments);
void pm_command_fpga(const char *arguments);
void pm_command_fpgatest(const char *arguments);
void pm_command_cfg(const char *arguments);
void pm_command_flashid(const char *arguments);
void pm_command_flashread(const char *arguments);
void pm_command_i2c(const char *arguments);
void pm_command_ina(const char *arguments);
void pm_command_power(const char *arguments);
#define PM_COMMANDS(X) \
    X("PING", pm_command_ping) \
    X("HELP", pm_command_help) \
    X("HW", pm_command_hw) \
    X("TEMP", pm_command_temp) \
    X("TDC", pm_command_tdc) \
    X("PLL", pm_command_pll) \
    X("PGTIME", pm_command_pgtime) \
    X("FPGA", pm_command_fpga) \
    X("FPGATEST", pm_command_fpgatest) \
    X("CFG", pm_command_cfg) \
    X("FLASHID", pm_command_flashid) \
    X("FLASHREAD", pm_command_flashread) \
    X("I2C", pm_command_i2c) \
    X("INA", pm_command_ina) \
    X("POWER", pm_command_power)
#endif
