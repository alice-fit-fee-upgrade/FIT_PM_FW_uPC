CC = avr-gcc
PYTHON = python3
CFLAGS = -mmcu=atxmega128a3u -Os -std=gnu99 -Wall -Wextra -Werror -ffunction-sections -fdata-sections -fno-lto
# Original leaf ABI: R16 is the only high scratch register, with no GNU arguments.
R16_LEAF = -fcall-used-r16 $(foreach n,17 18 19 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n))
R16_ONLY = $(R16_LEAF) $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15,-ffixed-r$(n))
all: build/flash_rebuilt.bin
	cmp ../reference/flash_golden.bin build/flash_rebuilt.bin
build:
	mkdir -p $@
generated/stamp: manifest.json tools/prepare_sources.py ../tools/convert_analysis.py ../asm_analysis/main.S ../asm_analysis/ATxmega128A3Udef.inc
	$(PYTHON) tools/prepare_sources.py
	touch $@
build/application.o: generated/stamp ../asm_analysis/ATxmega128A3Udef.inc Makefile | build
	$(CC) -mmcu=atxmega128a3u -I../asm_analysis -c generated/application.S -o $@
build/boot.o: ../exact_asm/src/boot.S Makefile | build
	$(CC) -mmcu=atxmega128a3u -c $< -o $@
R16_LEAF_OBJECTS = build/flash_interface_init.o
R16_ONLY_OBJECTS = build/flash_write_enable.o build/system_deinit.o build/power_fault_state.o build/flash_interface_deinit.o build/status_led.o build/console_hex_digit.o build/power_state_request.o build/flash_device_id.o build/power_portb_interrupt.o
R16_R17_OBJECTS = build/flash_sector_erase.o build/system_init.o build/adt_fault_clear.o
C_OBJECTS = build/timer_overflow_interrupt.o build/dma_channel_interrupt.o build/console_dispatch.o build/fpga_data_exchange.o build/dac_signed_scaled_product.o $(R16_LEAF_OBJECTS) $(R16_ONLY_OBJECTS) $(R16_R17_OBJECTS) build/console_crlf.o build/adt8.o build/adt16.o build/flash_address_send.o build/pll_control_write.o build/fpga_word_write.o build/fpga_word_read.o build/pll_control_read.o build/adt_bit_transfer.o build/dma_gpio_handshake.o build/console_hex_word.o build/dac_channels.o build/fpga_multiword_read.o build/fpga_timestamp_send.o build/pll_defaults_load.o build/power_portd_interrupt.o build/console_flash_message.o build/fpga_settings_reset.o build/fpga_settings_init.o build/tdc_host_write.o build/tdc_host_read.o build/dac_spi_send.o build/flash_wait_ready.o build/fpga_status_interrupt.o build/console_tx_interrupt.o build/console_cts_interrupt.o build/console_rx_interrupt.o build/console_character_read.o build/console_character_send.o build/pll_status_interrupt.o build/power_porte_interrupt.o build/eeprom_page_commit.o build/eeprom_settings_store.o build/alarm_state_clear.o build/pll_console_reset.o build/dac_offset_scale.o build/dac_gain_scale.o build/trigger_charge_command.o build/trigger_settings_command.o build/channel_adc_delay_command.o build/channel_cfd_zero_command.o build/channel_adc_zero_command.o build/channel_cfd_threshold_command.o build/channel_tdc_values_command.o build/channel_threshold_calibration_command.o build/channel_adc_range_command.o build/channel_tdc_adjust_command.o build/channel_mean_amplitude_display.o build/channel_baseline_dispersion_display.o build/channel_cdf_adc_display.o build/fpga_ready_guard.o build/programming_unlock.o build/channel_settings_display.o build/console_hex_parser.o build/flash_page_program.o build/flash_crc_stream_read.o build/flash_crc_bit_core.o build/tdc_channel_data_display.o build/dac_stored_settings_apply.o build/application_main.o build/system_status_display.o build/fpga_binary_programming_stream.o build/console_fixed2_entry.o build/console_temperature_entry.o build/console_fixed3_entry.o build/console_raw_byte_entry.o build/dac_saturating_scaled_product.o build/console_integer_parser.o build/console_decimal_formatter.o
$(C_OBJECTS): $(wildcard src/legacy_*.h)

$(R16_LEAF_OBJECTS): build/%.o: src/%.c Makefile | build
	$(CC) $(CFLAGS) $(R16_LEAF) -c $< -o $@
$(R16_ONLY_OBJECTS): build/%.o: src/%.c src/legacy_r16.h src/legacy_r16_c.h src/legacy_spi.h src/legacy_spi_c.h src/legacy_interrupt.h Makefile | build
	$(CC) $(CFLAGS) $(R16_ONLY) -c $< -o $@
$(R16_R17_OBJECTS): build/%.o: src/%.c src/legacy_r16.h src/legacy_r16_c.h src/legacy_spi.h src/legacy_spi_c.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r17 $(R16_ONLY) -c $< -o $@
build/console_crlf.o: src/console_crlf.c Makefile | build
	$(CC) $(CFLAGS) -c $< -o $@
