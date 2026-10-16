# First legacy PLL read migration

Software-qualified candidate; hardware trial is recorded separately when run.

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
