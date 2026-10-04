#ifndef PM_LEGACY_CLI_H
#define PM_LEGACY_CLI_H
#include <stdint.h>
/* Private parser ABI: R20:R21 is the result, R16 is its following delimiter.
 * Preserve historical signed range branches and shared error entries exactly. */
#define PM_PARSE_CHANNEL(result, error) \
    asm volatile("rcall cli_get_integer\n\tbrcs " error "\n\ttst r21\n\tbrne " error \
        "\n\tcpi r20, 12\n\tbrge " error : "=r" (result) : : "r16", "memory", "cc")
#define PM_PARSE_VALUE(result, error) \
    asm volatile("cpi r16, 0x20\n\tbrne " error "\n\trcall cli_get_integer\n\tbrcs " error \
        "\n\tcpi r16, 13\n\tbrne " error : "=r" (result) : : "r16", "memory", "cc")
#define PM_FPGA_GUARD(result) \
    asm volatile("rcall FUN_code_00108e\n\tbrcc 1f\n\tret\n1:" \
        : : "r" (result) : "r16", "r30", "r31", "memory", "cc")
#endif
