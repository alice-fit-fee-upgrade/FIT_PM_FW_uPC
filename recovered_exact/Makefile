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
C_OBJECTS = $(R16_LEAF_OBJECTS) $(R16_ONLY_OBJECTS) $(R16_R17_OBJECTS) build/console_crlf.o build/adt8.o build/adt16.o build/flash_address_send.o build/pll_control_write.o build/fpga_word_write.o build/fpga_word_read.o build/pll_control_read.o build/adt_bit_transfer.o build/dma_gpio_handshake.o build/console_hex_word.o build/dac_channels.o build/fpga_multiword_read.o build/fpga_timestamp_send.o build/pll_defaults_load.o build/power_portd_interrupt.o build/console_flash_message.o build/fpga_settings_reset.o build/fpga_settings_init.o
$(R16_LEAF_OBJECTS): build/%.o: src/%.c Makefile | build
	$(CC) $(CFLAGS) $(R16_LEAF) -c $< -o $@
$(R16_ONLY_OBJECTS): build/%.o: src/%.c src/legacy_r16.h src/legacy_spi.h src/legacy_interrupt.h Makefile | build
	$(CC) $(CFLAGS) $(R16_ONLY) -c $< -o $@
$(R16_R17_OBJECTS): build/%.o: src/%.c src/legacy_r16.h src/legacy_spi.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r17 $(R16_ONLY) -c $< -o $@
build/console_crlf.o: src/console_crlf.c Makefile | build
	$(CC) $(CFLAGS) -c $< -o $@
build/adt8.o: src/adt8.c Makefile | build
	$(CC) $(CFLAGS) -fcall-saved-r18 -fcall-saved-r19 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/adt16.o: src/adt16.c Makefile | build
	$(CC) $(CFLAGS) -fcall-saved-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/flash_address_send.o: src/flash_address_send.c src/legacy_spi.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 18 19 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/pll_control_write.o: src/pll_control_write.c src/legacy_spi.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r22 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/fpga_word_write.o build/fpga_word_read.o build/fpga_multiword_read.o: build/%.o: src/%.c src/legacy_spi.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r18 -fcall-saved-r21 -fcall-saved-r22 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 19 20 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/pll_control_read.o: src/pll_control_read.c src/legacy_spi.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r22 -fcall-saved-r23 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 17 18 19 20 21 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/adt_bit_transfer.o build/dma_gpio_handshake.o: build/%.o: src/%.c src/legacy_r16.h src/legacy_spi.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-used-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/console_hex_word.o: src/console_hex_word.c Makefile | build
	$(CC) $(CFLAGS) -fcall-saved-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/dac_channels.o: src/dac_channels.c Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 20 21 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
build/fpga_timestamp_send.o: src/fpga_timestamp_send.c src/legacy_spi.h Makefile | build
	$(CC) $(CFLAGS) -fcall-used-r16 -fcall-used-r17 -fcall-saved-r18 -fcall-saved-r21 -fcall-saved-r22 -fcall-saved-r30 -fcall-saved-r31 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 19 20 23 24 25 26 27 28 29,-ffixed-r$(n)) -c $< -o $@
build/pll_defaults_load.o: src/pll_defaults_load.c Makefile | build
	$(CC) $(CFLAGS) -fcall-saved-r18 -fcall-saved-r19 -fcall-saved-r20 -fcall-saved-r30 -fcall-saved-r31 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 21 22 23 24 25 26 27 28 29,-ffixed-r$(n)) -c $< -o $@
build/power_portd_interrupt.o: src/power_portd_interrupt.c src/legacy_interrupt.h src/legacy_r16.h Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks $(R16_ONLY) -c $< -o $@
build/console_flash_message.o: src/console_flash_message.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks $(R16_ONLY) -fcall-saved-r16 -c $< -o $@
build/fpga_settings_reset.o: src/fpga_settings_reset.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-saved-r18 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 20 21 22 23 24 25 26 27 30 31,-ffixed-r$(n)) -c $< -o $@
build/fpga_settings_init.o: src/fpga_settings_init.c Makefile | build
	$(CC) $(CFLAGS) -fno-reorder-blocks -fcall-used-r16 -fcall-used-r17 $(foreach n,2 3 4 5 6 7 8 9 10 11 12 13 14 15 19 20 21 22 23 24 25 26 27 28 29 30 31,-ffixed-r$(n)) -c $< -o $@
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
clean:
	rm -rf build
	rm -f generated/stamp
.PHONY: all clean exact-check
