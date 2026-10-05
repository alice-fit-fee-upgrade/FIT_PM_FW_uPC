extern void cli_send_buf(void);
/* Private entries: capture R16/R17 after calls; no GNU result ABI is used. */
extern void cli_get_next_byte(void);
extern void cli_send_32bit_hex(void);
extern void cli_send_crlf(void);
#include <stdint.h>
/* Register objects exist only in this translation unit, with no SRAM storage.
 * They reserve the original word pairs and suppress GNU save/restore frames. */
register uint16_t pm_stream_start_address asm("r28");
register uint16_t current asm("r20");
#define RAM8(address) (*(volatile uint8_t *)(address))
#define RECEIVE(byte) do { \
 cli_get_next_byte(); \
 asm volatile("" : "=r" (byte) : : "memory"); \
} while (0)
#define PRINT_HEX(word) do { \
 asm volatile("" : "+r" (word) : : "memory"); \
 cli_send_32bit_hex(); \
 asm volatile("" : "=r" (word) : : "memory"); \
} while (0)

/* Original binary programming stream: two page-address bytes, an upper byte,
 * three limit bytes, then chunks in the existing 512-byte circular buffer. */
void fpga_firmware_update(void)
{
    register uint8_t byte asm("r16");
    asm volatile("rcall unlock_programming\n\tbrcs LAB_code_000ad2\n\tclr r16" : "=r" (byte) : : "memory", "cc");
    RAM8(0x0100) = byte;
    asm volatile("rcall FUN_code_000b32\n\tclr r16" : "=r" (byte) : : "memory", "cc");
    register uint8_t *pointer asm("r26") = (uint8_t *)0x2435;
    asm volatile("st X+, r16\n\tst X+, r16\n\tst X+, r16\n\tst X, r16\n\trcall FUN_code_000c20\n\tclr r17"
                 : "+x" (pointer), "=r" (byte) : : "r17", "r18", "r19", "memory", "cc");
    register uint16_t word asm("r16"), second asm("r18");
    asm volatile("" : "=r" (word), "=r" (second));
    PRINT_HEX(word);
    word = second; PRINT_HEX(word);
    cli_send_crlf();
    RECEIVE(byte);
    register uint8_t address_low asm("r28") = byte;
    asm volatile("" : "+r" (address_low));
    RECEIVE(byte);
    register uint8_t address_middle asm("r29") = byte;
    asm volatile("" : "+r" (address_middle));
    RECEIVE(byte);
    register uint8_t address_high asm("r30") = byte;
    asm volatile("" : "+r" (address_high));
    /* The original limit lives in R2:R0, including a nonzero R1. */
    cli_get_next_byte();
    asm volatile("" : "=r" (byte) : : "memory");
    {
        register uint8_t limit_byte asm("r0") = byte;
        asm volatile("" : "+r" (limit_byte) : : "memory");
    }
    cli_get_next_byte();
    /* C value equivalent: limit_middle = received;
     * Keep the original live nonzero R1 limit byte, outside GNU zero ABI. */
    asm volatile("mov r1, r16" : : : "r1", "memory");
    cli_get_next_byte();
    asm volatile("" : "=r" (byte) : : "memory");
    {
        register uint8_t limit_byte asm("r2") = byte;
        asm volatile("" : "+r" (limit_byte) : : "memory");
    }

    /* Earlier local bindings (361/364) changed region size. Global pair
     * reservations reproduce the original MOVW in C (accepted step371). */
    asm volatile("" : "+r" (pm_stream_start_address) : "r" (address_low), "r" (address_middle));
    current = pm_stream_start_address;
    asm volatile("" : "+r" (current));
    register uint8_t current_high asm("r22") = address_high;
    asm volatile("" : "+r" (current_high));
next_page:
    asm volatile("rcall FUN_code_000b88" : : : "memory", "cc");
next_chunk:
    /* Capture the private low limit; R1 remains outside the GNU zero ABI. */
    {
        register uint8_t limit_low asm("r0");
        asm volatile("" : "=r" (limit_low));
        register uint8_t remaining_low asm("r17") = limit_low;
        asm volatile("" : "+r" (remaining_low));
    }
    asm volatile("mov r18, r1" : : : "r18");
    {
        register uint8_t limit_high asm("r2");
        asm volatile("" : "=r" (limit_high));
        register uint8_t remaining_high asm("r19") = limit_high;
        asm volatile("" : "+r" (remaining_high));
    }
    byte = 1;
    asm volatile("" : "+r" (byte) : : "memory");
    cli_send_buf();
    asm volatile("" : "=r" (byte) : : "memory");
    /* C value equivalent: remaining_limit -= current_address;
     * Keep original SUB/SBC flags and 24-bit private register ordering. */
    {
        register uint8_t remaining_low asm("r17"), current_low asm("r20");
        asm volatile("" : "=r" (remaining_low), "=r" (current_low) : : "memory");
        remaining_low -= current_low;
        asm volatile("" : "+r" (remaining_low));
    }
    /* Upper bytes consume the exact borrow from the C-generated SUB. */
    asm volatile("sbc r18, r21\n\tsbc r19, r22\n"
                 /* C value: ++remaining_count; trial419 with global R17:R19
                  * changed layout. No successful functional test is claimed. */
                 "subi r17, 0xff\n\tsbci r18, 0xff\n\tsbci r19, 0xff"
                 : "+r" (byte) : : "r17", "r18", "r19", "memory", "cc");
    {
        register uint8_t middle asm("r18"), high asm("r19");
        asm volatile("" : "=r" (middle), "=r" (high));
        middle |= high;
        asm volatile("" : "+r" (middle));
    }
    asm goto("breq %l[remaining_ready]" : : : : remaining_ready);
    /* C value: remaining = 0; retain the original flag-setting CLR. */
    asm volatile("clr r17" : : : "r17", "cc");
remaining_ready:;
    register uint8_t remaining asm("r17");
    asm volatile("" : "=r" (remaining));
    register uint16_t write_index asm("r24") = *(volatile uint16_t *)0x2435;
    asm volatile("" : "+r" (write_index));
receive_chunk:
    pointer = (uint8_t *)0x2235;
    asm volatile("" : "+x" (pointer));
    pointer += write_index; asm volatile("" : "+x" (pointer));
    RECEIVE(byte);
    *pointer = byte;
    write_index += 1; asm volatile("" : "+r" (write_index));
    write_index &= 0x01ff; asm volatile("" : "+r" (write_index));
    asm volatile("dec %0" : "+r" (remaining) : : "cc");
    asm goto("brne %l[receive_chunk]" : : : : receive_chunk);
    RAM8(0x2435) = (uint8_t)write_index;
    RAM8(0x2436) = write_index >> 8;
    asm volatile("rcall FUN_code_000b9d\n\trcall FUN_code_000b59" : : : "memory", "cc");
    asm goto("brcs %l[finished]" : : : : finished);
    asm goto("tst r21\n\tbrne %l[next_chunk]" : : : "cc" : next_chunk);
    asm volatile("rcall FUN_code_000b9d" : : : "memory", "cc");
    goto next_page;
finished:
    asm volatile("rcall FUN_code_000b9d\n\trcall FUN_code_000bb9" : "=r" (word), "=r" (second) : : "memory", "cc");
    PRINT_HEX(word);
    word = second; PRINT_HEX(word);
    cli_send_crlf();
    /* C operation equivalent: flash_deinit(); return_to_prompt();
     * Keep the short RCALL and shared absolute tail exactly encoded. */
    asm volatile("rcall FUN_code_000b3f\n\trjmp LAB_code_000ff1" : : : "memory", "cc");
    __builtin_unreachable();
}
