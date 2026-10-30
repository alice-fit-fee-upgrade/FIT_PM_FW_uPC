# PM12 ATxmega128A3U — digital twin handoff, 2026-10-30

## Snapshot and acceptance contracts

Repository: https://github.com/alice-fit-fee-upgrade/FIT_PM_FW_uPC
Branch: `development/dac-c-simulation`.
Firmware source checkpoint: **f0b2c7ef68e3ac737a1db17239a1af6f3a5f0ef1**.
Local checkout: `/home/codex-hil/fit-pm12-pll-c`.
The later handoff commit adds documentation/build scripts only; obtain its ID with
`git log -1 --format=%H -- docs/handoffs/digital-twin-2026-10-30/HANDOFF.md`.

Target ATxmega128A3U, device signature 1e9742, nominal CPU32MHz, programming PDI.
All addresses in this document are **byte addresses**. AVR instruction encodings
and legacy tool labels may use word addresses. Model the actual three-byte PC
return stack; do not assume a two-byte AVR return address.

Three distinct contracts must remain separate:

| Image | Canonical FLASH SHA256 | Acceptance |
|---|---|---|
| Original/recovered exact | e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0 | Complete binary identity, zero differences |
| Default development, PLL C gate | 9ece53e796f4089d1bbd7aaf1d6c00fa52fc4780e53fe2edec487f0ce9726d0a | Experimental; system HW regression FAILED |
| Optional DAC + scaling development | f98cae124c45c272935c1c22c5c70ab4fa082aba518a6d8c38f0c2256becfa69 | Simulation qualified ONLY |
| Historical physical rollback, release002 | a016c00d39617d106ca855c03a62170d918f2e9a93514e7962bdc0cb7017c23e | Last verified rollback; not a current live observation |

Canonical FLASH is 0x22000/139264 bytes: application0..1ffff, boot20000..21fff;
HEX holes are FF. EEPROM0x800 and user signature0x200 are separate images.
Do not modify `reference/`, `exact_asm/`, `dump/` or original analysis. Verify
`reference/SHA256SUMS`. The attached ELF is not presumed to be an executable;
its forensic section analysis is in `docs/forensic_inventory.md` and
`docs/original_elf_readelf.txt`. Golden FLASH is authoritative, annotations are hypotheses.

## Changes already implemented

`recovered_exact/`: accepted address-preserving mixed C/ASM. 89 application
functions:5 C_BINARY_EXACT,83 C_WITH_EXACT_ASM_HELPER,1 ASM_EXACT; boot9 ASM.
Accepted C instruction bytes7000/10836 (64.5995%); inline ASM counts as ASM.
Historical binary-different experiments do not increase these counts.

`development/`: separate extension window4000..7fff, GNU C ABI behind legacy
bridges. CLI gate12ea, temperature ownership gates077e/0788, retained PGOOD
observation/startup hooks; all allowed changes checked by layout verifier.
Timer body is retained, not the historical whole-ISR relocation. Shared I2C /
ADT ownership, bounded bitbang I2C, read-only INA diagnostics and powered guards
are implemented. See `development/README.md` (historical sections explicitly
superseded by later sections), `docs/i2c_development_candidate.md`.

PLL gate24ce -> C register8 read with private ABI bridge. Original body retained.
Optional scaling gates2130/214c use C for coefficients4188/020c/0272 only;
other coefficients replay original prefix then ASM. Optional DAC gates22aa and
09de replace SPI sender and48-setting updater; four preparation wrappers retained.
No candidate changes bootloader or writes EEPROM as part of its build.
Commit milestones:4674a9a PLL; d8c42a0 failed trial/rollback; e01d685 final state;
9c52fb1 scaling;08e6830 DAC;f0b2c7e simulator waiting gate.

## Reproducible software-only build

Read `/home/codex-hil/docs/toolchains/heavy-builds-remote-handoff.md` first.
Build on Dell `codex-hil-Precision-5820-Tower-X-Series`,192.168.2.7. Hardware stays
on its owning debug station. No programmer, device, UART or network is passed
into the build container. No existing Dell launcher profile covers PM12, so the
included standalone launcher uses the same shared build lock and CAD userspace.
Do not nest it inside `dell-build run` (that would take the lock twice).

