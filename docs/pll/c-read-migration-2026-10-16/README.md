# First legacy PLL read migration

Software qualification PASS; hardware system acceptance FAILED; candidate
rolled back. The source is retained as an experimental candidate, not a new
accepted deployment/release.

The application entry at byte0x24ce (`FUN_code_001267`) receives one four-byte
JMP into the reserved extension area0x4000..0x7fff. Its remaining original body
and all other legacy addresses remain intact. `pll_transport.c` expresses the
original register8 SPI transaction in ordinary GNU C. `pll_legacy_bridge.S`
adapts return R19:R16, preserves the other GPRs and extended bank registers,
and restores the original outgoing SREG contract. No interrupt policy, PLL
profile write, reset or EEPROM-copy behavior is changed.

`@PLL LEGACY` explicitly calls the old symbol through a GNU diagnostic adapter;
this exercises the real replacement gate. `@PLL` keeps its existing bounded
diagnostic API. The legacy call deliberately retains blocking SPI polling;
LEGACY is a laboratory diagnostic for a healthy controller, not a timeout API.
Both commands guard power/ownership and restore the previous SPI controller.

## Validation

- `make development-check`: PASS, including compiled linked AVR interpreter
  comparisons against the full original function in768 cases (all256 SREG
  values, three response/poll-delay scenarios, randomized GPRs). An additional
 768 calls exercise the GNU diagnostic entry, return value and callee saves.
- Exact traces include SPI control/data, polling and LE edges. No stubbed
  callee replaces the compiled AVR transport. Host command tests separately
  cover LEGACY, ALL and argument errors using an explicitly identified ABI model.
- `make exact-check`: PASS, zero original baseline differences. Original
  canonical SHA256 e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0.
- The intentionally extended candidate SHA and allowlisted differences are
  in `layout_report.json`; this is not claimed to match PM.hex.

The C function static stack report is11 bytes; the decoded old-entry comparison
observed26 pushed data bytes across adapter/callee. Neither is a total hardware
worst-case bound: call return addresses, interrupted callers and interrupt
nesting also consume stack. The simulator does not prove cycle timing,
asynchronous IRQ behavior, nonzero-bank bus addressing, electrical timing or
clock frequency/jitter.

## Hardware trial

`tools/hardware/pm12_pll_trial.py` holds both board leases and requires the
expected physical running FLASH SHA before any programming. It backs up and
compares all protected memories, writes only the128KiB application view,
verifies the full139264-byte readback, then restores the pinned existing
FPGA ADC/TDC/I2C/SPI image4bbe9dfc to SRAM. On failure it restores the exact
pre-trial MCU application and FPGA runtime. It never issues PC, sensor INIT/RESTORE,
PLL profile writes, configuration-flash or CPLD programming.

Physical acceptance compares three bounded/legacy/status reads and three
full PLL maps, then normal temperature/PGOOD, INA3221 routes4/5/6 and the
32-vector restored FPGA scratch test. Natural PLL lock remains required;
fault/lock-loss IRQ injection and power-loss qualification are separate work.

## Actual trial01 result

See [summary](hardware-trial-01/summary.json) and raw console/programmer logs.
The physical full candidate readback matched
`9ece53e796f4089d1bbd7aaf1d6c00fa52fc4780e53fe2edec487f0ce9726d0a`.
Three `@PLL LEGACY` replies matched bounded reads (`80009D38`) and register8
in three stable complete maps; PLL stayed locked.

System acceptance nevertheless FAILED: all eight ADT7311 registers read zero,
including ID00 instead ofC3; RS reported0.0 NORMAL. Temperature value alone
is therefore not a valid health check. The candidate was NOT left running.
The exact previous application was restored (full canonical SHA
`a016c00d39617d106ca855c03a62170d918f2e9a93514e7962bdc0cb7017c23e`).
Both candidate and rollback preserved boot, EEPROM, fuses, lock, production/user
signatures and device signature. The same FPGA4bbe9dfc SRAM image was restored.

After rollback ID C3 and approximately29–30°C returned; INA routes4/5/6 and
FPGA32-vector SPI scratch/restore test passed. However raw TLOW changed from
0500 before the trial toFFD0 after rollback. THYST remained0F; CONFIG50,
TCRIT2300 and THIGH1E00 returned. No explicit sensor INIT/RESTORE, PLL PC,
profile or power command was sent. MCU resets DO execute unchanged original
startup writes to ADT and the PLL RAM profile: the raw trial report's
`PLL_configuration_or_EEPROM_commands: false` denotes no explicit test command,
not absence of original startup SPI transactions.

The cause is NOT established. Pre-PGOOD startup writes/shared SCL and reset
sequencing are hypotheses; successful local PLL reads do not exonerate the
replacement. Further releases/replacements wait for controlled old-firmware
reset measurements and ADT startup diagnosis. No sensor thresholds were silently
rewritten to force acceptance. The existing old firmware is restored, but
peripheral TLOW is not claimed to equal its pre-trial value.

The initial runner did not retain a partially returned runtime object when
assertions failed, so candidate UART evidence is in `console-and-trial.log`.
The runner now retains partial attempts and names failed ID assertions.
Protected-memory contents remain local backups; only hashes/logs are committed.
