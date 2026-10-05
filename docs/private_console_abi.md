# Readable calls across the PM12 private console ABI

The original image, not a compiler identification, establishes these contracts.
`legacy_console_call_c.h` names three exact RCALL bridges. GNU statement
expressions evaluate each argument once and expand at the call site. They avoid
an ordinary GNU ABI boundary, including R24/R25 parameter diagnostics in files
which reserve those registers. These helpers are intended only for the existing
per-file legacy register profiles, not arbitrary ordinary C callers.

| Bridge | Original target | Input | Output |
| --- | --- | --- | --- |
| pm_console_send_character | cli_send_buf | R16 character | Post-call R16 |
| pm_console_send_hex_digit | cli_send_digit_hex | R16 value | R16 ASCII character |
| pm_console_read_character | cli_get_next_char | None | R16 character |

Memory and condition codes may change. Preservation of other live registers is
provided by the original callees; this does not promise SREG preservation.
Digit formatting changes R16, so the bridge returns it explicitly. The send
callee preserves its scratch registers through its original frame.

Accepted substitutions: hexadecimal digit and word formatting, decimal
formatting, FLASH messages and hexadecimal parsing. Each changed translation
unit passed a complete canonical FLASH exact-check; logs are in
`docs/exact_c_steps/readability_*.log`. Integer parsing produced a mismatch at
byte 9805 and was restored. Its direct exact ASM call remains.

Ordinary always-inline functions with byte parameters were rejected where
R24/R25 were fixed. The accepted statement-expression form avoids that GNU ABI
parameter boundary without relaxing warnings or altering register profiles.
The bridges still count as ASM, not native C bytes. Their explanatory register
state C model is compiled by `make c-comment-check`; that auxiliary test covers
instruction transitions, not hardware timing or asynchronous interrupt paths.

Earlier frame experiments 590–613 are recorded separately in
`exact_frame_call_results_590.json`. Accepted native outer saves and OS_main
retain zero binary differences. Immutable reference sources are unchanged.
