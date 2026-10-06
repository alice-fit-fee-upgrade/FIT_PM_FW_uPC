# Extension-ready PM12 source boundary

Two independent targets are now available:

* `make exact-check`: original canonical PM.hex, **zero differing bytes**.
* `make development-check`: extended firmware, intentional differences confined
  to a six-byte dispatcher gate and a bounded new FLASH section.

New code is compiled with the normal GNU AVR ABI in `development/src/`. The
existing mixed recovery objects keep their original profiles and addresses.
An explicit adapter handles register, SREG, zero-register and extended-bank
contracts. Ordinary first command bytes continue at the original dispatcher;
`@` commands run through the new standard-C handler list. `@PING` and `@HELP`
are integrated examples. No device programmer or hardware configuration was run.

The build uses source objects and GNU linker/assembler relocations. It patches
only the development ELF's six displaced CLI bytes. Every other original byte
and symbol remains unchanged; the bootloader remains unchanged. The baseline,
exact_asm and reference materials are preserved. The new code is not reported
as C recovery of original bytes and does not change baseline C/ASM percentages.

Current extended area: **1012 bytes**, starting at **0x4000**, within a reviewed
0x4000..0x7fff erased FLASH window. The canonical development image differs in
**1003 bytes**. The difference gate verifier rejects corruption even in an
otherwise erased but nonreserved part of FLASH.

Validation passed: decoded boundary tests **1788 cases**, native command tests,
negative FLASH-layout tests, and rejection of an uninitialized RAM global.
New C has stack-only state / explicit FLASH constants. Linker assertions reject
uninitialized RAM allocations because the original reset has no ordinary CRT
startup for them. Dynamic stack frames and individual C frames above 128 bytes
are rejected; complete stack depth/ISR latency still need review when adding
larger functionality. Existing console adapters retain their blocking queue
behavior and are for main-loop use.

I2C pin ownership, TWI selection, backend implementation and physical validation
remain separate integration work. The infrastructure is ready for a typed driver
and its command handler; it does not invent a bus assignment or successful read.

[Adding commands/drivers](../development/README.md),
[machine-readable validation](development_boundary_validation.json).

Baseline golden and rebuilt SHA256:
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.

Development SHA256:
`b8c9e420d19b668e5733121a2d83d46b6e099f8286a1db0d6bbf67dbca160b46`.
This image is intentionally different; it is never accepted as the original
binary-exact reconstruction. Hardware UART/timing and future sensor tests are
pending and are not claimed by the CPU/state and host validation.
