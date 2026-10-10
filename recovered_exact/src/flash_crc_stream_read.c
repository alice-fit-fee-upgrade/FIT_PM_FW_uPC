#include <stdint.h>
register uint16_t pm_crc_address asm("r28");
#define RAM8(address) (*(volatile uint8_t *)(address))
#define WAIT_READY(reg) do { \
 register uint8_t status asm(reg); \
 do { status = RAM8(0x0ac2); asm volatile("" : "+r" (status)); } \
 while (!(status & 0x80u)); \
} while (0)
/* Read the original 24-bit inclusive address range and stream bytes to the
 * exact CRC core. Its unusual byte ordering and live R2:R0 are preserved. */
void FUN_code_000bb9(void)
{
    register uint8_t byte asm("r16") = 0x10;
    asm volatile("" : "+r" (byte));
    RAM8(0x0686) = byte;
    byte = 3;
    asm volatile("" : "+r" (byte));
    RAM8(0x0ac3) = byte;
    register uint8_t dummy asm("r10");
    asm volatile("clr %0" : "=r" (dummy) : : "cc");
    register uint8_t low asm("r28"), middle asm("r29"), high asm("r30");
    asm volatile("" : "=r" (low), "=r" (middle), "=r" (high));
    WAIT_READY("r19");
    RAM8(0x0ac3) = high;
    WAIT_READY("r19");
    RAM8(0x0ac3) = middle;
    WAIT_READY("r19");
    RAM8(0x0ac3) = low;
    /* Separate byte initialization preserves all four original LDI opcodes. */
    register uint8_t crc0 asm("r16") = 0xff;
    asm volatile("" : "+r" (crc0));
    register uint8_t crc1 asm("r17") = 0xff;
    asm volatile("" : "+r" (crc1));
    register uint8_t crc2 asm("r18") = 0xff;
    asm volatile("" : "+r" (crc2));
    register uint8_t crc3 asm("r19") = 0xff;
    asm volatile("" : "+r" (crc3));
    register uint32_t crc asm("r16");
    asm volatile("" : "=r" (crc));
    register uint8_t polynomial0 asm("r23") = 0xb7;
    asm volatile("" : "+r" (polynomial0));
    register uint8_t polynomial1 asm("r24") = 0x1d;
    asm volatile("" : "+r" (polynomial1));
    register uint8_t polynomial2 asm("r25") = 0xc1;
    asm volatile("" : "+r" (polynomial2));
    register uint8_t polynomial3 asm("r26") = 4;
    asm volatile("" : "+r" (polynomial3));
    WAIT_READY("r22");
    RAM8(0x0ac3) = dummy;
next_byte:
    WAIT_READY("r22");
    register uint8_t received asm("r22") = RAM8(0x0ac3);
    asm volatile("" : "+r" (received));
    asm goto("cp r0, r28\n\tcpc r1, r29\n\tcpc r2, r30\n\tbreq %l[last_byte]" : : : "cc" : last_byte);
    RAM8(0x0ac3) = dummy;
    asm volatile("rcall FUN_code_000bf2"
                 : "+r" (crc), "+r" (low), "+r" (middle), "+r" (high), "+r" (received)
                 : "r" (dummy), "r" (polynomial0), "r" (polynomial1), "r" (polynomial2), "r" (polynomial3) : "r20", "memory", "cc");
    asm volatile("" : "=r" (pm_crc_address) : : "memory");
    pm_crc_address += 1;
    asm volatile("" : "+r" (pm_crc_address) : : "memory");
    /* C 24-bit carry equivalent: high += carry_out; exact ADC retains flags. */
    asm volatile("adc r30, r10" : "+r" (high) : "r" (dummy) : "cc");
    goto next_byte;
last_byte:
    asm volatile("rcall FUN_code_000bf2\n\trjmp LAB_code_000c43"
                 : : "r" (crc), "r" (received), "r" (polynomial0), "r" (polynomial1), "r" (polynomial2), "r" (polynomial3) : "memory", "cc");
    __builtin_unreachable();
}
