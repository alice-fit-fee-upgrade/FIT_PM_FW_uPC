# Mixed C/ASM firmware development build

`make mixed-check` clean-builds and compares actual linked AVR execution with
original firmware. `make exact-check` independently checks the unchanged golden
assembly. `make c-check` retains the portable C recovery suite. `make c-progress`
reports original entry-point counts and writes build/function_inventory.json.

There are 89 recognized application entries: 16 now route to compiled C and 73
remain ASM. Functional C exists for 45 entries, including 29 awaiting integration.
Boot has nine additional ASM procedures. See [inventory](../docs/function_inventory.md)
and [current checkpoint](../docs/mixed_recovery_settings_checkpoint.md).

Current integrated entries include both scalers, four DAC preparations, the status
guard, CRC byte update and four FLASH SPI functions, FPGA state transition, status/LED update and FPGA settings initialization/reset. The FLASH status-wait entry
retains its original eight-byte 1600-iteration timed prefix. All 806 original text
symbols stay fixed. Bridges occupy 1324 bytes at 0x3000; compiled C/GCC helpers use
1632 bytes at 0x4000. No new static SRAM, BSS, initialized data or CRT is permitted.
Original vectors, tables, strings, startup and boot remain unchanged. Exact ASM
still has zero differing FLASH bytes; sixteen mixed entry spans cover 714 original
bytes, including retained ASM and ABI scaffolding, not pure C byte coverage.

Clean mixed-check and the expanded settings suite cover 2595680 compiled AVR
comparisons; the unchanged portable c-check suite previously passed
390601 portable cases. Whole register/SREG contracts, nonzero incoming R1, ordered
scripted MMIO/IRQ effects, three-byte return PCs and local stack frames are checked.
Two clean builds reproduce identical ELF/HEX/BIN hashes. Outputs are build/mixed.elf,
mixed.hex and flash_mixed.bin, plus map and JSON reports. Maximum extra tested stack
is still 35 bytes through DAC callers. Hardware, asynchronous interrupts, SPL timing
and total firmware stack budget remain unvalidated. No device is programmed.
