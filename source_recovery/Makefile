CC = avr-gcc
HOST_CC = cc
CFLAGS = -std=c99 -Os -Wall -Wextra -Werror -Iinclude
SOURCES = hex_digit.c src/console.c src/scaling.c src/transport.c src/peripherals.c src/settings.c
OBJECTS = $(patsubst %.c,build/%.o,$(SOURCES)) build/src/avr_backend.o
all: build/libpm_recovered.a build/link_check.elf
build/%.o: %.c include/pm_recovered.h include/pm_addresses.h
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -mmcu=atxmega128a3u -ffunction-sections -c $< -o $@
build/libpm_recovered.a: $(OBJECTS)
	avr-ar rcs $@ $^
build/libpm_recovered.so: $(SOURCES) include/pm_recovered.h include/pm_addresses.h
	mkdir -p build
	$(HOST_CC) $(CFLAGS) -g -fPIC -shared $(SOURCES) -o $@
check: all build/libpm_recovered.so
	cd ../reference && sha256sum -c SHA256SUMS
	python3 tests/differential.py
	python3 tests/coverage.py
clean:
	rm -rf build
.PHONY: all check clean

build/link_check.elf: tests/link_check.c build/libpm_recovered.a
	$(CC) $(CFLAGS) -mmcu=atxmega128a3u tests/link_check.c -Wl,--whole-archive build/libpm_recovered.a -Wl,--no-whole-archive -o $@
