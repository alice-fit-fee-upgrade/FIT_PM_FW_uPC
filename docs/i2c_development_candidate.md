# Direct MCU I2C candidate (2026-10-15)

User authorized direct-MCU sensor support, before/after console tests and physical
programming after confirming wiring. Bring-up explicitly handed over ownership.
Only this thread acquires PM12/ICE leases for the update; its checkout is untouched.

This candidate explicitly integrates the already-running diagnostic modules from
fit-pm12-mcu-diag code f9869c9, preserving TEMP/TDC/PLL/PGTIME/FPGA/CFG/FLASHREAD
commands. Existing PG observation hooks are retained after review (not imported
as an unrelated power-policy change); hooks inside the relocated timer return to
the relocated successors. No changes to golden, exact_asm or recovered sources.

## Bus ownership

GPIORE (0x0e) is saturated temperature deferral count. GPIORF (0x0f) bits0..1 are
owner (1=main I2C,2=temperature), bit7 pending temperature, bit6 stuck-SCL fault.
They are reset-cleared hardware scratch registers, explicitly initialized in the
retained startup hook, not uninitialized SRAM. Original application/boot direct
IO references do not use these registers; original GPIOR0 remains unchanged.

Main code atomically claims ownership, preserving SREG/I. ISR claims before
ADT7311 and releases immediately after; busy ISR marks deferred and skips only
temperature-dependent work, not earlier PSU processing. Deferred temperature
prevents another I2C request until the timer services it. No spin in ISR and no
interrupt mask across I2C. Busy responses are explicit. Native TEMP is serialized by its preexisting short IRQ exclusion and main-loop
execution; it does not add a redundant ownership wrapper. Other existing short
SPI/TDC diagnostic IRQ exclusions are retained.

PA1 SCL and PF7 SDA are open-drain during I2C; PB6 RESET remains alarm input.
PF7 internal pull-down is removed and SDA remains released between commands.
After STOP, PA1 returns to the original output-HIGH clock when it is physically
HIGH. A stuck LOW leaves it released and latches a fault that blocks further
SPI/I2C ownership; diagnose/repair and reset rather than driving against a target.
Other PSU/IRQ processing continues; stale temperature is a limitation of a stuck
shared clock and must not be mistaken for a fresh reading. Native read budget is
2000 stretch polls at nominal 4 us shared by the whole monitor transaction.

## Commands

* `@I2C`: input line levels, ownership/pending/fault byte and deferred count.
* `@INA 4`, `@INA 5`, `@INA 6`: save both TCA masks, disconnect both, select one
  branch on 0x71, verify IDs 5449/3220, read config and six raw measurement words,
  restore and verify original masks, release bus before printing.
* `@POWER`: perform the three monitor reads on demand, no new periodic ISR work.

INA reads use repeated START, MSB first, ACK then NACK. No configuration writes.
Output contains raw words, mV and nominal uncalibrated mA; R15/P3V8 and TP1/TP2
never yield a current. P3V8 value is bus input only. Clipped shunt codes yield NA.
Current board monitors/muxes require switched P3V3: unavailable/NACK is not 0V.
Mask restore is attempted after errors and failed restoration is explicit.

## Validation scope

Line-level host tests compile actual C: mux routing/restoration, IDs, ACK/NACK,
repeated START, signed decoding, busy/deferred/off/reset guards and bounded
stuck/clock-stretch failures. Decoded ISR tests cover every SREG/owner combination.
Software checks do not prove hardware timing or power-off electrical isolation.

Before-image console capture and fresh physical-memory backup are under
`/home/codex-hil/artifacts/pm12-i2c-validation-2026-10-15/`.
Legacy persistent/programming/calibration and power-changing commands are not
replayed as a console regression; captures list those exclusions. Config HOLD/
BOOT/flash reads require a separate sequence restoring known FPGA SRAM. No
EEPROM, fuse, bootloader or configuration-FLASH content writes are authorized by
this implementation. Programming uses application-page erase and full readback,
with exact rollback to the current diagnostic application if validation fails.

## Iteration record

Initial ab52b49 image read all three INA monitors correctly but failed ADT7311
ID/temperature regression. Exact previous FLASH/protected memories were restored;
the old firmware recovered C3/normal temperature. Repeating preflight isolated
failure before any I2C transfer or FPGA reload. Do not call that candidate a
hardware PASS. The next isolated iteration removes the redundant native TEMP
ownership wrapper while retaining ISR-versus-main I2C protection and stricter
preflight/rollback. Physical evidence is outside the repo until final review.

Third iteration retains the original timer body in place. Two four-byte JMPs at
0x077E/0x0788 redirect only temperature acquisition/release to small ASM helpers
in free FLASH, replaying displaced instructions and retaining the original
successors/flag behavior. No vector or original ISR entry changes. This narrows
the physical regression surface after both full-relocation variants failed
pre-I2C temperature checks and were rolled back with exact memory verification.
The prior whole-ISR relocation remains in history/release001, not as this image's
active timer path. Tests check original ISR bytes outside explicit hooks plus
all SREG cases for acquisition/defer and displaced release instructions.
`@I2C TEST` performs a brief, SREG-preserving scratch-register readback test.
