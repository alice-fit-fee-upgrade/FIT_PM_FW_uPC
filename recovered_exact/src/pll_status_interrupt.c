#include "legacy_interrupt.h"
#include "legacy_r16.h"
#define WRITE_STATE(address, constant) do { value = (constant); \
    asm volatile("" : "+r" (value)); PM_RAM8(address) = value; } while (0)

void PORTF_INT1_vect_isr(void)
{
    asm volatile("push r31\n\tin r31, 0x3f\n\tpush r16\n\tpush r17\n\tpush r18\n\tpush r19" : : : "memory");
    register uint8_t value asm("r16") = PORTF_IN;
    register uint8_t b1 asm("r17"), b2 asm("r18"), status asm("r19");
    asm goto("bst %0, 6\n\tbrtc %l[read_status]\n\tsbrc %0, 5\n\trjmp %l[read_status]" : : "r" (value) : "cc" : read_status);
    value = 0xf5; asm volatile("" : "+r" (value));
    b1 = 0x0f; asm volatile("" : "+r" (b1));
    b2 = 4; asm volatile("" : "+r" (b2));
    status = 0x40;
    asm volatile("call CDCE62005_send_control_settings" : "+r" (value), "+r" (b1), "+r" (b2), "+r" (status) : : "memory", "cc");
    value = 0x20; asm volatile("" : "+r" (value));
    PORTF_OUTSET = value;
read_status:
    asm volatile("call FUN_code_001267" : "=r" (value), "=r" (b1), "=r" (b2), "=r" (status) : : "memory", "cc");
    asm volatile("swap %0" : "+r" (status));
    status &= 0x0e; asm volatile("" : "+r" (status));
    asm volatile("bld %0, 0" : "+r" (status) : : "cc");
    value = pm_read_absolute(0x2162);
    value &= 0x0e; asm volatile("" : "+r" (value));
    PM_RAM8(0x2162) = status;
    value ^= status; asm volatile("" : "+r" (value));
    asm goto("breq %l[power_state]" : : "r" (value) : : power_state);
    value = 0x20; asm volatile("" : "+r" (value));
    asm goto("sbrs %0, 1\n\trjmp %l[second_alarm]" : : "r" (status) : : second_alarm);
    PORTA_OUTCLR = value;
    asm volatile("cbi 0, 2" : : : "memory");
    goto power_state;
second_alarm:
    asm goto("sbrs %0, 2\n\trjmp %l[third_alarm]" : : "r" (status) : : third_alarm);
    asm volatile("sbi 0, 2" : : : "memory");
third_alarm:
    asm goto("sbrs %0, 3\n\trjmp %l[power_state]" : : "r" (status) : : power_state);
    value = 0x20; asm volatile("" : "+r" (value));
    PORTA_OUTSET = value;
    asm volatile("cbi 0, 2" : : : "memory");
power_state:
    asm goto("brtc %l[fault]" : : : : fault);
    WRITE_STATE(0x215a, 10);
    WRITE_STATE(0x2159, 1);
    goto finished;
fault:
    asm volatile("rcall FUN_code_0005b4" : "=r" (value) : : "memory", "cc");
    WRITE_STATE(0x2441, 2);
finished:
    asm volatile("pop r19\n\tpop r18\n\tpop r17\n\tpop r16\n\tout 0x3f, r31\n\tpop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}
