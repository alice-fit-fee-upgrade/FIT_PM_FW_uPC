# Exact C/ASM checkpoint

The accepted baseline is recovered_exact. Root make and make exact-check now build
and accept only the complete original FLASH image. exact_asm, reference files and
upstream analysis remain unchanged. Historical behavioral tests are retained but
are no longer acceptance evidence for C integration.

## Accepted functions

| Byte address | Original entry | Classification | Compiler bytes | ASM helper bytes |
|---|---|---|---|---|
| 0x01E2 | DMA_CH1_vect_isr | C_WITH_EXACT_ASM_HELPER | 360 | 290 |
| 0x046C | FUN_code_000236 | C_WITH_EXACT_ASM_HELPER | 28 | 22 |
| 0x049E | TCC0_OVF_vect_isr | C_WITH_EXACT_ASM_HELPER | 632 | 462 |
| 0x08E4 | fpga_settings_init | C_WITH_EXACT_ASM_HELPER | 122 | 44 |
| 0x098A | fpga_settings_reset | C_WITH_EXACT_ASM_HELPER | 70 | 14 |
| 0x09DE | FUN_code_0004ef | C_WITH_EXACT_ASM_HELPER | 32 | 78 |
| 0x0A4C | PORTD_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER | 54 | 26 |
| 0x0A9C | FUN_code_00054e | C_WITH_EXACT_ASM_HELPER | 22 | 10 |
| 0x0ABC | PORTB_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER | 18 | 18 |
| 0x0AE0 | PORTF_INT1_vect_isr | C_WITH_EXACT_ASM_HELPER | 88 | 48 |
| 0x0B68 | FUN_code_0005b4 | C_WITH_EXACT_ASM_HELPER | 40 | 6 |
| 0x0B96 | PORTE_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER | 92 | 56 |
| 0x0C2A | PORTE_INT1_vect_isr | C_WITH_EXACT_ASM_HELPER | 58 | 26 |
| 0x0C7E | set_status_and_vd8_led | C_BINARY_EXACT | 24 | 0 |
| 0x0C96 | system_deinit | C_WITH_EXACT_ASM_HELPER | 108 | 14 |
| 0x0D10 | system_init | C_WITH_EXACT_ASM_HELPER | 168 | 36 |
| 0x0DDC | CDCE62005_control_rst | C_WITH_EXACT_ASM_HELPER | 56 | 12 |
| 0x0E20 | PORTF_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER | 46 | 58 |
| 0x0E88 | USARTF0_DRE_vect_isr | C_WITH_EXACT_ASM_HELPER | 32 | 56 |
| 0x0EE0 | USARTF0_RXC_vect_isr | C_WITH_EXACT_ASM_HELPER | 60 | 56 |
| 0x0F54 | main | C_WITH_EXACT_ASM_HELPER | 418 | 102 |
| 0x115C | fpga_data_exchange | C_WITH_EXACT_ASM_HELPER | 240 | 158 |
| 0x12EA | cli_prompt_parse | C_WITH_EXACT_ASM_HELPER | 344 | 312 |
| 0x157A | unlock_programming | C_WITH_EXACT_ASM_HELPER | 20 | 24 |
| 0x15A6 | fpga_firmware_update | C_WITH_EXACT_ASM_HELPER | 110 | 80 |
| 0x1664 | FUN_code_000b32 | C_BINARY_EXACT | 26 | 0 |
| 0x167E | FUN_code_000b3f | C_WITH_EXACT_ASM_HELPER | 46 | 6 |
| 0x16B2 | FUN_code_000b59 | C_WITH_EXACT_ASM_HELPER | 70 | 24 |
| 0x1710 | FUN_code_000b88 | C_WITH_EXACT_ASM_HELPER | 10 | 4 |
| 0x171E | FUN_code_000b8f | C_BINARY_EXACT | 28 | 0 |
| 0x173A | FUN_code_000b9d | C_WITH_EXACT_ASM_HELPER | 52 | 4 |
| 0x1772 | FUN_code_000bb9 | C_WITH_EXACT_ASM_HELPER | 94 | 20 |
| 0x17E4 | FUN_code_000bf2 | C_WITH_EXACT_ASM_HELPER | 14 | 22 |
| 0x1808 | FUN_code_000c04 | C_BINARY_EXACT | 56 | 0 |
| 0x1840 | FUN_code_000c20 | C_WITH_EXACT_ASM_HELPER | 76 | 2 |
| 0x188E | cli_send_ch_mean_amplitude | C_WITH_EXACT_ASM_HELPER | 16 | 32 |
| 0x18BE | cli_send_adc_baseline_dispersion | C_WITH_EXACT_ASM_HELPER | 32 | 50 |
| 0x1910 | cli_send_tdc_data | C_WITH_EXACT_ASM_HELPER | 38 | 120 |
| 0x19AE | eeprom_settings_save | C_WITH_EXACT_ASM_HELPER | 48 | 48 |
| 0x1A0E | FUN_code_000d07 | C_WITH_EXACT_ASM_HELPER | 36 | 12 |
| 0x1A3E | cdce62005_rst | C_WITH_EXACT_ASM_HELPER | 16 | 22 |
| 0x1A64 | cli_send_system_status | C_WITH_EXACT_ASM_HELPER | 262 | 122 |
| 0x1BE4 | alarms_clear | C_WITH_EXACT_ASM_HELPER | 108 | 18 |
| 0x1C62 | channels_read | C_WITH_EXACT_ASM_HELPER | 58 | 86 |
| 0x1CF2 | cli_send_channel_cdf_adc | C_WITH_EXACT_ASM_HELPER | 48 | 64 |
| 0x1D62 | fpga_set_trg_charge_lvls | C_WITH_EXACT_ASM_HELPER | 16 | 32 |
| 0x1D92 | fpga_set_trg_settings | C_WITH_EXACT_ASM_HELPER | 12 | 24 |
| 0x1DB6 | fpga_set_adc_range_corr | C_WITH_EXACT_ASM_HELPER | 34 | 86 |
| 0x1E2E | fpga_set_tdc_values | C_WITH_EXACT_ASM_HELPER | 20 | 58 |
| 0x1E7C | fpga_set_threshold_calibration | C_WITH_EXACT_ASM_HELPER | 32 | 68 |
| 0x1EE0 | FUN_code_000f70 | C_WITH_EXACT_ASM_HELPER | 26 | 64 |
| 0x1F3A | fpga_set_ch_adc_delay | C_WITH_EXACT_ASM_HELPER | 26 | 58 |
| 0x1F8E | fpga_set_ch_cfd_threshold | C_WITH_EXACT_ASM_HELPER | 34 | 60 |
| 0x1FEC | fpga_set_adc_zero | C_WITH_EXACT_ASM_HELPER | 34 | 64 |
| 0x204E | fpga_set_ch_cfd_zero | C_WITH_EXACT_ASM_HELPER | 30 | 58 |
| 0x20A6 | FUN_code_001053 | C_WITH_EXACT_ASM_HELPER | 28 | 14 |
| 0x20D0 | FUN_code_001068 | C_WITH_EXACT_ASM_HELPER | 24 | 4 |
| 0x20EC | dac_set_value_2 | C_WITH_EXACT_ASM_HELPER | 18 | 6 |
| 0x2104 | dac_set_value | C_WITH_EXACT_ASM_HELPER | 18 | 6 |
| 0x211C | FUN_code_00108e | C_WITH_EXACT_ASM_HELPER | 10 | 10 |
| 0x2130 | fpga_is_ready | C_WITH_EXACT_ASM_HELPER | 10 | 18 |
| 0x214C | FUN_code_0010a6 | C_WITH_EXACT_ASM_HELPER | 14 | 26 |
| 0x2174 | ths788_write | C_WITH_EXACT_ASM_HELPER | 118 | 30 |
| 0x2208 | ths788_read | C_WITH_EXACT_ASM_HELPER | 138 | 24 |
| 0x22AA | dac_send_value | C_WITH_EXACT_ASM_HELPER | 84 | 16 |
| 0x230E | fpga_msg_send_t2 | C_WITH_EXACT_ASM_HELPER | 84 | 6 |
| 0x2368 | fpga_msg_read_t1 | C_WITH_EXACT_ASM_HELPER | 94 | 4 |
| 0x23CA | FUN_code_0011e5 | C_WITH_EXACT_ASM_HELPER | 186 | 2 |
| 0x2486 | CDCE62005_send_control_settings | C_BINARY_EXACT | 72 | 0 |
| 0x24CE | FUN_code_001267 | C_WITH_EXACT_ASM_HELPER | 88 | 10 |
| 0x2530 | fpga_send_mcu_ts | C_WITH_EXACT_ASM_HELPER | 96 | 8 |
| 0x2598 | adt7311_8bit_rw | C_WITH_EXACT_ASM_HELPER | 34 | 4 |
| 0x25BE | adt7311_16bit_rw | C_WITH_EXACT_ASM_HELPER | 38 | 6 |
| 0x25EA | adt7311_faults_clr | C_WITH_EXACT_ASM_HELPER | 22 | 8 |
| 0x2608 | adt7311_byte_rw | C_WITH_EXACT_ASM_HELPER | 28 | 16 |
| 0x2634 | cli_get_integer | C_WITH_EXACT_ASM_HELPER | 10 | 110 |
| 0x26AC | cli_get_hex | C_WITH_EXACT_ASM_HELPER | 28 | 48 |
| 0x26F8 | cli_send_32bit_hex | C_WITH_EXACT_ASM_HELPER | 32 | 8 |
| 0x2720 | cli_send_digit_hex | C_WITH_EXACT_ASM_HELPER | 8 | 6 |
| 0x272E | FUN_code_001397 | C_WITH_EXACT_ASM_HELPER | 2 | 6 |
| 0x2736 | cli_send_temperature | C_WITH_EXACT_ASM_HELPER | 2 | 6 |
| 0x273E | FUN_code_00139f | C_WITH_EXACT_ASM_HELPER | 2 | 6 |
| 0x274E | cli_send_uint16 | C_WITH_EXACT_ASM_HELPER | 40 | 168 |
| 0x281E | cli_send_crlf | C_WITH_EXACT_ASM_HELPER | 6 | 2 |
| 0x2826 | cli_send_msg | C_WITH_EXACT_ASM_HELPER | 8 | 8 |
| 0x2836 | cli_get_next_byte | C_WITH_EXACT_ASM_HELPER | 2 | 4 |
| 0x283C | cli_get_next_char | C_WITH_EXACT_ASM_HELPER | 52 | 60 |
| 0x28AC | cli_send_buf | C_WITH_EXACT_ASM_HELPER | 50 | 56 |

