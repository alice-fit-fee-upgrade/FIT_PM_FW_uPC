# Exact C/ASM checkpoint

The accepted baseline is recovered_exact. Root make and make exact-check now build
and accept only the complete original FLASH image. exact_asm, reference files and
upstream analysis remain unchanged. Historical behavioral tests are retained but
are no longer acceptance evidence for C integration.

## Accepted functions

| Byte address | Original entry | Classification | Compiler bytes | ASM helper bytes |
|---|---|---|---|---|
| 0x0B68 | FUN_code_0005b4 | C_WITH_EXACT_ASM_HELPER | 40 | 6 |
| 0x0C7E | set_status_and_vd8_led | C_BINARY_EXACT | 24 | 0 |
| 0x0C96 | system_deinit | C_WITH_EXACT_ASM_HELPER | 108 | 14 |
| 0x0D10 | system_init | C_WITH_EXACT_ASM_HELPER | 168 | 36 |
| 0x1664 | FUN_code_000b32 | C_BINARY_EXACT | 26 | 0 |
| 0x167E | FUN_code_000b3f | C_WITH_EXACT_ASM_HELPER | 42 | 10 |
| 0x1710 | FUN_code_000b88 | C_WITH_EXACT_ASM_HELPER | 10 | 4 |
| 0x171E | FUN_code_000b8f | C_WITH_EXACT_ASM_HELPER | 20 | 8 |
| 0x2598 | adt7311_8bit_rw | C_WITH_EXACT_ASM_HELPER | 24 | 14 |
| 0x25BE | adt7311_16bit_rw | C_WITH_EXACT_ASM_HELPER | 26 | 18 |
| 0x25EA | adt7311_faults_clr | C_WITH_EXACT_ASM_HELPER | 22 | 8 |
| 0x281E | cli_send_crlf | C_WITH_EXACT_ASM_HELPER | 6 | 2 |

Every accepted replacement was followed by a complete clean build and exact-check.
Final totals are **89 application entries: 2 C_BINARY_EXACT, 10
C_WITH_EXACT_ASM_HELPER, 77 ASM_EXACT**, plus **9 boot ASM procedures**.
Compiler-generated instructions cover **516/10836 executable bytes (4.7619%)**;
ASM covers **10320/10836 (95.2381%)**, including **120 helper bytes**. Application-only
C coverage is **5.1302%**, excluding all data, padding and erased FLASH. The 636 bytes
hosted in C function regions are not counted as 636 pure C bytes.

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
remains unchanged ASM. Eight-bit transactions still discard RX; 16-bit results still
appear in R20/R21; fault clearing still sends four FF bytes.

## Retained candidates

A direct clean C trial of HEX digit formatting under the R16 profile produced
34 bytes instead of the original 14: GCC combined the two ASCII additions and
introduced a frame/spill to preserve the nibble. It was not admitted. The original
0x2720 function remains ASM_EXACT; no exhaustive behavior test was used to justify
its differing code. Complex scaling, interrupt, parser, serializer and formatter
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
