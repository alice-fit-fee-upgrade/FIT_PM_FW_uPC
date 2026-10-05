/* Original private console entries: fixed-register barriers, no GNU result ABI. */
extern void cli_get_next_char(void);
extern void cli_get_next_byte(void);
extern void cli_send_msg(void);
#include <stdint.h>

/* Compare the eight original FLASH password bytes; retain carry-result ABI. */
void unlock_programming(void)
{
    register uint8_t received asm("r16");
    cli_get_next_char();
    asm volatile("" : "=r" (received) : : "memory");
    /* C value equivalent: if (received != 13) goto original_error;
     * Keep its exact conditional branch into the shared error tail. */
    asm volatile("cpi r16, 13\n\tbrne LAB_code_000ad0" : : "r" (received) : "cc");
    register const uint8_t *password asm("r30") = (const uint8_t *)0x2b76;
    asm volatile("" : "+z" (password));
next_byte:;
    register uint8_t expected asm("r17");
    /* C value equivalent: expected = flash_byte(password++);
     * Exact LPM Z+ remains; earlier __flash pointer trials needed other code. */
    asm volatile("lpm %0, Z+" : "=r" (expected), "+z" (password) : : "memory");
    cli_get_next_byte();
    asm volatile("" : "=r" (received));
    asm volatile("cp %0, %1\n\tbrne LAB_code_000ad0" : : "r" (received), "r" (expected) : "cc");
    asm goto("cpi r30, 0x7e\n\tbrne %l[next_byte]" : : "z" (password) : "cc" : next_byte);
    password = (const uint8_t *)0x2998;
    asm volatile("" : "+z" (password));
    cli_send_msg();
    /* C status equivalent: return_success_with_carry_clear();
     * The private carry-result entry cannot use a normal C return value. */
    asm volatile("clc\n\tret" : : : "memory", "cc");
    __builtin_unreachable();
}
asm(".pushsection .text.unlock_programming,\"ax\",@progbits\n"
    ".subsection 1\nsec\nret\nrjmp LAB_code_0009e4\n.popsection");
