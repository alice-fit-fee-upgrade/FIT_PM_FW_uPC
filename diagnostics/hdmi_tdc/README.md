# PM–Kasli HDMI and TDC diagnostic preparation

Status: pinned source extraction and test architecture. No FPGA programming,
HDL simulation, synthesis or hardware qualification of this new diagnostic yet.
Current MCU firmware and running FPGA are untouched.

## Reproduce extraction

```
python3 extract.py /path/to/FIT_TCM_GW_FPGA
```

The exporter reads git objects at revision
`857b773eec28282b357d4668ab934e95f41b6fe7` from
https://github.com/alice-fit-fee-upgrade/FIT_TCM_GW_FPGA.
It stages 13 original files and a verbatim PM HDMI SPI slave process, preserving
original bytes and recording SHA256 in generated/manifest.json. Generated files
are ignored; the original repositories and baseline firmware are not edited.
This source version must still be checked against the intended historical PM
release and board revision before hardware use. Source provenance is not proof
of correspondence with the original installed FPGA bitstream.

## Blocks and extraction boundaries

| Purpose | Original source under firmware/FT0 | Dependencies / boundary |
|---|---|---|
| PM HDMI trigger transmitter | PM/hdl/trigger.vhd | PM12_pkg; clk320 and mt_cou; inputs are per-channel time/amplitude/trigger masks |
| PM output stage | PM/hdl/fit.vhd:858–865,919,1959 | Four LVDS OBUFDS; extra registered tto/tao stage must be preserved |
| PM HDMI SPI slave | PM/hdl/fit.vhd:1250–1470 | Embedded HSCKI/HSELI process, register maps, side effects, ownership/CDC dependencies; NOT a standalone entity |
| TCM HDMI receiver | TCM_v1/hdl/HDMIRX.vhd | IDELAYE2, ISERDESE2, clk320/90, IDELAYCTRL/refclk and phase controller |
| TCM receiver clock/phase integration | TCM_v1/hdl/tcm_side.vhd | Original MMCM320_PH, master selection, mt_cou, psen/phase loop |
| TCM SPI master for PM | TCM_v1/hdl/pm-spi.vhd | COUNTER_FIFO, local register interface, read inversion and special MCU mode |
| PM TDC receiver | PM/hdl/TDCCHAN.vhd | pin_capt, TDC_FIFO, clk300/600/600_90, tdcclk, rstr/rdata, coarse counter and BC timing |
| TDC edge capture | PM/hdl/pin_capt.vhd | ISERDESE2 OVERSAMPLE; must retain clock phase and physical placement assumptions |

`pm_trigger_tx.vhd` provides the small transmitter wrapper around the original
trigger entity, including its original registered output stage and LVDS buffers.
It requires extracted PM12_pkg.vhd and trigger.vhd; HDL compilation is pending.

This is the HDMI-facing HSPI slave, not the MCU-facing SPI diagnostic slave.
Do not replace the latter. The extracted PM SPI process still references the
original full register map; a minimal standalone adapter is subsequent work.
Keep original read side effects in documented legacy tests; use a separate
scratch bank in the new diagnostic to avoid writing real peripheral controls.

PM upstream constraints specify HSPI period50ns (20MHz), LVCMOS33. PM trigger
outputs use LVDS. These are reference constraints, not an approved Kasli adapter
pinout. Kasli model, connector, bank voltage, P/N mapping and buffers must be
confirmed from actual board schematics before assigning outputs. HDMI here is
custom detector signaling, not video/TMDS. Never copy PM pin locations to Kasli.
HDMIRX samples P and N separately through delay paths: preserve its unusual
polarity/termination arrangement rather than substituting a generic receiver.

## Packaging

Use two small diagnostic tops, with common register definitions and host runner:

* PM: source-selected legal trigger stimulus -> original trigger -> original
  register/output stage -> connector. Original HSPI slave transport connects to
  a bounded diagnostic scratch/register bank. Existing MCU SPI, power behavior,
  IPbus ADC buffers and passive I2C observation remain available.
