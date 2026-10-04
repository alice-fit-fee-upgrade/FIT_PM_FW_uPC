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