build/adt8.o: src/adt8.c Makefile | build
	$(CC) $(CFLAGS) -fcall-saved-r18 -fcall-saved-r19 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/adt16.o: src/adt16.c Makefile | build
	$(CC) $(CFLAGS) -fcall-saved-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/flash_address_send.o: src/flash_address_send.c src/legacy_spi.h src/legacy_spi_c.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 18 19 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/pll_control_write.o: src/pll_control_write.c src/legacy_spi.h src/legacy_spi_c.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r22 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/fpga_word_write.o build/fpga_word_read.o build/fpga_multiword_read.o: build/%.o: src/%.c src/legacy_spi.h src/legacy_spi_c.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r18 -fcall-saved-r21 -fcall-saved-r22 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 19 20 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/pll_control_read.o: src/pll_control_read.c src/legacy_spi.h src/legacy_spi_c.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r22 -fcall-saved-r23 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 17 18 19 20 21 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/adt_bit_transfer.o build/dma_gpio_handshake.o: build/%.o: src/%.c src/legacy_r16.h src/legacy_r16_c.h src/legacy_spi.h src/legacy_spi_c.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-used-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/console_hex_word.o: src/console_hex_word.c Makefile | build
	$(CC) $(CFLAGS) -fcall-saved-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/dac_channels.o: src/dac_channels.c Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 20 21 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/fpga_timestamp_send.o: src/fpga_timestamp_send.c src/legacy_spi.h src/legacy_spi_c.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r18 -fcall-saved-r21 -fcall-saved-r22 -fcall-saved-r30 -fcall-saved-r31 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 19 20 23 24 25 26 27 28 29,-ffixed-r$(n)) -c $< -o $@
build/pll_defaults_load.o: src/pll_defaults_load.c Makefile | build
	$(CC) $(CFLAGS) -fcall-saved-r18 -fcall-saved-r19 -fcall-saved-r20 -fcall-saved-r30 -fcall-saved-r31 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 21 22 23 24 25 26 27 28 29,-ffixed-r$(n)) -c $< -o $@
build/power_portd_interrupt.o: src/power_portd_interrupt.c src/legacy_interrupt.h src/legacy_r16.h src/legacy_r16_c.h Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks $(R16_ONLY) -c $< -o $@
build/console_flash_message.o: src/console_flash_message.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks $(R16_ONLY) -fcall-saved-r16 -c $< -o $@
build/fpga_settings_reset.o: src/fpga_settings_reset.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-saved-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 20 21 22 23 24 25 26 27 30 31,-ffixed-r$(n)) -c $< -o $@
build/fpga_settings_init.o: src/fpga_settings_init.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/tdc_host_write.o: src/tdc_host_write.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-saved-r18 -fcall-saved-r19 -fcall-saved-r20 -fcall-saved-r21 -fcall-saved-r22 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/tdc_host_read.o: src/tdc_host_read.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-saved-r19 -fcall-saved-r20 -fcall-saved-r21 -fcall-saved-r22 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/dac_spi_send.o: src/dac_spi_send.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-saved-r22 -fcall-saved-r23 -fcall-saved-r24 -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/flash_wait_ready.o: src/flash_wait_ready.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/fpga_status_interrupt.o: src/fpga_status_interrupt.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_tx_interrupt.o: src/console_tx_interrupt.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_cts_interrupt.o: src/console_cts_interrupt.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_rx_interrupt.o: src/console_rx_interrupt.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_character_read.o: src/console_character_read.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_character_send.o: src/console_character_send.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/pll_status_interrupt.o: src/pll_status_interrupt.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/power_porte_interrupt.o: src/power_porte_interrupt.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/eeprom_page_commit.o: src/eeprom_page_commit.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/eeprom_settings_store.o: src/eeprom_settings_store.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/alarm_state_clear.o: src/alarm_state_clear.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/pll_console_reset.o: src/pll_console_reset.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/dac_offset_scale.o: src/dac_offset_scale.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-saved-r18 -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/dac_gain_scale.o: src/dac_gain_scale.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-saved-r18 -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/trigger_charge_command.o: src/trigger_charge_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/trigger_settings_command.o: src/trigger_settings_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_adc_delay_command.o: src/channel_adc_delay_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_cfd_zero_command.o: src/channel_cfd_zero_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_adc_zero_command.o: src/channel_adc_zero_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_cfd_threshold_command.o: src/channel_cfd_threshold_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_tdc_values_command.o: src/channel_tdc_values_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_threshold_calibration_command.o: src/channel_threshold_calibration_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_adc_range_command.o: src/channel_adc_range_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_tdc_adjust_command.o: src/channel_tdc_adjust_command.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r23 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_mean_amplitude_display.o: src/channel_mean_amplitude_display.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_baseline_dispersion_display.o: src/channel_baseline_dispersion_display.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_cdf_adc_display.o: src/channel_cdf_adc_display.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -fcall-used-r20 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r18 -ffixed-r19 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/fpga_ready_guard.o: src/fpga_ready_guard.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/programming_unlock.o: src/programming_unlock.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/channel_settings_display.o: src/channel_settings_display.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -fcall-used-r20 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r18 -ffixed-r21 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_hex_parser.o: src/console_hex_parser.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -ffixed-r18 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r17 -ffixed-r19 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/flash_page_program.o: src/flash_page_program.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r16 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/flash_crc_stream_read.o: src/flash_crc_stream_read.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/flash_crc_bit_core.o: src/flash_crc_bit_core.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r20 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r21 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/tdc_channel_data_display.o: src/tdc_channel_data_display.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/dac_stored_settings_apply.o: src/dac_stored_settings_apply.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/application_main.o: src/application_main.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/system_status_display.o: src/system_status_display.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/fpga_binary_programming_stream.o: src/fpga_binary_programming_stream.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_fixed2_entry.o: src/console_fixed2_entry.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_temperature_entry.o: src/console_temperature_entry.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_fixed3_entry.o: src/console_fixed3_entry.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_raw_byte_entry.o: src/console_raw_byte_entry.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 -ffixed-r18 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/dac_saturating_scaled_product.o: src/dac_saturating_scaled_product.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_integer_parser.o: src/console_integer_parser.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/console_decimal_formatter.o: src/console_decimal_formatter.c $(wildcard src/legacy_*.h) Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@
build/reconstructed.elf: build/application.o build/boot.o $(C_OBJECTS) generated/stamp linker/exact.ld
	$(CC) -mmcu=atxmega128a3u -nostdlib -Wl,-T,linker/exact.ld,-Map,build/reconstructed.map build/application.o build/boot.o $(C_OBJECTS) -o $@