* Kasli: connector -> original HDMIRX and phase/alignment control -> scoreboard
  and bounded capture RAM. Original pm_spi master accesses PM scratch bank.
  Control/status exposed over the chosen Kasli host transport, preferably IPbus.

Do not silently overwrite existing IPbus addresses. Allocate a versioned window
in coordination with the active integrated-image owner. Counters are saturating
64-bit with coherent snapshots: acquired/lost lock, bits compared, bit errors,
frames compared/errors, missing/duplicate frames, SPI requests/replies/timeouts,
first-error expected/actual/position and bounded pre/post-trigger trace.
Frozen traces require explicit ARM; JTAG cursor/control scans must not rearm them.

## HDMI test modes

1. Legacy idle/training and PM request handshakes. Prove link_ready, bitpos_ok,
   stable phase, no sync_error. Interrupt/restart link and measure reacquisition.
2. Deterministic legal trigger vectors: empty, each channel alone, all channels,
   amplitude/time boundaries and mixed signs. Compare receiver DATA_OUT against
   a cycle-accurate reference of original trigger and receiver word packing.
3. Long legal stream driven by a seeded PRBS generator in the *stimulus fields*.
   Keep original mt_cou framing, reserved encodings and periodic idle/training.
   Seed and accepted-frame index determine the expected payload. Random inputs
   are not necessarily transmitted verbatim: original trigger sums/encodes them.
4. Separate raw-lane BERT bypass: per-lane different nonzero PRBS seeds, initial
   walking-one/zero patterns to identify swapped/inverted lanes, then PRBS7 and
   PRBS31. Bypass legacy decoding for raw BERT; random raw bits need not satisfy
   HDMIRX's idle/sync rules. Raw BERT results do not establish protocol validity.

PRBS must specify polynomial, shift direction, output bit, first emitted bits,
seed and advance-enable. TX advances on the actual symbol clock; checker locks
once and then compares a locally predicted sequence. Do not continually reseed
from received data: that can conceal drop/insert errors. Account for every
unlock/relock, exclude training bits explicitly, and report actual compared bits.
Inject a known single bit error, burst, dropped/repeated symbol and lane swap in
simulation; checker must detect each and count reacquisition separately.
With zero errors, report a statistical BER upper bound rather than BER=0.

SPI test in parallel: walking patterns/address-dependent values/seeded PRBS,
16/32-bit accesses, legacy byte order, bursts/address increment, CS abort,
bounded timeout and recovery. Preserve original master inversion (`not spi_miso`)
and active/select conventions, confirm with simulation before wiring. Do not
label generic mode0 SPI a substitute for the extracted original protocol.

## TDC test strategy

Yes: reuse original TDCCHAN + pin_capt, initially one channel, then all12 across
three THS788. In the original code, falling rstr starts a seven-bit serial read;
tdc_raw combines six coarse bits with seven serial bits (13bits), whereas the
processed output is12bits with CH_CORR correction. Capture BOTH, not just the
processed value. Original TDC_FIFO is common-clock clk300 but its output is
consumed in tdcclk logic; preserve and analyze this timing contract explicitly.
FIFO modes/latency/reset behavior must be regenerated from archived properties;
a generic FIFO is not automatically equivalent.

Three distinct levels:

1. Receiver simulation: modeled rstr/rdata/coarse events, boundary corrections,
   reset, FIFO empty/full and bursts. Establish bit order and latency against
   original receiver, including source clock crossings. This validates decoding,
   not the physical TDC or analog input.
2. Hardware raw observation: original pin capture + receiver with event counts,
   FIFO overflow/drop counters and raw timing snapshots over IPbus. No received
   events is not a pass; record reference/PLL and TDC clocks, configuration and
   number of applied stimuli. Read existing MCU TDC settings before changes.
3. Known physical pulses: apply a matched split reference/test pulse pair at the
   actual front-end/TDC path, vary delay and pulse rate, then channels. Measure
   count conservation, delay slope, wrap behavior, offset, spread and crosstalk.
   Sweep phase across a reference-clock period for code-density diagnostics.
   Precision claims require an independently characterized stimulus; Kasli GPIO
   pulses alone do not prove picosecond resolution/jitter. PRBS event spacing is
   useful for burst/dead-time stress, not as a replacement for known delays.

