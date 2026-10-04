#include <avr/io.h>

/* No GNU argument call: byte transfer uses the original R16/17/18 contract. */
#define PM_ADT_SEND_BYTE(byte) \
 do { register uint8_t value asm("r16")=(byte); \
 asm volatile ("rcall adt7311_byte_rw" : "+r" (value) \
     : : "r17", "r18", "memory", "cc"); } while (0)

void adt7311_faults_clr(void)
{
    PORTA_OUTCLR=0x10;
    PM_ADT_SEND_BYTE(0xff);
    PM_ADT_SEND_BYTE(0xff);
    PM_ADT_SEND_BYTE(0xff);
    PM_ADT_SEND_BYTE(0xff);
    PORTA_OUTSET=0x10;
}
