# Difference analysis and resolution

Final: EXACT MATCH YES, 0 differing bytes across 139,264 bytes. Golden/rebuilt SHA256:
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.
Initial and fill-only byte reports are preserved in experiment_existing_diff.txt and
experiment_filled_diff.txt. Canonical comparison, never textual HEX identity, is decisive.

| Region (byte address) | Class | Cause | Resolution |
|---|---|---|---|
| Erased vector gaps 0x0002..0x01E1 | G/H | GNU `.org` defaults to 00; device has FF | Explicit FF fill before data |
| 0x1576..0x1579 | B/E | Historical RET substitution loses boot entry jump | Restore `jmp boot_020308` (4 bytes) |
| 0x2960..0x2961 | G | Constant-to-banner gap is zero-filled in original | Data-region `.org` fill 00 |
| 0x2964..0x2966, 0x296A..0x296B | L | Analysis changed INR PM12 into WUT PMxx | Restore original string, same length |
| Scattered string alignment through 0x2BC3 | G | FF fill is wrong for original zero alignment | Data-region `.org` fill 00 |
| 0x201A0..0x204E5 | A | Boot code excluded/commented, only partial bytes available | Recover symbolic ASM and label relative branches |
| Boot/application region layout | F | Default linker cannot fix independent boot base | Dedicated section at 0x20000 |
| GNU linker experiment | F/N | Numeric ENTRY syntax and avr:107 incompatible with target avr:106 | Absolute reset symbol, avr:106 output architecture |

Repairs followed ascending address order, using direct-assembly, fill-only and final canonical comparisons.
No bug fixes, startup reorderings, clock changes or peripheral changes were made.
All application anchors remain intact, and boot instructions each have an address anchor.

Classification vocabulary: A missing region; B wrong instruction; C wrong immediate;
D relative branch relocation; E CALL/JMP address; F section/layout; G padding/alignment;
H vectors; I startup; J code mistaken for data; K data mistaken for code; L missing constant/table;
M linker generated; N syntax/encoding; O unknown. No unresolved mismatch exists.
Instruction diff heuristics explicitly distinguish target/operand changes from mnemonic changes;
equal mnemonics alone do not prove semantic equality. Code/data ambiguities are reported conservatively.

Coverage: 10,836 symbolic ASM bytes; 0 literal code bytes; 710 data bytes; 127,718 erased/padding bytes.
The separate used-byte metric is 11,354 original non-FF bytes, all matching. This includes data;
FF-valued instruction/data bytes count toward coverage but not toward non-erased byte metrics.
