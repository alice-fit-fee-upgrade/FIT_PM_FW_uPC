# Validation evidence

- Existing upstream assembler source compiled without diagnostics.
- Two consecutive root `make exact-check` clean builds succeeded.
- ELF, HEX and canonical BIN SHA256 values were identical between those builds.
- Golden/reference hash validation passed for every attachment and normalized memory snapshot.
- Final `cmp`: 0 differing bytes over 139,264; 11,354 non-erased golden bytes all matching.
- Instruction-aware diff: 0 differing instruction/data windows.
- Address audit: 394 Ghidra-style symbols retain original word-to-byte addresses.
- 760 classified 32-bit instruction encodings match original memory.
- Six forensic-tool behavior checks passed: segment/linear addressing, sparse normalization,
  corrupt HEX/conflicting overlap/missing EOF/bounds rejection, honest used-byte metrics,
  added programming in erased space, and branch relocation reporting.
- Boot extraction reproduced checked-in symbolic source exactly; no extractor runs in the build.
- First isolated C arithmetic candidate: all 256 inputs match verified original opcode arithmetic;
  target AVR compilation passed `-Os -Wall -Wextra -Werror`.

Software evidence only. No programming/debugger access or physical execution was performed.
No claim of complete recovered C, dynamic EEPROM path coverage, or hardware protocol validation.
GNU AVR toolchain reused from /home/codex-hil/.local/opt/avr (GCC 7.3.0 / binutils 2.26).

## Subsequent C-recovery checkpoint

390,601 functional differential cases passed against original FLASH instruction execution,
including MMIO/IRQ traces, parser partial-error states, formatting and settings/shutdown.
AVR library compilation and whole-library link check passed with warnings treated as errors.
Exact FLASH check was repeated after C development: still 0 differing bytes.
See c_recovery.md, c_recovery_validation.json and c_recovery_coverage.json for scope and limits.

## First compiled-AVR mixed firmware checkpoint

796,432 direct ABI comparisons passed on linked AVR C/ASM opcodes, comparing all registers,
SREG and architectural three-byte return stack. Both original DAC callers passed another
131,072 full-input cases with matching register/flag/stack state and SPI/IRQ traces.
Native C regression still passed 390,601 cases. The exact baseline still matches all FLASH bytes.
Mixed layout differs at 195 positions only within the designated 28-byte replacement and new
76-byte bridge / 94-byte C+GCC-helper additions. No new static SRAM is linked.
Reports: mixed_abi_result.json, mixed_callers_result.json, mixed_layout_result.json,
mixed_byte_comparison.txt and mixed_instruction_diff.txt. Physical/timing validation remains pending.
