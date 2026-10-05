# FIT PM12 exact recovery contract

The accepted recovered/development firmware is `recovered_exact/`.
Root `make` builds this baseline; root `make exact-check` is authoritative.
The final full canonical FLASH must equal original PM.hex with zero differences.

Keep `exact_asm`, `reference`, `dump` and upstream `asm_analysis` unchanged.
They are reference implementation/evidence, not refactoring targets.
Keep historical `mixed_c_asm` / `source_recovery` code and tests, but do not count
behaviorally matching, binary-different implementations as recovered baseline C.

For each ASM-to-C candidate: compile, build complete FLASH and exact-check.
Keep only zero-difference results. Try a small reasonable compiler adjustment or
small exact ASM helper where appropriate; otherwise retain ASM and continue.
Do not distort readable C to imitate old code generation. Do not replace executable
code with a whole-image blob. Count inline ASM helper bytes as ASM.

Classifications are C_BINARY_EXACT, C_WITH_EXACT_ASM_HELPER and ASM_EXACT.
`make c-progress` reports actual accepted classifications and coverage.
All original byte addresses and symbols are preserved by generated linker regions.
Per-file legacy register profiles are not ordinary GNU C argument contracts.

Reuse the shared AVR GCC 7.3.0 / binutils 2.26 toolchain on PATH. Do not install a
second toolchain without checking the workspace toolchain notes.
Once compiled bytes are identical, do not spend substantial compute on exhaustive
emulator comparisons. Preserve existing suites as auxiliary historical tools.
No bugs, clocks, timeouts, protocol, GPIO or startup semantics are changed here.

When retaining exact ASM after a binary-different C attempt, include the C
equivalent as a nearby comment if successful functional validation is recorded.
Reference the archived implementation, test evidence and scope, including required
ASM bridges/callees. Do not claim tests that were never run. See
`docs/asm_c_alternatives.md` and its JSON index. These comments are explanatory;
only whole-FLASH zero-difference exact-check accepts a baseline implementation.

For newly retained fragments also include an explanatory C value equivalent when
there is no historical successful test evidence. Label it as unvalidated and
state relevant live flags/private ABI; never present it as a tested alternative.
