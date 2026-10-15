# Passive FPGA capture: ADT / I2C shared clock

Hardware capture PASS; original MCU application stayed running throughout.
This is a healthy, main-power-ON reference, **not** an unpowered-hub test or an
accepted new MCU firmware. No ADT configuration/reset command was sent.

The new SRAM instrument samples the existing H22 SDA, H21 SCL and B26 RESET_N
inputs, using two synchronizer stages, event compression and JTAG readback.
No SDA/SCL driver or bias was added. The known RESET pull-up and MCU-to-FPGA
SPI slave were retained. ADT CS/DIN/DOUT are not available at these pads.
It is a standalone BRAM/JTAG analyzer, without a LiteX CPU dependency.

## Physical result

* Host-timed sample-clock estimate: 39876044 Hz, approximately
  40 MHz / 25 ns resolution. Host/JTAG latency limits the absolute timing scale.
* Three-second window: 289 records = initial state + 288 SCL transitions.
* All changes alternate falling/rising SCL: 144 complete clock cycles.
* SDA and RESET_N stayed HIGH throughout the window.
* Four console register reads require 16+16+24+16 = 72 clocks; three periodic
  original ISR reads require 3 x 24 = 72 clocks. The recorded burst sizes agree.
* LOW widths: 5, 10 or 11 sample cycles (~125, 251 or 276 ns). The two short/long
  classes correspond to distinct read implementations; no CS was captured, so
  attribution uses the known console transaction and burst timing.
* HIGH widths include inter-byte/frame and idle gaps. The minimum was 16 sample
  cycles (~401 ns); do not treat the largest HIGH interval as a clock period.
* Console returned sensor ID C3 and temperature 30.5 C NORMAL, PSU ON, PLL LOCKED.

| Burst start (s) | SCL transitions | Complete clocks |
|---|---:|---:|
| 0.003673 | 48 | 24 |
| 0.046034 | 32 | 16 |
| 0.046184 | 32 | 16 |
| 0.046329 | 48 | 24 |
| 0.046497 | 32 | 16 |
| 1.014787 | 48 | 24 |
| 2.025901 | 48 | 24 |

This window contains no unexplained extra digital SCL edges and no persistent
LOW clock. It does not exclude analog loading, narrow missed pulses or a fault
under a different supply state. It does not establish correct comparator
setpoints (the old command reads only registers 0..3).

## Reproducibility and restoration

Source: [analyzer](../tools/hardware/fpga_analyzer/README.md).
Simulation covers arm, initial record, changes/timestamps, timeout, rearm,
RAM full stop and indexed report layout. Vendor BSCAN is validated by the
physical indexed readback, not by the behavioral primitive stubs.
Vivado DRC: zero checks; setup WNS 5.504 ns, hold WHS 0.085 ns at an 8.333 ns
constraint. Two RAMB36 blocks hold the event buffer. Build required the shared
Ubuntu libudev preload workaround; an earlier non-preloaded run aborted after
routing and was not used for programming.

FPGA bitstream SHA256:
`e215a8c36572a6ffdd266c1c288f4106c92d3d1550d6c2bfaede08bdd63d15c6`.

Both PM12 and Atmel-ICE leases covered load/capture/restore. No MCU programming,
chip erase, boot/EEPROM/fuse/signature/configuration-FLASH writes occurred.
The known passive FPGA magic 504d5a02 was restored. Final ADT ID C3, 30.5 C
NORMAL, PSU ON, PLL LOCKED and FPGA scratch test PASS/RESTORED were verified.
The unchanged recovered baseline passes `make exact-check`, zero differences.

First attempt `fpga-analyzer-01` stopped the host read too early because it
assumed a 120 MHz clock. Automatic restoration and console checks passed.
The accepted attempt `fpga-analyzer-02` derives its wait from the measured rate.
Its hexadecimal HEADER log line used Tcl `%x`, which truncates bignums to
64 bits; event/count/address validation used full-width arithmetic and all
289 event records are present. The retained tool now uses `%llx` for that
logging line. This logging-only correction does not change capture hardware.

Evidence: [report](validation/fpga-analyzer-2026-10-15/report.json),
[CSV](validation/fpga-analyzer-2026-10-15/events.csv),
[VCD](validation/fpga-analyzer-2026-10-15/capture.vcd).
Full raw load/capture/restore/build logs remain under
`/home/codex-hil/artifacts/pm12-i2c-validation-2026-10-15/fpga-analyzer-02`.

## Remaining power-domain question

The user's unpowered-hub concern remains open. Shared SCL/pull-ups connect to
switched P3V3; ADT's independent P3V3_LDO supply does not prove a usable clock.
A fault-state capture must keep the FPGA sampling clock, core and relevant
I/O bank supplies valid while the hub supply is absent. Loss of FPGA power
also loses its BRAM trace. Do not infer this condition from the healthy trace
or enable off-state ADT reads by removing the current power guard.
