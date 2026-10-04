# Second function integrated into the mixed firmware: unsigned scaler

The entry at byte 0x214C (`FUN_code_0010a6`, original word address 0x10A6)
now jumps to an ASM ABI bridge and calls actual AVR-compiled
`pm_scale_unsigned_abi`. The original 40-byte routine becomes a 4-byte JMP
and 36 FF padding bytes. The signed scaler at 0x2130 remains integrated.
The exact assembly firmware remains untouched and bit-identical.

The C adapter implements the original four partial products, low-product
rounding and two saturation paths. It also computes the six arithmetic flags:
the early saturation path preserves H from the preceding high-byte ADC,
sets C from the final MUL, and sets N/S from AND of the product high byte.
The other path uses flags from the last high-byte ADD; SER does not change them.
T/I preserve the incoming SREG bits. No compiler instruction-choice assumptions
are used for arithmetic flags.

The bridge saves the GNU C call-clobbered registers, clears R1 during the
C call, maps the returned value and SREG to the original ABI, restores inputs,
then executes one MUL to restore the original R1:R0. Finally it writes the
computed full SREG and restores the temporary register without changing flags.
All other registers and the three-byte return address are preserved.

The internal saturation label at 0x216E is retained as an address marker in
FF padding. The converter refuses source references to it outside this routine;
its two original branch references are removed with the replaced body.
This marker is not an independently supported callable entry point.

Layout: 160 bridge bytes at 0x3000, 460 C/helper bytes at 0x3100; 679 differing
byte positions exclusively in the two replacement ranges and these additions.
All 806 original text-symbol addresses are unchanged. No new static SRAM,
BSS, initialization data, vectors, tables or boot behavior is introduced.
Two integrated procedures replace 68 original symbolic bytes (0.63% of 10,836).
The existing 23.02% functional recovery measure counts recovered routine spans,
not how much original executable code has actually been replaced in the build.

Reproduce with `make mixed-check`, `make exact-check`, and `make c-check`.
The mixed checks execute linked AVR opcodes, not native C arithmetic callbacks:
796,432 direct cases per scaler compare all 32 registers, eight SREG bits and
balanced physical return stacks. The unsigned path executes 149..192
interpreter instruction steps and needs 22 additional stack bytes below its
entry SP, with a minimum SP of 0x3FE6; caller paths peak at 26 additional bytes.
262,144 caller cases cover all input words
through both signed and both unsigned DAC preparation routines. Calibration
RAM reads use deterministic test values; DAC SPI ready polling is scripted,
with ordered reads/writes and IRQ events compared to the original execution.
Unsupported instructions fail closed. Evidence is in the accompanying JSON.

This is functional evidence from a bounded custom interpreter. Instruction
counts and stack usage increase; cycle equivalence, asynchronous interrupt
scheduling, total firmware stack budget and physical module execution remain
unvalidated. No firmware was programmed onto a physical device.
