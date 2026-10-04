#include <avr/io.h>
#define chip_select(address) \
 do { register uint8_t mask asm("r18")=0x10; \
 asm volatile ("sts %0, %1" : : "n" (address), "r" (mask) : "memory"); } while (0)
void adt7311_8bit_rw(void)
{
    register uint8_t command asm("r16");
    asm volatile ("" : "=r" (command));
    chip_select(_SFR_MEM_ADDR(PORTA_OUTCLR));
    register uint8_t input_data asm("r17");
    asm volatile ("" : "=r" (input_data));
    register uint8_t data asm("r19");
    asm volatile ("mov %0, %1" : "=r" (data) : "r" (input_data));
    asm volatile ("rcall adt7311_byte_rw" : "+r" (command)
        : : "r17", "r18", "memory", "cc");
    command=data;
    asm volatile ("rcall adt7311_byte_rw" : "+r" (command)
        : : "r17", "r18", "memory", "cc");
    chip_select(_SFR_MEM_ADDR(PORTA_OUTSET));
}
