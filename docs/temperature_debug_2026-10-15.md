# ADT7311 / red LED investigation, 2026-10-15

Hardware investigation is in progress. No I2C candidate is accepted or released.
Exact `recovered_exact/`, `exact_asm/` and reference data are unchanged.

## Separate three different states

* ADT7311 compares temperature with its own setpoints. In 13-bit mode its
  temperature register carries TCRIT/THIGH/TLOW flags in bits 2/1/0.
* The original timer ISR reads that register (`0x077e..0x07be`), maps these
  flags to state bits `0x2157 & 6`, and can latch software failure flags.
  It does not perform a numeric temperature/threshold comparison there.
* `GPIOR0 & 2` drives the status LED through `set_status_and_vd8_led`, but
  can also be set by power/startup faults. It is not a dedicated thermal latch.
  A `NORMAL` console temperature does not prove this shared latch is clear.

Sources: `recovered_exact/src/timer_overflow_interrupt.c`, `status_led.c`,
`power_porte_interrupt.c`, `alarm_state_clear.c`, `application_main.c`;
[ADT7311 datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/adt7311.pdf),
register tables and serial-interface chapter. A status-register read can clear
sensor interrupt flags; even diagnostic reads are not entirely side-effect-free.
The schematic connects ADT7311 CT to `OVERTEMPn` in the hardware power-good path.
The physical rear LED designation has not yet been positively identified.

## Reproducibility control

A clean build of diagnostic commit `f9869c9` equals the pre-update physical
backup across the full 139264-byte FLASH:

```
56fe5ccb19340b05a8810d93beb5cb496d24e7c1f7e0418828923b30ad2806d9
```

The canonical original PM.hex is a different, separately preserved reference.
No source mismatch in this clean diagnostic control was found.

## Trials and limits

Evidence root on this host:
`/home/codex-hil/artifacts/pm12-i2c-validation-2026-10-15/`.

| Trial | Change / observation | Result |
|---|---|---|
| earlier candidates 1/2/3 | full ISR relocation, then native TEMP wrapper removal, then original ISR with two small temperature gates | all failed the ADT ID/temperature gate; exact previous application restored |
| `alarm-debug-current.json` | read current restored console only | 31.2–31.3 C NORMAL, ID C3; no actual setpoint capture possible with this firmware |
| `alarm-probe-program` | `f9869c9` plus GPIOR0/PA.OUT capture and ADT registers 4–7; no I2C/ownership gates; normal sparse application write | all ADT reads zero; therefore the ownership gates are not a sufficient explanation |
| `alarm-probe-full-app` | same probe image, bounded full application write | ID C3 and temperature returned; GPIOR0 changed 00→02, suspicious config/setpoint readings; not accepted as reliable configuration evidence |
| `control-f9869-sparse` | exact known old binary, normal sparse application write | ID C3, 28.6 C NORMAL; sparse writes alone do not explain the failure |

The full-application probe reported CONFIG=1F, TCRIT=4980, THYST=05,
THIGH=2000, TLOW=FFD0. These are raw observations, not verified settings:
we have not established that the native transfer is reliable. Do not claim
that comparator settings were preserved, nor that these values prove a write.
The old firmware's startup contains a serial reset and writes CONFIG=50,
TCRIT=2300 and THIGH=1E00; source inspection does not prove their readback.

Every programming trial verifies canonical FLASH and compares boot, EEPROM,
fuses, lock, user/production signatures and device signature with the physical
backup. Probe mode restores the exact previous application and the known passive
FPGA SRAM image before releasing both hardware leases. No chip erase or
protected-memory/configuration-FLASH content write is used.

## Next controlled experiment

Replace only the probe's native SPI byte transfer with an adapter calling the
unchanged original `adt7311_byte_rw` at 0x2608. Preserve native callee-saved
R16/R17; the original byte cell clobbers R18/SREG. Keep whole transaction IRQ
exclusion and CS handling. The adapter passed 256 decoded-instruction ABI
cases with substituted byte results; this does not prove electrical timing.
Read configuration/setpoints repeatedly and check whether values and flags
remain stable. No reset or intentional sensor-register write is added.

Reproduction fixtures in `tools/hardware/fixtures/` apply to diagnostic commit
`f9869c9` in a separate worktree. These are experiments, not accepted firmware.

