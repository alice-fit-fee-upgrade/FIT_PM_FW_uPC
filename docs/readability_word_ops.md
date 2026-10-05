# Readability checkpoint: shared word operations

The accepted baseline now names eight signed bounds in four command handlers
and four little-endian settings stores in four handlers (five distinct source
files). `legacy_word_ops.h` retains exact register/flag contracts while each call
shows a full signed threshold or the intended store operation. Repeated obsolete
trial prose beside these bounds was replaced with concise intent comments;
the full tested logical C equivalents remain in every accepted source.

Three initial immediate-constraint attempts failed compilation and were restored.
A constant full-width low-byte expression resolved the constraint problem; three
retrials passed exact-check. No GCC patch, warning suppression, altered ABI or
binary substitution was introduced. All individual logs (703–710 and 711–713) are retained in the result index.
Step 711 is the first successful retrial. The interrupted store probe for
TDC adjustment was inapplicable: that handler stores one byte, not this word
pattern, and no change was made there by that probe.

The development guide documents helper contracts, zero-byte register captures,
IRQ-window preservation and the future boundary for ordinary GNU C drivers.
The README links it and refreshes the previously stale coverage snapshot.

Final root exact-check: zero differing bytes; original 806 text symbols fixed.
Native C remains 6880/10836 bytes; inline ASM remains 3148 bytes. This is a
readability refactor, not a claim that hidden ASM became compiler-generated C.
Logical C comment validation: 390208 transition cases, all PASS. Existing
historical exhaustive emulator suites were not rerun.

Canonical golden and rebuilt SHA256:
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.
