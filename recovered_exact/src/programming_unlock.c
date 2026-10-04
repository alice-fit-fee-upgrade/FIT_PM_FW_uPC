#include <stdint.h>

/* Compare the eight original FLASH password bytes; retain carry-result ABI. */
void unlock_programming(void)
{
    register uint8_t received asm("r16");
    asm volatile("call cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000ad0"
                 : "=r" (received) : : "memory", "cc");
    register const uint8_t *password asm("r30") = (const uint8_t *)0x2b76;
    asm volatile("" : "+z" (password));
next_byte:;
    register uint8_t expected asm("r17");
    asm volatile("lpm %0, Z+\n\tcall cli_get_next_byte" : "=r" (expected), "+z" (password), "=r" (received) : : "memory");
    asm volatile("cp %0, %1\n\tbrne LAB_code_000ad0" : : "r" (received), "r" (expected) : "cc");
    asm goto("cpi r30, 0x7e\n\tbrne %l[next_byte]" : : "z" (password) : "cc" : next_byte);
    password = (const uint8_t *)0x2998;
    asm volatile("call cli_send_msg\n\tclc\n\tret" : "+z" (password) : : "memory", "cc");
    __builtin_unreachable();
}
asm(".pushsection .text.unlock_programming,\"ax\",@progbits\n"
    ".subsection 1\nsec\nret\nrjmp LAB_code_0009e4\n.popsection");
