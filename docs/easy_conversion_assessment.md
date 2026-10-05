# Remaining application ASM after further exact conversions

Steps 84–88 moved the signed scaler, FPGA exchange, CLI dispatcher, DMA ISR
and timer ISR into C plus exact helpers. Steps 89, 91 and 92 additionally replaced
31 single-instruction arithmetic/logic helpers with ordinary C expressions.
Every accepted step passed a complete clean `make exact-check`.

One application entry remains entirely ASM: `cli_send_int16`, byte
0x2746–0x274D (8 bytes). It contains PUSH R20, CLR R20, SEC and RJMP 0x2754.
GCC zero assignment uses LDI rather than the original flag-setting CLR;
PUSH/SEC/shared-entry RJMP need the private formatter frame. An all-ASM C wrapper
would contribute zero compiler-generated instruction bytes and is not counted
as a C conversion. It remains exact GNU AVR assembly.

The application still has 4742 inline ASM bytes inside 83 mixed functions.
These are live-flag operations, ISR frames, carry chains, private calls, pointer
accesses and exact encodings. Moving an entry into a C file does not make these
bytes C: independent GCC APP/NOAPP provenance counts every emitted ASM byte.
Further work can replace individual helpers when clean C emits the same bytes.
Boot remains 9 ASM procedures, 778 executable bytes.

Of 10836 executable bytes, C accounts for 5286 (48.7818%) and ASM for 5550
(51.2182%). Application-only C coverage is 52.5552%. All original 806 symbols
retain their addresses and complete canonical FLASH has zero differences.

Further steps 93–190 accepted 67 changes across 41 translation units and
rejected 31 candidates. Accepted work covers C SPI polling/stores/reads, selected
SRAM loads, comparisons, bit tests, address packing and GNU AVR CPU primitives.
That checkpoint reported 184 primitive bytes; steps 191–208 raise this subset to
228 bytes in the current baseline.
Rejected pointer updates, delay loops, register-starved loads, skip selection and
bit-copy ordering attempts were restored; no behavioral test was used to override
any difference or compiler failure. The remaining complete ASM entry is unchanged.
See `exact_continuation_results.json` for every attempt and its evidence log.

## Earlier assessment, before steps 84–92


The continuation accepted 50 more entries (steps 34–83). All 89 original application
entries were revisited, including main, the ISRs and shared formatter entry points.
The remaining six are exact symbolic GNU AVR ASM, not binary-mismatching C.

This is a stopping point for small, clean, locally checkable conversions. It is not
a proof that the four large control-flow functions cannot be recovered further.
They need a separate bounded conversion of their state machines and shared labels,
with exact-check after each fragment; no whole-function inline-ASM blob or empty C
wrapper is used to increase the C count.

| Byte range (inclusive) | Original entry | Bytes | Reason to retain exact ASM | Original evidence / next useful approach |
|---|---|---:|---|---|
| 0x01E2–0x046B | DMA_CH1_vect_isr | 650 | Large interrupt/state machine; exact frame, polling, page programming and live 24-bit state | 0x031A–0x031E loads R2:R0; programming/CRC branches return through a shared ISR frame. Recover state paths separately before replacing the entry. |
| 0x049E–0x08E3 | TCC0_OVF_vect_isr | 1094 | Multiple power/TDC/PLL/temperature states in a single ISR with long-lived register flags | Save frame 0x049E–0x04C0; MUL/MULSU at 0x07D2/0x07D8; FPGA/TDC and initialization calls across many branches. Start with one bounded status/timer region. |
| 0x115C–0x12E9 | fpga_data_exchange | 398 | 64-bit update bitmap in R15:R8, condition-specific clamping and interrupt exits | Eight-byte shift at 0x117C–0x118A; early SEI/RET at 0x116C; each channel branch clamps/stores before DAC/FPGA calls. Recover one channel branch with fixed placement. |
| 0x12EA–0x1579 | cli_prompt_parse | 656 | Shared command/error tails, exact CALL/RJMP widths, reset/programming paths and protected stores | External error target 0x13C8; CCP stores at 0x1402, 0x142E, 0x1558, 0x156C; final JMP into boot at 0x1576. Split dispatcher and protected-action fragments, retaining shared anchors. |
| 0x2130–0x214B | fpga_is_ready | 28 | Misleading name: this is a signed scaled multiply/rounding core with non-GNU live R1 | MUL/SBRC/INC-R1 and MULSU at 0x2140/0x2146; R1 is not cleared. A clean C arithmetic expression would need different multiply/rounding code. Preserve this small complete helper. |
| 0x2746–0x274D | cli_send_int16 | 8 | Four exact instructions, with no meaningful compiler-generated C operation | PUSH R20; CLR R20; SEC; RJMP 0x2754. Replacing CLR with C zero initialization changes encoding; making all four instructions inline ASM only creates an empty C wrapper. Preserve the shared entry. |

The final image matches original PM.hex in every byte. No remaining entry blocks
acceptance of the mixed development baseline. Boot remains 9 exact ASM procedures.

`docs/function_inventory.json` contains complete classifications and byte counts.
`docs/exact_c_provenance.json` checks C/ASM accounting against GCC APP/NOAPP markers.
`docs/exact_c_steps/` retains the whole-image conversion checks; final artifact
hashes and reference hashes are in `docs/exact_c_reproducibility.json`.

Steps 191–208 accepted another 18 fragment changes across 13 translation units,
replacing 96 ASM bytes with C. See exact_latest_continuation_results.json.
