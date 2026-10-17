.PHONY: all check clean
all:
	python3 tools/build.py
check: all
	python3 tests/check_temperature_guards.py
	python3 tests/check_i2c.py
	python3 tests/check_bus_lock.py
	python3 tests/check_pg_timing.py
	python3 tests/check_bridge.py
	python3 tests/check_pll_legacy.py
	python3 tests/check_layout_guards.py
	python3 tests/check_ram_guard.py
	cc -std=c99 -Wall -Wextra -Werror -Isrc src/extension.c src/hw_commands.c src/hw_drivers.c src/i2c_commands.c src/i2c_diag.c src/pll_transport.c tests/pll_legacy_host.c tests/check_commands.c -o build/check_commands
	build/check_commands
	cc -std=c99 -Wall -Wextra -Werror -Isrc src/hw_drivers.c src/pll_transport.c tests/pll_legacy_host.c tests/check_hw_drivers.c -o build/check_hw_drivers
	build/check_hw_drivers
	cc -std=c99 -Wall -Wextra -Werror -Isrc src/extension.c src/hw_commands.c src/hw_drivers.c src/i2c_commands.c src/i2c_diag.c src/pll_transport.c tests/pll_legacy_host.c tests/check_pll_commands.c -o build/check_pll_commands
	build/check_pll_commands
	cc -std=c99 -Wall -Wextra -Werror -Isrc src/extension.c src/hw_commands.c src/hw_drivers.c src/i2c_commands.c src/i2c_diag.c src/pll_transport.c tests/pll_legacy_host.c tests/check_fpga_flash.c -o build/check_fpga_flash
	build/check_fpga_flash
clean:
	rm -rf build

# Experimental simulation-only image; never programs hardware.
.PHONY: scaling-check
scaling-check:
	PM_SCALING_CANDIDATE=1 $(MAKE) check
	python3 tests/check_scaling_candidate.py
	python3 tests/check_scaling_native_api.py
	python3 tests/check_scaling_callers.py

# DAC driver +48-setting updater; simulation only, includes scaling candidates.
.PHONY: dac-check
dac-check:
	PM_DAC_CANDIDATE=1 $(MAKE) check
	python3 tests/check_scaling_candidate.py
	python3 tests/check_scaling_native_api.py
	python3 tests/check_scaling_callers.py
	python3 tests/check_dac_candidate.py