Toolchain: Ubuntu extracted GCC7.3.0+Atmel3.7.0, binutils2.26.20160125,
avr-libc2.0.0. Exact per-file options live in `recovered_exact/Makefile`;
new GNU C uses `-mmcu=atxmega128a3u -Os -std=gnu99 -Wall -Wextra -Werror
-ffunction-sections -fdata-sections -fno-lto -fstack-usage`. No LTO/backend patch.
Dependencies: Python3 (3.14.4 on Dell host; container version in build log), make,
host C compiler for host tests, coreutils, bash; rootless Podman for Dell launcher.
No pip dependency is required for the build. Existing custom decoded-AVR test
interpreters live in the repository; they are not a full cycle-accurate twin.

Pinned package SHA256 (packages retained on Optiplex under `.local/opt/avr/packages`):

```
a810bfd9103264bfb225105db1d39c379882b4f3b3fc964575bc9776d45f257f  avr-libc 1:2.0.0+Atmel3.7.0-1
a4e84a866d03d87e5d5d1d1aa2284991f665c62834014c741149d8defe63040d  binutils-avr 2.26.20160125+Atmel3.7.0-2
ac9dae48c82b1f174434fa04de543014fbaf334c787ba16adfb7fc26eb180ce9  gcc-avr 1:7.3.0+Atmel3.7.0-1
```

Dell toolchain copied from the shared installation, not replaced globally:
`/srv/nvme-fast-2/toolchains/pm12-avr-7.3/usr`.
Runtime: `localhost/dell-cad-base:20261005` with read-only usr/etc/var from
`/srv/nvme-fast-2/toolchains/cad-runtime/rootfs`. This is an available laboratory
runtime, not a claim of a self-contained published container distribution.
Build log records tool versions; runtime inspection is retained in `runtime.txt`.

Exact clean reproduction of the transferred snapshot on Dell:

```bash
cd /srv/nvme-fast-4/remote-builds/pm12-digital-twin-f0b2c7e
bash tools/handoff/dell_build.sh > handoff-build.log 2>&1
sha256sum -c handoff-artifacts/SHA256SUMS
```

For a fresh clone with the pinned toolchain on PATH, the inner commands are:

```bash
unset PM_DAC_CANDIDATE PM_SCALING_CANDIDATE
make clean
make exact-check
make -C development clean
make development
# Save default outputs BEFORE the next variant overwrites development/build.
make -C development clean
PM_DAC_CANDIDATE=1 make development
```

`tools/handoff/build_offline.sh` performs exactly this sequence and saves all
variants separately. Golden reference is never generated by this script.
It builds ELF/HEX/canonical BIN and runs exact baseline checks; it does not run
the expensive historical behavioral matrices. To repeat them, separately use
`make dac-check` in the same Dell runtime; this also runs the development checks,
scaling suites and DAC suite. `make scaling-check` excludes DAC; root
`make development-check` checks the default development image. These overwrite
`development/build`, so archive each result with its variant name.
Never use `flash`, `program`, fuse targets or `tools/hardware/*` for reproduction.

## Artifacts and integrity

New clean-build outputs: Dell path above, `handoff-artifacts/{exact,development,dac}`.
Fetched local bundle:
`/home/codex-hil/artifacts/pm12-digital-twin-handoff-2026-10-30/`.
`SHA256SUMS` beside this document lists all delivered ELF/HEX/BIN/maps/disassembly
with individual hashes (paths relative to the fetched bundle). ELF debug paths
may differ between workspaces; canonical BIN identity is authoritative.
`reference_SHA256SUMS` preserves the original reference manifest.
`handoff-build.log` is fresh clean-build evidence; these are NOT new hardware tests.
Original DAC software outputs/evidence remain under
`/home/codex-hil/artifacts/pm12-dac-simulation-2026-10-17/` and
`docs/dac/validation-2026-10-17/`.
Raw physical EEPROM/fuse/signature backups remain private outside Git, under
`/home/codex-hil/artifacts/pm12-pll-validation-2026-10-16/c-read-trial-01/`.
Do not publish those memory dumps as generic twin fixtures without review.

## Validation: evidence versus assumptions

