#include "legacy_cpu.h"
#include <avr/io.h>

void pm_eeprom_page_commit(void) asm("FUN_code_000d07");
void pm_eeprom_page_commit(void)
{
    register uint8_t dirty asm("r19"), low asm("r28"), high asm("r29");
    asm volatile("" : "=r" (dirty), "=r" (low), "=r" (high));
    if (dirty == 0) goto finished;
    register uint8_t command asm("r16") = 0x35;
    asm volatile("" : "+r" (command));
    NVM_CMD = command;
    NVM_ADDR0 = low;
    NVM_ADDR1 = high;
    command = 1;
    asm volatile("" : "+r" (command));
    register uint8_t unlock asm("r17") = 0xd8;
    asm volatile("" : "+r" (unlock));
    pm_cpu_disable_irq();
    asm volatile("sts %0, %1" : : "n" (_SFR_MEM_ADDR(CCP)), "r" (unlock) : "memory");
    NVM_CTRLA = command;
    pm_cpu_enable_irq();
wait_ready:
    command = NVM_STATUS;
    asm goto("sbrc %0, 7\n\trjmp %l[wait_ready]" : : "r" (command) : : wait_ready);
    asm volatile("clr %0" : "=r" (dirty) : : "cc");
finished:
    asm volatile("" : : "r" (dirty));
}
/* Adjacent error jump is another entry, not padding or part of the C return. */
asm(".pushsection .text.FUN_code_000d07,\"ax\",@progbits\n"
    ".subsection 1\nrjmp LAB_code_0009e4\n.popsection");
