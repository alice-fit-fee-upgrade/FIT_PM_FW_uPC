/* C store equivalent of the retained protected instruction: CCP = previous;
 * GCC may choose OUT instead of the original four-byte STS. Keep STS and
 * the original protected-write window; the comment is explanatory only. */
#include "legacy_cpu.h"
#include <avr/io.h>
#include <stdint.h>
register uint8_t *eeprom asm("r28");
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
    asm volatile("" : : "r" (previous), "r" (value) : "memory");
    pm_cpu_disable_irq();
    asm volatile("sts %0, %1" : : "n" (_SFR_MEM_ADDR(CCP)), "r" (previous) : "memory");
    NVM_CTRLA = value;
    pm_cpu_enable_irq();
    register const uint8_t *settings asm("r30") = (const uint8_t *)0x2163;
    asm volatile("" : "+z" (settings));
    eeprom = (uint8_t *)0x0fff;
    asm volatile("" : "+y" (eeprom));
    end_high = 0x22; asm volatile("" : "+r" (end_high));
wait_buffer:
    value = NVM_STATUS;
    asm goto("sbrc %0, 7\n\trjmp %l[wait_buffer]" : : "r" (value) : : wait_buffer);
    GPIOR0 |= (1u << 3);
    /* C value: dirty = 0; retain flag-setting CLR rather than LDI/MOV zero. */
    asm volatile("clr %0" : "=r" (dirty) : : "cc");
next_byte:
    eeprom += 1;
    asm volatile("" : "+y" (eeprom) : : "memory");
    asm volatile("ld %0, Z+" : "=r" (value), "+z" (settings) : : "memory");
    previous = *eeprom;
    asm volatile("" : "+r" (previous) : : "memory");
    previous ^= value; asm volatile("" : "+r" (previous));
    asm goto("breq %l[unchanged]" : : "r" (previous) : : unchanged);
    *eeprom = value;
    asm volatile("" : : : "memory");
    dirty = 1; asm volatile("" : "+r" (dirty));
unchanged:
    asm goto("cpi r30, 0x35\n\tcpc r31, %1\n\tbreq %l[finished]"
        : : "z" (settings), "r" (end_high) : "cc" : finished);
    offset = (uint8_t)(uintptr_t)eeprom;
    asm volatile("" : "+r" (offset));
    offset &= 0x1f; asm volatile("" : "+r" (offset));
    if (offset != 0x1f) goto next_byte;
    asm volatile("rcall FUN_code_000d07" : "+r" (dirty), "=r" (value), "=r" (previous)
        : "y" (eeprom) : "memory", "cc");
    goto next_byte;
finished:
    asm volatile("rcall FUN_code_000d07" : : "y" (eeprom), "r" (dirty) : "memory", "cc");
    GPIOR0 &= (uint8_t)~(1u << 3);
    asm volatile("rjmp LAB_code_000ff1" : : : "memory");
    __builtin_unreachable();
}
