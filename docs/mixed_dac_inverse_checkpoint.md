# Third original entry integrated: inverse DAC preparation

Byte 0x20D0 (`FUN_code_001068`) now routes to `pm_dac_prepare_inverse`
compiled for ATxmega128A3U. Its C calculation performs modulo-65536
`20000 - input`, original unsigned scaling by 0x0272, and modulo-byte
channel multiplication by four. An eight-byte return packet carries the
value, transformed input, original final MUL product and control byte.
The ASM adapter restores the original register contract and T/I before
calling the unchanged original DAC SPI routine. That routine determines
all six arithmetic flags and enables interrupts before return.

No new SRAM globals or CRT are introduced. All 806 original symbol addresses
remain fixed; only the 28-byte original entry and prior replacement ranges
change. Bridges stay at 0x3000; the C region moves to 0x4000 to reserve space
for subsequent bridges without repeatedly moving C. The old C region at
0x3100 returns to golden FF. Layout checks enforce permitted changes.

The bounded AVR execution model now supports SPL/SPH and compiler-local
SRAM stack accesses, requiring reads/writes to lie above the current SP.
Physical three-byte return PCs and balanced SP remain checked. Full caller
regressions passed 262,144 cases; maximum additional caller stack increased
to 32 bytes. The exact assembly baseline still passes cmp with zero differences.
Evidence: mixed_dac_inverse_callers.json and mixed_dac_inverse_layout.json.

These tests execute compiled opcodes; they do not prove timing equivalence,
SPL interrupt-suppression timing, asynchronous interrupt behavior, whole-firmware
stack budget or physical module operation. No hardware was programmed.
