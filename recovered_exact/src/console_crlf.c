#include <stdint.h>
static inline void send_flash_message(uint16_t address)
{
    register uint16_t z asm("r30") = address;
    /* Historical relative-call encoding; callee preserves other GPRs. */
    asm volatile ("rcall cli_send_msg" : "+z" (z) : : "memory", "cc");
}
void cli_send_crlf(void)
{
    send_flash_message(0x2984);
}