Current C coverage is 6086/10836 executable bytes (56.1646%), or 60.5090% of the application. Inline ASM is counted separately. Complete FLASH differs in zero bytes.

## Accepted implementation choices

R16-only leaf profiles let ordinary volatile C stores generate the original LDI/STS
without a new register-save sequence. LED control uses ordinary C conditionals and
bit operations; GCC generates the exact original SBIC/RJMP/CBI ordering. No inline
ASM is needed in these two C_BINARY_EXACT functions.

Compiler-sensitive helper fragments retain relative-call width, absolute addressing,
CLR/LDI flag distinctions, a repeated literal reload, GPIO atomic bit clearing and
original DMA/PLL sequencing. For sector erase, an initial trial differed at 0x1716
and 0x1718 because flag order made R17 allocatable. Putting its call-used setting
before the fixed-register profile restored R16 and zero differences. No behavioral
test was run to justify that differing trial.

System initialization initially could not compile with all pointer registers reserved:
GCC hoisted the repeated DMA_CTRL address into a pointer. A single exact absolute
STS helper for the second write removed that compiler-sensitive fragment. The rest
of the ordered setup is C, including all unchanged configuration constants.

ADT7311 transaction wrappers preserve the original register ABI through constrained
inline operands and per-file call-saved settings. GCC itself emits the exact original
prologues, epilogues and data moves. The byte-level bit-banged serializer at 0x2608
now uses C GPIO stores with exact bit, delay and loop instruction helpers. Eight-bit transactions still discard RX; 16-bit results still
appear in R20/R21; fault clearing still sends four FF bytes.