## Subsequent observations

`alarm-legacy-cell-program`: three normal-speed reads through the original
bit cell returned CONFIG=00, TCRIT=43FF, THYST=05, THIGH=2000, TLOW=FFD0.
Temperature stayed 27.4 C NORMAL while GPIOR0 changed 00→02. The repeated
values are useful evidence, but an independent slower read is still required.
No hardware alarm cause has yet been identified from this shared software bit.

`candidate3-full-app`: the existing I2C candidate, without another code change,
passed ID C3 before I2C and after restoring the passive FPGA and reading all
three monitors. Temperatures were 27.1 C before / 29.3 C afterward. Canonical
FLASH readback and every protected region matched the expected image/backup.
This establishes a successful run, not why earlier runs failed. Full-application
programming is the currently successful procedure; it is not a proven root cause.

`after-candidate3-console.json` / `candidate3-console-comparison.json`: 33 common
read-only/guarded commands compared, 27 replies exactly equal. The six different
replies were HELP additions, HW live counters, two RS temperatures, TEMP data,
and the FPGA scratch-test write counter. RC/RF parameter output and other static
responses matched. State-changing/calibration/power commands remain NOT_RUN.

`candidate3-stress.json`: 12 cycles, 60 POWER requests, 178 completed monitor
reads and two expected BUSY results. ISR deferral counter increased to 2 and
ownership returned to zero. Each cycle retained valid ADT ID/temperature,
PSU ON and PLL lock. Runtime 16.621 s. This is real contention coverage, not
an injected stuck-bus, DCDC-loss or thermal-fault test.

The next candidate adds `@TEMP ALL` (original bit cell), `@TEMP SLOW` (C, 1 us
phase delays), and GPIOR0/PA.OUT in HW output. Both reads must agree on the
configuration and all setpoints before and after I2C. Native TEMP also refuses
any active owner/stuck-bus latch before driving the shared clock. No intentional
ADT reset/configuration write has been added.

A separate static issue: original startup addresses the sensor immediately after
its 32-one serial reset. The datasheet specifies at least 500 us before another
address; the source has no such delay there. This is a hypothesis for startup
configuration readback, not yet a demonstrated explanation of the earlier zeros.

## Reset-ready / alarm-history experiment

Candidate `17bccd7` (candidate4) failed again: normal and slow SPI both returned
zero, including ID. The full application write alone therefore does not solve
this problem. It was rolled back; protected memory comparison and restored
runtime (ID C3 / 29.0 C NORMAL) passed.

The next experiment redirects only the original CALL at 0x1100 to an adapter
that calls the same serial reset and waits >=600 us before returning. The
following original CONFIG=50, TCRIT=2300 and THIGH=1E00 writes are untouched.
The counted loop is >=19201 cycles at 32 MHz. Sixteen decoded ABI cases verify
that the adapter preserves every register and SREG returned by the reset.

The already-required temperature release helper now stores last/sticky raw
sensor alarm bits in the low bytes of disabled TCE0 compare channels C/D
(0x0a2c/0x0a2e). These storage channels are cleared in the existing diagnostic
TCE0 init. No compare outputs or interrupts are enabled. HW prints TEMPFLAGS
and TEMPSEEN alongside the shared GPIOR0 flag. Bit 2 means TCRIT, bit 1 THIGH,
bit 0 TLOW. This distinguishes thermal samples seen since reset from unrelated
power/startup faults; it does not retrospectively reconstruct a previous boot.
3840 decoded release cases verify unchanged registers, SREG and successor
branches plus the two explicit telemetry writes. Canonical ISR bytes outside
the existing gates remain untouched.

Candidate5 (`6faaa1c`) recovered normal-speed ID C3 and temperature, but was
rejected: slow reads returned C2 instead of C3, CONFIG FE vs FF, THYST 04 vs 05
and TCRIT 43FE vs 43FF. The last data bit is a specific sampling issue.
The datasheet permits DOUT to go low shortly after the final rising/inactive
SCLK edge (t10). The initial slow experiment incorrectly waited a whole us
after that edge before sampling. The corrected slow read captures settled
DOUT during the low phase before raising SCLK. A linked-AVR protocol test models
DOUT going low after the last rising edge and validates all eight registers,
including odd final bytes. This is not an electrical timing measurement.