build/reconstructed.hex: build/reconstructed.elf
	avr-objcopy -O ihex $< $@
build/flash_rebuilt.bin: build/reconstructed.hex
	$(PYTHON) ../tools/memory_image.py $< $@
exact-check:
	cd ../reference && sha256sum -c SHA256SUMS
	$(MAKE) clean
	$(MAKE) all
	$(PYTHON) ../tools/compare_flash.py ../reference/flash_golden.bin build/flash_rebuilt.bin --report build/comparison.txt --report-only
	cmp ../reference/flash_golden.bin build/flash_rebuilt.bin
	avr-objdump -D build/reconstructed.elf > build/rebuilt.dis
	$(PYTHON) ../tools/instruction_diff.py ../reference/flash_golden.bin build/flash_rebuilt.bin > build/instruction_diff.txt
	$(PYTHON) tools/inventory.py
	$(PYTHON) tools/audit_c_provenance.py
clean:
	rm -rf build
	rm -f generated/stamp
.PHONY: all clean exact-check

build/dac_signed_scaled_product.o: src/dac_signed_scaled_product.c Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -ffixed-r30 -ffixed-r31 -c $< -o $@

build/fpga_data_exchange.o: src/fpga_data_exchange.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r16 -fcall-used-r17 -fcall-used-r18 -fcall-used-r19 -fcall-used-r20 -fcall-used-r21 -fcall-used-r22 -fcall-used-r23 -fcall-used-r24 -fcall-used-r30 -fcall-used-r31 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -c $< -o $@

build/console_dispatch.o: src/console_dispatch.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r3 -fcall-used-r4 -fcall-used-r5 -fcall-used-r16 -fcall-used-r17 -fcall-used-r18 -fcall-used-r20 -fcall-used-r21 -fcall-used-r22 -fcall-used-r30 -fcall-used-r31 -ffixed-r2 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r19 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -c $< -o $@

build/dma_channel_interrupt.o: src/dma_channel_interrupt.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r2 -fcall-used-r16 -fcall-used-r17 -fcall-used-r18 -fcall-used-r19 -fcall-used-r20 -fcall-used-r22 -fcall-used-r26 -fcall-used-r27 -fcall-used-r30 -fcall-used-r31 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r21 -ffixed-r23 -ffixed-r24 -ffixed-r25 -ffixed-r28 -ffixed-r29 -c $< -o $@

build/timer_overflow_interrupt.o: src/timer_overflow_interrupt.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fno-crossjumping -fcall-used-r16 -fcall-used-r17 -fcall-used-r18 -fcall-used-r19 -fcall-used-r20 -fcall-used-r21 -fcall-used-r22 -fcall-used-r23 -fcall-used-r24 -fcall-used-r25 -fcall-used-r30 -fcall-used-r31 -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6 -ffixed-r7 -ffixed-r8 -ffixed-r9 -ffixed-r10 -ffixed-r11 -ffixed-r12 -ffixed-r13 -ffixed-r14 -ffixed-r15 -ffixed-r26 -ffixed-r27 -ffixed-r28 -ffixed-r29 -c $< -o $@
