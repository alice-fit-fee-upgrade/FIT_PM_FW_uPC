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
#define PM_COMMANDS(X) \
    X("PING", pm_command_ping) \
    X("HELP", pm_command_help)
#endif
