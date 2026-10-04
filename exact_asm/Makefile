CC = avr-gcc
PYTHON = python3
BUILD = build
all: $(BUILD)/flash_rebuilt.bin
$(BUILD):
	mkdir -p $@
generated/application.S: ../asm_analysis/main.S ../tools/convert_analysis.py
	$(PYTHON) ../tools/convert_analysis.py
$(BUILD)/application.o: generated/application.S ../asm_analysis/ATxmega128A3Udef.inc | $(BUILD)
	$(CC) -mmcu=atxmega128a3u -I../asm_analysis -c $< -o $@
$(BUILD)/boot.o: src/boot.S | $(BUILD)
	$(CC) -mmcu=atxmega128a3u -c $< -o $@
$(BUILD)/reconstructed.elf: $(BUILD)/application.o $(BUILD)/boot.o linker/exact.ld
	$(CC) -mmcu=atxmega128a3u -nostdlib -Wl,-T,linker/exact.ld,-Map,$(BUILD)/reconstructed.map $(BUILD)/application.o $(BUILD)/boot.o -o $@
$(BUILD)/reconstructed.hex: $(BUILD)/reconstructed.elf
	avr-objcopy -O ihex $< $@
$(BUILD)/flash_rebuilt.bin: $(BUILD)/reconstructed.hex
	$(PYTHON) ../tools/memory_image.py $< $@
exact-check:
	cd ../reference && sha256sum -c SHA256SUMS
	$(MAKE) clean
	$(MAKE) all
	$(PYTHON) ../tools/compare_flash.py ../reference/flash_golden.bin $(BUILD)/flash_rebuilt.bin --report $(BUILD)/comparison.txt --report-only
	avr-objdump -D $(BUILD)/reconstructed.elf > $(BUILD)/rebuilt.dis
	avr-objdump -D -b binary -m avr:106 ../reference/flash_golden.bin > $(BUILD)/golden.dis
	$(PYTHON) ../tools/instruction_diff.py ../reference/flash_golden.bin $(BUILD)/flash_rebuilt.bin > $(BUILD)/instruction_diff.txt
	cmp ../reference/flash_golden.bin $(BUILD)/flash_rebuilt.bin
clean:
	rm -rf $(BUILD)
	rm -f generated/application.S
.PHONY: all clean exact-check

