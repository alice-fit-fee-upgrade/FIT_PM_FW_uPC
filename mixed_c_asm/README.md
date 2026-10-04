# Mixed C/ASM firmware development build

`make mixed-check` performs a clean build, validates reference hashes and layout,
executes linked AVR opcodes against the original full CPU/register contract, and
compares DAC SPI traces. `make exact-check` separately checks the unchanged exact
assembly baseline. `make c-check` runs the broader recovered portable C suite.

Original addresses, vectors, boot and tables remain anchored. New bridges live
at 0x3000; compiled C starts at 0x4000. Static SRAM, BSS and initialized data are
forbidden by the linker. Outputs are build/mixed.elf, mixed.hex and flash_mixed.bin.

Integrated original entries: signed scaler 0x2130, unsigned scaler 0x214C,
inverse DAC preparation 0x20D0, and calibrated DAC preparation 0x20A6, and signed DAC command 2 at 0x20EC. See docs/mixed_dac_inverse_checkpoint.md
and the earlier signed/unsigned checkpoint documents for ABI and test evidence.
The original SPI sender stays in ASM. Incoming R1 may be nonzero; outgoing R1,
SREG, all other registers and three-byte return PCs are checked against original
execution. Compiler-local stack frames are supported by the test model.

Tests use a bounded custom functional AVR interpreter, with scripted calibration
RAM and SPI polling. Unsupported instructions fail closed. Asynchronous interrupts,
cycle equivalence, total firmware stack budget and physical-device behavior remain
unvalidated. No hardware programming is performed. Functional C recovery coverage
and the count of entries actually substituted in this firmware are separate metrics.
