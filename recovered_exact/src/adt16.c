#include <avr/io.h>
#define CS_REGISTER(address, reg) \
 do { register uint8_t mask asm(reg)=0x10; \
 asm volatile ("sts %0, %1" : : "n" (address), "r" (mask) : "memory"); } while (0)
#define TRANSFER(value) \
 asm volatile ("rcall adt7311_byte_rw" : "+r" (value) \
     : : "r17", "r18", "memory", "cc")
void adt7311_16bit_rw(void)
{
    register uint8_t command asm("r16");
    asm volatile ("" : "=r" (command));
    CS_REGISTER(_SFR_MEM_ADDR(PORTA_OUTCLR), "r20");
    register uint8_t tx_low asm("r17");
    register uint8_t tx_high asm("r18");
    asm volatile ("" : "=r" (tx_low), "=r" (tx_high));
    register uint8_t low asm("r20");
    register uint8_t high asm("r21");
    asm volatile ("mov %0, %2\n\tmov %1, %3" : "=r" (low), "=r" (high)
        : "r" (tx_low), "r" (tx_high));
    TRANSFER(command);
    command=high;
    TRANSFER(command);
    high=command;
    command=low;
    TRANSFER(command);
    low=command;
    CS_REGISTER(_SFR_MEM_ADDR(PORTA_OUTSET), "r18");
    asm volatile ("" : : "r" (low), "r" (high));
}
