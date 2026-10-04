#include "legacy_interrupt.h"
#include "legacy_r16.h"

void PORTE_INT1_vect_isr(void)
{
    asm volatile("push r31\n\tin r31, 0x3f\n\tpush r16\n\tpush r17\n\tpush r18" : : : "memory");
    register uint8_t lo asm("r16"), hi asm("r17"), address asm("r18") = 0x7f;
    asm volatile("call fpga_msg_read_t1" : "=r" (lo), "=r" (hi), "+r" (address) : : "memory", "cc");
    asm goto("sbrs %0, 1\n\trjmp %l[merge_status]" : : "r" (hi) : : merge_status);
    address = PM_RAM8(0x2006);
    address |= 1;
    asm volatile("" : "+r" (address));
    PM_RAM8(0x2006) = address;
merge_status:
    asm volatile("swap %0" : "+r" (lo));
    lo &= 0x0f;
    asm volatile("" : "+r" (lo));
    asm volatile("swap %0" : "+r" (hi));
    hi &= 0x10;
    asm volatile("" : "+r" (hi));
    hi |= lo;
    asm volatile("" : "+r" (hi));
    lo = pm_read_absolute(0x2158);
    lo &= 0xfe;
    asm volatile("" : "+r" (lo));
    lo |= hi;
    asm volatile("" : "+r" (lo));
    PM_RAM8(0x2158) = lo;
    /* Original ANDI both masks the scratch byte and produces the branch flags. */
    asm goto("andi %0, 0x1c\n\tbreq %l[finished]" : : "r" (hi) : "cc" : finished);
    lo = pm_read_absolute(0x2157);
    lo |= 0x80;
    asm volatile("" : "+r" (lo));
    PM_RAM8(0x2157) = lo;
    lo = 0x40;
    asm volatile("" : "+r" (lo));
    PORTA_OUTCLR = lo;
finished:
    asm volatile("pop r18\n\tpop r17\n\tpop r16\n\tout 0x3f, r31\n\tpop r31\n\treti" : : : "memory");
    __builtin_unreachable();
}
