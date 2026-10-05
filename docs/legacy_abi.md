# Private Carry-return ABI

Evidence: original FLASH instructions and all 22 RCALL/BRCS windows in
[asm_idioms.json](asm_idioms.json). Addresses below are FLASH **byte addresses**.
This is a local firmware convention, not the GNU AVR C calling convention.

| Entry | Call sites in this family | Result | Meaning of C=1 |
|---|---:|---|---|
| cli_get_integer, 0x2634 | 19 | R21:R20, following delimiter R16 | Parser rejected input |
| unlock_programming, 0x157a | 2 | Carry status | Password/leading delimiter rejected |
| flash_page_program / FUN_code_000b59, 0x16b2 | 1 | Updated stream/address state | Transfer limit reached; **not an error** |

Each caller tests Carry immediately after RCALL. Other SREG bits are not
promised preserved: arithmetic/comparisons in the callee update them before
CLC/SEC. Transitive callees can manipulate IRQ state; do not assume I preservation.
The occurrence inventory conservatively keeps registers/flags live across calls;
it does not infer standard GCC call-used/call-saved sets.

## Integer parser

The original frame saves R22, R17, R18, R19, R1 and R0 and restores them before
return. R20/R21 hold the parsed result and R16 its terminating character.
Explicit CLC denotes success and SEC denotes rejection. Rejection includes the
original count/sign-only checks, nonzero multiplication high byte, high-byte
addition overflow and the negative-conversion BRPL path. This is not a claim
that it implements a normal strtol range: preserve those actual checks, including
surprising boundary behavior. Its nested console call is not a GNU ABI call.

REFERENCE_C_INFERRED (value-only description):

```c
struct parse_result { uint16_t value; uint8_t delimiter; bool carry; };
/* Carry, not a materialized bool register, crosses the original call boundary. */
result = parse_private_console_input();
if (result.carry) goto original_parse_error;
```

## Password gate

The leading character must be CR (13); eight subsequent input bytes are compared
with FLASH at 0x2b76..0x2b7d using LPM. Success ends in CLC; the rejection tail
executes SEC then RET. Directly used registers include R16, R17 and Z. This list
is not a complete transitive clobber contract for its console callees.

REFERENCE_C_INFERRED: `if (!leading_cr || !password_matches) reject;`
The leading-CR comparison and error-tail RET now compile as exact C; SEC remains
ASM because Carry itself is the result. The original error labels still retain
all addresses.

## FLASH stream/page writer

The 24-bit current address is R22:R21:R20; the transfer limit is R2:R1:R0.
R1 must not be assumed zero here. CP/CPC/CPC compares these values; equality
returns with SEC. Otherwise the current address increments. The writer continues
while its low byte is nonzero; reaching the page boundary returns with CLC.
Buffer state includes R25:R24, queue cursors at RAM 0x2437/0x2438 and the buffer
at 0x2235. Directly used scratch registers include R16, R19 and X, but nested
calls prevent claiming this is a complete GNU clobber list.

REFERENCE_C_INFERRED:

```c
if (current_address == transfer_limit) return transfer_complete;
/* Other path sends bytes and advances the original address/circular buffer. */
return page_boundary_continue;
```

## Shared exact primitive

[legacy_carry.h](../recovered_exact/src/legacy_carry.h) supplies an instruction
string for `RCALL target; BRCS destination`, plus the parser-specific name.
It expands **in each original caller** with the original operands and clobbers.
All 22 occurrences use this primitive directly or through the parser macros.
There is no new function body, bool conversion, trampoline or change of RCALL
displacement. The number of executable ASM bytes removed by this refactor is
**zero**; its benefit is naming the shared contract without changing machine code.

The compiled flag-aware C comments and their transition tests cover the actual
instructions separately: `PASS_INSTRUCTION_TRANSITIONS` is not a whole-path
behavioral proof for the value-only snippets above. Those snippets remain
`REFERENCE_C_INFERRED`; none is labeled REFERENCE_C_BEHAVIOR_VERIFIED without
corresponding harness evidence. Whole-FLASH exact-check is the acceptance gate.
