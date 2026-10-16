# PLL C recovery and PM12 validation branch

Branch: `development/pll-c-recovery`, based on board-tested PGOOD checkpoint
`535f6aa` (`development/pgood-adt-readback`). This initial checkpoint creates
an isolated working branch and acceptance contract; it does not replace a PLL
function or program hardware. Existing ADT startup limitation remains present.

The immutable original baseline remains `recovered_exact/` plus `exact_asm/`;
root `make exact-check` must retain zero differences. Controlled binary-different
C replacements belong in `development/`, with explicitly bounded patches and
separate hardware acceptance; never count them as exact baseline conversions.

## Confirmed original PLL behavior

CDCE62005 uses SPI controller C. Recovered `pll_control_write.c` selects LSB-first
SPIC_CTRL=0xf1, lowers PF4/SPI_LE, transmits R16..R19, raises LE. Readback helper
`pll_control_read.c` sends0x0000008e to select register8, followed by32 dummy
clock bits. Current `@PLL` diagnostic likewise reads register8 only: it is NOT
a complete register-map dump. The candidate now implements full register0..8 readback (`@PLL ALL`); it is
software-tested but not yet installed on the board. It reports raw words without
guessed status masks; comparison/decoding remains a separate step.

The timer's `CDCE62005_control_rst()` writes10 words from original FLASH0x2916.
It configures working registers, including calibration transitions, without the
EEPROM-copy instruction. Initial system_init also writes0x00001008.

Console `PC`+CR calls `cdce62005_rst()`: nine words from FLASH0x293e. The last
word at0x295e is0x0000001f, Copy RAM to EEPROM without permanent lock. Thus PC
IS A PERSISTENT PLL EEPROM WRITE, not merely reset/default RAM configuration.
Ordinary read-only regression must keep PC excluded. No PLL EEPROM writes are
authorized merely by creating this branch or running its ordinary test suite.

Source: https://www.ti.com/lit/gpn/cdce62005, tables38 and section8.5.1.7.
After EEPROM copy, TI requires at least50ms stable power/no device access.
Preserve original semantics in the exact reference; any correction to EEPROM
write timing is a separately identified development change, not a C-only rewrite.

## Acceptance order

1. Record known source/image hashes and PLL register map. Capture legacy SPI
   configuration waveforms, current lock state and ADC-clock measurement using
   the independent FPGA oscillator. Add that frequency counter to diagnostic
   FPGA only in coordination with its owner; it is not implemented by this branch.
2. Simulate original and standard-ABI C transport: byte/bit order, LE transitions,
   exact commands, polling, IRQ state and caller contracts. Never claim the
   instruction-transition comment suite is a complete PLL/peripheral simulation.
3. Replace transport/write/read one bounded function at a time, then normal
   initialization. Preserve original reset/SYNC/calibration order and IRQ behavior
   unless an explicit separate fix is under test. Prefer standard C plus a small
   legacy adapter over retaining private registers throughout the new driver.
4. Compare original and candidate from the same physical PLL startup state.
   MCU reset alone is insufficient; inspect actual POWER_DOWN wiring/control
   before claiming it resets PLL. If necessary use a controlled real supply
   cycle. SYNC, SLEEP and table writes are not substitutes for EEPROM reload.
   Any PM power cycle also affects FPGA SRAM and must be coordinated/restored.
5. Repeat readback, independent frequency/clock-present and lock/settling checks,
   and UART/temperature/I2C regression. Record applied configuration, dynamic
   register masks, oscillator accuracy and actual waveform timing. Frequency
   agreement does not establish jitter performance or all PLL outputs.
6. Validate cold boot from existing PLL EEPROM separately from MCU programming.
   Only a distinct deliberate persistence trial may exercise PC/EEPROM-copy.
   Bootloader, MCU EEPROM/fuses/signatures and FPGA configuration FLASH remain
   outside this change. Keep a known rollback image and small commits/releases.

Hardware results from the PGOOD base remain historical base evidence, not test
results for future PLL replacements. The current board may run integrated FPGA
build882901f6 / USER1magic504d5a04 rather than the old standalone analyzer.
Read `/home/codex-hil/fit-testing-publish/diagnostics/pm12/ipbus_adc/CURRENT_STATE.md`
and coordinate current owners before any hardware action. Both PM12 and
Atmel-ICE leases remain mandatory. No reset/programming is needed for this
initial branch checkpoint.

## First test checkpoint,2026-10-16

See `validation-2026-10-16/` for raw baseline captures and software results.
Physical MCU UART/PGOOD/PLL/ADT/INA reads work, but MCU FPGA access returns
ERR POWER and the diagnostic IPbus endpoint times out. MCU was not reset or
programmed; candidate deployment, full-map physical readback, clock measurement,
cold-boot and ASM/C initialization A/B remain NOT_RUN pending FPGA state
clarification. Do not turn software PASS into board qualification. Existing
ADT configuration was read, never reset/restored/rewritten in this checkpoint.
