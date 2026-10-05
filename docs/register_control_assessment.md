# Register control investigation, AVR GCC 7.3.0

This investigation revisits previous allocation failures using actual compiler
support probes, fixed-register operands and translation-unit register objects.
Twelve of sixteen complete-firmware candidates passed, across nine translation
units: **46 additional executable bytes now come from C**. Four failed candidates
were restored. No functional/emulator result substitutes for binary equality.

## Which controls actually work

The installed AVR backend rejects `#pragma GCC target("fixed-r18")` and
`#pragma GCC target("short-calls")`. Both `#pragma GCC optimize("fixed-r18")`
and `__attribute__((optimize("fixed-r18")))` reject the register option.
`#pragma GCC optimize("O1")` works, with push/pop scope, but selects optimization
passes rather than a register assignment. O1 did not fix the tested UART or
parser allocation failures. Unsupported pragmas are absent from accepted source.

Documentation for the actual compiler version:
[function-specific pragmas](https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/Function-Specific-Option-Pragmas.html),
[local register variables](https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/Local-Register-Variables.html),
[global register variables](https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/Global-Register-Variables.html).

A local `register uint8_t value asm("r16")` alone does not reserve R16 throughout
C expressions. Extended-ASM operand constraints enforce the register at those
boundaries. This exact baseline therefore uses explicit input/output capture
and zero-byte read/write barriers; byte equality verifies all intermediate code.

File-scope `register uint16_t cursor asm("r28")` reserves the whole pair in that
translation unit and excludes ordinary save/restore generation for that object.
It creates no SRAM object. This changed MOVW and word arithmetic generation where
local bindings had failed. Allocator/call-saved settings remain in per-file
Makefile profiles: they cannot be passed as these optimize pragmas. Profiles apply
only to the original private entries, not to a general GNU argument/result API.

Reproduce isolated support/encoding probes:

```sh
python3 recovered_exact/tools/register_control_probes.py \
    --output /tmp/pm_register_control_probes.json
```

`register_control_probes.json` records source, flags, diagnostics and assembly.
Successful isolated probes do not themselves accept any firmware conversion.
Every accepted integration passed a clean root `make exact-check` independently.

## Accepted fragments

| Fragment | Control | Additional C instruction bytes |
|---|---|---:|
| FPGA programming starting-address copy | Global R28:R29 and R20:R21 objects; original MOVW | 2 |
| Two DAC channel sign-bit conversions | R17 destination; read/write R18 mask prevents folding | 4 |
| DMA address copy | Global R28:R29 and R20:R21; original MOVW | 2 |
| Two DMA ring wraps | Global Y word; original SBIW Y,8 | 4 |
| DMA buffer cursor advance | Global X word; original ADIW X,1 | 2 |
| FPGA skipped-channel advance | Global Y word; original ADIW Y,2 | 2 |
| EEPROM cursor advance | Global Y pointer; original ADIW Y,1 | 2 |
| CRC address low-word advance | Global Y word; original ADIW Y,1; upper ADC stays ASM | 2 |
| Decimal parser result-byte negations | Bound R20/R21 byte values; original NEG, exact ADC retained | 4 |
| Decimal formatter sign/magnitude negations | Bound R15/R16/R17; original NEG, branches/frame retained | 6 |
| Four TDC signed-byte extensions | Bound R16/R17, opaque read/write R16; original SBRC/COM | 16 |
| **Total** | | **46** |

Per-entry addresses and deltas are in `register_control_accepted_bytes.json`.
Every candidate/log is in `exact_continuation_results_371_386.json`.

The important TDC difference was not a different algorithm or instruction width.
Trial382 differed at byte **0x1936** only:

```text
original: 07 fd    sbrc r16,7
rebuilt:  47 fd    sbrc r20,7
```

R16 held a copy of the saved R20 byte, and GCC tested the original value instead.
A read-only R16 input did not prevent that substitution. Making the original CLR
helper also a read/write boundary for R16 forces the subsequent C test to consume
R16. Trial383 then reproduced all four SBRC/COM pairs, with zero full-image
changes. The boundary emits no added instruction. Evidence is retained in
`exact_c_steps/step_382_instruction_diff.txt`.

## What register controls did not resolve

- UART TX postincrement reads: even with Z allocatable, the O1 variant tried to
  use a temporary R18 pair as an address and failed reload allocation. It remains
  ASM with its explanatory C comment. The earlier TX/RX/CTS local-profile trials
  are also preserved, not silently replaced by a behavior-only implementation.
- Decimal parser signed digit bounds: separated C conditions with an opaque R16
  barrier still required allocator temporaries; O1 did not remove the failure.
  The actual reload diagnostics include a comparison against 47, whereas the
  original threshold instruction uses 48. Retained comparisons/branches remain
  the exact reference encoding; no successful binary match is claimed for them.
- A global pointer postincrement probe compiles but emits extra MOVW/ADIW/address
  temporaries. Strong reservation alone does not guarantee original LD Z+.
- Register selection cannot alone replace short RCALL with wide CALL, original
  CLR/EOR zeroing with LDI, GNU versus private frames, or live carry/T chains.
  These require keeping exact instructions or a separately verified local
  transformation. No whole-function layout relaxation was introduced.

## Further useful candidates

The strongest demonstrated candidates are byte NEG/COM/EOR with explicit opaque
operands, and fixed word copies/arithmetic using whole-pair reservation. For
remaining LD/ST helpers, test the complete addressing sequence and output flags,
not just register names. For comparisons, inspect whether the difference is a
register substitution, threshold canonicalization, branch direction, or layout;
only the first category was fixed by the opaque R16 operand in this investigation.
Do not apply global reservations or optimize pragmas indiscriminately to every
function: they may conflict with existing byte views, calls and address reloads.

## Acceptance checkpoint

C: **6236/10836 executable bytes (57.5489%)**, application **62.0004%**.
ASM: **4600 bytes (42.4511%)**, including **3792 inline helper bytes**.
89 application entries: C_BINARY_EXACT **5**, C_WITH_EXACT_ASM_HELPER **83**,
ASM_EXACT **1**. Boot: **9 ASM procedures / 778 executable bytes**.
All **806** original text symbols retain their addresses. Data/padding excluded.
Two final clean builds reproduce identical ELF, HEX and BIN artifacts.
Canonical FLASH: **139264 bytes; differing bytes = 0**.
Golden and rebuilt SHA256:

```text
e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0
```

`exact_asm`, `reference`, `dump` and original `asm_analysis` are unchanged.