## SPI continuation

Two more entries were accepted, each after a complete exact-check:
0x1808 sends a command and three FLASH address bytes in R16/R22/R21/R20 order;
0x2486 writes the PLL word in R16/R17/R18/R19 order. A shared twelve-byte
SPI primitive preserves each original STS/LDS/SBRS/RJMP sequence. All 96 added
primitive bytes are counted as ASM; the two functions add only 32 compiler-generated
bytes, including the PLL's original R22 PUSH/POP. C now describes the wire sequence,
with the compiler-sensitive transfer mechanism in one reusable helper.

A clean FLASH-string sender trial at 0x2826 failed register allocation in the
constrained profile: GCC needed a general-register spill for its zero comparison.
That trial was rejected; the later exact terminator-branch helper now permits a
C/ASM implementation with the original 16 bytes. The byte-mode console entry at 0x2836 now has C mode selection with exact PUSH
and shared-tail jump helpers. Its partial stack contract remains unchanged.
No behavioral test was used as binary acceptance evidence.

## Retained candidates

A direct clean C trial of HEX digit formatting under the R16 profile produced
34 bytes instead of the original 14: GCC combined the two ASCII additions and
introduced a frame/spill to preserve the nibble. It was not admitted. A later four-byte exact threshold branch and zero-byte
register boundary now permit C arithmetic with the original 14 bytes. No behavioral
test was used to accept the differing trial. Later conversions admitted exact C/ASM scaling, GPIO/UART interrupts, parsers,
serializers and the shared decimal formatter. The six remaining ASM entries are
reviewed in easy_conversion_assessment.md. No exhaustive compiler-profile search
or emulator campaign was used to justify a differing image.

