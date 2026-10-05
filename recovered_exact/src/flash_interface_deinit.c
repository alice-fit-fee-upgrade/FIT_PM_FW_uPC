#include "legacy_cpu.h"
#include <avr/io.h>
#include "legacy_r16.h"
#define RAM(a) (*(volatile uint8_t *)(a))
void pm_flash_interface_deinit(void) asm("FUN_code_000b3f");
void pm_flash_interface_deinit(void)
{
    pm_cpu_disable_irq();
    uint8_t zero=pm_scratch_zero();
    PORTD_INTCTRL=zero; SPIE_CTRL=zero;
    PORTE_DIRCLR=0xb0;
    GPIOR0 &= (uint8_t)~(1u << 1);
    PORTA_OUTSET=1; PORTD_OUTSET=2;
    RAM(0x215c)=0x88; RAM(0x215d)=0x13;
    register uint8_t state asm("r16")=4;
    asm volatile ("rcall FUN_code_00054e" : "+r" (state) : : "memory", "cc");
    pm_cpu_enable_irq();
}

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 304 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_flash_interface_deinit(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x167e: { // cli
        pm_irq(s, false);
        return 5760;
    }
    case 0x1680: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 5762;
    }
    case 0x1682: { // sts 0x0669, r16
        uint16_t address = 1641;
        pm_write(s, address, s->r[16]);
        return 5766;
    }
    case 0x1686: { // sts 0x0AC0, r16
        uint16_t address = 2752;
        pm_write(s, address, s->r[16]);
        return 5770;
    }
    case 0x168a: { // ldi r16, 0xB0
        s->r[16] = 176;
        return 5772;
    }
    case 0x168c: { // sts 0x0682, r16
        uint16_t address = 1666;
        pm_write(s, address, s->r[16]);
        return 5776;
    }
    case 0x1690: { // cbi 0x00, 1
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 1));
        return 5778;
    }
    case 0x1692: { // ldi r16, 0x01
        s->r[16] = 1;
        return 5780;
    }
    case 0x1694: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 5784;
    }
    case 0x1698: { // ldi r16, 0x02
        s->r[16] = 2;
        return 5786;
    }
    case 0x169a: { // sts 0x0665, r16
        uint16_t address = 1637;
        pm_write(s, address, s->r[16]);
        return 5790;
    }
    case 0x169e: { // ldi r16, 0x88
        s->r[16] = 136;
        return 5792;
    }
    case 0x16a0: { // sts 0x215C, r16
        uint16_t address = 8540;
        pm_write(s, address, s->r[16]);
        return 5796;
    }
    case 0x16a4: { // ldi r16, 0x13
        s->r[16] = 19;
        return 5798;
    }
    case 0x16a6: { // sts 0x215D, r16
        uint16_t address = 8541;
        pm_write(s, address, s->r[16]);
        return 5802;
    }
    case 0x16aa: { // ldi r16, 0x04
        s->r[16] = 4;
        return 5804;
    }
    case 0x16ac: { // rcall .-3090
        s->calls[s->call_depth++] = 5806;
        return 2716;
    }
    case 0x16ae: { // sei
        pm_irq(s, true);
        return 5808;
    }
    case 0x16b0: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
