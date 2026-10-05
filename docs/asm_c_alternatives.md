# Commented C alternatives beside exact ASM

The accepted source now includes 25 commented historical C alternatives beside
retained ASM helpers. The comments reproduce the archived function bodies;
types, MMIO definitions, callees and private-ABI bridges remain in the explicitly
referenced historical sources. These are explanatory comments, not replacements
or new accepted implementations.

Every entry in `asm_c_alternatives.json` links the source, function and recorded
per-function validation checkpoint. The historical mixed builds differed from
PM.hex. Functional comparison success applies to the integrated C plus its ASM
bridge/callees, not to a standalone C function with the original private ABI.
No timing, asynchronous interrupt or physical hardware equivalence is claimed.
The interrupted final historical aggregate run is not treated as a successful
run; completed per-function checkpoints provide the evidence.

Previously untested alternatives are not labelled as validated. For future
conversions, include the rejected C equivalent beside the retained ASM when
successful functional test evidence exists, and reference that evidence with
its scope. If there is no such evidence, state that explicitly rather than
claiming successful tests. Comments may describe the whole routine once instead
of duplicating it for each register constraint or ASM fragment.

Acceptance remains `make exact-check`: complete canonical FLASH must match the
original PM.hex with zero differing bytes. Historical functional tests are
retained; no expensive emulator suite was rerun for this documentation change.

The steps 209–233 continuation additionally comments retained ADC carry chains,
DEC counters, queue/calibration pointer loads and private call wrappers. These
new value equivalents explicitly state when they lack independent functional
validation. The historical index still contains the same 25 tested alternatives.