No altered protocol, timeout, peripheral configuration, startup order or bug fix was
introduced. The full-image match preserves original behavior without emulation as
a substitute for the byte comparison.

## Final evidence

Canonical FLASH is 139264 bytes (0x22000), independently normalized from original
PM.hex with FF holes. Golden and rebuilt SHA256 are both:
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.
**Differing bytes: 0; first/last mismatch: none; mismatch regions: 0.**

All **806** original text symbols retain their byte addresses. No new static SRAM,
CRT, data, runtime helper library or executable blob is added. Linker assertions
check every C and ASM region size and reject unexpected code/data.

Final exact_c_validation.log, exact_c_reproducibility.json, exact_c_comparison.txt,
exact_c_instruction_diff.txt and function_inventory.json retain the build and
comparison evidence. Per-conversion full build logs are in docs/exact_c_steps/.
No exhaustive emulator suite was continued after the strategy change, and no device
was programmed.

## Continued recovery with small exact helpers

The previous wave accepted nineteen additional entries (steps 15–33). Every
complete-image comparison reported zero differences. `exact_c_steps/23_24_dac_channels.log`
validates both entries compiled from the shared DAC source.

The earlier FPGA write trial differed at 0x2314 because GCC borrowed R18, the
incoming address register, for peripheral constants. Three four-byte exact R22
stores solved this while GCC retained the original PUSH/POP and LDI instructions.
FPGA read preserves the caller's initial R21 bits; it does not insert a clear.
The eight-byte FPGA response retains its original low register result pairs.
Timestamp transfer tests ZL alone and emits exactly the original two words.
PLL read retains the 0x8e request and the original zero dummy byte.

HEX digit C arithmetic is separated at the legacy register boundary; an exact
CPI/BRCS helper avoids the compiler's extra spill and preserves the original
threshold. Four-nibble formatting keeps SWAP/RCALL helpers with C register moves
and compiler-generated saves. The historical `cli_send_32bit_hex` symbol actually
emits four nibbles of R17:R16; its name is retained without changing behavior.
DAC selectors use two byte additions with register boundaries instead of promoting
a shift to a 16-bit multiply. The multiply and send RCALLs retain their original
contracts. The historical `fpga_is_ready` helper performs arithmetic; its misleading
name is not treated as ground truth.

The GPIO/DMA handshake reads PORTD_IN (0x0668), not PORTD_INTFLAGS, and writes
DMA_CH0_CTRLB (0x0111), not DMA_CTRL. ADT7311 clock writes and its counter setup
are C; SBRC/SBRS, shift/NOP, BST/BLD and DEC/BRNE retain the original sampling
and flags. The default PLL loader writes PORTF_INTCTRL (0x06a9), not OUTTGL.
All values and peripheral addresses are unchanged and validated by the bytes.