| Evidence | Result | Limits |
|---|---|---|
| Fresh Dell clean exact build | Canonical original equals rebuilt,0 differences | Binary identity, no new HW execution |
| Fresh default and DAC builds | Match historical canonical candidate hashes | Build reproduction only |
| Historical DAC matrix |917504 sender +2048 updater comparisons;98304 frames | Compiled AVR, full GPR/SREG/banks, ordered RAM/MMIO/SPI/IRQ traces |
| Historical scaling/API/callers |1188864 comparisons | Full stated16-bit application domains; not all coefficient/state combinations |
| Combined historical software total |2108416 PASS | Not analog/timing/asynchronous-IRQ proof |
| PGOOD release002 physical |43 console checks;12 stress cycles;174 INA reads,6 BUSY | Stateful/persistent legacy commands excluded; lab prerelease |
| PLL trial physical |3 matching legacy/native/full-map reads, PLL locked | Whole-system FAIL: ADT ID00/all registers zero |
| Rollback physical |Full readback a016..., ADT IDC3, INA4/5/6 and32 SPI vectors PASS | Historical Oct16 observation only |

Scope/source: `docs/pgood_sensor_validation_2026-10-15.md`,
`docs/evidence/pgood-2026-10-15/`, `docs/pll/c-read-migration-2026-10-16/`,
`docs/scaling/README.md`, `docs/dac/README.md` and their JSON/logs.
No exhaustive behavioral rerun is needed for identical-byte baseline changes.

Known blockers: ADT startup writes still precede stable PSU PGOOD; root cause of
PLL-trial ADT regression is NOT_ESTABLISHED. Shared-clock clamping by an unpowered
hub remains an unproven hypothesis. TLOW changed0500->FFD0 through reset/rollback;
THYST0F remained. Final new candidate stack observations: sender24 vs old3,
updater68 vs old24 bytes below entry; not a system/ISR nesting bound. Readiness
of fixture and analog acceptance remains NOT_RUN. User requested waiting for
simulator: no new physical deployment/release until readiness is established.

## CPU–FPGA and external communication contracts

Use executable serializers as the wire truth; never infer a standard protocol
from a historical function name. See `docs/hardware_anchors.md`,
`docs/fpga_settings_abi_audit.md`, `docs/legacy_abi.md`.

* Console: USARTF0 PF2 RX/PF3 TX,115200 8N1, CR input, CRLF replies.
  Corrected physical flow-control evidence: PF1 CTS active-low input, PF0 RTS
  output; older generic pin tables reverse the labels. X6 pins2TX/3RX/5GND/7CTS/
  8RTS per schematic. Latest shared station identity: CP2103 USB1-1.3 serial0001;
  old Prolific paths are historical, never identify by ttyUSB number.
* Original commands remain in `recovered_exact/src/console_dispatch.c` and
  `asm_analysis/main.S`; preserve queued-line counter, folding and parser errors.
  `RS` status, `RF` RAM setting shadows. `PC` persists PLL EEPROM, `WR` persists
  settings; ON/OF also write EEPROM D1. Do not replay them as read-only vectors.
* New namespace `@`: PING/HELP/HW/TEMP/TDC/PLL/PGTIME/FPGA/FPGATEST/CFG/FLASHID/
  FLASHREAD/I2C/INA/POWER; authoritative grammar is `development/src/extension.h`
  plus handler files/tests.47 characters after @, overflow drained to CR.
  `@TEMP ALL`: registers0..7; `@TEMP RESTORE` changes thresholds/config, never
  automatic. `@PLL ALL`: regs0..8 read selectors0e..8e; `@PLL LEGACY` blocking
  migrated entry. `@INA 4/5/6` mux routes, `@POWER` demand reads all3 monitors.
* SPIC PC5 MOSI/PC6 MISO/PC7 SCLK is shared by FPGA register access, PLL and DAC;
  peripheral mode/config changes per transaction. FPGA chip-select PD0 active-low,
  SPIC_CTRL d1. At230e, FPGA write sends four bytes:
  `address>>2`, `(address&3)<<6`, data high, data low. Read at2368 sets bit7
  of first header; second header high bits come from address and lower bits
  retain legacy R21 shift state. Do not silently clear unspecified live bits.
  `fpga_multiword_read.c` sends bc,00 and8 dummy bytes, receives8 bytes into
  R9:R8..R15:R14. IRQ and register contracts are part of the model.
* FPGA PROGRAM_B/INIT_B/DONE PD1/PD2/PD3; configuration/serial-FLASH SPIE:
  PE5 MOSI,PE6 MISO,PE4 FCS,PE7 CCLK. Board FLASH part/revision discrepancy
  N25Q032 vs S25FL064L remains; check fitted hardware. Configuration path and
  normal register SPI are distinct. FPGA-originated external HSPI is not the
  MCU UART nor automatically MCU SPIC; consult matching FPGA sources.
