# Recovery progress: 12 application entry points integrated

This batch integrated six additional original entries, building and comparing
actual AVR execution after each replacement. The exact ASM remains unchanged.

| Original byte address | New C implementation | Original span |
|---|---|---|
| 0x211C | Status bit-4 guard, retaining original UART failure sender | 20 |
| 0x17E4 | Original left-shift/LSB-first CRC byte update | 36 |
| 0x1664 | Original FLASH hardware-SPI initialization | 26 |
| 0x171E | FLASH command 06 with ready-status polling | 28 |
| 0x1808 | Command and three address bytes, CS remains asserted | 56 |
| 0x173A | FLASH busy status transaction/retry; exact timed ASM prefix retained | 56 |

## How many are left

The application has 89 recognized original entry points. This includes 10
interrupt handlers, main, five shared decimal-format entries and two receive
entries. It is an entry-point count, not 89 independent C function bodies.
All original direct CALL targets and vector destinations are included. The
function annotations include two non-undefined return types, which explains
why simply counting `;undefined` gives an incomplete number (87).

12 entries now route to compiled C; 77 application entries remain unintegrated.
43 entries have functional C recovery across the portable library and mixed
firmware, leaving 31 recovered entries to integrate and 46 without functional
C recovery. The boot has nine separate recognized procedures: six directly
called helpers and three interrupt bodies reached through vector stubs. Boot
vector stubs are aliases, not additional functions. All nine stay in ASM.
Thus 86 original entries remain outside C integration when boot is included.
Some timing-critical and boot/NVM code may appropriately remain ASM permanently.

Reproduce the inventory with `make c-progress`. It reads actual mixed ELF/BIN,
checks original entry coverage against golden direct calls and vectors, and
writes mixed_c_asm/build/function_inventory.json. The retained-delay entry is
recognized through its JMP after eight unchanged original bytes. The saved
function_inventory.json/Markdown list gives every remaining address and name.
Original inherited names may still be hypotheses; e.g. fpga_is_ready is a scaler.

## Layout and original contracts

All 806 original text-symbol addresses remain fixed. The twelve substituted
entry spans cover 408 original bytes (3.77% of 10,836 symbolic baseline bytes),
including eight still-executed original delay bytes and ABI scaffolding. This
metric is distinct from pure emitted C bytes and portable semantic recovery.
Bridges occupy 980 bytes at 0x3000; compiled C/helpers occupy 1232 at 0x4000.
There are 2559 differing byte positions, all inside explicitly allowed original
replacement regions and previously erased additions. Vectors, startup, other
instructions, constants, strings and boot remain identical. Static SRAM/BSS/data
and a new CRT are forbidden by the linker. No literal whole-image blob is used.

The CRC bridge was corrected after a differential test caught swapped GNU C
argument halfwords; no failing version was committed. Polynomial and bit order
retain original behavior. The wait-status entry executes its original 1600-loop
SBIW/BRNE sequence before every transaction; bytes are checked against golden.
No timeout, sanitization or alternate hardware setup is introduced.

## Final clean-build validation

`make mixed-check` passed from a clean build:
- 796432 direct signed-scaler and 796432 unsigned-scaler cases.
- 262144 DAC-caller and 87040 channel/calibration boundary cases.
- 70656 status-guard cases, including queued/busy/wrap/full UART behavior.
- 77864 direct CRC and 280 original SPI-stream caller cases.
- 294912 FLASH helper/erase-caller cases across status/command/flag combinations.
- 5120 FLASH busy-wait cases, including repeated trips through original delay.

Total: 2390880 compiled-AVR comparisons, checking full original register/SREG
contracts, ordered scripted MMIO/IRQ effects and physical three-byte return-stack
balance. No native C shortcut stands in for compiled C execution. `make c-check`
separately passed 390601 portable semantic comparisons. The mixed target verifies
reference hashes and runs exact-check; golden cmp still has zero differing bytes.

Evidence snapshots and full target log are saved as mixed_recovery_wave_*.
A second clean build reproduces identical ELF/HEX/BIN SHA256 values, matched to
the tested image. Per-function commits/checkpoints retain prior independent tests.

## Limits and subsequent work

These are bounded custom functional-interpreter checks, not independent hardware
certification. Maximum additional stack remains 35 bytes for DAC callers; new
status/CRC/FLASH depths are recorded in individual reports (15..32 extra bytes).
Total firmware stack usage with nested interrupts, exact peripheral timing, SPL
interrupt suppression and asynchronous observations remain unvalidated. The old
fixed delay is retained but this does not prove cycle equivalence of the C SPI
transaction itself. No hardware programming was performed.

The next candidate block is GPIO/status control and the existing FPGA settings
C routines, with original register and interrupt contracts audited before routing.
Serializers that produce clock edges in software and boot/NVM routines retain ASM
until their hardware-sensitive contracts can be verified. Exact_asm is retained.
