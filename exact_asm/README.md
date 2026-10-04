# Exact GNU AVR reconstruction

Run `make exact-check` at repository root, or `make -C exact_asm exact-check`.
Requires Python 3, GNU make, avr-gcc 7.3.0, AVR binutils 2.26. No Python packages.
On this host: `export PATH=/home/codex-hil/.local/bin:$PATH`.

The full byte-addressed FLASH is 0x00000..0x21FFF (128 KiB application + 8 KiB boot).
The linker pins application to 0 and boot to 0x20000. Original `.org` anchors remain.
Do not enable relaxation or introduce CRT/startup libraries. Original startup is already ASM.
EEPROM, fuses, signature and lock are separate device states, never FLASH linker payloads.

`generated/application.S` is reproducibly converted from untouched upstream `asm_analysis/main.S`.
`src/boot.S` contains checked-in symbolic recovered instructions, labels and explicit FF gaps;
it was initially recovered with `avr-objdump -D -b binary -m avr:106`, replacing relative
operands with target labels. The build does not disassemble or embed golden memory as source.
Changes belong in the converter or checked-in ASM; editing generated files is temporary.
Symbols inherited from upstream remain hypotheses until peripheral/dataflow evidence supports them.

`make exact-check` cleans compiled artifacts, regenerates application source, links without CRT,
emits ELF/HEX, normalizes HEX with FF holes, verifies reference hashes, runs byte comparison,
`cmp`, and instruction-aware diff. Outputs reside in ignored `build/`.
Golden files under `reference/` must never be overwritten. `SHA256SUMS` records originals and
canonical snapshots. Integrity checking detects changes; file permissions are an additional guard.

Address safety: per-region `.org` anchors reject growth past the next anchor. Internal local labels
can still move within a region; exact-check detects any byte change. Linker assertions reject overflow.
This is a source reconstruction baseline, not a hardware-programming prescription.

Measured reconstruction coverage (docs/asm_audit.json): 10,836 symbolic instruction bytes,
0 literal code bytes, 710 constant/string/table bytes, 127,718 padding/erased bytes.
Code/data classification is static and conservative; exact byte identity is independent of inferred names.
