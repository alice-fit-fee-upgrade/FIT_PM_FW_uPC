#include <avr/io.h>
/* Calls retain private fixed-register inputs/results through zero-byte
 * barriers. Void declarations deliberately introduce no GNU arguments/results;
 * recapture after each call observes the original registers. Every conversion
 * is accepted only if GNU CALL, frames and the entire FLASH remain identical. */
extern void adt7311_16bit_rw(void);
extern void adt7311_8bit_rw(void);
extern void adt7311_faults_clr(void);
extern void cli_send_msg(void);
extern void adt7311_faults_clr(void);
#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))
#define SET_VALUE(constant) do { \
 value = (constant); asm volatile("" : "+r" (value)); \
} while (0)
#define CLOCK_WRITE(address) asm volatile("sts %1, %0" : : "r" (value), "n" (address) : "memory")
#define CLOCK_WAIT(bit) asm volatile("1: lds r17, 0x51\n\tsbrs r17, " bit "\n\trjmp 1b" : : : "r17", "memory")

/* Original reset sequence, including wide low-I/O stores and its stack reset.
 * GPIO/peripheral values and the unusual timer/default states are unchanged. */
int __attribute__((section(".text.main"))) main(void)
{
    register uint8_t value asm("r16");
    SET_VALUE(0xcb); CLOCK_WRITE(0x52);
    SET_VALUE(9); CLOCK_WRITE(0x50);
    CLOCK_WAIT("3");
    SET_VALUE(0xc2); CLOCK_WRITE(0x55);
    SET_VALUE(0x19); CLOCK_WRITE(0x50);
    CLOCK_WAIT("4");
    register uint8_t key asm("r17") = 0xd8;
    asm volatile("" : "+r" (key));
    SET_VALUE(4);
    asm volatile("sts 0x34, r17\n\tsts 0x40, r16" : : "r" (key), "r" (value) : "memory");
    SET_VALUE(0x18); CLOCK_WRITE(0x50);
    {
        register uint8_t stack_byte asm("r25") = 0xff;
        asm volatile("" : "+r" (stack_byte) : : "memory");
        RAM8(0x3d) = stack_byte;
        stack_byte = 0x3f;
        asm volatile("" : "+r" (stack_byte) : : "memory");
        RAM8(0x3e) = stack_byte;
    }
    SET_VALUE(0xfb);
    RAM8(0x0601) = value;
    SET_VALUE(0xf3);
    RAM8(0x0605) = value;
    SET_VALUE(0x18);
    RAM8(0x0612) = value;
    SET_VALUE(0x80);
    RAM8(0x0611) = value;
    RAM8(0x0613) = value;
    RAM8(0x0614) = value;
    SET_VALUE(0x02);
    RAM8(0x0636) = value;
    SET_VALUE(0x80);
    RAM8(0x0653) = value;
    SET_VALUE(0x30);
    RAM8(0x0654) = value;
    SET_VALUE(0x28);
    RAM8(0x0671) = value;
    RAM8(0x0672) = value;
    SET_VALUE(0x04);
    RAM8(0x0666) = value;
    SET_VALUE(0x02);
    RAM8(0x0665) = value;
    SET_VALUE(0x06);
    RAM8(0x0661) = value;
    SET_VALUE(0x00);
    RAM8(0x0673) = value;
    SET_VALUE(0x04);
    RAM8(0x0681) = value;
    SET_VALUE(0x01);
    RAM8(0x0690) = value;
    SET_VALUE(0x00);
    RAM8(0x0691) = value;
    SET_VALUE(0x80);
    RAM8(0x0692) = value;
    SET_VALUE(0x00);
    RAM8(0x0693) = value;
    SET_VALUE(0x0a);
    RAM8(0x068a) = value;
    SET_VALUE(0x01);
    RAM8(0x068b) = value;
    SET_VALUE(0x09);
    RAM8(0x06a1) = value;
    SET_VALUE(0x00);
    RAM8(0x06b1) = value;
    SET_VALUE(0x00);
    RAM8(0x06b6) = value;
    SET_VALUE(0x10);
    RAM8(0x06b7) = value;
    SET_VALUE(0x40);
    RAM8(0x062a) = value;
    SET_VALUE(0x08);
    RAM8(0x066a) = value;
    SET_VALUE(0x02);
    RAM8(0x06aa) = value;
    SET_VALUE(0x40);
    RAM8(0x06ab) = value;
    SET_VALUE(0x01);
    RAM8(0x06a5) = value;
    SET_VALUE(0xd0);
    RAM8(0x0ba7) = value;
    SET_VALUE(0x83);
    RAM8(0x0ba6) = value;
    SET_VALUE(0x03);
    RAM8(0x0ba5) = value;
    SET_VALUE(0x18);
    RAM8(0x0ba4) = value;
    SET_VALUE(0x20);
    RAM8(0x0ba3) = value;
    SET_VALUE(0x06);
    RAM8(0x0800) = value;
    SET_VALUE(0x7d);
    RAM8(0x0826) = value;
    SET_VALUE(0x00);
    RAM8(0x0827) = value;
    SET_VALUE(0x00);
    RAM8(0x0801) = value;
    SET_VALUE(0x00);
    RAM8(0x0804) = value;
    SET_VALUE(0x01);
    RAM8(0x0806) = value;
    SET_VALUE(0x08);
    RAM8(0x01cc) = value;
    register uint8_t scratch asm("r25");
    asm volatile("clr %0" : "=r" (scratch) : : "cc");
    register uint8_t *destination asm("r30") = (uint8_t *)0x2000;
    asm volatile("" : "+z" (destination));
    register uint8_t *end asm("r28") = (uint8_t *)0x2443;
    asm volatile("1: st Z+, r25\n\tcp r30, r28\n\tcpc r31, r29\n\tbrne 1b"
                 : "+z" (destination) : "y" (end), "r" (scratch) : "memory", "cc");
    SET_VALUE(3); RAM8(0x2442) = value;
    value = RAM8(0x06a8);
    asm volatile("sbrs r16, 1\n\tinc r25" : "+r" (scratch) : "r" (value) : "cc");
    RAM8(0x2004) = scratch;
    value = RAM8(0x0688);
    asm volatile("bst r16, 1\n\tclr r16\n\tbld r16, 0" : "+r" (value) : : "cc");
    RAM8(0x2157) = value;
    asm goto("brts %l[power_present]" : : : : power_present);
    GPIOR0 |= (1u << 1);
    SET_VALUE(1); RAM8(0x0606) = value;
    goto power_initialized;
power_present:
    SET_VALUE(1); RAM8(0x215b) = value;
    SET_VALUE(0xd0); RAM8(0x215c) = value;
    SET_VALUE(7); RAM8(0x215d) = value;
power_initialized:
    SET_VALUE(0xe8); RAM8(0x215e) = value;
    SET_VALUE(3); RAM8(0x215f) = value;
    SET_VALUE(1); RAM8(0x0689) = value;
    SET_VALUE(2); RAM8(0x06a9) = value;
    SET_VALUE(3); RAM8(0x00a2) = value;
    do {

        adt7311_faults_clr();

    } while (0);
    SET_VALUE(8); key = 0x50;
    do {
        asm volatile("" : "+r" (value), "+r" (key) :  : "memory");
        adt7311_8bit_rw();
        asm volatile("" : "=r" (value), "=r" (key) : : "memory");
    } while (0);
    SET_VALUE(0x20);
    key = 0;
    asm volatile("" : "+r" (key));
    register uint8_t high asm("r18") = 0x23;
    do {
        asm volatile("" : "+r" (value), "+r" (key), "+r" (high) :  : "memory");
        adt7311_16bit_rw();
        asm volatile("" : "=r" (value), "=r" (key), "=r" (high) : : "memory");
    } while (0);
    SET_VALUE(0x30);
    key = 0;
    asm volatile("" : "+r" (key));
    high = 0x1e;
    do {
        asm volatile("" : "+r" (value), "+r" (key), "+r" (high) :  : "memory");
        adt7311_16bit_rw();
        asm volatile("" : "=r" (value), "=r" (key), "=r" (high) : : "memory");
    } while (0);
    destination = (uint8_t *)0x2163;
    asm volatile("" : "+z" (destination));
    register uint8_t *eeprom asm("r28");
    eeprom = (uint8_t *)0x1000;
    asm volatile("" : "+y" (eeprom));
    key = 0x22;
    asm volatile("1: ld r16, Y+\n\tst Z+, r16\n\tcpi r30, 0x35\n\tcpc r31, r17\n\tbrne 1b\n\tcbi 0, 3\n\tsei"
                 : "+y" (eeprom), "+z" (destination), "=r" (value) : "r" (key) : "memory", "cc");
    destination = (uint8_t *)0x2962;
    do {
        asm volatile("" : "+z" (destination) :  : "memory");
        cli_send_msg();
        asm volatile("" : "=z" (destination) : : "memory");
    } while (0);
    SET_VALUE(1); RAM8(0x06a6) = value;
main_loop:
    value = RAM8(0x2006);
    asm goto("tst r16\n\tbreq %l[console_check]" : : "r" (value) : "cc" : console_check);
    asm volatile("rcall fpga_data_exchange" : : : "memory", "cc");
console_check:
    value = RAM8(0x2005);
    asm goto("tst r16\n\tbreq %l[main_loop]" : : "r" (value) : "cc" : main_loop);
    asm volatile("rcall cli_prompt_parse" : : : "memory", "cc");
    goto main_loop;
}
