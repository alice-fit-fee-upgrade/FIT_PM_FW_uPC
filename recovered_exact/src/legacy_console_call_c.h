#ifndef PM_LEGACY_CONSOLE_CALL_H
#define PM_LEGACY_CONSOLE_CALL_H
#include <stdint.h>

/* PM12 private ABI: R16 holds the argument/result. Statement expressions
 * expand at the call site, evaluate the value once and emit one exact RCALL.
 * Ordinary inline parameters still trigger GNU ABI diagnostics when R24/R25
 * are reserved by a legacy profile. No ordinary C call boundary is introduced.
 * Logical C: return legacy_console_dispatch(value).r16;
 * Flags and memory may change; other registers follow the original callee
 * contract. See docs/private_console_abi.md and the tested model in tests/logical_models/legacy_console_call_c.c. */
#define PM_CONSOLE_VALUE_CALL(target, value) __extension__ ({ \
    register uint8_t pm_call_r16 asm("r16") = (value); \
    asm volatile("rcall " target : "+r" (pm_call_r16) : : "memory", "cc"); \
    pm_call_r16; \
})
#define pm_console_send_character(value) PM_CONSOLE_VALUE_CALL("cli_send_buf", value)
#define pm_console_send_hex_digit(value) PM_CONSOLE_VALUE_CALL("cli_send_digit_hex", value)
#define pm_console_read_character() __extension__ ({ \
    register uint8_t pm_call_r16 asm("r16"); \
    asm volatile("rcall cli_get_next_char" : "=r" (pm_call_r16) : : "memory", "cc"); \
    pm_call_r16; \
})
#endif
