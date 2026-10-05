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
    asm volatile("1: sei\n\tcli\n\tlds r18, %2\n\tsbrs r18, 4\n\trjmp 1b"
        : "+r" (state), "+r" (enable) : "n" (_SFR_MEM_ADDR(PORTD_IN))
        : "r18", "memory");
    RAM8(0x243c) = state;
    DMA_CH0_CTRLB = enable;
    pm_cpu_enable_irq();
wait_dma:
    state = DMA_CH0_CTRLB;
    asm goto("sbrs %0, 4\n\trjmp %l[wait_dma]" : : "r" (state) : : wait_dma);
    RAM8(0x243c) = pm_scratch_zero();
wait_gpio:
    {
        register uint8_t pins asm("r18") = PORTD_IN;
        asm goto("sbrs %0, 4\n\trjmp %l[wait_gpio]" : : "r" (pins) : : wait_gpio);
    }
}
