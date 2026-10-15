# Function-by-function development releases

The team changes one function or one coherent fragment at a time. Each candidate
has a source commit, an immutable tag, downloadable programming artifacts and
an explicit test status. Keep the bootloader and working unrelated functions
unchanged unless a separate task requires them. Do not pursue 100% C as a goal
at the expense of working, maintainable code.

The exact baseline remains separately reproducible with `make exact-check`.
Intentional new C code with different bytes belongs in `development/`; do not
claim it is C_BINARY_EXACT or silently change golden references. The current
step relocates the original timer ISR. Its next implementation change is a
separate release, not part of the relocation checkpoint.

## Per-step procedure

1. Identify the previous working release and exactly which function changes.
2. Implement under `development/` with explicit legacy/native ABI boundaries.
   Keep peripheral side effects and PSU/IRQ protection accounted for. Review
   layout allowlists per region; never authorize arbitrary FLASH differences.
3. Run software checks and build candidate + exact-original rollback artifacts:
   `python3 tools/package_development_release.py TAG` from a clean committed tree.
   Output is `development/build/releases/TAG/`. Do not overwrite it.
4. Publish a GitHub **prerelease**, pinned to that source commit, with the entire
   bundle: application HEX, ELF, canonical audit BINs, SHA256 manifest, logs and
   limitations. Say whether the step is relocation or an implementation change.
5. Compare with the original/previous accepted firmware on the physical PM12 and
   on the independent tester. Use the same controlled inputs, EEPROM/config and
   relevant FPGA state. Exercise nominal behavior, fault inputs, timeouts,
   concurrent UART/ISR operation, PSU protection and recovery. The tester can
   inject faults that are impractical on the board. Keep raw traces and image
   hashes; do not treat software tests as hardware proof.
6. Record board/tester PASS, FAIL or NOT_RUN separately. Accept for team use only
   within the measured scope. Retain prior releases and their binaries for
   rollback. Add evidence without replacing existing binary assets or retagging.

The full canonical audit BIN includes the unchanged bootloader. The intended
programming HEX contains application addresses only (<0x20000), with EEPROM,
fuses, signatures and bootloader omitted. The PDI procedure must preserve those
regions; merely omitting them from HEX does not make a chip erase safe. No
programming is performed by the packaging tool.

## First checkpoint

`pm12-dev-001-timer-relocated`: six-byte CLI entry patch plus four-byte timer entry
patch; relocated ISR preserves original instruction operations/control flow but
changes transfer timings. @PING/@HELP are available. New INA reads, ownership
lock and FPGA-independent electrical operation are NOT implemented by this
release. Hardware comparison remains NOT_RUN until evidence is supplied.
