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

| Region (byte address) | Source |
|---|---|
| 0x0000..0x212F | Original application ASM |
| 0x2130..0x214B | Original scaler entry: 4-byte JMP followed by 24 FF padding bytes |
| 0x214C..0x2BDB | Original application ASM, constants and strings |
| 0x3000..0x304B | ASM ABI bridge (76 bytes) |
| 0x3100..0x315D | AVR-compiled C scaler + two GCC multiply helpers (94 bytes) |
| 0x20000..0x204E5 | Original boot region |

806 original code-symbol addresses remain fixed. The mixed image differs at 195 byte positions,
all within the replaced 28-byte scaler region or the two additions above. Original vectors, all
other application bytes, tables, constants, boot bytes and unused space outside additions match.
No new static SRAM, BSS, initialization data or CRT is permitted by the linker. Added code is
below the application-table region. The unused unsigned C scaler is removed with gc-sections.
Only one original procedure has been integrated; the remaining recovered C modules stay separate.

## Original ABI

Input coefficient R19:R18, signed input R21:R20. Outputs R17:R16 AND the final multiply R1:R0.
All other registers preserved. All eight SREG bits must match (T/I retained, C/Z/N/V/S/H computed
by the final original ADD). Incoming R1 is not assumed zero; outgoing R1 must not be forced zero.
The bridge clears R1 only for the GCC call, saves call-clobbered registers, restores original inputs,
replays the final MULSU, and reconstructs the final ADD's prestate from C's returned high byte.
`src/signed_scale_bridge.S` documents each step. See docs/mixed_c_asm_checkpoint.md for proof.

ATxmega128A3U compiler target defines __AVR_3_BYTE_PC__; tests physically model three-byte return
addresses and actual SRAM push/pop. Original startup sets SP=0x3FFF. A simulated calling frame
enters at SP=0x3FFC. Maximum additional scaler stack is 22 bytes (minimum SP=0x3FE6);
from either DAC caller it is 26 bytes. Return SP and saved return addresses are checked.
Dead stack scratch bytes change and are not required to equal old contents. Static/live nonstack
RAM and peripheral effects are checked separately; this does not prove worst-case firmware stack
usage with nested asynchronous interrupts.

796,432 direct compiled-AVR cases compare all 32 registers, SREG and return/stack balance.
131,072 end-to-end caller cases compare registers, flags, DAC SPI polling/write/IRQ traces and stack.
The custom bounded interpreter fails closed on unsupported instructions. It is a functional model,
not cycle-accurate hardware proof. Executed instruction count rises from 13/14 to 81/82;
full firmware timing/interrupt scheduling remains pending validation. Golden exact_asm is retained.
