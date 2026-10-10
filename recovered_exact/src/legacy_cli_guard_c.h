#ifndef PM_LEGACY_CLI_GUARD_C_H
#define PM_LEGACY_CLI_GUARD_C_H
#include "legacy_cli.h"

/* Opt-in C return from the private carry-result guard. The short call and
 * carry branch remain exact ASM; the compiler emits the ordinary RET.
 * Use only in a void legacy entry after whole-FLASH exact-check. */
#undef PM_FPGA_GUARD
#define PM_FPGA_GUARD(result) do { \
    __label__ ready; \
    asm goto("rcall FUN_code_00108e\n\tbrcc %l[ready]" \
        : : "r" (result) : "r16", "r30", "r31", "memory", "cc" : ready); \
    return; \
ready:; \
} while (0)
/* A caller which already performed the short call can retain the original
 * carry branch and express only its early RET in C. */
#define PM_RETURN_UNLESS_CARRY_CLEAR() do { \
    __label__ ready; \
    asm goto("brcc %l[ready]" : : : "memory", "cc" : ready); \
    return; \
ready:; \
} while (0)
#endif
