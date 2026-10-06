#include "extension.h"
#define LINE_CAPACITY 48u

static const PM_FLASH char pong[] = "PONG\r\n";
static const PM_FLASH char unknown[] = "ERR UNKNOWN\r\n";
static const PM_FLASH char overflow[] = "ERR TOO LONG\r\n";
static const PM_FLASH char arguments_error[] = "ERR ARGUMENTS\r\n";

void pm_console_write(const PM_FLASH char *text)
{
    while (*text) pm_console_putc((uint8_t)*text++);
}
void pm_command_ping(const char *arguments)
{
    pm_console_write(*arguments ? arguments_error : pong);
}
void pm_command_help(const char *arguments)
{
    if (*arguments) { pm_console_write(arguments_error); return; }
#define LIST_COMMAND(name, handler) do { \
        static const PM_FLASH char description[] = "@" name "\r\n"; \
        pm_console_write(description); \
    } while (0);
    PM_COMMANDS(LIST_COMMAND)
#undef LIST_COMMAND
}
static uint8_t name_matches(const char *name, const PM_FLASH char *expected)
{
    while (*name && *name == *expected) { ++name; ++expected; }
    return *name == *expected;
}
void pm_extension_dispatch(void)
{
    char line[LINE_CAPACITY];
    uint8_t length = 0, too_long = 0, c;
    /* The adapter consumed '@'. Drain through CR even on overflow, preserving
     * the original RX line counter and leaving the next command in its queue. */
    while ((c = pm_console_getc()) != '\r') {
        if (length + 1u < sizeof line) line[length++] = (char)c;
        else too_long = 1;
    }
    line[length] = 0;
    if (too_long) { pm_console_write(overflow); return; }
    char *arguments = line;
    while (*arguments && *arguments != ' ') ++arguments;
    if (*arguments) *arguments++ = 0;
    while (*arguments == ' ') ++arguments;
#define TRY_COMMAND(name, handler) do { \
        static const PM_FLASH char command_name[] = name; \
        if (name_matches(line, command_name)) { handler(arguments); return; } \
    } while (0);
    PM_COMMANDS(TRY_COMMAND)
#undef TRY_COMMAND
    pm_console_write(unknown);
}
