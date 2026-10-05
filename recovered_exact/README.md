# Binary-exact recovered development source

This is the accepted firmware baseline. `make` at repository root builds this
tree and checks complete canonical FLASH equality. `make exact-check` clean-builds
ELF, Intel HEX and the full 0x22000-byte BIN, verifies reference hashes and runs
`cmp` against reference/flash_golden.bin. There is no emulator acceptance step.
`make c-progress` reports the three exact-source classifications.

Current result: 89 application entries, 5 C_BINARY_EXACT, 83
C_WITH_EXACT_ASM_HELPER, 1 ASM_EXACT. All nine boot procedures remain exact ASM.
All 806 original text symbols keep their addresses. FLASH differs in zero bytes.
Compiler-generated instructions occupy 5190 of 10836 executable bytes (47.8959%);
ASM occupies 5646 bytes (52.1041%), including 4838 inline-helper bytes. Of C bytes,
184 are compiler-generated AVR primitives (CLI/SEI/NOP/SWAP/BST/BLD); the other
5006 are other C operations. GNU AVR built-ins are counted as compiler output,
not as inline assembly. Instruction-index and GCC-provenance counts agree.
Data and padding do not contribute to these percentages. Application-only C is
51.6007%. The entry count includes main, ISRs and shared formatter entries,
rather than 89 independent C-style function bodies.

## Build and source layout

Use the shared avr-gcc 7.3.0 / AVR binutils 2.26 toolchain. The build uses GNU C99,
-Os, per-function sections, no LTO, no CRT, no library-generated code or new SRAM.
The linker rejects unexpected text, data, rodata and BSS and asserts every region's
original size. Different compiler versions are accepted only if the complete
binary still passes exact-check.

- src/: readable C and small exact inline ASM fragments.
- manifest.json: accepted function ranges, compiler profiles and helper ranges.
- tools/prepare_sources.py: deterministic conversion and splitting of original
  symbolic analysis around accepted C regions; never reads golden image bytes.
- linker/exact.ld: fixed memory contract and generated per-region placement.
- generated/: derived ASM, linker regions and assertions; regenerated on demand.
- tools/inventory.py: validates binary equality, symbol positions and instruction
  coverage, then classifies all original application entries.
- build/: ELF/HEX/BIN, map, disassembly, byte/instruction diff and inventory.

Remaining application ASM is mechanically derived from the existing verified
analysis; boot is built directly from ../exact_asm/src/boot.S. The immutable
exact_asm project and all golden inputs remain unchanged. No whole-image incbin,
literal executable blob or link-time opcode substitution is used.

## Legacy register contracts

The original ASM uses register contracts that differ from GNU AVR function calls.
Per-file compiler profiles reserve registers and define which are call-saved.
For R16 leaf routines, this permits ordinary C MMIO assignments to produce the
original LDI/STS instructions. Profiles are private to these no-GNU-argument entry
points; they are not a global ABI for future C code.

Small helpers retain compiler-sensitive details: relative RCALL rather than CALL,
CLR rather than LDI zero, an otherwise redundant scratch reload, absolute LDS/STS,
DMA polling and the original PLL register setup/CLI/CALL/SEI sequence. Zero-byte
inline-ASM operands capture legacy register inputs/outputs without adding code.
They are register contracts, not byte patches. Manifest helper ranges count every
emitted inline ASM instruction as ASM, including MOV captures and CS stores.

ADT7311 wrappers use GNU C for sequencing and compiler-generated saves/restores;
the byte primitive combines C GPIO/loop setup with exact bit tests, shifts,
sampling delay and flag-sensitive loop helpers. Ordinary C calls cannot silently
replace these register interfaces. A change in generated instructions is rejected
by the full-image gate, even if emulator behavior would agree.

## Continuing recovery

For each candidate, compile and build the full image, then run root
`make exact-check`. Keep it only at zero differing bytes. A small compiler/profile
adjustment or a short exact helper is reasonable; otherwise retain the symbolic
ASM and try another function. Update the manifest only for accepted regions.
No behavioral-equivalence classification is used in this baseline.

Earlier mixed_c_asm and source_recovery trees and exhaustive tests remain useful
historical evidence. They do not contribute C counts to this baseline.
See [checkpoint](../docs/exact_c_checkpoint.md) and
[inventory](../docs/function_inventory.md).

GPIO interrupt entries keep a short exact register/SREG save and restore frame,
with C status/GPIO updates. These entry points are called by the original vectors;
do not call them as ordinary C functions. PORTD uses `-fno-reorder-blocks` to keep
the original branch layout. The FLASH-string sender uses the same option and an
exact terminator branch, with GCC generating its original R16 save/restore.

Each exact-check also audits inline ASM byte ranges against GCC APP/NOAPP markers
in assembly emitted with the actual per-file build flags. This independently
checks the C/ASM coverage figures, including subsection tails and C-generated
loop-back branches. See [remaining ASM review](../docs/easy_conversion_assessment.md).

Exact C helpers for SPI polling, absolute SRAM reads and AVR CPU primitives are
opted into only after individual whole-image checks. Some entry profiles need an
ASM absolute load or pointer operation because GCC cannot allocate another
register without changing the original frame. Those helpers remain ASM.
See [continuation results](../docs/exact_continuation_results.json) for all
98 attempts, including rejected candidates, and the complete-image logs.
