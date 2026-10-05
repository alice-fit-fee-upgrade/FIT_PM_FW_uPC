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
int __attribute__((OS_main, section(".text.main"))) main(void)
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

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 2976 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_application_main(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0f54: { // ldi r16, 0xCB
        s->r[16] = 203;
        return 3926;
    }
    case 0x0f56: { // sts 0x0052, r16
        uint16_t address = 82;
        pm_write(s, address, s->r[16]);
        return 3930;
    }
    case 0x0f5a: { // ldi r16, 0x09
        s->r[16] = 9;
        return 3932;
    }
    case 0x0f5c: { // sts 0x0050, r16
        uint16_t address = 80;
        pm_write(s, address, s->r[16]);
        return 3936;
    }
    case 0x0f60: { // lds r17, 0x0051
        uint16_t address = 81;
        s->r[17] = pm_read(s, address);
        return 3940;
    }
    case 0x0f64: { // sbrs r17, 3
        return (!!(s->r[17] & (1u << 3)) == 1) ? 3944 : 3942;
    }
    case 0x0f66: { // rjmp .-8
        return 3936;
    }
    case 0x0f68: { // ldi r16, 0xC2
        s->r[16] = 194;
        return 3946;
    }
    case 0x0f6a: { // sts 0x0055, r16
        uint16_t address = 85;
        pm_write(s, address, s->r[16]);
        return 3950;
    }
    case 0x0f6e: { // ldi r16, 0x19
        s->r[16] = 25;
        return 3952;
    }
    case 0x0f70: { // sts 0x0050, r16
        uint16_t address = 80;
        pm_write(s, address, s->r[16]);
        return 3956;
    }
    case 0x0f74: { // lds r17, 0x0051
        uint16_t address = 81;
        s->r[17] = pm_read(s, address);
        return 3960;
    }
    case 0x0f78: { // sbrs r17, 4
        return (!!(s->r[17] & (1u << 4)) == 1) ? 3964 : 3962;
    }
    case 0x0f7a: { // rjmp .-8
        return 3956;
    }
    case 0x0f7c: { // ldi r17, 0xD8
        s->r[17] = 216;
        return 3966;
    }
    case 0x0f7e: { // ldi r16, 0x04
        s->r[16] = 4;
        return 3968;
    }
    case 0x0f80: { // sts 0x0034, r17
        uint16_t address = 52;
        pm_write(s, address, s->r[17]);
        return 3972;
    }
    case 0x0f84: { // sts 0x0040, r16
        uint16_t address = 64;
        pm_write(s, address, s->r[16]);
        return 3976;
    }
    case 0x0f88: { // ldi r16, 0x18
        s->r[16] = 24;
        return 3978;
    }
    case 0x0f8a: { // sts 0x0050, r16
        uint16_t address = 80;
        pm_write(s, address, s->r[16]);
        return 3982;
    }
    case 0x0f8e: { // ldi r25, 0xFF
        s->r[25] = 255;
        return 3984;
    }
    case 0x0f90: { // out 0x3d, r25
        pm_io_write(s, 61, s->r[25]);
        return 3986;
    }
    case 0x0f92: { // ldi r25, 0x3F
        s->r[25] = 63;
        return 3988;
    }
    case 0x0f94: { // out 0x3e, r25
        pm_io_write(s, 62, s->r[25]);
        return 3990;
    }
    case 0x0f96: { // ldi r16, 0xFB
        s->r[16] = 251;
        return 3992;
    }
    case 0x0f98: { // sts 0x0601, r16
        uint16_t address = 1537;
        pm_write(s, address, s->r[16]);
        return 3996;
    }
    case 0x0f9c: { // ldi r16, 0xF3
        s->r[16] = 243;
        return 3998;
    }
    case 0x0f9e: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 4002;
    }
    case 0x0fa2: { // ldi r16, 0x18
        s->r[16] = 24;
        return 4004;
    }
    case 0x0fa4: { // sts 0x0612, r16
        uint16_t address = 1554;
        pm_write(s, address, s->r[16]);
        return 4008;
    }
    case 0x0fa8: { // ldi r16, 0x80
        s->r[16] = 128;
        return 4010;
    }
    case 0x0faa: { // sts 0x0611, r16
        uint16_t address = 1553;
        pm_write(s, address, s->r[16]);
        return 4014;
    }
    case 0x0fae: { // sts 0x0613, r16
        uint16_t address = 1555;
        pm_write(s, address, s->r[16]);
        return 4018;
    }
    case 0x0fb2: { // sts 0x0614, r16
        uint16_t address = 1556;
        pm_write(s, address, s->r[16]);
        return 4022;
    }
    case 0x0fb6: { // ldi r16, 0x02
        s->r[16] = 2;
        return 4024;
    }
    case 0x0fb8: { // sts 0x0636, r16
        uint16_t address = 1590;
        pm_write(s, address, s->r[16]);
        return 4028;
    }
    case 0x0fbc: { // ldi r16, 0x80
        s->r[16] = 128;
        return 4030;
    }
    case 0x0fbe: { // sts 0x0653, r16
        uint16_t address = 1619;
        pm_write(s, address, s->r[16]);
        return 4034;
    }
    case 0x0fc2: { // ldi r16, 0x30
        s->r[16] = 48;
        return 4036;
    }
    case 0x0fc4: { // sts 0x0654, r16
        uint16_t address = 1620;
        pm_write(s, address, s->r[16]);
        return 4040;
    }
    case 0x0fc8: { // ldi r16, 0x28
        s->r[16] = 40;
        return 4042;
    }
    case 0x0fca: { // sts 0x0671, r16
        uint16_t address = 1649;
        pm_write(s, address, s->r[16]);
        return 4046;
    }
    case 0x0fce: { // sts 0x0672, r16
        uint16_t address = 1650;
        pm_write(s, address, s->r[16]);
        return 4050;
    }
    case 0x0fd2: { // ldi r16, 0x04
        s->r[16] = 4;
        return 4052;
    }
    case 0x0fd4: { // sts 0x0666, r16
        uint16_t address = 1638;
        pm_write(s, address, s->r[16]);
        return 4056;
    }
    case 0x0fd8: { // ldi r16, 0x02
        s->r[16] = 2;
        return 4058;
    }
    case 0x0fda: { // sts 0x0665, r16
        uint16_t address = 1637;
        pm_write(s, address, s->r[16]);
        return 4062;
    }
    case 0x0fde: { // ldi r16, 0x06
        s->r[16] = 6;
        return 4064;
    }
    case 0x0fe0: { // sts 0x0661, r16
        uint16_t address = 1633;
        pm_write(s, address, s->r[16]);
        return 4068;
    }
    case 0x0fe4: { // ldi r16, 0x00
        s->r[16] = 0;
        return 4070;
    }
    case 0x0fe6: { // sts 0x0673, r16
        uint16_t address = 1651;
        pm_write(s, address, s->r[16]);
        return 4074;
    }
    case 0x0fea: { // ldi r16, 0x04
        s->r[16] = 4;
        return 4076;
    }
    case 0x0fec: { // sts 0x0681, r16
        uint16_t address = 1665;
        pm_write(s, address, s->r[16]);
        return 4080;
    }
    case 0x0ff0: { // ldi r16, 0x01
        s->r[16] = 1;
        return 4082;
    }
    case 0x0ff2: { // sts 0x0690, r16
        uint16_t address = 1680;
        pm_write(s, address, s->r[16]);
        return 4086;
    }
    case 0x0ff6: { // ldi r16, 0x00
        s->r[16] = 0;
        return 4088;
    }
    case 0x0ff8: { // sts 0x0691, r16
        uint16_t address = 1681;
        pm_write(s, address, s->r[16]);
        return 4092;
    }
    case 0x0ffc: { // ldi r16, 0x80
        s->r[16] = 128;
        return 4094;
    }
    case 0x0ffe: { // sts 0x0692, r16
        uint16_t address = 1682;
        pm_write(s, address, s->r[16]);
        return 4098;
    }
    case 0x1002: { // ldi r16, 0x00
        s->r[16] = 0;
        return 4100;
    }
    case 0x1004: { // sts 0x0693, r16
        uint16_t address = 1683;
        pm_write(s, address, s->r[16]);
        return 4104;
    }
    case 0x1008: { // ldi r16, 0x0A
        s->r[16] = 10;
        return 4106;
    }
    case 0x100a: { // sts 0x068A, r16
        uint16_t address = 1674;
        pm_write(s, address, s->r[16]);
        return 4110;
    }
    case 0x100e: { // ldi r16, 0x01
        s->r[16] = 1;
        return 4112;
    }
    case 0x1010: { // sts 0x068B, r16
        uint16_t address = 1675;
        pm_write(s, address, s->r[16]);
        return 4116;
    }
    case 0x1014: { // ldi r16, 0x09
        s->r[16] = 9;
        return 4118;
    }
    case 0x1016: { // sts 0x06A1, r16
        uint16_t address = 1697;
        pm_write(s, address, s->r[16]);
        return 4122;
    }
    case 0x101a: { // ldi r16, 0x00
        s->r[16] = 0;
        return 4124;
    }
    case 0x101c: { // sts 0x06B1, r16
        uint16_t address = 1713;
        pm_write(s, address, s->r[16]);
        return 4128;
    }
    case 0x1020: { // ldi r16, 0x00
        s->r[16] = 0;
        return 4130;
    }
    case 0x1022: { // sts 0x06B6, r16
        uint16_t address = 1718;
        pm_write(s, address, s->r[16]);
        return 4134;
    }
    case 0x1026: { // ldi r16, 0x10
        s->r[16] = 16;
        return 4136;
    }
    case 0x1028: { // sts 0x06B7, r16
        uint16_t address = 1719;
        pm_write(s, address, s->r[16]);
        return 4140;
    }
    case 0x102c: { // ldi r16, 0x40
        s->r[16] = 64;
        return 4142;
    }
    case 0x102e: { // sts 0x062A, r16
        uint16_t address = 1578;
        pm_write(s, address, s->r[16]);
        return 4146;
    }
    case 0x1032: { // ldi r16, 0x08
        s->r[16] = 8;
        return 4148;
    }
    case 0x1034: { // sts 0x066A, r16
        uint16_t address = 1642;
        pm_write(s, address, s->r[16]);
        return 4152;
    }
    case 0x1038: { // ldi r16, 0x02
        s->r[16] = 2;
        return 4154;
    }
    case 0x103a: { // sts 0x06AA, r16
        uint16_t address = 1706;
        pm_write(s, address, s->r[16]);
        return 4158;
    }
    case 0x103e: { // ldi r16, 0x40
        s->r[16] = 64;
        return 4160;
    }
    case 0x1040: { // sts 0x06AB, r16
        uint16_t address = 1707;
        pm_write(s, address, s->r[16]);
        return 4164;
    }
    case 0x1044: { // ldi r16, 0x01
        s->r[16] = 1;
        return 4166;
    }
    case 0x1046: { // sts 0x06A5, r16
        uint16_t address = 1701;
        pm_write(s, address, s->r[16]);
        return 4170;
    }
    case 0x104a: { // ldi r16, 0xD0
        s->r[16] = 208;
        return 4172;
    }
    case 0x104c: { // sts 0x0BA7, r16
        uint16_t address = 2983;
        pm_write(s, address, s->r[16]);
        return 4176;
    }
    case 0x1050: { // ldi r16, 0x83
        s->r[16] = 131;
        return 4178;
    }
    case 0x1052: { // sts 0x0BA6, r16
        uint16_t address = 2982;
        pm_write(s, address, s->r[16]);
        return 4182;
    }
    case 0x1056: { // ldi r16, 0x03
        s->r[16] = 3;
        return 4184;
    }
    case 0x1058: { // sts 0x0BA5, r16
        uint16_t address = 2981;
        pm_write(s, address, s->r[16]);
        return 4188;
    }
    case 0x105c: { // ldi r16, 0x18
        s->r[16] = 24;
        return 4190;
    }
    case 0x105e: { // sts 0x0BA4, r16
        uint16_t address = 2980;
        pm_write(s, address, s->r[16]);
        return 4194;
    }
    case 0x1062: { // ldi r16, 0x20
        s->r[16] = 32;
        return 4196;
    }
    case 0x1064: { // sts 0x0BA3, r16
        uint16_t address = 2979;
        pm_write(s, address, s->r[16]);
        return 4200;
    }
    case 0x1068: { // ldi r16, 0x06
        s->r[16] = 6;
        return 4202;
    }
    case 0x106a: { // sts 0x0800, r16
        uint16_t address = 2048;
        pm_write(s, address, s->r[16]);
        return 4206;
    }
    case 0x106e: { // ldi r16, 0x7D
        s->r[16] = 125;
        return 4208;
    }
    case 0x1070: { // sts 0x0826, r16
        uint16_t address = 2086;
        pm_write(s, address, s->r[16]);
        return 4212;
    }
    case 0x1074: { // ldi r16, 0x00
        s->r[16] = 0;
        return 4214;
    }
    case 0x1076: { // sts 0x0827, r16
        uint16_t address = 2087;
        pm_write(s, address, s->r[16]);
        return 4218;
    }
    case 0x107a: { // ldi r16, 0x00
        s->r[16] = 0;
        return 4220;
    }
    case 0x107c: { // sts 0x0801, r16
        uint16_t address = 2049;
        pm_write(s, address, s->r[16]);
        return 4224;
    }
    case 0x1080: { // ldi r16, 0x00
        s->r[16] = 0;
        return 4226;
    }
    case 0x1082: { // sts 0x0804, r16
        uint16_t address = 2052;
        pm_write(s, address, s->r[16]);
        return 4230;
    }
    case 0x1086: { // ldi r16, 0x01
        s->r[16] = 1;
        return 4232;
    }
    case 0x1088: { // sts 0x0806, r16
        uint16_t address = 2054;
        pm_write(s, address, s->r[16]);
        return 4236;
    }
    case 0x108c: { // ldi r16, 0x08
        s->r[16] = 8;
        return 4238;
    }
    case 0x108e: { // sts 0x01CC, r16
        uint16_t address = 460;
        pm_write(s, address, s->r[16]);
        return 4242;
    }
    case 0x1092: { // eor r25, r25
        s->r[25] ^= s->r[25];
        pm_nzv(s, s->r[25], false);
        return 4244;
    }
    case 0x1094: { // ldi r30, 0x00
        s->r[30] = 0;
        return 4246;
    }
    case 0x1096: { // ldi r31, 0x20
        s->r[31] = 32;
        return 4248;
    }
    case 0x1098: { // ldi r28, 0x43
        s->r[28] = 67;
        return 4250;
    }
    case 0x109a: { // ldi r29, 0x24
        s->r[29] = 36;
        return 4252;
    }
    case 0x109c: { // st Z+, r25
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        pm_write(s, address, s->r[25]);
        return 4254;
    }
    case 0x109e: { // cp r30, r28
        pm_sub(s, s->r[30], s->r[28], 0, false);
        return 4256;
    }
    case 0x10a0: { // cpc r31, r29
        pm_sub(s, s->r[31], s->r[29], pm_getflag(s, CARRY), true);
        return 4258;
    }
    case 0x10a2: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 4252 : 4260;
    }
    case 0x10a4: { // ldi r16, 0x03
        s->r[16] = 3;
        return 4262;
    }
    case 0x10a6: { // sts 0x2442, r16
        uint16_t address = 9282;
        pm_write(s, address, s->r[16]);
        return 4266;
    }
    case 0x10aa: { // lds r16, 0x06A8
        uint16_t address = 1704;
        s->r[16] = pm_read(s, address);
        return 4270;
    }
    case 0x10ae: { // sbrs r16, 1
        return (!!(s->r[16] & (1u << 1)) == 1) ? 4274 : 4272;
    }
    case 0x10b0: { // inc r25
        s->r[25]++;
        pm_nzv(s, s->r[25], s->r[25] == 128);
        return 4274;
    }
    case 0x10b2: { // sts 0x2004, r25
        uint16_t address = 8196;
        pm_write(s, address, s->r[25]);
        return 4278;
    }
    case 0x10b6: { // lds r16, 0x0688
        uint16_t address = 1672;
        s->r[16] = pm_read(s, address);
        return 4282;
    }
    case 0x10ba: { // bst r16, 1
        pm_flag(s, TRANSFER, s->r[16] & (1u << 1));
        return 4284;
    }
    case 0x10bc: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 4286;
    }
    case 0x10be: { // bld r16, 0
        s->r[16] = (s->r[16] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 4288;
    }
    case 0x10c0: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 4292;
    }
    case 0x10c4: { // brts .+10
        return (pm_getflag(s, 6) == 1) ? 4304 : 4294;
    }
    case 0x10c6: { // sbi 0x00, 1
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 1));
        return 4296;
    }
    case 0x10c8: { // ldi r16, 0x01
        s->r[16] = 1;
        return 4298;
    }
    case 0x10ca: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 4302;
    }
    case 0x10ce: { // rjmp .+18
        return 4322;
    }
    case 0x10d0: { // ldi r16, 0x01
        s->r[16] = 1;
        return 4306;
    }
    case 0x10d2: { // sts 0x215B, r16
        uint16_t address = 8539;
        pm_write(s, address, s->r[16]);
        return 4310;
    }
    case 0x10d6: { // ldi r16, 0xD0
        s->r[16] = 208;
        return 4312;
    }
    case 0x10d8: { // sts 0x215C, r16
        uint16_t address = 8540;
        pm_write(s, address, s->r[16]);
        return 4316;
    }
    case 0x10dc: { // ldi r16, 0x07
        s->r[16] = 7;
        return 4318;
    }
    case 0x10de: { // sts 0x215D, r16
        uint16_t address = 8541;
        pm_write(s, address, s->r[16]);
        return 4322;
    }
    case 0x10e2: { // ldi r16, 0xE8
        s->r[16] = 232;
        return 4324;
    }
    case 0x10e4: { // sts 0x215E, r16
        uint16_t address = 8542;
        pm_write(s, address, s->r[16]);
        return 4328;
    }
    case 0x10e8: { // ldi r16, 0x03
        s->r[16] = 3;
        return 4330;
    }
    case 0x10ea: { // sts 0x215F, r16
        uint16_t address = 8543;
        pm_write(s, address, s->r[16]);
        return 4334;
    }
    case 0x10ee: { // ldi r16, 0x01
        s->r[16] = 1;
        return 4336;
    }
    case 0x10f0: { // sts 0x0689, r16
        uint16_t address = 1673;
        pm_write(s, address, s->r[16]);
        return 4340;
    }
    case 0x10f4: { // ldi r16, 0x02
        s->r[16] = 2;
        return 4342;
    }
    case 0x10f6: { // sts 0x06A9, r16
        uint16_t address = 1705;
        pm_write(s, address, s->r[16]);
        return 4346;
    }
    case 0x10fa: { // ldi r16, 0x03
        s->r[16] = 3;
        return 4348;
    }
    case 0x10fc: { // sts 0x00A2, r16
        uint16_t address = 162;
        pm_write(s, address, s->r[16]);
        return 4352;
    }
    case 0x1100: { // call 0x25ea
        s->calls[s->call_depth++] = 4356;
        return 9706;
    }
    case 0x1104: { // ldi r16, 0x08
        s->r[16] = 8;
        return 4358;
    }
    case 0x1106: { // ldi r17, 0x50
        s->r[17] = 80;
        return 4360;
    }
    case 0x1108: { // call 0x2598
        s->calls[s->call_depth++] = 4364;
        return 9624;
    }
    case 0x110c: { // ldi r16, 0x20
        s->r[16] = 32;
        return 4366;
    }
    case 0x110e: { // ldi r17, 0x00
        s->r[17] = 0;
        return 4368;
    }
    case 0x1110: { // ldi r18, 0x23
        s->r[18] = 35;
        return 4370;
    }
    case 0x1112: { // call 0x25be
        s->calls[s->call_depth++] = 4374;
        return 9662;
    }
    case 0x1116: { // ldi r16, 0x30
        s->r[16] = 48;
        return 4376;
    }
    case 0x1118: { // ldi r17, 0x00
        s->r[17] = 0;
        return 4378;
    }
    case 0x111a: { // ldi r18, 0x1E
        s->r[18] = 30;
        return 4380;
    }
    case 0x111c: { // call 0x25be
        s->calls[s->call_depth++] = 4384;
        return 9662;
    }
    case 0x1120: { // ldi r30, 0x63
        s->r[30] = 99;
        return 4386;
    }
    case 0x1122: { // ldi r31, 0x21
        s->r[31] = 33;
        return 4388;
    }
    case 0x1124: { // ldi r28, 0x00
        s->r[28] = 0;
        return 4390;
    }
    case 0x1126: { // ldi r29, 0x10
        s->r[29] = 16;
        return 4392;
    }
    case 0x1128: { // ldi r17, 0x22
        s->r[17] = 34;
        return 4394;
    }
    case 0x112a: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 4396;
    }
    case 0x112c: { // st Z+, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        pm_write(s, address, s->r[16]);
        return 4398;
    }
    case 0x112e: { // cpi r30, 0x35
        pm_sub(s, s->r[30], 53, 0, false);
        return 4400;
    }
    case 0x1130: { // cpc r31, r17
        pm_sub(s, s->r[31], s->r[17], pm_getflag(s, CARRY), true);
        return 4402;
    }
    case 0x1132: { // brne .-10
        return (pm_getflag(s, 1) == 0) ? 4394 : 4404;
    }
    case 0x1134: { // cbi 0x00, 3
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 3));
        return 4406;
    }
    case 0x1136: { // sei
        pm_irq(s, true);
        return 4408;
    }
    case 0x1138: { // ldi r30, 0x62
        s->r[30] = 98;
        return 4410;
    }
    case 0x113a: { // ldi r31, 0x29
        s->r[31] = 41;
        return 4412;
    }
    case 0x113c: { // call 0x2826
        s->calls[s->call_depth++] = 4416;
        return 10278;
    }
    case 0x1140: { // ldi r16, 0x01
        s->r[16] = 1;
        return 4418;
    }
    case 0x1142: { // sts 0x06A6, r16
        uint16_t address = 1702;
        pm_write(s, address, s->r[16]);
        return 4422;
    }
    case 0x1146: { // lds r16, 0x2006
        uint16_t address = 8198;
        s->r[16] = pm_read(s, address);
        return 4426;
    }
    case 0x114a: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 4428;
    }
    case 0x114c: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 4432 : 4430;
    }
    case 0x114e: { // rcall .+12
        s->calls[s->call_depth++] = 4432;
        return 4444;
    }
    case 0x1150: { // lds r16, 0x2005
        uint16_t address = 8197;
        s->r[16] = pm_read(s, address);
        return 4436;
    }
    case 0x1154: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 4438;
    }
    case 0x1156: { // breq .-18
        return (pm_getflag(s, 1) == 1) ? 4422 : 4440;
    }
    case 0x1158: { // rcall .+400
        s->calls[s->call_depth++] = 4442;
        return 4842;
    }
    case 0x115a: { // rjmp .-22
        return 4422;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
