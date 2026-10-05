# Logical C equivalents embedded in the exact source

Every accepted source file in `recovered_exact/src` contains a compiled logical C
comment: **87 translation units and 9 private-ABI headers**. This includes the five
files without emitting ASM helpers. Immutable `exact_asm`, golden images and
historical analysis are unchanged; these comments belong to the development tree.

The comments supplement the existing readable, historical C alternatives. They
make otherwise implicit register, carry, transfer-bit, memory and control-flow
state explicit in `PMLogical`. They are C state models of the original routines,
not ordinary GNU-ABI implementations. They retain the original branches and
callee addresses, including unusual original behavior. Call transitions are
returned to a dispatcher; they do not invoke host versions of hardware routines.
Header models cover instantiated primitive instructions rather than inventing a
single arbitrary macro argument configuration.

`make c-comment-check` extracts and compiles the **actual comment bodies** with
host C99, then compares them with execution of original golden FLASH instructions
in the retained AVR oracle. It fails when a comment no longer matches its expected
model hash. The negative mutation test also demonstrated this rejection; its log
is in `logical_c_test_logs/comment_tamper_detection.log`. Regenerate models
explicitly with `python3 recovered_exact/tests/generate_logical_comments.py`.

The completed run passed **384,240 transition cases**, comprising sixteen boundary
and deterministic random states at every model instruction site, plus exhaustive
256 byte/SREG cases per each of 1,084 opcode/operand shapes. The test compares:

- all 32 registers and all SREG bits;
- next instruction address, including skip over a four-byte instruction;
- abstract byte-stack and call-continuation state;
- ordered RAM/MMIO reads, writes and explicit IRQ-enable events.

The report `logical_c_validation.json` contains the per-file models, hashes,
addresses, case counts and runtime-helper hash. Each comment is labelled
`PASS_INSTRUCTION_TRANSITIONS` with its individual case count. A PASS is tied to
the matching model, runtime and golden SHA256; modifying them requires rerunning
the test. The historical interpreter omits I restoration on RETI; this suite
explicitly checks architectural I=1 without modifying the archived interpreter.

This is **instruction-transition coverage**, not exhaustive whole-function path
coverage or all combinations of register inputs. It uses an abstract call stack;
architectural three-byte return-stack tests remain in the historical integration
suites. It does not establish cycle timing, asynchronous IRQ behavior or physical
peripheral operation. These limitations are included beside every model.

The existing historical, higher-level C alternatives receive separate complete
suite reruns. `source_recovery` passed **390,601 functional cases**. **All 23 retained mixed C/ASM integration suites passed.** Results and
logs are in `logical_c_historical_tests.json`
and `logical_c_test_logs/`; `asm_c_alternatives.json` links per-function evidence.
Run all auxiliary suites with `make c-alternative-check`. These alternatives can
be functionally valid while producing different machine code, so they remain
comments rather than accepted baseline replacements.

Earlier speculative, rejected snippets do not acquire an unqualified whole-
function PASS merely because a corrected state model passed. In particular,
trial 492 used a value overwritten by intervening SPI calls and was withdrawn;
the new model consumes the original saved SREG.T. Rejected SUBI/CPI and EOR/CP
variants likewise do not preserve all private register/flag effects. Their
corrected, flag-aware C equivalents are covered by the new model tests. Earlier
"unvalidated" remarks refer to the historical candidate, not the new compiled
model and its explicitly scoped evidence.

The authoritative firmware acceptance remains `make exact-check`. Two clean
final ELF/HEX/BIN builds were identical, with canonical FLASH SHA256:

`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`

**Differing bytes: 0.** Comments emit no executable bytes and contribute zero to
C coverage. The executable baseline remains 6,680 C bytes / 10,836 total
(61.6464% overall; 66.4148% of application executable bytes).

## Per-file compiled comment validation

These counts cover the sixteen boundary/random states at each site in that file.
The shared operand-shape sweep accounts for the remaining cases in the total.

