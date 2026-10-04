# First mixed C/ASM integration: signed fixed-point scaler

A separate firmware image now routes the original function at byte address 0x2130 through an
ASM bridge to **AVR-compiled C**, including actual GCC multiplication helpers. Both original
callers continue to call the same original address. No native-C stubs are used in these tests.
Original exact_asm and its golden FLASH remain the bit-exact source baseline.

Reproduce: `make mixed-check` at repository root. This verifies reference hashes, builds from
scratch, runs the exact-ASM checkpoint, validates permitted layout changes, and executes direct
CPU-state and DAC-caller comparisons. `make c-check` retains the broader portable C tests.

## Layout and original-address preservation

| Byte region | Implementation |
|---|---|
| 0x2130..0x214B | Replace original 28-byte scaler by JMP + FF padding |
| 0x3000..0x304B | 76-byte original-ABI-to-C bridge |
| 0x3100..0x315D | 94 bytes of compiled scaler and GCC helpers |

All 806 original code-symbol addresses are preserved. All differences lie within these three
regions: 195 differing byte positions. New regions were entirely FF in golden FLASH. Boot,
vectors, other application instructions, tables, strings and every byte outside the permitted
regions remain identical. The linker rejects static SRAM/BSS/rodata additions and code growth
into the application-table section. There is no C runtime startup or new global-data initialization.
The unsigned scaler object section is discarded; the original unsigned ASM remains at 0x214C.

Only 28 original instruction bytes have been substituted in the mixed firmware, about 0.26% of
the 10,836 symbolic code-byte baseline. Functional C recovery still covers 2,494 routine-span
bytes (23.02%), but the remainder is not yet integrated. These are distinct coverage measures.

## ABI contract and reconstruction proof

Original inputs: unsigned coefficient R19:R18, signed input R21:R20. Outputs:
- R17:R16: round-to-nearest fixed-point multiply, modulo 65536.
- R1:R0: the final MULSU of signed R21 and unsigned R19.
- C/Z/N/V/S/H: flags of the original final `ADD R17,R0`.
- T/I: retain incoming values. All other registers retain incoming values.

The bridge saves incoming SREG and GCC call-clobbered registers including all four input bytes.
It clears R1 only during the C call, passes coefficient in R25:R24 and input in R23:R22, and
maps the returned R25:R24 to R17:R16. It restores saved registers and SREG, then repeats the
original final MULSU to recover R1:R0. Given returned high byte h and multiply low byte p,
the original pre-ADD high byte is uniquely `(h-p) mod 256`. SUB followed by ADD reconstructs
that prestate and then the original final operation, matching all six arithmetic flags. T/I
are unaffected by MULSU/SUB/ADD. Returned R1 intentionally may be nonzero, exactly as original.

C's signed product fits int32 for every input: -32768*65535 through 32767*65535. Casting that
product to uint32, adding 128 and shifting right by eight preserves returned bits 8..23 for
either sign, equivalent to the original partial-product rounding. This avoids an unnecessary
signed-division helper without relying on implementation-defined signed right shift.
The original coefficient and input bits are restored before reconstructing original MULSU.

## Compiled-AVR evidence

796,432 direct cases passed, comparing all 32 registers and eight SREG bits:
- Every 16-bit input for coefficients 0000, 0001, 00FF, 0100, 4188, 8000 and FFFF.
- Every coefficient for inputs 0000, 0001, 7FFF, 8000 and FFFF.
- 10,000 deterministic random pairs and caller states, including nonzero incoming R1.
- All 256 incoming SREG patterns are exercised.

131,072 additional cases run both original DAC callers (0x20EC, 0x2104; call sites 0x20F2,
0x210A), every input word, cycling all 12 channel indices. Full final registers/SREG, ordered
SPI polling/writes, IRQ effects and returned stack state match original execution. This also
checks composition with unchanged caller assembly, not just the isolated arithmetic value.
The updated host-C regression suite separately passed all 390,601 previous cases.

The stack model uses actual push/pop bytes in SRAM and three-byte, word-addressed return PCs.
GCC target preprocessing confirms `__AVR_3_BYTE_PC__`; original startup sets SP=0x3FFF.
A simulated original caller enters the scaler at SP=0x3FFC. Extra scaler depth is 22 bytes,
minimum SP=0x3FE6; through the DAC callers extra depth is 26 bytes. Saved return PCs and
balanced SP are checked. Inactive stack scratch contents may change; static/live memory
outside stack is forbidden for the isolated scaler. No peripheral access is permitted there.

Semantics sources used to audit the model:
- Microchip AVR Instruction Set Manual (ADD, ADC, MULSU, PUSH, CALL, RET):
  https://www.microchip.com/content/dam/mchp/documents/MCU08/ProductDocuments/ReferenceManuals/AVR-InstructionSet-Manual-DS40002198.pdf
- Microchip stack description (word-addressed return PC, least-significant byte at higher stack address):
  https://onlinedocs.microchip.com/oxy/GUID-7BB81102-57F6-474D-9BFE-B2AF8EA02A1E-en-US-5/GUID-E5ECF175-C6D7-442E-B548-A9B9E96D90D1.html

## Limits

Evidence comes from a bounded custom functional interpreter executing objdump-decoded original
and linked opcodes. Unsupported instructions fail closed. It is not independent physical-device
or cycle-accurate certification. The procedure executes 81/82 instructions instead of 13/14,
uses more stack, and can change asynchronous interrupt timing. Full nested-interrupt stack budget
and physical-module timing remain unvalidated. No hardware programming was performed.
Original bit-exact FLASH still passes exact-check with zero differing bytes.

A second clean mixed build reproduced identical ELF, HEX and canonical BIN SHA256 values.
The rebuilt BIN fingerprint equals the tested image recorded in mixed_abi_result.json.
The final exact-ASM build was also rebuilt and compared again after the shared converter/diff-tool
changes: still zero differing bytes. Evidence snapshots are saved under docs/mixed_*.
