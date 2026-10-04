#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))

/* Carry is the result: clear means ready, set means the error was printed. */
void FUN_code_00108e(void)
{
    asm volatile("clc" : : : "cc");
    register uint8_t status asm("r16") = RAM8(0x2157);
    asm volatile("sbrc %0, 4\n\tret" : : "r" (status));
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2ab4;
    asm volatile("rcall cli_send_msg\n\tsec" : "+z" (message) : : "memory", "cc");
}
