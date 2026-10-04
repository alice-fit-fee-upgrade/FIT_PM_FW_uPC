# Controlled C recovery

Exact ASM remains the immutable binary reconstruction baseline. Recovered C has its own
C ABI, injectable register/stream access, an AVR backend, AVR library and native differential tests.
Existing incomplete repository C stays untouched; new source is independently derived from exact ASM.

```
make -C source_recovery check
make exact-check
```

`check` builds `build/libpm_recovered.a` for ATxmega128A3U with warnings as errors,
links all modules including libgcc helpers into `build/link_check.elf`, builds a native test library,
and compares C against bounded execution of original FLASH instructions. The link-check ELF is an
empty-main linker test, NOT firmware to program. Test runtime is about a minute on this host.
No Python packages or additional AVR toolchain are required.

Files:
- `src/console.c`: integer/hex parsers, hex and signed/unsigned decimal formatting, FLASH strings.
- `src/transport.c`: original UART RX/TX rings, RTS/flow-control, RXC/DRE ISR bodies.
- `src/scaling.c`: signed and saturating unsigned fixed-point arithmetic.
- `src/peripherals.c`: ADT7311, FPGA SPI, PLL, DAC and THS788 transaction sequences.
- `src/settings.c`: FPGA settings initialization/reset and original shutdown sequence.
- `src/avr_backend.c`: actual volatile data-space accesses, LPM and interrupt operations.
- `include/pm_recovered.h`: explicit C contracts, bus and stream interfaces.
- `tests/avr_oracle.py`: bounded functional interpreter driven by disassembly of golden memory.
- `tests/differential.py`: reproducible differential results, wire/MMIO/IRQ access order.
- `tests/coverage.py`: original address ranges and non-overlapping coverage accounting.

Verified scope: values, parser input consumption and partial-error results, output characters,
and register/memory access order. The interpreter fails on unsupported instructions and bounds loops.
SPI/bit-banging time, asynchronous interrupt schedules, physical peripherals, original caller register/SREG
ABI and ISR prologue/epilogue are not established by these tests. Decimal formatter's temporary scratch
RAM is replaced by a local C buffer; no claim of identical scratch bytes/register side effects.

Original routines covered span 2,494 bytes (23.02% of 10,836 symbolic code bytes), 33 original entries.
This counts routine spans INCLUDING original ABI scaffolding; it is semantic recovery coverage,
not binary substitution coverage. Zero bytes of exact_asm have been replaced by C.
The original `cli_send_32bit_hex` actually prints a 16-bit word, so the C name is corrected.
Signed/unsigned scaling retains original rounding and the original staged unsigned saturation.
TX indices wrap at 256 while RX wraps at 64; signed character comparison excludes bytes >=0x80.
The settings-init code writes a ONE-byte gate value and final 0x0FFF unlock value, not 0xFFFF.

First integration: ../mixed_c_asm now links the signed scaler through a verified original-ABI
bridge and compares compiled AVR register/SREG/stack state. Other routines remain separate.

Remaining integration work: prove remaining original ABI contracts and ISR preservation, map callback/data storage
into original RAM without disturbing buffers, retain cycle-sensitive assembly until timing is proven,
then create a separate mixed development ELF. Keep exact_asm available to compare every region.
The original formatter exposes signed decimal places 0..3 and unsigned integer mode only.
No bootloader, command dispatcher, full power/timer state machine, or complete C firmware is claimed.
