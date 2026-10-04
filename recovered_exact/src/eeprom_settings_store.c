#include <avr/io.h>
#define SET_VALUE(constant) do { value=(constant); asm volatile("" : "+r" (value)); } while (0)

void eeprom_settings_save(void)
{
    register uint8_t value asm("r16"), previous asm("r17"), offset asm("r18"), dirty asm("r19"), end_high asm("r20");
    asm volatile("rcall cli_get_next_char\n\tcpi r16, 13\n\tbrne LAB_code_000cd6" : "=r" (value) : : "memory", "cc");
wait_nvm:
    value = NVM_STATUS;
    asm goto("sbrc %0, 7\n\trjmp %l[wait_nvm]" : : "r" (value) : : wait_nvm);
    SET_VALUE(0x36); NVM_CMD = value;
    SET_VALUE(1);
    previous = 0xd8; asm volatile("" : "+r" (previous));
    asm volatile("cli\n\tsts %0, %1" : : "n" (_SFR_MEM_ADDR(CCP)), "r" (previous) : "memory");
    NVM_CTRLA = value;
    asm volatile("sei" : : : "memory");
    register const uint8_t *settings asm("r30") = (const uint8_t *)0x2163;
    asm volatile("" : "+z" (settings));
    register uint8_t *eeprom asm("r28") = (uint8_t *)0x0fff;
    asm volatile("" : "+y" (eeprom));
    end_high = 0x22; asm volatile("" : "+r" (end_high));
wait_buffer:
    value = NVM_STATUS;
    asm goto("sbrc %0, 7\n\trjmp %l[wait_buffer]" : : "r" (value) : : wait_buffer);
    asm volatile("sbi 0, 3\n\tclr %0" : "=r" (dirty) : : "memory", "cc");
next_byte:
    asm volatile("adiw %0, 1\n\tld %1, Z+\n\tld %2, Y"
        : "+y" (eeprom), "=r" (value), "=r" (previous), "+z" (settings) : : "memory", "cc");
    previous ^= value; asm volatile("" : "+r" (previous));
    asm goto("breq %l[unchanged]" : : "r" (previous) : : unchanged);
    asm volatile("st Y, %0" : : "r" (value), "y" (eeprom) : "memory");
    dirty = 1; asm volatile("" : "+r" (dirty));
unchanged:
    asm goto("cpi r30, 0x35\n\tcpc r31, %1\n\tbreq %l[finished]"
        : : "z" (settings), "r" (end_high) : "cc" : finished);
    asm volatile("mov %0, r28" : "=r" (offset) : "y" (eeprom));
    offset &= 0x1f; asm volatile("" : "+r" (offset));
    asm goto("cpi %0, 0x1f\n\tbrne %l[next_byte]" : : "r" (offset) : "cc" : next_byte);
    asm volatile("rcall FUN_code_000d07" : "+r" (dirty), "=r" (value), "=r" (previous)
        : "y" (eeprom) : "memory", "cc");
    goto next_byte;
finished:
    asm volatile("rcall FUN_code_000d07\n\tcbi 0, 3\n\trjmp LAB_code_000ff1"
        : : "y" (eeprom), "r" (dirty) : "memory", "cc");
    __builtin_unreachable();
}
