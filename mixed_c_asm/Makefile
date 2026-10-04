CC = avr-gcc
PYTHON = python3
CFLAGS = -mmcu=atxmega128a3u -Os -std=c99 -Wall -Wextra -Werror -ffunction-sections -fdata-sections -fstack-usage
all: build/flash_mixed.bin
build:
	mkdir -p $@
generated/application.S: ../asm_analysis/main.S ../tools/convert_analysis.py tools/patch_application.py
	$(PYTHON) tools/patch_application.py
build/application.o: generated/application.S ../asm_analysis/ATxmega128A3Udef.inc | build
	$(CC) $(CFLAGS) -I../asm_analysis -c $< -o $@
build/boot.o: ../exact_asm/src/boot.S | build
	$(CC) $(CFLAGS) -c $< -o $@
build/bridge.o: src/signed_scale_bridge.S | build
	$(CC) $(CFLAGS) -c $< -o $@
build/scaling.o: ../source_recovery/src/scaling.c ../source_recovery/include/pm_recovered.h | build
	$(CC) $(CFLAGS) -I../source_recovery/include -c $< -o $@
build/dac_prepare.o: src/dac_prepare.c ../source_recovery/include/pm_recovered.h | build
	$(CC) $(CFLAGS) -I../source_recovery/include -c $< -o $@
build/status_gate.o: src/status_gate.c | build
	$(CC) $(CFLAGS) -c $< -o $@
build/crc_byte.o: src/crc_byte.c | build
	$(CC) $(CFLAGS) -c $< -o $@
build/flash_spi.o: src/flash_spi.c | build
	$(CC) $(CFLAGS) -c $< -o $@
build/mixed.elf: build/application.o build/boot.o build/bridge.o build/scaling.o build/dac_prepare.o build/status_gate.o build/crc_byte.o build/flash_spi.o linker/mixed.ld
	$(CC) -mmcu=atxmega128a3u -nostdlib -Wl,--gc-sections,-T,linker/mixed.ld,-Map,build/mixed.map build/application.o build/boot.o build/bridge.o build/scaling.o build/dac_prepare.o build/status_gate.o build/crc_byte.o build/flash_spi.o -lgcc -o $@
build/mixed.hex: build/mixed.elf
	avr-objcopy -O ihex $< $@
build/flash_mixed.bin: build/mixed.hex
	$(PYTHON) ../tools/memory_image.py $< $@
check:
	cd ../reference && sha256sum -c SHA256SUMS
	$(MAKE) clean
	$(MAKE) all
	$(MAKE) -C ../exact_asm exact-check
	$(PYTHON) tests/check_layout.py
	$(PYTHON) ../tools/instruction_diff.py ../reference/flash_golden.bin build/flash_mixed.bin --code-range 0x3000:0x4000 --code-range 0x4000:0x1e000 > build/instruction_diff.txt
	$(PYTHON) tests/check_abi.py
	$(PYTHON) tests/check_unsigned_abi.py
	$(PYTHON) tests/check_callers.py
	$(PYTHON) tests/check_dac_edges.py
	$(PYTHON) tests/check_status_gate.py
	$(PYTHON) tests/check_crc_byte.py
	$(PYTHON) tests/check_flash_spi.py
	$(PYTHON) tests/check_flash_wait.py
	$(PYTHON) ../tools/function_inventory.py
clean:
	rm -rf build
	rm -f generated/application.S
.PHONY: all check clean
