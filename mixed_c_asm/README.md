# Historical non-exact C/ASM experiment

This tree changes the original FLASH and is excluded from the accepted development
baseline. Use `../recovered_exact` and root `make exact-check` for firmware recovery.
Tests and implementations are retained for reference; behavioral agreement does not
qualify a conversion for the exact baseline.

`make mixed-check` clean-builds and compares actual linked AVR execution with
original firmware. `make exact-check` independently checks the unchanged golden
assembly. `make c-check` retains the portable C recovery suite. `make experimental-c-progress`
reports original entry-point counts and writes build/function_inventory.json.

There are 89 recognized application entries: 27 now route to compiled C and 62
remain ASM. Functional C exists for 48 entries, including 21 awaiting integration.
Boot has nine additional ASM procedures. See [inventory](../docs/mixed_recovery_reads_function_inventory.md)
and [current checkpoint](../docs/mixed_recovery_reads_checkpoint.md).

Current integrated entries include both scalers, four DAC preparations, status
and LED control, CRC update, four FLASH SPI functions, FPGA settings init/reset,
system init/deinit, PLL control reset/read, three ADT7311 transaction wrappers,
FPGA MCU timestamp, HEX digit/16-bit output and CRLF. The FLASH status-wait entry
retains its original eight-byte 1600-iteration timed prefix. All 806 original text
symbols stay fixed. Bridges occupy 2268 bytes at 0x3000; compiled C/GCC helpers use
2464 bytes at 0x4000. No new static SRAM, BSS, initialized data or CRT is permitted.
Original reset vectors, tables, strings and boot bytes remain unchanged. Exact ASM
still has zero differing FLASH bytes; twenty-seven mixed entry spans cover 1484 original
bytes, including retained ASM and ABI scaffolding, not pure C byte coverage.

The full regression for this 27-entry experiment was interrupted on the user's
strategy change during the ADT suite; it has no complete current-image pass.
The last completed 19-entry regression covered 2730848 compiled AVR comparisons.
The unchanged portable c-check suite previously passed
390601 portable cases. Whole register/SREG contracts, nonzero incoming R1, ordered
scripted MMIO/IRQ effects, three-byte return PCs and local stack frames are checked.
Historical reproducibility reports belong to their documented prior checkpoints.
Outputs are build/mixed.elf,
mixed.hex and flash_mixed.bin, plus map and JSON reports. Maximum extra tested stack
is still 35 bytes through DAC callers. Hardware, asynchronous interrupts, SPL timing
and total firmware stack budget remain unvalidated. No device is programmed.
