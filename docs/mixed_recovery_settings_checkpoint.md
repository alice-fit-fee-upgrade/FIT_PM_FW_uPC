# FPGA settings initialization/reset in C

Original entries 0x08E4 (166-byte span) and 0x098A (84-byte span) now route to
compiled C in mixed_c_asm/src/fpga_settings.c. RAM reads remain ordered low-byte
then high-byte. The gate and restart reason remain single-byte reads, and the
last initialization value is 0x0FFF, retaining original behavior.

A GNU-callable assembly adapter maps the register/value arguments to original
R18/R17:R16 and executes CLI, the unchanged fpga_msg_send_t2 serializer at
0x230E, and SEI. It preserves GNU callee-saved R16..R18. No SPI serializer,
timeout, hardware configuration or original software-generated clock is changed.

Entry bridges preserve original registers and reproduce final SREG contracts.
Initialization leaves R28=0xB7, preserving the original asymmetric PUSH R29/PUSH
R30 prologue. Reset preserves all 32 registers. Both force final I=1 and retain
incoming T. Initialization's final SREG is 0x82 | incoming T; reset's is
0x8C | incoming T. Nonzero original R1 is preserved, with R1 cleared only while
executing GNU C code.

The compiled-AVR settings suite compares every register, all flags, ordered RAM,
SPI and IRQ traces, and balanced three-byte return stacks. It covers both
entries, 256 incoming SREG patterns, four ready-poll delays and four RAM profiles
(random, zero, all ones, address bytes): 8192 cases. The prior 2048-case audit of
golden-only execution remains available and is not counted as C integration.

There are now 16 integrated application entries and 73 remaining, out of 89.
Functional C remains available for 45 entries, 29 awaiting integration. Nine
boot procedures remain ASM; 82 entries are unintegrated including boot.
Replaced original spans total 714 bytes (6.59% of 10836), including retained ASM
and ABI scaffolding. This is not pure generated-C byte coverage.

All 806 original text symbols retain their addresses. Bridges occupy 1324 bytes
at 0x3000, C/helpers 1632 at 0x4000. The 3601 differing positions in the mixed
image are permitted replacements and previously erased additions. Original
vectors, startup, constants, strings, remaining code and boot retain their bytes.
No static SRAM, BSS, initialized data or new CRT is introduced.

The independent exact ASM source remains golden and bit-exact. Functional
interpreter tests do not establish cycle equality, asynchronous interrupt safety,
SPL timing or whole-firmware stack headroom. No device has been programmed.

Maximum additional tested stack for either settings routine is 33 bytes, below
the existing 35-byte maximum through DAC callers. Whole-firmware interrupt
stack headroom remains unverified.

Next candidate: system_deinit at 0x0C96. Its last EOR R16,R16 sets Z=1
and clears N/V/S while preserving incoming C/H/T/I; the preceding masked
status write does not determine the final flags. Its recovered portable C
body is already available, but the original entry remains ASM in this checkpoint.

## Final validation

Clean `make mixed-check` passed. Its initially launched settings test used the
2048-case version; the expanded 8192-case suite subsequently passed against the
same image and supersedes that smaller settings report. The retained validation
set totals 2595680 compiled-AVR cases, without counting the superseded settings
run twice. The current mixed-check recipe invokes the expanded suite directly.
Golden-only settings audits and synthetic low-I/O architecture fixtures remain
separate from compiled-firmware counts. The portable C library is unchanged in
this batch; its previously recorded 390601-case result remains historical.

Reference SHA256 verification and exact-check passed, with zero golden FLASH
differences. All 806 original text-symbol addresses are preserved. Two clean
builds reproduce identical ELF/HEX/BIN hashes. Tested mixed BIN SHA256:
`a2c536c2f88864651999c57d3e12ee26e203d8f45b907294dd7718dc03fbca21`.
Logs, per-suite reports, layout/byte/instruction comparisons and reproducibility
proof are retained as mixed_recovery_settings_*.

The separate mixed image intentionally changes original entries with JMPs
(class E), fills their vacated bodies with FF (class G), and adds bridges/C in
previously erased space (class F). All 3601 differing positions are permitted
by check_layout.py; no unresolved exact-reconstruction differences exist.