Candidate5 also reproduced shared GPIOR0=02 with TEMPFLAGS=00/TEMPSEEN=00 and
normal temperature: no sensor alarm bits were observed by that ISR since reset.
The initial reset-ready change alone did not establish correct setpoint readback;
no claim that the startup issue is solved is made.

Candidate6 (`5eecbba`) still returned zeros through both read methods before I2C.
It was rejected and restored. This rules out correcting the diagnostic sample
point alone as a sufficient fix for startup/interface failure.

Next controlled candidate also redirects the original SPI byte entry 0x2608
through a four-byte JMP. All original reset/configuration/ISR callers keep their
addresses and private ABI. The shared-clock cell adds 32-cycle (1 us) phases,
sets DIN before the falling edge and captures DOUT before the rising edge.
It preserves the private byte-cell GPR/SREG contract under stable MISO (192
linked instruction comparisons with golden). Both linked native read paths pass
16 register cases where DOUT can fall after the final rising edge. This tests
whether the added shared-clock load makes the original short phases unreliable;
no electrical waveform measurement has yet been made. Acceptance now requires
both read methods to agree with the original intended startup settings:
CONFIG=50, TCRIT=2300 (70 C), THYST=05, THIGH=1E00 (60 C), TLOW=0500 (10 C),
and retain those settings across FPGA restoration and INA reads.

Candidate7 (`8b6757a`) returned ID C3 and matching normal/C-slow configuration
reads, but CONFIG=1F, TCRIT=43FF, THIGH=2000 and TLOW=FFD0 failed the required
original-settings check. It was rejected and the old application/passive FPGA
were restored. TEMPSEEN remained zero despite the shared system-fault latch.
The slow shared cell alone therefore does not establish correct startup writes.

The next explicit laboratory experiment adds `@TEMP INIT`. It sends the same
32-one reset, waits >=600 us, then sends the original CONFIG=50, TCRIT=2300
and THIGH=1E00 writes after the board is already powered. No new threshold
values are chosen. TEMP ownership excludes ISR SPI across the operation; IRQs
remain available during the reset wait, and each selected frame has bounded
IRQ exclusion. The reply explicitly says SENT/VERIFY_READBACK, not PASS.
The programming supervisor invokes this only with `--recover-adt`, records
before/after data and requires both read paths to match the original settings.
Host GPIO framing tests verify all 12 transmitted initialization bytes and the
busy/power failure cleanup. This is a state-changing diagnostic, not an ordinary
read-only console regression command, and is not automatically invoked at boot.


## Shared-clock power-domain limitation / candidate8

Candidate8 (`6d310af`, evidence `candidate8-program/`) returned zero ADT
registers before recovery; explicit `@TEMP INIT` returned `ERR POWER`.
The failing initialization frame is not instrumented, so this does not prove
that main power was already absent at command entry. The supervisor restored
the exact previous application and passive FPGA; rollback runtime validation
passed with ID C3, NORMAL temperature and PSU ON.

The ADT and MCU being powered from P3V3_LDO does **not** establish that ADT
communication works with main P3V3 absent. PA1/SCLK is shared with the TCA
upstream SCL, whose pull-ups R224/R225 are on switched P3V3. An unpowered hub
or a pull-up to an unpowered rail can affect the shared clock. Neither the
actual off-state waveform nor hub clamping has been measured. Idle GPIO
readback cannot establish edge timing or absence of back-powering.

A proposed removal of native temperature main-power guards was withdrawn
before programming; candidate9 was never flashed. Keep those guards pending
power-domain investigation. This is not a demonstrated electrical root cause
of previous failures. Next establish SCL amplitude/edges and P3V3 correlation
with physical instrumentation, and identify the failing INIT frame if needed.
Do not claim diagnostics during a main-rail failure have been validated.


## Passive FPGA observation

See [physical shared-clock capture](fpga_shared_clock_capture_2026-10-15.md).
A powered reference captured exactly the expected 144 SCL cycles (console +
three ISR reads), with SDA/RESET HIGH and valid ADT ID C3. This does not test
the unpowered-hub condition. Original MCU and passive FPGA remain restored.
