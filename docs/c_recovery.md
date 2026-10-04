# C recovery checkpoint

Functional recovery now covers 33 original entries spanning 2,494 original routine bytes,
23.02% of the 10,836 symbolic instruction bytes in exact_asm. Addresses and API mapping are
recorded in c_recovery_coverage.json. Coverage counts original routine spans including their
register-save/ISR scaffolding; it is NOT an assertion that those scaffolds were ported or that
23.02% of a running firmware image has been replaced. Exact firmware has zero C substitutions.

Modules: console parsers/formatting; UART transport and RXC/DRE bodies; signed/unsigned fixed-point
scaling; ADT7311 transactions; FPGA SPI read/write/bulk read; PLL read/write; DAC/THS788 transactions;
FPGA settings initialization/reset; shutdown sequence. AVR backend provides volatile MMIO, FLASH
reads and IRQ operations. C library compiles and links on the shared AVR toolchain with errors on
warnings. All modules are forcibly linked to catch unresolved compiler/runtime helpers.
The linked empty-main ELF is a linker check and is not a recovered firmware to program.

## Differential evidence

`make c-check` compiles target objects and native C, then compares native functions to bounded
execution of real original FLASH disassembly. Unsupported instructions fail rather than guessing.
The test program comes from reference/flash_golden.bin, not recovered C or upstream annotations.
Native callbacks provide input bytes and deterministic register response schedules. Stream hooks
bypass UART routines for parser/format tests; UART routines themselves are independently trace-tested.
The byte parser returns include consumed input, terminator, partial value on failure and original Carry.
MMIO tests compare the entire ordered read/write/IRQ trace, not just the final state.

390,601 deterministic cases passed. This includes every positive decimal 16-bit input, all negative
magnitudes through -32769 and overflow/8-bit-count-wrap cases; every four-digit hexadecimal input;
all 65,536 hex formatting values; 14,700 decimal output cases across five original entry modes;
all 65,536 inputs for each of two fixed-point coefficients plus 4,000 random pairs each; UART
wrap/RTS/busy-loop/error branches; sampled bus inputs and transaction sequences; settings and
shutdown; original FLASH-string and CRLF output. Detailed counts: c_recovery_validation.json.

Evidence limits: custom functional interpreter, not an independently certified AVR simulator.
No cycle timing, analog/peripheral behavior, asynchronous interrupt interleaving or complete
original register/SREG ABI equivalence is claimed. Scratch-memory implementation differences
are intentional in the separate C API (e.g. local formatter buffer), never in exact_asm.
C ISR functions represent bodies; installing vectors/prologues is pending integration.

## Findings that changed prior interpretation

- The supposed 32-bit hex formatter actually prints R17:R16 as four digits.
- The supposed fpga_is_ready routine at 0x2130 is signed fixed-point multiplication.
- Decimal field width is 5 + signed-mode + presence of decimal point, not uniformly six.
- UART TX wraps at 256, RX at 64. Receive case folding uses signed character comparison.
- RTS release at exact threshold writes the full prior PORTF.OUT mask, not just bit0.
- FPGA read header preserves bits from incoming R21, unlike the write header which clears it.
- Settings-init gate field uses one byte at 0x222F. Trigger and board-ID words start at 0x2230
  and 0x2232; this resolves the earlier incorrect EEPROM overlap hypothesis. eeprom_map.md is corrected.
- Final settings unlock write is 0x0FFF, although the earlier README says 0xFFFF.

All of these original behaviors are retained and differentially checked. Existing root C files and
asm_analysis remain untouched. The command dispatcher, startup/timer/power state machines, EEPROM
write path, remaining application logic and bootloader still await C recovery. ABI-safe integration
requires preserving caller-visible flags/registers, allocating C data/stack within original RAM,
and proving timing before replacing bit-banged routines. Those constraints are not hidden by a
successful native functional test. exact_asm remains the binary oracle and its exact-check still passes.

## First ABI-safe mixed-image checkpoint

The signed scaler at 0x2130 is now integrated in a separate mixed C/ASM firmware image.
Direct execution of AVR-compiled C and its GCC helpers passed 796,432 full CPU-state cases;
both original DAC callers passed 131,072 end-to-end cases. Exact firmware still has zero C
substitutions. See mixed_c_asm_checkpoint.md for the 28-byte replacement, preserved addresses,
stack overhead, flag reconstruction and timing limits.
