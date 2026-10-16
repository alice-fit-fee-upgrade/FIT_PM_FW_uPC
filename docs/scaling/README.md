# Arithmetic recovery: exhaustive application-input domains

Branch `development/scaling-c-simulation`, based on `e01d685`.
No hardware was programmed/reset in this pass. The board remains on the
previous PGOOD application; the failed PLL/ADT trial is recorded separately.
The original exact baseline and `exact_asm` are unchanged.

## Scope and route selection

| Original byte entry | Recovered role | C route | Full input domain |
|---|---|---|---|
| 0x2130, misleading symbol `fpga_is_ready` | Signed-input DAC product, rounded /256, modulo16-bit output | coefficient0x4188 | All65536 input bit patterns |
| 0x214c, `FUN_code_0010a6` | Unsigned product with original staged saturation | coefficients0x020c and0x0272 | All65536 inputs for each coefficient |

Original direct calls are0x20f2/0x210a (signed),0x20ac (unsigned0x020c),
0x20e0 (unsigned0x0272). The test audits their immediate coefficient loads
against golden disassembly. This static audit does not rule out arbitrary
indirect calls. Consequently an assembly guard routes every OTHER coefficient
to the retained original ASM, after restoring entry registers/flags and
replaying the displaced CLR/MUL prefix.

Only two four-byte JMPs are patched, at0x2130 and0x214c. Every remaining byte
of both original functions, including the internal saturation label0x216e,
retains its original address and contents. New code lives in the existing
reviewed extension window0x4000..0x7fff. No globals/CRT initialization or new
SRAM allocation is introduced. The layout test rejects writes into retained
bodies, other legacy regions or bootloader.

## Maintainable C and necessary ABI assembly

`development/candidates/scaling/scaling_candidate.c` contains ordinary GNU C,
with interfaces in `scaling_candidate.h`. It derives from the archived scaling
analysis without editing that historical source. Signed input is a16-bit bit
pattern, explicitly converted to a signed32-bit value; rounding uses an unsigned
modulo product shift, avoiding implementation-defined negative right shifts.
Unsigned arithmetic retains staged16-bit truncations and saturation. Replacing
it with a conventional32-bit product/clamp would change historical results.

The legacy ABI returns R17:R16 and a live final product in R1:R0, preserves the
other GPRs, and exposes arithmetic SREG bits. Standard GNU C does not have this
contract. `scaling_legacy_candidate.S` therefore saves GNU call-clobbered inputs
and bank registers, normalizes R1/banks for C, then reconstructs legacy outputs.
Signed SREG is reconstructed by the final original ADD; unsigned C returns a
value/SREG packet. This packet is a separate adapter contract, not the public
numeric return type. Only T/I from its incoming SREG argument are used.

The unsigned value-only API is also tested separately with the GNU calling
convention. New code can use the plain typed numeric API without private
register declarations. The full legacy wrappers remain only at old call sites.

## Reproduce

```
make scaling-check
make exact-check
```

`scaling-check` builds the complete opt-in development ELF/HEX/canonical BIN,
runs the normal development guards/tests, then executes actual linked AVR
instructions against golden original instructions. C/GCC helpers are not
replaced by host callbacks. This is a functional CPU/stack interpreter with
three-byte call returns, not a cycle-accurate full-device emulator.

The target performs:

- 786432 legacy comparisons: three coefficient domains ×65536 inputs ×four
  T/I combinations. The other six incoming SREG bits are varied; original
  dataflow overwrites them before function return.
- 6144 additional comparisons: all256 SREG values at eight arithmetic edges
  for each of the three supported coefficient domains.
- 262144 route/fallback comparisons: both functions ×all65536 coefficients
  ×two extreme inputs. Unsupported coefficients must use original ASM;
  measured stack depth checks this routing independently of numeric results.
- 3072 caller-composition comparisons: four unchanged original DAC callers
  ×16 boundary inputs ×all12 channels ×four SREG settings; ordered calibration
  RAM reads, SPI polls/writes and explicit IRQ events must match. One scripted
  polling pattern and deterministic calibration-RAM image are used, not every
  possible calibration or analog response.
- 131072 unsigned GNU numeric-API comparisons: both supported coefficients
  ×all65536 input words, against original AVR results, with callee-save/R1/
  stack checks.

Legacy comparisons check all32 GPRs, all8 SREG bits, saved bank registers,
balanced physical stack and absence of MMIO/static-memory writes. Scratch stack
contents may differ after return. The test intentionally models dirty legacy R1
and randomized preserved registers. Bank restoration tests do not prove extended
bus behavior with nonzero bank registers.

This is **100% of the stated16-bit application input domains**, not all2^32
coefficient/input pairs or every possible CPU/RAM state. Unsupported coefficients
remain ASM. Hardware, timing, asynchronous IRQ behavior and total nested system
stack bounds remain NOT_RUN. No production acceptance or exact-C baseline
classification is inferred from simulator coverage.

## Build isolation and deployment

Ordinary `make development` excludes these candidates; enable only through
`make scaling-check` or `PM_SCALING_CANDIDATE=1 make development`.
The opt-in build reuses `development/build`; archive its evidence before an
ordinary build replaces those outputs. `link_objects.json` records actual link
inputs so the no-CRT RAM rejection test works with both modes.

This branch inherits the experimental PLL replacement whose system trial failed
on ADT startup. Both that issue and the new arithmetic integration need explicit
hardware qualification. The PLL programming runner rejects arithmetic-enabled
images; this pass creates no release or programming artifact for deployment.
The ordinary build is also still an unaccepted PLL experimental build, not the
firmware currently running on the board.

## Recorded result2026-10-16

**SIMULATION_QUALIFIED_ONLY: PASS,1188864 comparisons.** Reports and logs are
in [validation-2026-10-16](validation-2026-10-16/summary.json). Legacy comparison
peak was27 additional stack bytes below entry; the four-caller result separately
records its deeper composition bound. These are observed test bounds, not total
firmware worst-case stack bounds.

Opt-in canonical SHA256:
`bdebd471aa90f7b97f023fead0cac61b332eddda328cc81471deeba9df393a8a`.
New extension code is906 bytes:558 bytes emitted from C and348 bytes of ABI,
coefficient guards and patch templates. Old executable bodies are retained;
this is no claim of removing906 bytes of old ASM or increasing exact-baseline
C coverage. Default opt-out image remains byte-identical to the pre-pass
experimental development image9ece53e7…; hardware still runs the rolled-back
PGOOD release002, not either experimental build.

The full-domain and API/caller suites were each run once against the same
canonical candidate. The direct-call audit was also checked in the updated
smoke harness. Later header-only declaration integration was verified by
byte-identical full candidate rebuild; no exhaustive run was repeated for
identical machine code. Root exact-check remains0 differences.
