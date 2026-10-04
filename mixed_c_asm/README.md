# First integrated C/ASM firmware build

```
make mixed-check
make exact-check
```

`mixed-check` builds from scratch, validates reference hashes and layout, executes **linked AVR
machine code** through the original entry point, and checks both original callers through DAC SPI.
It does not substitute native C callbacks for the scaler or GCC runtime helpers. Python 3 standard
library, GNU make and the existing AVR GCC/binutils are sufficient. The full test takes a few minutes.
Root `exact-check` separately proves the untouched assembly baseline remains bit-identical.

Outputs: `build/mixed.elf`, `mixed.hex`, `flash_mixed.bin`, linker map and JSON test reports.
This is a mixed firmware development image with original vectors/startup/boot, distinct from
source_recovery's empty-main linker smoke test. Physical module execution is still pending.

Two original procedures are now integrated: signed scaler at 0x2130 (28 original
bytes) and unsigned scaler at 0x214C (40 original bytes). Both keep their original
entry addresses, with JMP trampolines and FF padding. All 806 original code-symbol
addresses remain fixed, including the unsigned routine's internal saturation label
at 0x216E; the converter verifies that no external source reference targets it.

The ABI bridges occupy 0x3000..0x309F (160 bytes), and C plus GCC helpers occupy
0x3100..0x32CB (460 bytes). The mixed image differs at 679 byte positions only within
these replacements and additions. Original vectors, other instructions, constants,
boot and unused space outside additions remain unchanged. The linker permits no new
static SRAM, BSS or initialized data. The exact assembly baseline remains separate.

See [unsigned integration evidence](../docs/mixed_unsigned_checkpoint.md) and the
[historical signed checkpoint](../docs/mixed_c_asm_checkpoint.md). Functional recovery
coverage and actual replacement coverage are separate: two integrated entries cover
68 original code bytes (0.63% of 10,836 symbolic baseline bytes).

## Original ABI

Input coefficient R19:R18, signed input R21:R20. Outputs R17:R16 AND the final multiply R1:R0.
All other registers preserved. All eight SREG bits must match (T/I retained, C/Z/N/V/S/H computed
by the final original ADD). Incoming R1 is not assumed zero; outgoing R1 must not be forced zero.
The bridge clears R1 only for the GCC call, saves call-clobbered registers, restores original inputs,
replays the final MULSU, and reconstructs the final ADD's prestate from C's returned high byte.
`src/signed_scale_bridge.S` documents each step. See docs/mixed_c_asm_checkpoint.md for proof.

ATxmega128A3U compiler target defines __AVR_3_BYTE_PC__; tests physically model three-byte return
addresses and actual SRAM push/pop. Original startup sets SP=0x3FFF. A simulated calling frame
enters at SP=0x3FFC. Maximum additional signed-scaler stack is 22 bytes (minimum SP=0x3FE6);
from either signed DAC caller it is 26 bytes. Return SP and saved return addresses are checked.
Dead stack scratch bytes change and are not required to equal old contents. Static/live nonstack
RAM and peripheral effects are checked separately; this does not prove worst-case firmware stack
usage with nested asynchronous interrupts.

796,432 direct compiled-AVR cases per scaler compare all 32 registers, SREG and return/stack balance.
262,144 end-to-end caller cases compare registers, flags, DAC SPI polling/write/IRQ traces and stack.
The custom bounded interpreter fails closed on unsupported instructions. It is a functional model,
not cycle-accurate hardware proof. Executed instruction count rises from 13/14 to 81/82;
full firmware timing/interrupt scheduling remains pending validation. Golden exact_asm is retained.
