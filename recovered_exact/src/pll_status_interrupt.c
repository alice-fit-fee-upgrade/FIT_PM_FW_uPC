#include <avr/io.h>
/* Calls retain private fixed-register inputs/results through zero-byte
 * barriers. Void declarations deliberately introduce no GNU arguments/results;
 * recapture after each call observes the original registers. Every conversion
 * is accepted only if GNU CALL, frames and the entire FLASH remain identical. */
extern void CDCE62005_send_control_settings(void);
extern void FUN_code_001267(void);
#include "legacy_cpu.h"
#include "legacy_interrupt.h"
#include "legacy_r16_c.h"
#define WRITE_STATE(address, constant) do { value = (constant); \
    asm volatile("" : "+r" (value)); PM_RAM8(address) = value; } while (0)

void PORTF_INT1_vect_isr(void)
{
    asm volatile("push r31" : : : "memory");
    {
        register uint8_t saved_status asm("r31") = SREG;
        asm volatile("" : "+r" (saved_status) : : "memory");
    }
    asm volatile("push r16\n\tpush r17\n\tpush r18\n\tpush r19" : : : "memory");
    register uint8_t value asm("r16") = PORTF_IN;
    register uint8_t b1 asm("r17"), b2 asm("r18"), status asm("r19");
    asm goto("bst %0, 6\n\tbrtc %l[read_status]\n\tsbrc %0, 5\n\trjmp %l[read_status]" : : "r" (value) : "cc" : read_status);
    value = 0xf5; asm volatile("" : "+r" (value));
    b1 = 0x0f; asm volatile("" : "+r" (b1));
    b2 = 4; asm volatile("" : "+r" (b2));
    status = 0x40;
    do {
        asm volatile("" : "+r" (value), "+r" (b1), "+r" (b2), "+r" (status) :  : "memory");
        CDCE62005_send_control_settings();
        asm volatile("" : "=r" (value), "=r" (b1), "=r" (b2), "=r" (status) : : "memory");
    } while (0);
    value = 0x20; asm volatile("" : "+r" (value));
    PORTF_OUTSET = value;
read_status:
    do {

        FUN_code_001267();
        asm volatile("" : "=r" (value), "=r" (b1), "=r" (b2), "=r" (status) : : "memory");
    } while (0);
    status = __builtin_avr_swap(status); asm volatile("" : "+r" (status));
    status &= 0x0e; asm volatile("" : "+r" (status));
    asm volatile("bld %0, 0" : "+r" (status) : : "cc");
    value = pm_read_absolute(0x2162);
    value &= 0x0e; asm volatile("" : "+r" (value));
    PM_RAM8(0x2162) = status;
    value ^= status; asm volatile("" : "+r" (value));
    asm goto("breq %l[power_state]" : : "r" (value) : : power_state);
    value = 0x20; asm volatile("" : "+r" (value));
    if (!(status & (1u << 1))) goto second_alarm;
    PORTA_OUTCLR = value;
    GPIOR0 &= (uint8_t)~(1u << 2);
    goto power_state;
second_alarm:
    if (!(status & (1u << 2))) goto third_alarm;
    /* C equivalent: GPIOR0 |= (1u << 2);
     * Steps351/405 changed encoding/layout, including an opaque status retry.
     * No new standalone functional test is claimed. */
    asm volatile("sbi 0, 2" : : : "memory");
third_alarm:
    if (!(status & (1u << 3))) goto power_state;
    value = 0x20; asm volatile("" : "+r" (value));
    PORTA_OUTSET = value;
    GPIOR0 &= (uint8_t)~(1u << 2);
power_state:
    asm goto("brtc %l[fault]" : : : : fault);
    WRITE_STATE(0x215a, 10);
    WRITE_STATE(0x2159, 1);
    goto finished;
fault:
    asm volatile("rcall FUN_code_0005b4" : "=r" (value) : : "memory", "cc");
    WRITE_STATE(0x2441, 2);
finished:
    asm volatile("pop r19\n\tpop r18\n\tpop r17\n\tpop r16" : : : "memory");
    {
        register uint8_t saved_status asm("r31");
        asm volatile("" : "=r" (saved_status) : : "memory");
        SREG = saved_status;
    }
    /* Restore the private frame; ordinary C returns cannot express RETI. */
    asm volatile("pop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}
