# Assessment of upstream asm_analysis

This is category E: a mixture, predominantly real GNU AVR assembly with manual annotations
and Ghidra-style function/label names, not an avr-objdump listing. It contains `.section`, `.org`,
labels, instructions, `.string` and `.byte`, and assembles directly with GNU assembler-with-cpp.
Ghidra-style type signatures and unknown-memory comments survive as semicolon comments.
The include was adapted to GNU syntax but identifies itself as the older A3 definitions;
peripheral meaning must be checked against A3U and hardware revision.

Initial direct command: `avr-gcc -mmcu=atxmega128a3u -x assembler-with-cpp -c asm_analysis/main.S`.
No assembly diagnostics. The upstream Makefile's `OBJ = $(SRC:.asm=.o)` fails to replace `.S`;
this was avoided in the isolated build rather than changing historical experiments.
Direct link with `-nostdlib` succeeded. Initial byte mismatch count: 1,221.
Simply adding erased FF fill to `.org` reduces differences to those itemized in diff_analysis.md.

Addresses in Ghidra-derived `FUN_code_xxxxxx`, `LAB_code_xxxxxx` are AVR word addresses.
Their GNU `.org (... << 1)` operands convert to byte addresses. ELF symbols, GNU linker addresses,
Intel HEX, canonical BIN, and objdump display addresses are bytes. Runtime LPM Z addresses
are bytes; CALL/JMP encoding contains word addresses although GNU symbolic targets are bytes.
394 address-bearing FUN/LAB symbols checked against linked ELF: no address errors.
Vector definitions in this include are word offsets, not slot numbers; their shifts yield 4-byte slots.

All 760 32-bit instructions in the classified code are rebuilt byte-for-byte; CALL/JMP and
classic LDS/STS are decoded with four bytes. `docs/instruction_index.json` records every classified
instruction with original bytes. This establishes encoding/addresses, not correctness of all function names.
The original 0x1576..0x1579 JMP was deliberately replaced by RET in analysis. It is restored.

Application code 0x1E2..0x2915; constants/strings/tables 0x2916..0x2BDB.
Vectors: 11 occupied slots (including reset), each RJMP plus FF half-slot. Other slots erased.
Boot vectors at 0x201A0, 0x201DC, 0x201E0; recovered boot instructions through 0x204E5.
The boot fragment in upstream is a single commented byte line; it does not cover all boot code.
All original `.org` anchors are retained. Holes before constants use FF; between string/table
anchors use 00, exactly as the observed original image (including trailing string alignment).
Constants include PLL programming words, strings, unlock marker at 0x2B76, timestamp at 0x2B92,
and byte-address string pointers at 0x2BA8. These remain data, not reinterpreted instructions.
The table at 0x2BA8 includes a pointer starting at odd byte address 0x2BAE; no forced word alignment.

Result: exact assembly reconstruction is possible with minimal mechanical conversion plus recovered
boot assembly. There is no `.incbin` and no executable region represented as a raw blob.
Static code/data classification does not prove unreachable bytes or absence of computed control flow.
Existing recovered C, protocol descriptions and function names remain evidence to check, not ground truth.
