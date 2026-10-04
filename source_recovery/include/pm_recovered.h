#ifndef PM_RECOVERED_H
#define PM_RECOVERED_H
#include <stdint.h>
#include <stdbool.h>
/* Data-space addresses are bytes, never AVR word addresses. Callbacks permit
 * differential MMIO/stream testing without changing the recovered algorithm. */
typedef struct {
    void *context;
    uint8_t (*read8)(void *, uint16_t);
    void (*write8)(void *, uint16_t, uint8_t);
    uint8_t (*flash8)(void *, uint16_t);
    void (*irq)(void *, bool);
} pm_bus;
typedef struct {
    void *context;
    uint8_t (*get)(void *);
    void (*put)(void *, uint8_t);
} pm_stream;
typedef struct {
    uint16_t value;
    uint8_t terminator;
    bool error; /* Original return Carry, including partially accumulated errors. */
} pm_number;
uint8_t pm_hex_digit(uint8_t value);
pm_number pm_parse_integer(const pm_stream *stream);
pm_number pm_parse_hex(const pm_stream *stream);
void pm_send_hex_digit(const pm_stream *, uint8_t);
/* Historical name cli_send_32bit_hex actually sends FOUR digits of R17:R16. */
void pm_send_hex16(const pm_stream *, uint16_t);
void pm_send_crlf(const pm_stream *);
void pm_send_flash_string(const pm_bus *, const pm_stream *, uint16_t);
/* Original combinations: signed=true places=0..3; signed=false places=0. */
void pm_send_decimal(const pm_stream *, uint16_t, bool signed_value, uint8_t decimal_places);
uint16_t pm_scale_signed(uint16_t coefficient, uint16_t signed_input_bits);
uint16_t pm_scale_unsigned(uint16_t coefficient, uint16_t input);
/* Original AVR ABI adapter: result bits 0..15, full SREG bits 16..23. */
uint32_t pm_scale_unsigned_abi(uint16_t coefficient, uint16_t input, uint8_t entry_sreg);
uint8_t pm_console_next(const pm_bus *, bool raw);
void pm_console_send(const pm_bus *, uint8_t);
uint8_t pm_adt7311_byte(const pm_bus *, uint8_t);
void pm_adt7311_write8(const pm_bus *, uint8_t command, uint8_t data);
uint16_t pm_adt7311_exchange16(const pm_bus *, uint8_t command, uint16_t data);
void pm_adt7311_clear(const pm_bus *);
void pm_fpga_write16(const pm_bus *, uint8_t reg, uint16_t data);
/* Original read header's unused six low bits derive from incoming R21! */
uint16_t pm_fpga_read16(const pm_bus *, uint8_t reg, uint16_t dummy, uint8_t incoming_r21);
void pm_fpga_read_bc(const pm_bus *, uint8_t result[8]);
void pm_pll_write32(const pm_bus *, uint32_t);
uint32_t pm_pll_read32(const pm_bus *);
void pm_dac_send(const pm_bus *, uint8_t control, uint16_t data);
void pm_ths_write24(const pm_bus *, uint8_t control, uint8_t r16, uint8_t r17, uint8_t r18);
/* Invalid selector (>=3) returns original incoming value without bus access. */
uint16_t pm_ths_read16(const pm_bus *, uint8_t selector, uint8_t command, uint16_t incoming);
void pm_console_dre(const pm_bus *);
void pm_console_rxc(const pm_bus *);
void pm_fpga_settings_init(const pm_bus *);
void pm_fpga_settings_reset(const pm_bus *);
void pm_system_deinit(const pm_bus *);
#ifdef __AVR__
extern const pm_bus pm_avr_bus;
extern const pm_stream pm_avr_console;
#endif
#endif
