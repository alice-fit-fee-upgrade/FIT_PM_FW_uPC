.PHONY: all check clean
all:
	python3 tools/build.py
check: all
	python3 tests/check_bridge.py
	python3 tests/check_layout_guards.py
	python3 tests/check_ram_guard.py
	cc -std=c99 -Wall -Wextra -Werror -Isrc src/extension.c tests/check_commands.c -o build/check_commands
	build/check_commands
clean:
	rm -rf build
