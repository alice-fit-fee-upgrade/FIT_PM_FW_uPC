#include "legacy_cpu.h"
#include <avr/io.h>
#include "legacy_r16.h"
#define RAM8(address) (*(volatile uint8_t *)(address))

void pm_dma_gpio_handshake(void) asm("FUN_code_000236");
void pm_dma_gpio_handshake(void)
{
    register uint8_t state asm("r16") = 0x80;
    register uint8_t enable asm("r17") = 0x10;
    /* The original briefly enables interrupts on every first-loop iteration. */
    asm volatile("" : "+r" (state), "+r" (enable) : : "memory");
    register uint8_t ready_pins asm("r18");
    do {
        pm_cpu_enable_irq();
        pm_cpu_disable_irq();
        ready_pins = PORTD_IN;
        asm volatile("" : "+r" (ready_pins));
    } while (!(ready_pins & 0x10u));
    RAM8(0x243c) = state;
    DMA_CH0_CTRLB = enable;
    pm_cpu_enable_irq();
wait_dma:
    state = DMA_CH0_CTRLB;
    asm volatile("" : "+r" (state));
    if (!(state & 0x10u)) goto wait_dma;
    RAM8(0x243c) = pm_scratch_zero();
wait_gpio:
    {
        register uint8_t pins asm("r18") = PORTD_IN;
        asm volatile("" : "+r" (pins));
        if (!(pins & 0x10u)) goto wait_gpio;
    }
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
uint32_t pm_logical_dma_gpio_handshake(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x046c: { // ldi r16, 0x80
        s->r[16] = 128;
        return 1134;
    }
    case 0x046e: { // ldi r17, 0x10
        s->r[17] = 16;
        return 1136;
    }
    case 0x0470: { // sei
        pm_irq(s, true);
        return 1138;
    }
    case 0x0472: { // cli
        pm_irq(s, false);
        return 1140;
    }
    case 0x0474: { // lds r18, 0x0668
        uint16_t address = 1640;
        s->r[18] = pm_read(s, address);
        return 1144;
    }
    case 0x0478: { // sbrs r18, 4
        return (!!(s->r[18] & (1u << 4)) == 1) ? 1148 : 1146;
    }
    case 0x047a: { // rjmp .-12
        return 1136;
    }
    case 0x047c: { // sts 0x243C, r16
        uint16_t address = 9276;
        pm_write(s, address, s->r[16]);
        return 1152;
    }
    case 0x0480: { // sts 0x0111, r17
        uint16_t address = 273;
        pm_write(s, address, s->r[17]);
        return 1156;
    }
    case 0x0484: { // sei
        pm_irq(s, true);
        return 1158;
    }
    case 0x0486: { // lds r16, 0x0111
        uint16_t address = 273;
        s->r[16] = pm_read(s, address);
        return 1162;
    }
    case 0x048a: { // sbrs r16, 4
        return (!!(s->r[16] & (1u << 4)) == 1) ? 1166 : 1164;
    }
    case 0x048c: { // rjmp .-8
        return 1158;
    }
    case 0x048e: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1168;
    }
    case 0x0490: { // sts 0x243C, r16
        uint16_t address = 9276;
        pm_write(s, address, s->r[16]);
        return 1172;
    }
    case 0x0494: { // lds r18, 0x0668
        uint16_t address = 1640;
        s->r[18] = pm_read(s, address);
        return 1176;
    }
    case 0x0498: { // sbrs r18, 4
        return (!!(s->r[18] & (1u << 4)) == 1) ? 1180 : 1178;
    }
    case 0x049a: { // rjmp .-8
        return 1172;
    }
    case 0x049c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
