#include "legacy_cpu.h"
#include <stdint.h>
#define RAM8(address) (*(volatile uint8_t *)(address))
#define SET_MESSAGE(address) do { message = (const uint8_t *)(address); asm volatile("" : "+z" (message)); } while (0)
#define SEND_MESSAGE() asm volatile("rcall cli_send_msg" : "+z" (message) : : "memory", "cc")

/* Original status message order and early returns are intentionally retained.
 * Message addresses name existing FLASH strings, never linker-created copies. */
void cli_send_system_status(void)
{
    register uint16_t word asm("r16");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000d1e" : "=r" (word) : : "memory", "cc");
    register const uint8_t *message asm("r30");
    SET_MESSAGE(0x2b6a); SEND_MESSAGE();
    word = *(volatile uint16_t *)0x2232;
    asm volatile("rcall cli_send_32bit_hex" : "+r" (word) : : "memory", "cc");
    SET_MESSAGE(0x2b7e); SEND_MESSAGE();
    SET_MESSAGE(0x2b92);
    register uint16_t second asm("r18");
    asm volatile("lpm r18, Z+\n\tlpm r19, Z+\n\tlpm r16, Z+\n\tlpm r17, Z+\n\trcall cli_send_32bit_hex"
                 : "=r" (second), "=r" (word), "+z" (message) : : "memory", "cc");
    word = second;
    asm volatile("rcall cli_send_32bit_hex" : "+r" (word) : : "memory", "cc");
    SET_MESSAGE(0x29a6); SEND_MESSAGE();
    pm_cpu_disable_irq();
    register uint8_t status asm("r18") = RAM8(0x2157);
    asm volatile("" : "+r" (status));
    word = *(volatile uint16_t *)0x2160;
    asm volatile("" : "+r" (word));
    register uint8_t power asm("r20") = RAM8(0x0688);
    asm volatile("sei" : : "r" (power) : "memory");
    SET_MESSAGE(0x2998);
    asm goto("sbrc r18, 0\n\trjmp %l[power_state]" : : "r" (status) : : power_state);
    SET_MESSAGE(0x299e);
power_state:
    SEND_MESSAGE();
    register uint8_t thermal asm("r19") = status;
    asm volatile("" : "+r" (thermal), "+r" (status));
    SET_MESSAGE(0x29c0); SEND_MESSAGE();
    asm volatile("rcall cli_send_temperature" : "+r" (word) : : "memory", "cc");
    SET_MESSAGE(0x29ce);
    thermal &= 6; asm volatile("" : "+r" (thermal));
    asm goto("breq %l[thermal_state]" : : : : thermal_state);
    SET_MESSAGE(0x29e2);
    asm goto("cpi r19, 6\n\tbreq %l[thermal_state]" : : "r" (thermal) : "cc" : thermal_state);
    SET_MESSAGE(0x29dc);
    thermal &= 4; asm volatile("" : "+r" (thermal));
    asm goto("brne %l[thermal_state]" : : : : thermal_state);
    SET_MESSAGE(0x29d6);
thermal_state:
    SEND_MESSAGE();
    SET_MESSAGE(0x29ec); SEND_MESSAGE();
    asm goto("sbrc r20, 2\n\trjmp %l[pll_powered]" : : "r" (power) : : pll_powered);
    SET_MESSAGE(0x2a02);
message_and_return:
    SEND_MESSAGE();
    asm volatile("ret"); __builtin_unreachable();
pll_powered:
    SET_MESSAGE(0x29fc); SEND_MESSAGE();
    SET_MESSAGE(0x2998);
    asm goto("sbrc r18, 3\n\trjmp %l[pll_locked]" : : "r" (status) : : pll_locked);
    SET_MESSAGE(0x299e); SEND_MESSAGE();
    asm volatile("sbrs r18, 6\n\tret" : : "r" (status));
    SET_MESSAGE(0x2af2);
    goto message_and_return;
pll_locked:
    SEND_MESSAGE();
    SET_MESSAGE(0x2a28); SEND_MESSAGE();
    thermal = RAM8(0x2162);
    asm volatile("" : "+r" (thermal));
    SET_MESSAGE(0x2a36);
    asm goto("sbrc r19, 1\n\trjmp %l[pll_configuration]" : : "r" (thermal) : : pll_configuration);
    SET_MESSAGE(0x2a3e);
    asm goto("sbrc r19, 2\n\trjmp %l[pll_configuration]" : : "r" (thermal) : : pll_configuration);
    SET_MESSAGE(0x2a48);
pll_configuration:
    SEND_MESSAGE();
    SET_MESSAGE(0x2a52); SEND_MESSAGE();
    SET_MESSAGE(0x2a5a);
    asm goto("sbrs r19, 0\n\trjmp %l[pll_control]" : : "r" (thermal) : : pll_control);
    SET_MESSAGE(0x2a5e);
pll_control:
    SEND_MESSAGE();
    SET_MESSAGE(0x2b04);
    asm goto("sbrc r18, 4\n\trjmp %l[fpga_ready]" : : "r" (status) : : fpga_ready);
    SET_MESSAGE(0x2ab4);
    goto message_and_return;
fpga_ready:
    SEND_MESSAGE();
    SET_MESSAGE(0x2b12); SEND_MESSAGE();
    SET_MESSAGE(0x2998);
    asm goto("sbrs r18, 7\n\trjmp %l[tdc_inactive]" : : "r" (status) : : tdc_inactive);
    SET_MESSAGE(0x2b20); SEND_MESSAGE();
    power = RAM8(0x2158);
    register uint8_t device asm("r17");
    asm volatile("clr r17" : "=r" (device) : : "cc");
tdc_alarm:
    asm goto("sbrs r20, 2\n\trjmp %l[next_tdc]" : : "r" (power) : : next_tdc);
    SET_MESSAGE(0x2b28); SEND_MESSAGE();
    register uint8_t character asm("r16") = '0';
    asm volatile("" : "+r" (character));
    character += device;
    asm volatile("rcall cli_send_buf" : "+r" (character) : : "memory", "cc");
    SET_MESSAGE(0x2b2e); SEND_MESSAGE();
next_tdc:
    asm goto("cpi r17, 2\n\tbrcc %l[finished]" : : "r" (device) : "cc" : finished);
    asm volatile("inc r17\n\tlsr r20" : "+r" (device), "+r" (power) : : "cc");
    goto tdc_alarm;
tdc_inactive:
    SEND_MESSAGE();
    SET_MESSAGE(0x2b96); SEND_MESSAGE();
    power = RAM8(0x2441);
    power += power; asm volatile("" : "+r" (power));
    SET_MESSAGE(0x2ba8);
    register uint8_t zero asm("r21");
    asm volatile("clr r21\n\tadd r30, r20\n\tadc r31, r21\n\tlpm r20, Z+\n\tlpm r21, Z"
                 : "=r" (zero), "+z" (message), "+r" (power) : : "memory", "cc");
    register uint16_t selected_message asm("r20");
    asm volatile("" : "=r" (selected_message));
    message = (const uint8_t *)selected_message;
    SEND_MESSAGE();
    SET_MESSAGE(0x2984); SEND_MESSAGE();
    power = RAM8(0x2158);
    SET_MESSAGE(0x2b44); SEND_MESSAGE();
    SET_MESSAGE(0x2a02);
    asm goto("sbrc r20, 0\n\trjmp %l[tdc_powered]" : : "r" (power) : : tdc_powered);
    SEND_MESSAGE();
    asm volatile("ret"); __builtin_unreachable();
tdc_powered:
    SET_MESSAGE(0x29fc); SEND_MESSAGE();
    SET_MESSAGE(0x2b4e);
    asm goto("sbrs r20, 1\n\trjmp %l[tdc_configuration]" : : "r" (power) : : tdc_configuration);
    SET_MESSAGE(0x2b5c);
tdc_configuration:
    SEND_MESSAGE();
finished:
    asm volatile("ret"); __builtin_unreachable();
}