No assumptions yet about stimulus connector/levels or access to TDC inputs.
A generator on FPGA pins must never be enabled onto nets already driven by TDC.

## Remaining implementation and acceptance

Need actual Kasli identity/adapter pin map, verified board constraints, standalone
PM slave adapter (transport separated from legacy register effects), clock/phase
wrappers and regenerated FIFO/MMCM IP. Then HDL simulation with original
primitives, synth/place/route with timing checks on both targets, followed by
bounded PM–Kasli hardware tests under both board leases. Retain rollback images,
resident analyzer functionality and MCU FLASH/EEPROM/fuses unchanged. Existing
ADC/IPbus successes do not qualify this new HDMI/TDC diagnostic.

## Selected Kasli interface: DIO-RJ45-LVDS

User selected the Sinara DIO-RJ45-LVDS module. Plan two logical RJ45 ports:

| Port | Channels | Direction relative to Kasli | Function |
|---|---|---|---|
| RX | four pairs | all input | PM TT0/TT1/AT0/AT1, final permutation to be verified |
| SPI | three pairs | output | SCK, MOSI, select |
| SPI | one pair | input | MISO |

The Sinara wiki gives RJ45 pair pinout: 1P/2N SCK, 4P/5N MOSI,
3P/6N MISO, 7P/8N CS, shield ground. These are module-side assignments,
not yet a complete PM HDMI adapter pinout. All eight channels fit the nominal
single-EEM capacity; actual Kasli connector mapping and module revision remain
unconfirmed. Configure both module direction and FPGA I/O direction; do not
assume ordinary ARTIQ TTL switch polarity. Current M-Labs documentation says
ON fixes input, OFF permits I2C direction control and defaults to output.

The module uses differential repeaters, not video conversion. Kasli gateware
must retain the native HDL receiver/master and clocking; generic RTIO TTL edge
capture is not a substitute for original320MHz receiver sampling. Published
150MHz verified operation is a historical test limit; it neither qualifies nor
rules out this particular320Mbit/s signal path. Validate timing and BER on the
actual module/cable. A shielded adapter must preserve pair polarity and ground.

IMPORTANT: upstream PM HDL identifies HSPI FPGA pins as LVCMOS33; current
PMv1.1 schematic also names HSEL/HMOSI/HMISO/HSCK at MCU PD4..PD7.
Do not connect the module's differential SPI pairs straight to these nets or
assume they are separate from the live MCU bus. Trace actual HDMI connector,
transceivers/muxes and physical board nets first. If single-ended access or a
shared master remains, provide level conversion and explicit master isolation;
firmware-only chip select is not proof against output contention. The four
trigger LVDS pairs and the SPI adapter are distinct wiring questions.

Sources checked:
- https://github.com/sinara-hw/DIO_LVDS_RJ45/wiki
- https://m-labs.hk/docs/sinara-datasheets/2245.pdf (July2025)
- Local available DIOT variant repository c8cb69a084ef14d6f700eccb27d9d49566c85b5b;
  do not assume it matches the user's physical module revision.

## Confirmed shared-SPI selection (recovered MCU + original PM HDL)

Recovered system_init sets SPID_CTRL=0x44: hardware SPID enabled, slave,
mode1, MSB first. PORTD_DIRSET=0x41 includes PD6/MISO output; DMA channels0/1
use trigger0x6a with SPID_DATA0x09c3 as destination/source.
Original PM HDL fit.vhd sets hspi_rden<=not HSELI and drives HMISO through
OBUFT(T=>hspi_rden); its slave process resets while HSELI=0. Thus HSEL high
selects FPGA, HSEL low selects the MCU's active-low hardware SS. This is
complementary slave selection, not two masters or software address arbitration.
The TCM master's separate xmg_md path is consistent with MCU accesses.
Keep the original select behavior: low is NOT an idle state for both slaves.
FPGA HSPI accepts a read bit plus9-bit address and mapped16/32-bit data with
increment/side effects; retain separate original MCU command framing.
