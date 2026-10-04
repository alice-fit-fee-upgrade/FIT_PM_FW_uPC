#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))

void fpga_set_trg_settings(void)
{
    register uint16_t requested asm("r20");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 0x20\n\tbrne LAB_code_000eb0\n"
                 "rcall cli_get_integer\n\tbrcs LAB_code_000eb0\n\ttst r21\n\tbrne LAB_code_000eb0"
        : "=r" (requested) : : "r16", "memory", "cc");
    asm volatile("rcall FUN_code_00108e\n\tbrcc 1f\n\tret\n1:" : : "r" (requested) : "r16", "r30", "r31", "memory", "cc");
    RAM8(0x222f) = (uint8_t)requested;
    register uint8_t address asm("r18") = 0;
    asm volatile("" : "+r" (address));
    register uint16_t word asm("r16") = requested;
    asm volatile("cli\n\trcall fpga_msg_send_t2\n\tsei\n\trjmp LAB_code_000ff1"
        : : "r" (word), "r" (address) : "memory", "cc");
    __builtin_unreachable();
}
