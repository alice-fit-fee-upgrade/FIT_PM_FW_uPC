# Reading and extending the recovered PM12 firmware

Start with `recovered_exact/src/application_main.c`, then the relevant device or
console handler. This tree is the accepted mixed C/ASM baseline. The original
instruction addresses, IRQ windows and memory layout are part of its contract.
The source is intended to be readable without changing those contracts.

## Small helpers and repeated operations

| Header | Purpose | Contract to check before use |
| --- | --- | --- |
| `legacy_cpu.h` | IRQ enable/disable, NOP and bit copying | Original SREG and interrupt-window effects |
| `legacy_console_call_c.h` | Read/send character, send hex digit | R16 value/result; memory and flags may change |
| `legacy_cli.h`, `legacy_cli_guard_c.h` | Parse channel/value; FPGA readiness guard | Private result registers, carry status, shared error labels |
| `legacy_word_ops.h` | Signed word bounds; settings word store | Preloaded comparison high byte; R21:R20 value and Y cursor |
| `legacy_spi_c.h` | SPI data stores and completion polling | Original scratch register and peripheral status semantics |
| `legacy_interrupt.h`, `legacy_interrupt_register_c.h` | Original ISR frames and return | SREG, saved registers and RETI; use existing ISR profile only |

These headers are adapters to the original instruction contracts, not a generic
HAL for arbitrary standard-ABI C functions. Many are macros so no extra call,
parameter transfer or stack frame appears. Macros should name one operation and
expose meaningful values, pointer effects and failure destinations.

Examples from accepted channel handlers:

```c
PM_REJECT_I16_GE(requested, limit_high, 501, "LAB_code_000ff5");
PM_REJECT_I16_LT(requested, limit_high, -500, "LAB_code_000ff5");
PM_STORE_WORD_LE_Y(settings, requested);
```

The first two checks reject values outside -500..500. The caller preloads the
high byte of each signed limit into the original scratch register. The helper
emits the original CPI/CPC and conditional branch directly to the shared error
tail. The full numeric limit in the call explains its meaning; the exact low
byte is derived at compile time.

The store writes low then high bytes and advances the cursor **by one byte**,
leaving it at the high byte. It does not advance past the entire word and does
not change IRQ state. This unusual pointer contract deliberately matches PM.hex.

## Register barriers and flags

`register uint8_t value asm("r16")` describes a local bound register. An empty
ASM capture/barrier communicates original machine state to GCC and emits no
instruction. It is not a new hardware operation. An empty ASM without a memory
clobber is not a memory barrier; do not use it for synchronization.

An ordinary GNU C function does not adopt this private ABI just because a local
variable is bound to R16. Keep the exact adapter at the boundary. Some original
routines return status through carry, alter R1, or return through a shared tail.
C value equivalence alone does not describe those effects. The tested logical C
comments model registers, SREG and control flow explicitly.

Do not merge, expand or restore IRQ windows as a readability edit. Existing
CLI/SEI pairs and ISR save order are original behavior, including surprising
sequences. Helper names must document rather than silently repair them.

## How to review a readability change

1. Read the original byte range and surrounding register/flag use.
2. Extract one repeated operation, keeping constraints, clobbers and sequencing.
3. Build the complete image and run root `make exact-check` after each adoption.
4. Count emitted inline ASM as ASM even when it is hidden behind a readable name.
5. Add a short contract beside the helper and an entry in this guide if public
   within the recovered tree. Preserve archived rejected-C evidence and scope.

`make exact-check` clean-builds ELF/HEX/BIN, compares the full canonical FLASH,
checks original symbols, and audits actual C/ASM compiler provenance. The
acceptance result is zero differing bytes. `make c-comment-check` compiles and
validates logical C comments when new models are added or changed; it is an
auxiliary transition test, not a hardware or timing test. Already byte-identical
changes do not require rerunning exhaustive historical emulator suites.

## Future I2C and other new devices

Write new drivers using ordinary GNU C ABI and normal typed interfaces. Keep
legacy register contracts inside explicit adapters. Before selecting a bus,
check the schematic, MCU port ownership, existing peripheral use and timing.
Choose the integration point and IRQ policy from that evidence; do not assume a
free TWI instance or pins from the MCU name alone.

Keep the current exact baseline as the reference target. A firmware that adds a
new sensor intentionally changes FLASH and needs a separate development build
and tests. The present fixed-region linker rejects new sections and moved code;
it must not be silently relaxed in the baseline. No development target or I2C
driver is implemented by this readability refactor.