PORTB and PORTD interrupts retain short exact save/restore frames including SREG
and RETI, with C RAM/GPIO updates. PORTD initially compiled the final restore block
before its second path: the first mismatch was 0xa66. `-fno-reorder-blocks` restores
the original layout. These functions use a private vector-entry ABI and no GNU
interrupt prologue. The FLASH-message sender now uses an exact TST/BREQ terminator
helper; GCC emits its original PUSH/POP/RET and the loop back branch.

There is no new executable blob, opcode patch, data initialization, clock adjustment,
protocol change or bug fix. Two final clean ELF, HEX and BIN builds are identical;
the canonical FLASH also matches a fresh normalization of original PM.hex.

FPGA settings initialization and reset now express both SRAM banks and final state
words in C. Their LD Y+, INC/CPI/BRNE and CALL/CLI/SEI helpers retain the original
register and flag contracts. Reset uses the compiler-generated original save frame;
initialization keeps its asymmetric original frame, including the unused R30 save
and the absence of a YL save. Reserving Y registers (rather than declaring R28
call-used, which AVR GCC rejects) permits that frame without added GNU saves.

## Function-by-function continuation: steps 34–83

Fifty additional application entries were accepted. Every retained source passes
whole-image exact-check; the step logs record full clean compilation and canonical
comparison. The mean-amplitude and baseline display helpers first passed together;
step 62 also has a final independent complete-image check. Historical log byte
counts are snapshots; final totals include audited helper-range corrections.

The added C covers THS788 host transactions, DAC SPI/scaling, GPIO/UART interrupts,
console queues, EEPROM page/storage commands, PLL/trigger/channel commands, status
and channel displays, FLASH page programming/read/CRC, main startup/dispatch,
FPGA programming-stream reception, decimal/HEX parsers and decimal formatting.

Main preserves low-I/O STS width, CCP timing, stack reset, literal LDI-zero values,
RAM clearing and memory-mapped EEPROM copy. A section attribute prevents GCC from
placing main into .text.startup.main. The rest of its peripheral constants and
ordered stores are ordinary C. No oscillator, PLL, timer or GPIO value changed.

EEPROM retains the original CCP STS instruction rather than the shorter OUT chosen
by ordinary GCC MMIO. Shared error tails use symbolic .subsection 1 regions so their
addresses remain intact after C bodies. UART send disables cross-jumping because
GCC otherwise merges the original distinct SEI paths. Receive/scaling helpers
preserve exact scratch registers and carry-sensitive widths and increments.

FLASH and CRC helpers retain the live R2:R0 24-bit state, inclusive-end comparison,
bit order and original R1 contents. Separate C byte initializers preserve the four
CRC LDI instructions instead of GCC's shorter MOVW. The signed multiply core stays
in ASM; the unsigned saturating product combines exact MUL/flags with a C result.

Decimal/HEX parser and formatter helpers preserve their original delimiter, sign,
rounding, division and carry-result logic. Short shared-entry C wrappers keep exact
PUSH/SEC/RJMP helpers. Pure ASM wrappers with no compiler-generated C instructions
are not counted as C conversions.

Both final clean ELF/HEX/BIN builds are identical. Fresh normalization of original
PM.hex equals rebuilt FLASH. All 806 original symbols retain their byte addresses.
Every inline-ASM range also matches independent GCC assembly provenance; C loop
jumps are counted as C, while predecrement stores and all inline instructions are
counted as ASM. The immutable reference project and images are unchanged.

## Further exact continuation: steps 84–92

Five more entries now use compiler-generated C plus exact helpers: signed scaler
0x2130 (6 C bytes), FPGA exchange 0x115c (168 C bytes), console dispatch 0x12ea
(208 C bytes), DMA ISR 0x01e2 (300 C bytes) and timer ISR 0x049e (436 C bytes).
The total increase is 1118 C instruction bytes. The complete image passed
exact-check after each accepted entry and each subsequent helper reduction.

The dispatcher and ISR branches retain zero-byte local ASM label anchors beside
C labels. They prevent GCC merging original jump-only blocks or redirecting
short flag branches to shared distant tails. FPGA exchange retains the original
redundant jump to its next instruction as a two-byte ASM helper. All emitted
instructions, including these branches, are accounted for by GCC provenance.

