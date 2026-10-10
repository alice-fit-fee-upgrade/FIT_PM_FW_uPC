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