* Settings exchange: init at08e4 writes42 transactions; reset098a writes61.
  48 setting words RAM21cf..222e -> FPGA80..af;12 calibration words2163..217a
  ->b0..bb; byte2441->be. ADC gain24 words2187..21b6, first channel regs25/26.
  See ABI audit for original asymmetric R28 preservation and final flags.
* PLL CDCE62005: shared SPIC, CTRLf1 LSB-first, LEPF4/SYNCPF5/LOCKPF6.
  Original startup replays10 FLASH profile words into live registers. EEPROM
  copy is separate PC command, not a harmless reset. No PLL EEPROM change done.
* ADT7311: **software SPI**, PA1 clock/PA2 DOUT/PA3 DIN/PA4 CS; not hardware TWI.
  New I2C shares PA1 with ADT clock, uses PF7 SDA, PB6 shared alarm/reset input.
  TCA9548A muxes70/71; INA3221 reads repeated START, MSB first, ACK/NACK, restores
  masks. No INA config writes. GPIORF0f owner1main/2temperature,bit7pending,
  bit6stuck; GPIORE0e deferred count. Busy ISR skips temperature only. PGOOD
  checks EN_PSU PE2 and PE1/PE3 ready before reading. Startup remains separate.
* DAC8554:12 quad16-bit DACs,48 outputs;24-bit header/high/low frame; selected
  PORTC chip-select group then deassert mask7; CTRLd5. No SDO/readback. RF proves
  RAM only.48 sequential updates are not simultaneous. Analog fixture required.
* THS788 host: PB0 HCLK/PB1 HDATA/PB2..4 strobe/PB5reset/PB6alarm.
  Firmware bitbang serializer is authoritative for edge order/turnaround.
  Fast TDC/ADC data links are FPGA paths, not this MCU console/control stream.
* PSU/status: PE2 EN_PSU, PE1 IN_PWR_OK, PE3 P1V2LDO_PG. OFF/PG loss and stale
  readings must remain distinguishable from actual zero voltage/temperature.

Original FPGA source checkout (separate dependency, not rebuilt here):
`/home/codex-hil/fit-bringup/sources/alice-fit-fpga`, pinned historical eb4ced4;
`firmware/FT0/PM/hdl/{fit,Channel}.vhd`. Verify that commit in a twin workspace.
Dynamic integrator pedestal is FPGA IIR (weight1/1024), not MCU EEPROM offset;
per-integrator gain unity2048. Historical diagnostic SRAM image4bbe9dfc,
USER1magic504d5a04, bit SHA256
5b9c5fb30722a49cb8fdeea524f7541e0b10dce1e575b015a5d5e43d6ff6d5d4.
Its diagnostic map is not interchangeable with production registers. No new
FPGA build, register-map qualification or live board inspection in this handoff.

## Digital twin next steps and ownership

Start offline with canonical original and candidate differential CPU execution.
Keep models for CPU flags/private registers,3-byte return stack, MMIO side effects,
SPI completion, UART queue/flow control, EEPROM separate spaces and power timing.
Existing interpreters are functional transaction oracles, not validated timing
models. Add independent peripheral models and labeled fault cases before claiming
interrupt/timing coverage. Reset startup, ADT powered/unpowered behavior and shared
clock ownership are priority integration scenarios. Model measured facts separately
from assumed electrical behavior; never report synthetic observations as hardware.

Physical simulator audit: `/home/codex-hil/FIT_PM12_TEST_COVERAGE/` README,
CURRENT_ARCHITECTURE and OPEN_QUESTIONS. Latest plan:4051/4052 muxes,external DMM,
external differential DAC; old local ADC/DAC plans are superseded. Schematic,
actual channel/GPIO map and safe common-mode/HiZ assumptions remain unverified.
Connector-only analog measurements qualify the full path, not isolated DAC pins.
No physical simulator enumeration was performed by this handoff.

Future physical actions require identity checks and both sorted leases:
`~/.cache/atmel-ice-J41800043065.lock` and `~/.cache/pm12-210249A73E43.lock`.
Historical boardSN1803,ICEJ41800043065,FPGA cable210249A73E43. Confirm current
attachment/state before any UART/JTAG/PDI/network action; this document is not a
claim those IDs are currently attached. Preserve other threads' jobs and captures.
