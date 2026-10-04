# System and PLL control recovery: 19 integrated application entries

Three more original entries now execute compiled C in
mixed_c_asm/src/system_control.c. Each was built and differentially checked before
the next replacement. The original PLL SPI serializer remains ASM at 0x2486.

| Original byte range | C implementation | Original span |
|---|---|---|
| 0x0C96..0x0D0F | pm_system_deinit_c | 122 bytes |
| 0x0D10..0x0DDB | pm_system_init_c | 204 bytes |
| 0x0DDC..0x0E1F | pm_pll_control_reset_c | 68 bytes |

## Preserved behavior

Deinitialization retains all 22 ordered RAM/peripheral writes, including clearing
status bit 4 at 0x2157. It returns R16=0, preserves every other register, and
reproduces the final EOR flags: Z=1,N=V=S=0; C/H/T/I preserve their incoming values.
The preceding status ANDI does not determine final flags. The bridge replays
that original final EOR after restoring incoming SREG.

Initialization retains the GPIO, SPI and DMA setup order. DMA reset bit 6 at
0x0100 is polled until clear, without a new timeout. Transfer count is written
as two separate ordered bytes at 0x0124 and 0x0125. PLL control value 0x00001008
is passed through a GNU-callable adapter to original R16..R19, followed by the
original CLI/serializer/SEI sequence. Final R16..R19 are 8,0x10,0,0; other registers
are preserved. Final SREG is (incoming & 0x61) | 0x82, including forced I=1.

PLL reset reads the original ten little-endian control words from FLASH byte
addresses 0x2916..0x293D via real LPM instructions, not an embedded copy of the
table. Each value goes through the same original CLI/serializer/SEI sequence.
PORTF_INTCTRL=0x0A and PORTB_OUTCLR=0x20 follow the tenth word, as before.
Every register is preserved. Final DEC flags yield SREG (incoming & 0x61) | 0x82.
No bug correction, clock retuning, serializer replacement or timeout is introduced.

The GNU adapter maps the 32-bit value low-to-high from R22..R25 to R16..R19,
preserving the GNU callee-saved registers. Original R0/R1, including nonzero R1,
are restored by the entry bridges. R1 is zero only while running GNU C code.

## New differential suites

- Deinit: all 256 status bytes x all 256 incoming SREG patterns, 65536 cases.
- Init: all SREG patterns, every DMA-ready byte with bit 6 clear, every SPI-ready
  byte with bit 7 set, and scripted DMA/SPI poll delays, 36864 cases.
- PLL reset: all 256 SREG patterns x 128 SPI-ready bytes, with cycled poll delays,
  32768 cases. The actual 40 SPI data bytes match the immutable original table.

All compare the full 32 registers, SREG, ordered RAM/MMIO/IRQ effects and balanced
three-byte return stacks. Additional tested stack is 18 bytes for deinit, 26 for
init and 31 for PLL reset; the previous overall tested maximum remains 35 bytes.
The functional interpreter reads LPM data from golden FLASH; layout checks
confirm those table bytes are identical in the mixed image. It does not model
cycles, asynchronous interrupts, SPL interrupt suppression or hardware behavior.

## Progress and layout

89 application entries: 19 integrated C, 70 still ASM. Functional C is available
for 47 entries, including 28 awaiting integration; 42 have no recovered C.
Nine boot procedures remain ASM. Total unintegrated including boot is 79.
Counts include original ISRs and shared formatters, not independent source bodies.
Substituted original spans total 1108 bytes (10.23% of 10836 symbolic code bytes),
including retained ASM and ABI scaffolding. Broader functional recovery corresponds
to 3162 original span bytes (29.18%); neither is pure emitted-C byte coverage.

All 806 original text symbols retain their addresses. Bridges occupy 1600 bytes
at 0x3000; C/helpers occupy 2004 at 0x4000. All 4634 differing positions in the
mixed image are permitted replacement/addition regions. Original vectors, data,
startup, remaining instructions and boot are unchanged. No new static SRAM,
initialized data, BSS or CRT is introduced. Exact ASM is retained independently.

Whole-firmware stack headroom and physical/asynchronous interrupt validation
remain pending; no device programming occurs. The next candidates can use the
same adapter approach while keeping timing-sensitive serializers in original ASM.

## Final clean-build validation

`make mixed-check` passed from a clean build: 2730848 actual compiled-AVR
comparisons, including all three new suites and the existing scalers, DAC,
status, CRC, FLASH, GPIO and FPGA settings regressions. Golden-only settings
audits (2048) and synthetic opcode fixtures (16392) are recorded separately and
are not counted as C integration. The portable C library was unchanged; its
previous 390601-case validation remains historical.

Reference SHA256 verification and exact-check passed; golden FLASH cmp reports
zero differing bytes. A second clean build reproduced identical ELF, HEX and
BIN hashes. The tested mixed BIN SHA256 is
`2383f98d1a2fd5432290ccf50aeb2ca125e92ecf5e9e919bd4bd4107b6f7beb2`.
Full logs, suite JSON reports, layout/byte/instruction diffs and reproducibility
proof are retained as mixed_recovery_system_*.

In the separate mixed image, anchored JMP replacements are class E, vacated
original routine padding is class G, and bridges/C in previously erased space
are class F. All 4634 changed positions are permitted by check_layout.py.
The independent exact assembly reconstruction has no unresolved differences.