Signed multiplication keeps live R1, staged rounding and carry propagation in
ASM; the result copy and final high-byte addition are C. Its comment includes
the historical tested, binary-different C alternative and bridge/test scope.
For the newly recovered state machines no independent functional validation of
rejected plain-C alternatives is claimed. Full byte identity is the acceptance
evidence. No exhaustive emulator suite was run.

Final application counts: 89 entries, 2 C_BINARY_EXACT,
86 C_WITH_EXACT_ASM_HELPER, 1 ASM_EXACT. Boot remains 9 ASM procedures.
C is 4182/10836 executable bytes (38.5936%); ASM is 6654 bytes (61.4064%),
including 5846 inline-helper bytes. Application-only C is 41.5788%.
Canonical golden/rebuilt SHA256 remains
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.
Differing bytes: 0. The immutable exact ASM implementation and inputs are unchanged.

## Further exact continuation: steps 93–190

98 candidates were compiled and checked; 67 were accepted across 41 translation
units, and 31 were rejected/restored. See `exact_continuation_results.json` and
per-step logs for completed clean whole-image checks. No emulator suite was run.
The application entry count stays 89; the work removes ASM fragments from entries
already represented in C, rather than increasing a wrapper count.

SPI data stores, status polling and readbacks now use C in the accepted callers.
For the PLL writer, releasing a dead final status operand avoids needing a second
scratch register and reproduces the original sole-R22 save frame. The original
writer, FLASH address transaction and FLASH write-enable entry are now entirely
compiler-generated C, bringing C_BINARY_EXACT from 2 to 5.

Selected absolute SRAM reads, immediate/register comparisons, bit tests and FPGA
address shifts also moved to C. Register-starved loads, pointer increments,
predecrement stores, delay-loop changes, alternate skip encodings and reordered
bit copies were rejected. Existing exact helpers remain for those cases; no new
functional-validation success is claimed for a rejected candidate.

GNU AVR built-ins generate original CLI/SEI/NOP/SWAP instructions, and a named
bit-copy helper generates the original ADT BST/BLD pair. IRQ compiler barriers
emit no bytes but preserve memory ordering. These are compiler-generated target
operations, explicitly separated as a subset of C coverage: 184 bytes of 5190.
The remaining 5006 C bytes are other compiler-generated operations. Primitive
counts agree between the golden instruction index and GCC APP/NOAPP provenance.
The built-in mapping is documented by GNU GCC:
https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/AVR-Built-in-Functions.html

Final classifications: 5 C_BINARY_EXACT, 83 C_WITH_EXACT_ASM_HELPER,
1 ASM_EXACT; boot remains 9 ASM procedures. C is 5190/10836 bytes (47.8959%),
ASM is 5646 bytes (52.1041%), including 4838 inline-helper bytes.
Application-only C is 51.6007%. Both clean final builds and fresh normalization
of original PM.hex produce canonical SHA256
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.
Differing bytes: 0. Exact ASM, hardware state and reference artifacts are unchanged.

## Further exact continuation: steps 191–208

18 accepted fragment changes across 13 translation units replaced 96 ASM bytes
with compiler-generated C; all 18 independently passed clean `make exact-check`.
No candidate was accepted by behavioral equivalence, and no emulator runs were needed.
The manifest and GCC APP/NOAPP provenance audit agree on every helper range.

C now handles ADT7311 transfer-byte copies and chip-select stores, channel offset
scaling, two device-index shifts, byte additions/subtractions in DMA and timer
interrupts, and IRQ primitives around original FPGA/TDC/PLL calls. CALL/RCALL
widths and protected register operations remain in exact ASM. Existing annotated
historical C alternatives remain beside the retained helpers.

Application entries: 89; C_BINARY_EXACT 5, C_WITH_EXACT_ASM_HELPER 83, ASM_EXACT 1.
Boot: 9 ASM procedures. Compiler-generated C: 5286/10836 bytes (48.7818%);
ASM: 5550 bytes (51.2182%), including 4742 inline helpers. Application-only C:
52.5552%. Of C bytes, 228 are GNU AVR primitives; 5058 are other C instructions.
Golden and rebuilt SHA256: e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0.
Differing bytes: 0. Two final clean ELF/HEX/BIN builds are identical.
Machine-readable changes and per-step logs: exact_latest_continuation_results.json.

## Further exact continuation: steps 209–233

