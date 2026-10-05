#include <stdint.h>
/* Archived C alternative for the exact ASM helpers below (not compiled).
 * This implementation, with its historical private-ABI ASM bridge and callees,
 * passed the functional comparisons recorded in docs/mixed_status_gate_checkpoint.md.
 * The historical integrated FLASH differed from PM.hex; it is NOT an accepted
 * baseline implementation. Tests do not establish timing/async IRQ/hardware
 * equivalence. Full register/SREG restoration belongs to the archived bridge.
 * Dependencies (types, MMIO definitions, helper functions and bridge contracts):
 * mixed_c_asm/src/status_gate.c; see that source and the checkpoint for the full context.
 * Scope is the archived function shown; wrappers/callees in that tree cover
 * additional behavior. Individual ASM fragments were not independently
 * validated as standalone plain C implementations.
 *
 * uint8_t pm_status_gate_allowed(uint8_t status)
 * {
 *     return (uint8_t)((status & 0x10u) != 0);
 * }
 */

#define RAM8(address) (*(volatile uint8_t *)(address))

/* Carry is the result: clear means ready, set means the error was printed. */
void FUN_code_00108e(void)
{
    asm volatile("clc" : : : "cc");
    register uint8_t status asm("r16") = RAM8(0x2157);
    asm volatile("sbrc %0, 4\n\tret" : : "r" (status));
    register const uint8_t *message asm("r30") = (const uint8_t *)0x2ab4;
    asm volatile("rcall cli_send_msg\n\tsec" : "+z" (message) : : "memory", "cc");
}
