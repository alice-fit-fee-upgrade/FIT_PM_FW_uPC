**Experimental PLL branch: hardware acceptance failed on2026-10-16.**
The PLL-read C candidate passed simulation/local PLL reads but ADT failed after
reset; the board was rolled back to release002. Do not treat this branch build
as a newly accepted deployment. See
[trial record](../docs/pll/c-read-migration-2026-10-16/README.md).

Current direct-MCU I2C candidate: see [candidate contract](../docs/i2c_development_candidate.md).
It preserves the original timer body and uses two small ownership gates instead
of deploying the experimental whole-ISR relocation. Historical release001 below
remains available, not overwritten. Diagnostic compatibility commands from the
previous hardware image are retained explicitly.

# Adding PM12 commands and drivers

`make` / `make exact-check` still build the recovered **binary-exact baseline**.
`make development` builds an intentionally extended firmware in
`development/build/development.elf`, `.hex` and `.bin`.
`make development-check` builds it and validates its layout, command parser and
private/native ABI boundaries. No programming of hardware is performed.

## What is integrated

The extended console namespace starts with `@`, on the original UART and
CR-terminated input, for example `@PING\r` and `@HELP\r`.
The original console reader performs its existing character folding; the new
C parser operates on those bytes. PING replies `PONG\r\n` and HELP lists the
registered commands automatically from the same command list. Invalid arguments/unknown commands/oversized lines have
explicit errors. A line is limited to 47 characters after `@`. Overflow is
drained through CR so the next line remains available. There is no new IRQ,
peripheral initialization, UART driver or sensor configuration.

The hook replays the original dispatcher ANDI/STS and first character read.
This matters: the incoming R16 is the RX **line counter**, not the first command
character. Ordinary command bytes return to the original predicate at 0x12f4
with the post-read registers/flags restored. `@` commands call ordinary GNU C,
then return to the original main-loop caller. The original reader consumes CR
and decrements the line counter; the extension must not reset that counter and
lose a subsequent queued command.

A six-byte JMP+NOP replaces only the displaced dispatcher prefix at
0x12ea..0x12ef **in the development ELF**. Its target is assembled and relocated
by GNU binutils. The build relinks the existing recovered source objects and
new source objects, then updates this one ELF section. It does not splice a
whole golden blob into the image or modify recovered source/reference files.
The development ELF, HEX and BIN all contain the same gate.

New executable code and explicit FLASH constants occupy 0x4000..0x7fff.
The current gate is fixed at 0x4000; adding handlers changes only this reserved
extension area. Linker assertions limit its size and retain every original
region-size assertion. The layout verifier confirms erased reference bytes,
valid JMP destination, unchanged original symbols and bootloader, and rejects
any difference outside the six-byte gate and actual extension section.

## Adding a function

1. Put a standard GNU C function in `development/src/`; all `.c`/`.S` files there
   are included automatically. They use normal AVR GCC flags and calling
   conventions, with no private fixed-register profile.
2. Declare a command handler `void handler(const char *arguments)` in
   `extension.h`, and register its name/handler in `PM_COMMANDS(X)`.
3. Validate argument count, numeric range and output buffer lengths in that
   handler. Keep device timeouts explicit; a new I2C driver must not inherit the
   console's indefinite queue wait as its timeout policy.
4. Use `pm_console_getc`, `pm_console_putc` and `pm_console_write` for the console.
   These are native-ABI interfaces; do not declare an original `cli_*` routine
   as an ordinary C function with parameters/results.
5. Use `PM_FLASH` for persistent constant strings, as in `extension.c`, to avoid
   requiring unimplemented startup copies into SRAM.
6. Add host tests for parsing/driver state and run `make development-check`, then
   `make exact-check`. Inspect the map, stack-usage `.su` and layout report.

For an I2C read command, keep the command parser separate from a typed backend,
for example `status read_registers(uint8_t address, uint8_t reg,
uint8_t *output, uint8_t count)`. Its result buffer can be a bounded local array.
Select TWI instance/pins from the schematic and current ownership before writing
hardware initialization. This infrastructure contains no assumed bus assignment
or falsely successful I2C stub.

## Memory and calling contracts

The GNU boundary preserves all original GPRs, SREG and RAMPD/X/Y/Z/EIND. R1 and
extended banks are set to zero while native C runs, then restored. Each native
console adapter similarly preserves registers and bank/flag state; R24 carries
the ordinary uint8_t argument/result. IRQ policy follows the original console
routines, which contain CLI/SEI. This is a **main-loop interface**, not an ISR API.
No interrupt masking across the complete command is added.