| Source | Transition cases | Result |
|---|---:|---|
| [src/adt16.c](../recovered_exact/src/adt16.c) | 320 | PASS_INSTRUCTION_TRANSITIONS |
| [src/adt8.c](../recovered_exact/src/adt8.c) | 272 | PASS_INSTRUCTION_TRANSITIONS |
| [src/adt_bit_transfer.c](../recovered_exact/src/adt_bit_transfer.c) | 272 | PASS_INSTRUCTION_TRANSITIONS |
| [src/adt_fault_clear.c](../recovered_exact/src/adt_fault_clear.c) | 208 | PASS_INSTRUCTION_TRANSITIONS |
| [src/alarm_state_clear.c](../recovered_exact/src/alarm_state_clear.c) | 784 | PASS_INSTRUCTION_TRANSITIONS |
| [src/application_main.c](../recovered_exact/src/application_main.c) | 2976 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_adc_delay_command.c](../recovered_exact/src/channel_adc_delay_command.c) | 672 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_adc_range_command.c](../recovered_exact/src/channel_adc_range_command.c) | 960 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_adc_zero_command.c](../recovered_exact/src/channel_adc_zero_command.c) | 784 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_baseline_dispersion_display.c](../recovered_exact/src/channel_baseline_dispersion_display.c) | 656 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_cdf_adc_display.c](../recovered_exact/src/channel_cdf_adc_display.c) | 848 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_cfd_threshold_command.c](../recovered_exact/src/channel_cfd_threshold_command.c) | 752 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_cfd_zero_command.c](../recovered_exact/src/channel_cfd_zero_command.c) | 704 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_mean_amplitude_display.c](../recovered_exact/src/channel_mean_amplitude_display.c) | 368 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_settings_display.c](../recovered_exact/src/channel_settings_display.c) | 1152 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_tdc_adjust_command.c](../recovered_exact/src/channel_tdc_adjust_command.c) | 720 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_tdc_values_command.c](../recovered_exact/src/channel_tdc_values_command.c) | 624 | PASS_INSTRUCTION_TRANSITIONS |
| [src/channel_threshold_calibration_command.c](../recovered_exact/src/channel_threshold_calibration_command.c) | 800 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_character_read.c](../recovered_exact/src/console_character_read.c) | 816 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_character_send.c](../recovered_exact/src/console_character_send.c) | 784 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_crlf.c](../recovered_exact/src/console_crlf.c) | 64 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_cts_interrupt.c](../recovered_exact/src/console_cts_interrupt.c) | 752 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_decimal_formatter.c](../recovered_exact/src/console_decimal_formatter.c) | 1664 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_dispatch.c](../recovered_exact/src/console_dispatch.c) | 4304 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_fixed2_entry.c](../recovered_exact/src/console_fixed2_entry.c) | 64 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_fixed3_entry.c](../recovered_exact/src/console_fixed3_entry.c) | 64 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_flash_message.c](../recovered_exact/src/console_flash_message.c) | 128 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_hex_digit.c](../recovered_exact/src/console_hex_digit.c) | 112 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_hex_parser.c](../recovered_exact/src/console_hex_parser.c) | 608 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_hex_word.c](../recovered_exact/src/console_hex_word.c) | 320 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_integer_parser.c](../recovered_exact/src/console_integer_parser.c) | 960 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_raw_byte_entry.c](../recovered_exact/src/console_raw_byte_entry.c) | 48 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_rx_interrupt.c](../recovered_exact/src/console_rx_interrupt.c) | 816 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_temperature_entry.c](../recovered_exact/src/console_temperature_entry.c) | 64 | PASS_INSTRUCTION_TRANSITIONS |
| [src/console_tx_interrupt.c](../recovered_exact/src/console_tx_interrupt.c) | 656 | PASS_INSTRUCTION_TRANSITIONS |
| [src/dac_channels.c](../recovered_exact/src/dac_channels.c) | 384 | PASS_INSTRUCTION_TRANSITIONS |
| [src/dac_gain_scale.c](../recovered_exact/src/dac_gain_scale.c) | 224 | PASS_INSTRUCTION_TRANSITIONS |
| [src/dac_offset_scale.c](../recovered_exact/src/dac_offset_scale.c) | 336 | PASS_INSTRUCTION_TRANSITIONS |
| [src/dac_saturating_scaled_product.c](../recovered_exact/src/dac_saturating_scaled_product.c) | 320 | PASS_INSTRUCTION_TRANSITIONS |
| [src/dac_signed_scaled_product.c](../recovered_exact/src/dac_signed_scaled_product.c) | 224 | PASS_INSTRUCTION_TRANSITIONS |
| [src/dac_spi_send.c](../recovered_exact/src/dac_spi_send.c) | 656 | PASS_INSTRUCTION_TRANSITIONS |
| [src/dac_stored_settings_apply.c](../recovered_exact/src/dac_stored_settings_apply.c) | 816 | PASS_INSTRUCTION_TRANSITIONS |
| [src/dma_channel_interrupt.c](../recovered_exact/src/dma_channel_interrupt.c) | 4240 | PASS_INSTRUCTION_TRANSITIONS |
| [src/dma_gpio_handshake.c](../recovered_exact/src/dma_gpio_handshake.c) | 304 | PASS_INSTRUCTION_TRANSITIONS |
| [src/eeprom_page_commit.c](../recovered_exact/src/eeprom_page_commit.c) | 288 | PASS_INSTRUCTION_TRANSITIONS |
| [src/eeprom_settings_store.c](../recovered_exact/src/eeprom_settings_store.c) | 688 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_address_send.c](../recovered_exact/src/flash_address_send.c) | 304 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_crc_bit_core.c](../recovered_exact/src/flash_crc_bit_core.c) | 288 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_crc_stream_read.c](../recovered_exact/src/flash_crc_stream_read.c) | 704 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_device_id.c](../recovered_exact/src/flash_device_id.c) | 416 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_interface_deinit.c](../recovered_exact/src/flash_interface_deinit.c) | 304 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_interface_init.c](../recovered_exact/src/flash_interface_init.c) | 144 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_page_program.c](../recovered_exact/src/flash_page_program.c) | 592 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_sector_erase.c](../recovered_exact/src/flash_sector_erase.c) | 96 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_wait_ready.c](../recovered_exact/src/flash_wait_ready.c) | 336 | PASS_INSTRUCTION_TRANSITIONS |
| [src/flash_write_enable.c](../recovered_exact/src/flash_write_enable.c) | 160 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_binary_programming_stream.c](../recovered_exact/src/fpga_binary_programming_stream.c) | 1216 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_data_exchange.c](../recovered_exact/src/fpga_data_exchange.c) | 2944 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_multiword_read.c](../recovered_exact/src/fpga_multiword_read.c) | 1008 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_ready_guard.c](../recovered_exact/src/fpga_ready_guard.c) | 144 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_settings_init.c](../recovered_exact/src/fpga_settings_init.c) | 1088 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_settings_reset.c](../recovered_exact/src/fpga_settings_reset.c) | 608 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_status_interrupt.c](../recovered_exact/src/fpga_status_interrupt.c) | 544 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_timestamp_send.c](../recovered_exact/src/fpga_timestamp_send.c) | 656 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_word_read.c](../recovered_exact/src/fpga_word_read.c) | 576 | PASS_INSTRUCTION_TRANSITIONS |
| [src/fpga_word_write.c](../recovered_exact/src/fpga_word_write.c) | 544 | PASS_INSTRUCTION_TRANSITIONS |
| [src/legacy_cli.h](../recovered_exact/src/legacy_cli.h) | 7104 | PASS_INSTRUCTION_TRANSITIONS |
| [src/legacy_cli_guard_c.h](../recovered_exact/src/legacy_cli_guard_c.h) | 3808 | PASS_INSTRUCTION_TRANSITIONS |
| [src/legacy_cpu.h](../recovered_exact/src/legacy_cpu.h) | 208 | PASS_INSTRUCTION_TRANSITIONS |
| [src/legacy_interrupt.h](../recovered_exact/src/legacy_interrupt.h) | 800 | PASS_INSTRUCTION_TRANSITIONS |
| [src/legacy_interrupt_register_c.h](../recovered_exact/src/legacy_interrupt_register_c.h) | 800 | PASS_INSTRUCTION_TRANSITIONS |
| [src/legacy_r16.h](../recovered_exact/src/legacy_r16.h) | 5648 | PASS_INSTRUCTION_TRANSITIONS |
| [src/legacy_r16_c.h](../recovered_exact/src/legacy_r16_c.h) | 5648 | PASS_INSTRUCTION_TRANSITIONS |
| [src/legacy_spi.h](../recovered_exact/src/legacy_spi.h) | 6992 | PASS_INSTRUCTION_TRANSITIONS |
| [src/legacy_spi_c.h](../recovered_exact/src/legacy_spi_c.h) | 6992 | PASS_INSTRUCTION_TRANSITIONS |
| [src/pll_console_reset.c](../recovered_exact/src/pll_console_reset.c) | 304 | PASS_INSTRUCTION_TRANSITIONS |
| [src/pll_control_read.c](../recovered_exact/src/pll_control_read.c) | 560 | PASS_INSTRUCTION_TRANSITIONS |
| [src/pll_control_write.c](../recovered_exact/src/pll_control_write.c) | 400 | PASS_INSTRUCTION_TRANSITIONS |
| [src/pll_defaults_load.c](../recovered_exact/src/pll_defaults_load.c) | 496 | PASS_INSTRUCTION_TRANSITIONS |
| [src/pll_status_interrupt.c](../recovered_exact/src/pll_status_interrupt.c) | 912 | PASS_INSTRUCTION_TRANSITIONS |
| [src/power_fault_state.c](../recovered_exact/src/power_fault_state.c) | 240 | PASS_INSTRUCTION_TRANSITIONS |
| [src/power_portb_interrupt.c](../recovered_exact/src/power_portb_interrupt.c) | 224 | PASS_INSTRUCTION_TRANSITIONS |
| [src/power_portd_interrupt.c](../recovered_exact/src/power_portd_interrupt.c) | 496 | PASS_INSTRUCTION_TRANSITIONS |
| [src/power_porte_interrupt.c](../recovered_exact/src/power_porte_interrupt.c) | 1008 | PASS_INSTRUCTION_TRANSITIONS |
| [src/power_state_request.c](../recovered_exact/src/power_state_request.c) | 192 | PASS_INSTRUCTION_TRANSITIONS |
| [src/programming_unlock.c](../recovered_exact/src/programming_unlock.c) | 304 | PASS_INSTRUCTION_TRANSITIONS |
| [src/status_led.c](../recovered_exact/src/status_led.c) | 160 | PASS_INSTRUCTION_TRANSITIONS |
| [src/system_deinit.c](../recovered_exact/src/system_deinit.c) | 608 | PASS_INSTRUCTION_TRANSITIONS |
| [src/system_init.c](../recovered_exact/src/system_init.c) | 1120 | PASS_INSTRUCTION_TRANSITIONS |
| [src/system_status_display.c](../recovered_exact/src/system_status_display.c) | 2912 | PASS_INSTRUCTION_TRANSITIONS |
| [src/tdc_channel_data_display.c](../recovered_exact/src/tdc_channel_data_display.c) | 1264 | PASS_INSTRUCTION_TRANSITIONS |
| [src/tdc_host_read.c](../recovered_exact/src/tdc_host_read.c) | 928 | PASS_INSTRUCTION_TRANSITIONS |
| [src/tdc_host_write.c](../recovered_exact/src/tdc_host_write.c) | 912 | PASS_INSTRUCTION_TRANSITIONS |
| [src/timer_overflow_interrupt.c](../recovered_exact/src/timer_overflow_interrupt.c) | 7824 | PASS_INSTRUCTION_TRANSITIONS |
| [src/trigger_charge_command.c](../recovered_exact/src/trigger_charge_command.c) | 352 | PASS_INSTRUCTION_TRANSITIONS |
| [src/trigger_settings_command.c](../recovered_exact/src/trigger_settings_command.c) | 272 | PASS_INSTRUCTION_TRANSITIONS |
