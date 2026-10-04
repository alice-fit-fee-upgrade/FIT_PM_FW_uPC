# Mixed C/ASM firmware development build

`make mixed-check` clean-builds and compares actual linked AVR execution with
original firmware. `make exact-check` independently checks the unchanged golden
assembly. `make c-check` retains the portable C recovery suite. `make c-progress`
reports original entry-point counts and writes build/function_inventory.json.

There are 89 recognized application entries: 19 now route to compiled C and 70
remain ASM. Functional C exists for 47 entries, including 28 awaiting integration.
Boot has nine additional ASM procedures. See [inventory](../docs/function_inventory.md)
and [current checkpoint](../docs/mixed_recovery_system_checkpoint.md).

Current integrated entries include both scalers, four DAC preparations, status
and LED control, CRC update, four FLASH SPI functions, FPGA settings init/reset,
system init/deinit and PLL control reset. The FLASH status-wait entry
retains its original eight-byte 1600-iteration timed prefix. All 806 original text
symbols stay fixed. Bridges occupy 1600 bytes at 0x3000; compiled C/GCC helpers use
2004 bytes at 0x4000. No new static SRAM, BSS, initialized data or CRT is permitted.
Original reset vectors, tables, strings and boot bytes remain unchanged. Exact ASM
still has zero differing FLASH bytes; nineteen mixed entry spans cover 1108 original
bytes, including retained ASM and ABI scaffolding, not pure C byte coverage.

Full clean mixed-check covers 2730848 compiled AVR comparisons; the unchanged portable c-check suite previously passed
390601 portable cases. Whole register/SREG contracts, nonzero incoming R1, ordered
scripted MMIO/IRQ effects, three-byte return PCs and local stack frames are checked.
Two clean builds reproduce identical ELF/HEX/BIN hashes. Outputs are build/mixed.elf,
mixed.hex and flash_mixed.bin, plus map and JSON reports. Maximum extra tested stack
is still 35 bytes through DAC callers. Hardware, asynchronous interrupts, SPL timing
and total firmware stack budget remain unvalidated. No device is programmed.
