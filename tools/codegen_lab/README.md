# AVR codegen idiom lab

This lab uses the unmodified shared AVR GCC 7.3.0 toolchain and the exact expanded
Makefile command of the specified recovered translation unit. It adds no global
or candidate optimization flags. It does not replace the firmware acceptance gate.

```sh
make exact-check
python3 tools/codegen_lab/analyze.py
python3 tools/codegen_lab/run.py --spec tools/codegen_lab/spec.json
```

`analyze.py` records all retained occurrences of five families with operands,
context, registers, flags, branch/call targets and conservative CFG liveness.
Neighboring windows overlap: counts are occurrences, byte totals are unions.
The semantic labels are operand/source inferences, not claims of path proofs.

`run.py` compiles each candidate to an object using its real per-file profile,
links an isolated ELF at the original byte address without relaxation, disassembles
and compares the complete probe body byte-for-byte with the golden sequence.
Only an explicitly requested final RET is excluded. Extra prologue, pointer
increments, changed branch displacements and changed operand bytes all count as
DIFFERENT. Unresolved external calls cause LINK_ERROR, never a false match.
The comparison includes desired/generated instructions, first differing byte and
instruction, exact byte/instruction counts and mismatch totals. Compile errors
are recorded as DIFFERENT with their diagnostics.

A spec entry supplies `source`, `profile`, `address`, `length` and optional
`desired_bytes`. The latter permits a new explicit opcode sequence; desired
mnemonics are decoded from those actual bytes, not copied from another address.
`strip_final_ret` allows a minimal void probe's return to be outside the idiom.
The positive native-MMIO control demonstrates an EXACT result for explicit bytes.

Input/output empty ASM operands are a model of the pre-existing private register
ABI. Probes have no inserted *inter-access* barriers, dead code or arbitrary
volatile. The volatile configuration-load probe is LAB_ONLY, not an eligible
conversion absent a shared-memory reason. The volatile queue variant reflects
ISR-shared state. The uint16/int16 and two-byte/struct forms exercise different
memory codegen; enum equality retains the compiler's normal enum width.

A probe mismatch does not prove no natural C solution exists. Full-function
context can change register allocation; integrated candidates are recorded
separately. Conversely, a probe match never establishes a safe firmware change.
Every actual adoption requires root `make exact-check`, updated manifest ranges
where relevant, and the GCC provenance audit. No exhaustive emulator acceptance
is used for a byte-identical adoption.

Generated objects/ELFs/disassembly live under recovered_exact/build/codegen_lab
and are not committed. The candidate sources, spec and summarized result JSON
remain in the repo for the future, separate compiler-directive pass.

## Integrated trials

`integrate.py --spec <reviewed-spec.json> --output docs/<trial-directory>`
tries one source substitution at a time, builds the complete image, regenerates
manifest provenance for successful candidates, then runs clean root exact-check.
It restores source and manifest on any rejection. Header edits refresh all
affected provenance entries. Output directories must be within this repository;
paths are validated before changing source. Each trial records its source edit,
acceptance result and build log. A final clean check leaves restored build
artifacts after the last rejected candidate. Do not run two integration jobs
simultaneously, and do not use it with unrelated concurrent source edits.

The positive and deliberately mutated negative opcode controls assert both
comparison outcomes. Live-value signed probes distinguish destructive SBCI
from CPI/CPC when the original value remains needed. Complete output bodies
include the observation stores, so they are still full-body experiments, not
claims that an internal matching subsequence suffices for firmware acceptance.

The initial inventory is docs/asm_idioms.json. To avoid overwriting it after
conversions, run `analyze.py --output-prefix docs/asm_idioms_after`.
