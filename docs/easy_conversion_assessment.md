# Remaining application ASM after easy conversions

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