There is no standard CRT initialization at reset in this firmware. Consequently
new `.data`, `.bss`, ordinary `.rodata` requiring RAM, EEPROM and init/fini sections
are rejected. Stack locals and explicitly addressed FLASH constants are supported.
Before adding static state, allocate and initialize an independently reviewed
SRAM region and add overlap/stack checks; do not assume unused RAM from the FLASH
map. malloc and static-state libc APIs are not supported by this initial boundary.

The build rejects dynamic stack frames and native C frames over 128 bytes.
This is a per-function guard, not a total stack guarantee. Current GCC reports
59 bytes of static stack usage for the dispatcher (see `.su` and `stack_usage.json`);
the outer/native console adapters add explicit frames and existing interrupts
can nest. This report is not a measured whole-firmware worst-case stack bound.
Large buffers, recursion, long handlers and additional ISR work need a stack and
latency review. These restrictions cause build/test failures instead of silently
moving legacy code or reserving unverified SRAM.

## Validation limits

The decoded adapter tests exercise 1788 cases, including all non-extension first
bytes, all SREG values for the native gate and native queue adapters, randomized
GPR/bank state, and adversarial callees that clobber all registers/flags/banks.
C command tests cover syntax, argument errors, bounds and overflow recovery.
Negative layout tests prove rejection of differences in vectors, continuation,
nonreserved FLASH and bootloader. They are CPU/state and host tests, not timing
or asynchronous IRQ simulation. Hardware UART, interrupt latency and eventual
I2C operation still require device validation.

## Physical-board development

The agreed additive TWI/UART stage and subsequent measured legacy-function
replacement stage are described in [hardware development tracks](../docs/hardware_development_tracks.md).
Keep binary-different development replacements separate from the exact baseline.

## Historical release001: relocated timer ISR (superseded for I2C candidate)

The original timer vector is unchanged. Four bytes at 0x049E..0x04A1 now contain
JMP to `pm_timer_relocated` in the extension FLASH window; the remainder of the
old ISR body remains untouched. Together with the CLI gate, only ten old FLASH
bytes are authorized for patching. New parser/diagnostic handlers already live
in this window; sensor telemetry itself is not implemented yet.

`tools/relocate_timer.py` deterministically emits symbolic GNU AVR instructions
from the accepted source-built ISR (0x049E..0x08E3). Every instruction gets an
original-address label. Calls/jumps are widened; conditional branches become
inverse-branch/JMP; skip instructions become inverse-skip/JMP so they skip an
entire relocated operation. No whole-image blob is used. The readable C original
is `recovered_exact/src/timer_overflow_interrupt.c`; no baseline source changes.

`tests/check_timer_relocation.py` checks all 489 original instruction sites:
ordinary opcode bytes, internal/external targets, skip successors, fallthroughs
and RETI. The ordinary register/flag save/restore and peripheral operations stay
intact. Widened transfers change cycle counts and call return addresses; this is
NOT binary identity or a proof of IRQ latency/timing equivalence. Hardware tester
comparison against the original is required before deployment.

### Next: mutual exclusion for temperature SPI and on-demand sensor I²C

Voltage/current reads will be on UART demand (periodic polling, if later needed,
can be ten seconds or slower). Do not add slow INA transactions to the ISR.
A shared ownership byte must be explicitly allocated and initialized before use;
the existing no-unreviewed-SRAM linker guard remains enabled. No lock or I²C
transport has been implemented by the relocation change.

The main-loop command handler claims the bus BEFORE driving SDA/SCL; the ISR
claims it BEFORE lowering temperature CS. A busy ISR must defer temperature and
continue unrelated PSU/IRQ duties, never spin. A busy UART handler reports BUSY
or defers bounded work. UART RX/TX interrupts themselves remain operational.
Only a brief save-SREG/CLI/restore-SREG protects an ownership update, not the
whole transaction. Every timeout/error path releases ownership, leaves bus lines
released and records stale/deferred temperature. Bound transaction duration so
thermal monitoring cannot be indefinitely starved by repeated UART requests.

Before enabling direct MCU I²C, resolve the currently removed R226/R227/R228,
FPGA isolation with power absent, independent pull-ups/RESET and monitor/UART
power domains. The current handoff says FPGA is the only connected master.

