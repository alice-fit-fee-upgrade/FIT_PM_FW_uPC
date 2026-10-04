/* Real-device backend for later C integration. This adapter does not provide
 * startup, vectors or the original assembly register ABI. */
#include "pm_recovered.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
static uint8_t read_data(void *context, uint16_t address)
{
    (void)context;
    return *(volatile uint8_t *)(uintptr_t)address;
}
static void write_data(void *context, uint16_t address, uint8_t value)
{
    (void)context;
    *(volatile uint8_t *)(uintptr_t)address = value;
}
static uint8_t read_flash(void *context, uint16_t address)
{
    (void)context;
    return pgm_read_byte_near(address);
}
static void set_irq(void *context, bool enabled)
{
    (void)context;
    if (enabled) sei();
    else cli();
}
const pm_bus pm_avr_bus = {0, read_data, write_data, read_flash, set_irq};
static uint8_t get_console(void *context)
{
    return pm_console_next((const pm_bus *)context, false);
}
static void put_console(void *context, uint8_t byte)
{
    pm_console_send((const pm_bus *)context, byte);
}
const pm_stream pm_avr_console = {(void *)&pm_avr_bus, get_console, put_console};
