# Six original entries integrated in a validated mixed C/ASM firmware

This batch replaces four original DAC preparation routines one at a time, with
build/layout checks and original-versus-compiled AVR tests after each replacement.
The two previously integrated scalers remain intact.

| Original byte entry | Recovered calculation | Original span |
|---|---|---|
| 0x20A6 | Unsigned 0x020C scale, add RAM calibration, complement, command 3 | 42 bytes |
| 0x20D0 | Modulo-65536 `20000 - input`, unsigned 0x0272 scale, command 0 | 28 bytes |
| 0x20EC | Signed 0x4188 scale, XOR 0x8000, command 2 | 24 bytes |
| 0x2104 | Same signed scale and offset, command 1 | 24 bytes |
| 0x2130 | Original signed fixed-point scaler | 28 bytes |
| 0x214C | Original unsigned fixed-point scaler and saturation flags | 40 bytes |

The four new entries route to real C in mixed_c_asm/src/dac_prepare.c through
short ABI bridges. An eight-byte return packet carries the original value,
R21:R20 state, R1:R0 multiply product and R22 control. Adapters restore remaining
registers, the original coefficient high byte in R19, and T/I before calling
unchanged original DAC SPI ASM. The SPI sender determines all final arithmetic
flags and enables interrupts exactly as the original. The calibrated bridge
also preserves the two RAM reads and final Z pointer, with original byte-wrap
behavior for all 256 possible channel bytes. Calibration read timing relative
to the pure arithmetic changes; no cycle or asynchronous equivalence is claimed.

Six substituted entries cover 186 original routine-span bytes (1.72% of the
10,836 symbolic original code bytes). This is an entry-integration metric,
including ABI scaffolding; it is separate from the existing 23.02% semantic
recovery measure. It does not mean the entire original firmware is now C.

## Layout and reproducibility

All 806 original text-symbol addresses are unchanged. Each substituted original
body is an absolute JMP and FF padding; the unsigned internal saturation marker
retains its old address after static external-reference checks. Bridges occupy
430 bytes at 0x3000; compiled C plus GCC helpers occupy 926 bytes at 0x4000.
There are 1,518 differing byte positions, exclusively inside these original
entry spans and the two previously erased additions. All other original FLASH
bytes, vectors, startup, constants, strings and boot remain unchanged. The old
C region at 0x3100 is again erased. No new static SRAM, BSS or initialized data
is introduced, and no CRT is added. Layout assertions and byte-region checks
reject unrelated changes.

The separate exact ASM remains the golden source baseline: final `cmp` passes
with zero differing FLASH bytes. Reference originals and golden hashes are
verified by the mixed target. Repeated clean builds produce identical ELF, HEX
and canonical BIN SHA256 values, recorded in mixed_dac_final_reproducibility.json.

## Final validation

`make mixed-check` passed from a clean build, including its exact ASM checkpoint:
- 796,432 direct signed-scaler comparisons.
- 796,432 direct unsigned-scaler comparisons.
- 262,144 end-to-end comparisons across all four DAC callers, each input word.
- 87,040 channel-wrap, boundary-input and calibration-word comparisons.

Total: 1,942,048 compiled AVR comparisons in the final image. The interpreter
executes linked AVR instructions and GCC helpers with randomized register
states, including nonzero initial R1, all 256 incoming SREG patterns, physical
three-byte return PCs, compiler stack frames, balanced SP and ordered scripted
SPI/IRQ traces. No native C shortcut replaces the compiled calculations.
Calibrated tests also check read order and final Z; every calibration word is
covered with input 1234 and all channel bytes are tested at boundary inputs.

`make c-check` separately passed all 390,601 portable recovery regression cases.
The final JSON reports and byte/instruction diffs are saved as mixed_dac_final_*.
Each per-function commit retains its own earlier evidence for bisection.

## Practical limits

Maximum additional stack below the entry calling frame is 32 bytes for inverse
DAC preparation, 33 for calibrated preparation, and 35 for either signed DAC
preparation. Direct scaler integrations still require 22 extra bytes. Compiler
stack usage files describe only individual functions; these measured call depths
include nested calls. Dead stack scratch bytes may differ from old contents.
The model checks saved return addresses and balanced SP and restricts local
stack reads/writes to allocated addresses.

The bounded custom interpreter is functional and fails on unsupported instructions.
It does not establish cycle equivalence, SPL interrupt-suppression timing,
asynchronous interrupt behavior, whole-firmware stack safety or physical-module
operation. No device was programmed. SPI bit timing remains in original ASM.

Next candidate after this arithmetic batch is the status guard at 0x211C;
wire serializers should retain ASM until their timing contract is independently
verified. Existing exact_asm and upstream analysis are retained unchanged.
