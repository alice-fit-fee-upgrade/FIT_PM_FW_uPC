#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))

void fpga_set_trg_charge_lvls(void)
{
    register uint16_t requested asm("r20");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 0x20\n\tbrne LAB_code_000eb0\n"
                 "rcall cli_get_integer\n\tbrcs LAB_code_000e78\n\ttst r21\n\tbrmi LAB_code_000e78\n"
                 "ldi r26, 0x10\n\tcpi r20, 0\n\tcpc r21, r26\n\tbrge LAB_code_000eb0"
        : "=r" (requested) : : "r16", "r26", "memory", "cc");
    asm volatile("rcall FUN_code_00108e\n\tbrcc 1f\n\tret\n1:" : : "r" (requested) : "r16", "r30", "r31", "memory", "cc");
    RAM8(0x2230) = (uint8_t)requested;
    RAM8(0x2231) = requested >> 8;
    register uint8_t address asm("r18") = 0x3d;
    asm volatile("" : "+r" (address));
    register uint16_t word asm("r16") = requested;
    asm volatile("cli\n\trcall fpga_msg_send_t2\n\tsei\n\trjmp LAB_code_000ff1"
                 : : "r" (word), "r" (address) : "memory", "cc");
    __builtin_unreachable();
}
