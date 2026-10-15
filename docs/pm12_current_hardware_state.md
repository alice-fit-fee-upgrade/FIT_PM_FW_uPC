# PM12 current diagnostic hardware state — 2026-10-15

At the user's request, the passive shared-clock analyzer is **left resident**
in FPGA SRAM (USER1 magic 504d5a03), with the original diagnostic MCU firmware
unchanged. No boot/configuration-FLASH write. The trace is stopped and retained
in BRAM, ready for explicit rearm; it is not continuously recording.

Validation `fpga-analyzer-resident-02` loaded and retained it. A subsequent
`fpga-analyzer-resident-03` recognized the existing image, skipped programming,
rearmed it and read another valid capture. Final ADT ID C3, NORMAL temperature,
PSU ON and FPGA SPI scratch PASS/RESTORED were checked. GPIO readiness was
confirmed before sensor commands. Both board and ICE leases covered the runs.

The first retain-mode attempt had a host-script argument-variable bug after
capture. Its automatic rollback and runtime checks passed; the script was fixed
before the two successful retain/rearm runs. This was not a firmware acceptance.

[Final report](validation/fpga-analyzer-resident-2026-10-15/report.json)
contains raw GPIO readiness samples, fresh pre-sensor rechecks and console replies.
The hardware question about an unpowered hub remains open: these runs had PSU ON.

## Power readiness before host-requested reads

`tools/hardware/pm12_power.py` polls `@HW`, a GPIO/SRAM-only snapshot with no
ADT transfer. It checks PORTE inputs PE1 IN_PWR_OK and PE3 P1V2LDO_PG, plus
physical PE2 EN_PSU readback (mask 0x0e). Require >=3 good snapshots spanning
at least one second, timeout 20 s; a low/malformed snapshot resets readiness.
Take a fresh snapshot immediately before a sensor command and abort if bad.

The analyzer, console regression, candidate programming/recovery/rollback and
sensor stress runners use this gate. The old fixed eight-second pre-read delay
in the programming runner was replaced with the physical readiness gate.
This is host sequencing, not a modification of the original MCU ISR or startup.
Do not reinterpret previous failures as solved merely by this change.

Software validation: Python compilation; all 256 PORTE values and invalid reply
checked; simulated unstable-to-stable and never-ready timeout paths checked,
issuing only @HW. Physical readiness/capture/rearm and retained SPI/temperature
checks passed. Candidate programming and I2C stress were not rerun: the board
still runs the original diagnostic MCU application, without the new I2C firmware.

## Continue without reloading FPGA

```
python3 tools/hardware/fpga_analyzer/run_capture.py --leave-loaded --out <new-evidence-directory>
```

Do not automatically restore 504d5a02 after successful captures while this
resident mode is requested. Restore the known passive image only if a capture
or validation fails. FPGA power loss or configuration reset (including an MCU
programming/reset sequence that asserts PROGRAM_B) can remove the SRAM image;
after such an operation, deliberately reload this analyzer before continuing.
Existing older MCU programming tooling still restores its known 504d5a02 image,
so that tool alone does not preserve analyzer residency across MCU programming.
