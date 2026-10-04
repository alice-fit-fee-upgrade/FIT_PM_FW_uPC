# FPGA settings: next original ABI integration

Golden FLASH is authoritative; addresses below are byte addressed. Portable
functional recovery already exists in source_recovery/src/settings.c. Neither
entry below is routed to C in the current mixed build.

## Initialization, 0x08E4..0x0989

The actual prologue saves R18, R17, R16, **R29 and R30**, not both halves of Y.
Golden bytes at 0x08EA and 0x08EC are `df 93 ef 93`: PUSH R29, PUSH R30.
The epilogue restores R30, R29, R16, R17, R18. Therefore R28 is not restored.
The last range-correction loop reads 24 words from 0x2187 through 0x21B6,
leaving Y at 0x21B7. A future ABI bridge must return **R28 = 0xB7** while
preserving incoming R29 and R30. Correcting this to a symmetrical PUSH/POP Y
would change original behavior.

The gate is one byte at 0x222F, trigger a word at 0x2230, board ID a word at
0x2232. Initial register 0x7C receives zero, final register 0x7C receives 0x0FFF.
Every transaction explicitly executes CLI, calls the existing SPI serializer at
0x230E, then SEI, irrespective of incoming interrupt enable. Final I is one.
The serializer saves/restores R18/R21/R22; its LSR/ROR operations affect flags.
The final register 0x7C produces SREG arithmetic bits C=0,Z=1,N=0,V=0,S=0;
H is zero from the last equal loop CPI, and T is preserved. This contract is verified against actual golden execution; compiled-C integration
still requires its own differential verification.

## Reset, 0x098A..0x09DD

Unlike initialization, the prologue and epilogue really preserve both R28 and
R29, as well as R16..R18. There are 48 words at 0x21CF to FPGA registers
0x80..0xAF, then 12 words at 0x2163 to registers 0xB0..0xBB. The last value is
one byte at 0x2441, sent to register 0xBE with high data byte zero.

The final serializer's ROR of the second header byte yields 0x80. Derived final
arithmetic flags are C=0,Z=0,N=1,V=1,S=0,H=0; T is preserved and I is forced one.
All registers are expected preserved. This is verified against actual golden execution; compiled-C differential
confirmation is still required before replacing the entry.

## Integration approach

Keep fpga_msg_send_t2 as original ASM; it controls hardware SPI and chip select.
Add a GNU-callable assembly adapter preserving C callee-saved R16..R18, mapping
C arguments to the original R18 register/R17:R16 value, and retaining exact
CLI/serializer/SEI ordering. Keep MMIO low-byte/high-byte reads explicit.
Compare full registers and SREG, ordered RAM/MMIO/IRQ traces, nonzero original
R1, balanced three-byte return stack, and all preserved original symbol addresses.
Do not call these routines ISR-safe: both original paths force interrupts on.
Cycle timing, asynchronous interrupts and physical hardware require separate
validation; the existing interpreter is functional only.

## Golden execution checkpoint

`python3 mixed_c_asm/tests/check_fpga_settings_contract.py` passed 2048
baseline cases: two entries, all 256 SREG patterns, four ready-poll delays
and seeded RAM/register values. Full register outputs, derived flags, CLI/SEI
sequence, 42 initialization / 61 reset transactions and return-stack balance
match the contracts above. The settings routines still execute original ASM.
The result is saved in mixed_c_asm/build/fpga_settings_contract.json.
