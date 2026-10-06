# Two hardware development tracks

Agreed direction: first add peripherals through the existing development gate;
then replace legacy functions incrementally using measurements on the physical
PM12 board. The binary-exact source remains a reproducible reference target.
Hardware validation has not yet been performed in this task.

## Track 1: additive TWI and UART commands

Keep recovered handlers, UART queues and original peripheral configuration.
Add a typed, normal-GNU-ABI TWI driver and new `@` command handlers under
`development/src/`. The existing explicit adapters preserve the private MCU
register/flag contracts. Existing commands continue through the original path.

Before initializing TWI, record the board connector, MCU instance, SDA/SCL pins,
voltage, pull-ups, pin ownership and first sensor/address. The user supplied SDA=PF7, SCL=PA1 and switch RESET=PB6 on
2026-10-06. See `i2c_pin_assignment.md` for existing ownership conflicts. Bus
voltage, external pull-ups, switch model and first sensor/address remain unknown.
Do not initialize or drive these pins until electrical sharing is resolved.

The backend must expose bounded transactions and explicit results for timeout,
NACK and bus failure. Command handlers validate address/register/count arguments
and return measurements through the existing native console wrappers. Sensor
reads and new UART commands execute in main-loop context. Add parser/backend
software tests and run `make development-check`; keep `make exact-check` passing
for the separate original baseline. Physical acceptance includes repeat reads,
error handling, recovery and simultaneous legacy command/interrupt activity.

The extended image intentionally differs from PM.hex. "Unchanged" here means
preserved legacy implementation and configuration, not an identical full image
with additional code. New SRAM state needs reviewed allocation/initialization;
this initial development linker rejects unprepared data/bss globals.

## Track 2: measured function-by-function replacement

Use MCU PDI for programming/debug and FPGA JTAG for the FPGA debug path.
Determine what SPI/register observation the loaded FPGA design actually exposes
and whether additional debug logic is needed. Do not assume that attaching JTAG
alone provides a generic SPI/register API. Record the FPGA image and debug setup.
If instrumentation changes the FPGA image, collect the original-MCU reference
measurements with that same instrumented FPGA before comparing a candidate.

Define a debugger/probe pin map: signal, connector, direction (observe/drive),
voltage, trigger and capture settings. Establish electrical ownership before
actively driving a GPIO or bus signal. Keep observation settings repeatable.

For each function:

1. Capture the original firmware response and GPIO/SPI/register traces for
   explicit input cases on the board. Record clocks, reset state, EEPROM image,
   FPGA configuration, instruments and both firmware/image hashes.
2. Replace only one function or a small coherent fragment in the development
   target. Keep other handlers and hardware initialization stable. A candidate
   with different codegen belongs to this target, not the exact baseline.
3. Build and check the allowed layout changes. The current layout checker permits
   only the additive gate/extension section: replacing an original region needs
   a separately reviewed per-function allowed-difference range. Never relax it
   to permit arbitrary changes across FLASH.
4. Program the MCU via PDI and replay the same input cases. Compare payloads,
   register values, signal order, IRQ behavior, duration/timing limits and fault
   paths according to an explicit acceptance contract.
5. Run the caller/system regression as well as the local test: legacy console,
   queues/flow control, active interrupts, normal operation and applicable
   programming/reset paths. Save raw captures, not only a PASS label.
6. Commit the candidate and evidence after acceptance; keep an identified image
   and programming procedure for return to the measured reference firmware.

A tested binary-different function is a development replacement. It must not be
classified C_BINARY_EXACT or increase the original baseline's exact-C coverage.
The original exact-check remains canonical byte identity, while physical
acceptance for this development track uses recorded behavior and timing criteria.

## Evidence record for each hardware trial

| Field | Required record |
|---|---|
| Trial | Identifier, function/region, source commit |
| Images | Original/candidate MCU SHA256; FPGA image identifier/SHA256 |
| State | EEPROM SHA256, clocks, reset/power and relevant registers |
| Setup | PDI/JTAG tools, probe/pin map, sampling and triggers |
| Stimulus | Exact UART commands/input values/fault injection |
| Contract | Expected output, register effects, waveforms and timing bounds |
| Capture | Paths to original and candidate raw logs/traces |
| Result | PASS / FAIL / INCONCLUSIVE with scope and reason |

Do not label a simulated or software-only test as physical validation. The next
concrete inputs are the selected TWI pins/sensor and the available FPGA/probe
setup. Toolchain preparation is not proof that Vivado is installed; inspect the
shared environment according to `/home/codex-hil/docs/toolchains/vivado.md`
before FPGA implementation work.
