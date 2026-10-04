# Exact C/ASM checkpoint

The accepted baseline is recovered_exact. Root make and make exact-check now build
and accept only the complete original FLASH image. exact_asm, reference files and
upstream analysis remain unchanged. Historical behavioral tests are retained but
are no longer acceptance evidence for C integration.

## Accepted functions

| Byte address | Original entry | Classification | Compiler bytes | ASM helper bytes |
|---|---|---|---|---|
| 0x046C | FUN_code_000236 | C_WITH_EXACT_ASM_HELPER | 26 | 24 |
| 0x08E4 | fpga_settings_init | C_WITH_EXACT_ASM_HELPER | 50 | 116 |
| 0x098A | fpga_settings_reset | C_WITH_EXACT_ASM_HELPER | 38 | 46 |
| 0x0A4C | PORTD_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER | 40 | 40 |
| 0x0A9C | FUN_code_00054e | C_WITH_EXACT_ASM_HELPER | 22 | 10 |
| 0x0ABC | PORTB_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER | 18 | 18 |
| 0x0B68 | FUN_code_0005b4 | C_WITH_EXACT_ASM_HELPER | 40 | 6 |
| 0x0C7E | set_status_and_vd8_led | C_BINARY_EXACT | 24 | 0 |
| 0x0C96 | system_deinit | C_WITH_EXACT_ASM_HELPER | 108 | 14 |
| 0x0D10 | system_init | C_WITH_EXACT_ASM_HELPER | 168 | 36 |
| 0x0DDC | CDCE62005_control_rst | C_WITH_EXACT_ASM_HELPER | 48 | 20 |
| 0x1664 | FUN_code_000b32 | C_BINARY_EXACT | 26 | 0 |
| 0x167E | FUN_code_000b3f | C_WITH_EXACT_ASM_HELPER | 42 | 10 |
| 0x1710 | FUN_code_000b88 | C_WITH_EXACT_ASM_HELPER | 10 | 4 |
| 0x171E | FUN_code_000b8f | C_WITH_EXACT_ASM_HELPER | 20 | 8 |
| 0x1808 | FUN_code_000c04 | C_WITH_EXACT_ASM_HELPER | 8 | 48 |
| 0x1840 | FUN_code_000c20 | C_WITH_EXACT_ASM_HELPER | 16 | 62 |
| 0x20EC | dac_set_value_2 | C_WITH_EXACT_ASM_HELPER | 18 | 6 |
| 0x2104 | dac_set_value | C_WITH_EXACT_ASM_HELPER | 18 | 6 |
| 0x230E | fpga_msg_send_t2 | C_WITH_EXACT_ASM_HELPER | 20 | 70 |
| 0x2368 | fpga_msg_read_t1 | C_WITH_EXACT_ASM_HELPER | 20 | 78 |
| 0x23CA | FUN_code_0011e5 | C_WITH_EXACT_ASM_HELPER | 22 | 166 |
| 0x2486 | CDCE62005_send_control_settings | C_WITH_EXACT_ASM_HELPER | 24 | 48 |
| 0x24CE | FUN_code_001267 | C_WITH_EXACT_ASM_HELPER | 16 | 82 |
| 0x2530 | fpga_send_mcu_ts | C_WITH_EXACT_ASM_HELPER | 36 | 68 |
| 0x2598 | adt7311_8bit_rw | C_WITH_EXACT_ASM_HELPER | 24 | 14 |
| 0x25BE | adt7311_16bit_rw | C_WITH_EXACT_ASM_HELPER | 26 | 18 |
| 0x25EA | adt7311_faults_clr | C_WITH_EXACT_ASM_HELPER | 22 | 8 |
| 0x2608 | adt7311_byte_rw | C_WITH_EXACT_ASM_HELPER | 20 | 24 |
| 0x26F8 | cli_send_32bit_hex | C_WITH_EXACT_ASM_HELPER | 24 | 16 |
| 0x2720 | cli_send_digit_hex | C_WITH_EXACT_ASM_HELPER | 8 | 6 |
| 0x281E | cli_send_crlf | C_WITH_EXACT_ASM_HELPER | 6 | 2 |
| 0x2826 | cli_send_msg | C_WITH_EXACT_ASM_HELPER | 8 | 8 |

Every accepted replacement was compiled and checked against the complete FLASH.
The two DAC entry points share one translation unit and one complete-image check.
Final totals are **89 application entries: 2 C_BINARY_EXACT, 31
C_WITH_EXACT_ASM_HELPER, 56 ASM_EXACT**, plus **9 boot ASM procedures**.
Compiler-generated instructions cover **1016/10836 executable bytes (9.3762%)**;
ASM covers **9820/10836 (90.6238%)**, including **1082 helper bytes**. Application-only
C coverage is **10.1014%**, excluding all data, padding and erased FLASH. The 2098 bytes
hosted in C function regions include helpers and are not pure C coverage.

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
C/ASM implementation with the original 16 bytes. The byte-mode console entry at 0x2836 also remains ASM:
it enters a shared receiver with an existing partial stack frame, rather than
making a normal C call. No behavioral tests were used to justify either entry.

## Retained candidates

A direct clean C trial of HEX digit formatting under the R16 profile produced
34 bytes instead of the original 14: GCC combined the two ASCII additions and
introduced a frame/spill to preserve the nibble. It was not admitted. A later four-byte exact threshold branch and zero-byte
register boundary now permit C arithmetic with the original 14 bytes. No behavioral
test was used to accept the differing trial. Complex scaling, interrupt, parser, serializer and formatter
entries also remain their exact symbolic ASM implementations. This is an exact
source classification, not a claim that every remaining function has been exhausted
under all possible reasonable compiler profiles.

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

Nineteen additional entries were accepted in this wave (steps 15–33). Every
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