Final hardware acceptance compares original/candidate with the same tester:
legacy commands, temperature/alarms, PSU faults, concurrent UART/poll scheduling,
NACK/timeout recovery and intended FPGA/DC-DC power-loss conditions. Record raw
traces and firmware hashes. These tests have not been run here.

## PGOOD / read-only ADT investigation branch

`@TEMP ALL` reads registers 0..7 using the existing native byte timing. The
ordinary `@TEMP` response stays registers 0..3. Registers 2/4/6/7 are 16-bit;
the others are 8-bit. The read command does not change sensor configuration; the separate explicit RESTORE command below does.
This branch starts from 9b3abb2 (minimal ownership gates and INA diagnostics),
not the later reset-ready/slow-cell experiments. Host tests must wait for stable
physical PGOOD before these reads. This is an intentional development image;
canonical recovered exact firmware remains unchanged.

`@TEMP RESTORE` is an explicit, state-changing laboratory command. It sends
CONFIG=50, TCRIT=2300 and THIGH=1E00 through the unchanged native byte cell.
It does not send a serial reset or modify THYST/TLOW, and it is never invoked
by `@HW`, ordinary reads, startup or an automatic retry. The host must qualify
PGOOD and verify all registers afterward; the reply means SENT, not PASS.
The eight transmitted bytes are checked by the host GPIO framing harness for
both initial IRQ states, with power/busy cleanup and GPIO restoration checks.

The existing timer temperature hook now also checks PORTE_OUT.EN_PSU and both
PORTE_IN readiness bits (mask 0x0a). If not ready, it jumps through the original
ISR epilogue before acquiring the bus or clocking ADT. Other PSU ISR work still
runs. Owner/pending/defer are unchanged on this power-unavailable path.
Decoded instruction tests cover 5376 unready GPIO/SREG/ownership contexts and
1536 ready mutex contexts, including original register/SREG restoration.
This does not yet defer the original startup configuration writes: the explicit
RESTORE laboratory command remains separate from the read gate.

## PLL register-map readback candidate

On `development/pll-c-recovery`, `@PLL ALL` reads CDCE62005 registers0..8.
Each line is `CDCE62005 REG=00 XXXXXXXX` (register index then raw32-bit word).
Plain `@PLL` retains its register8-only response. Only read-selector commands
0x0e..0x8e and dummy clocks are sent; there is no profile, reset or EEPROM-copy
command in this diagnostic. Each register transaction independently restores
IRQ state, SPIC configuration and inactive PLL LE. The nine-register dump is
not an atomic snapshot across all registers; register8 status can change.

Host runner: `python3 tools/hardware/pm12_pll_readback.py --all --out REPORT.json`.
It needs this candidate installed; without `--all` it can inspect the existing
PGOOD firmware. No programming/reset/EEPROM operation is performed by the runner.
See `docs/pll/` for software tests, physical baseline and deployment status.

## Legacy PLL register8 C migration

The development image additionally patches only the first four bytes of
`FUN_code_001267` at0x24ce with a JMP to a new-FLASH ABI adapter. The original
body remains available in place and in the exact baseline. Ordinary C in
`src/pll_transport.c` retains the original blocking SPI read transaction.
`@PLL LEGACY` exercises that original entry; unlike plain `@PLL` it retains
blocking completion polling and is a healthy-controller laboratory test.
See [migration evidence](../docs/pll/c-read-migration-2026-10-16/README.md).
The layout verifier explicitly protects the rest of the original PLL body.

## Opt-in scaling simulation pass

`make scaling-check` builds the isolated arithmetic candidates and runs full
application input-domain comparisons. Ordinary development builds exclude
`candidates/scaling/`. See [scope, results and limitations](../docs/scaling/README.md).
The two old arithmetic entries retain their bodies and receive only four-byte
JMPs in this optional mode. No hardware programming or release is performed.

## Opt-in DAC driver and48-setting updater

`make dac-check` enables both the arithmetic candidates and two extra legacy
entry gates into ordinary C DAC send/update functions. Original bodies and
all addresses remain retained. Full-frame/CPU/IRQ simulation results and the
DAC8554 no-readback explanation are in [docs/dac](../docs/dac/README.md).
Ordinary development builds exclude this code; there is no new console write
command, automatic programming or hardware-qualified release.