21 of 25 candidates were accepted, replacing 100 ASM bytes with C. Every accepted
candidate independently passed a clean whole-image exact-check. The four rejected
candidates (218, 220, 228, 229) were restored: explicit TDC carry capture enlarged
its fixed region; three status-bit conditions changed layout/encoding. No emulator
runs or behavioral equivalence were used to accept these changes.

New C expresses status bits, the TDC low-byte shift, a CRC low-byte shift and
IRQ controls around exact queue/calibration loads, protected EEPROM writes and
FPGA read/print/command paths. Remaining ADC, DEC, pointer and private-call helpers
now include nearby explanatory C equivalents. Comments explicitly distinguish
historical functionally tested alternatives from unvalidated value descriptions;
they do not claim equivalence of the complete CPU flags, frame or hardware timing.

Application: 89 entries; C_BINARY_EXACT 5, C_WITH_EXACT_ASM_HELPER 83, ASM_EXACT 1.
Boot: 9 ASM procedures. C: 5386/10836 executable bytes (49.7047%); ASM: 5450
(50.2953%), including 4642 inline helpers. Application-only C: 53.5494%.
GNU AVR primitive subset: 290 bytes; other C instructions: 5096 bytes.
Golden/rebuilt canonical SHA256:
e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0.
Differing bytes: 0. Two final clean ELF/HEX/BIN builds are identical.
See exact_latest_continuation_results.json for individual logs.

## Further exact continuation: steps 234–317

84 candidates: 54 accepted across 29 translation units; 30 rejected/restored.
C replaces another 700 ASM instruction bytes. Each retained candidate passes
clean whole-image exact-check and independent GCC APP/NOAPP provenance. No
behavioral emulator acceptance or new exhaustive emulator runs were used.

New C covers wide private entry calls in dispatcher/ISRs/settings/programming,
16/24-bit counters, register-pair copies, an upper 32-bit bitmap shift, SPI-ready
polling, TDC GPIO bit writes and offset math, startup initializations, partial
DAC product-byte additions, IRQ builtins and return instructions. CALL width,
all 806 original symbols, fixed-region sizes and the complete FLASH are exact.

Private void declarations are adapters for original register entries, not public
GNU argument/result interfaces. Bound-register input barriers and output recapture
are part of the contract. RCALLs remain ASM: the actual GCC7.3 ATxmega128A3U probe
still generated CALL with -mshort-calls. No general linker relaxation or whole-image
blob is used. Whole uint64_t bitmap shifts generated __lshrdi3; only the native
upper-word shift was accepted. Remaining RORs propagate exact live carry.

Rejected candidates included parser branch/layout changes, __flash pointer
allocation, unsupported negative fixed-register options, private pair allocation
and carry arithmetic. The later corrected profile probes were also checked and
restored where they changed bytes. Original small ASM helpers remain with commented
C equivalents and explicit validation scope. Tested historical C alternatives
retain their original evidence; explanatory new alternatives claim no new tests.

Step309 initially failed because the filesystem was full. Removing old task
scratch objects and the reproducible pip download cache restored space. The
unchanged candidate then passed a full clean exact-check; this infrastructure
failure is not a rejected firmware conversion and did not change reference files.

Application: 89 entries; C_BINARY_EXACT 5, C_WITH_EXACT_ASM_HELPER 83, ASM_EXACT 1.
Boot: 9 ASM procedures. C: 6086/10836 executable bytes (56.1646%); ASM: 4750
(43.8354%), including 3942 inline helpers. Application-only C: 60.5090%.
GNU AVR primitive subset: 380 bytes; other C instructions: 5706 bytes.
Golden/rebuilt canonical SHA256:
e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0.
Differing bytes: 0. Two final clean ELF/HEX/BIN builds are identical.
See exact_latest_continuation_results.json for individual logs and reasons.

## Additional parser profile audit: steps 318–321

Four local parser candidates also disabled peephole2, tree-VRP and forward
propagation to test whether clean C branches could retain the original encoding.
All four failed fixed-region sizing or register allocation and were restored.
No source/compiler profile changes were retained. Existing C alternatives remain
in comments. A final clean exact-check revalidated the restored baseline.

remaining_inline_asm.json/.md now inventory all 3942 inline helper bytes against
complete original instructions, with per-function totals and opcode examples.
The tool is read-only with respect to firmware sources and reference files.
