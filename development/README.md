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
