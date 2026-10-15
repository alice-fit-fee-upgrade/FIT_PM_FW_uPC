# I²C wiring and shared-pin contract

User-provided assignment, 2026-10-06:

| Role | MCU pin | Mask | Data-space port base |
|---|---|---|---|
| SDA | PF7 | 0x80 | 0x06A0 |
| SCL | PA1 | 0x02 | 0x0600 |
| I²C switch RESET | PB6 | 0x40 | 0x0620 |

This pair is not a hardware TWI pin pair. The supplied PM12 schematic labels
PC0/PC1 and PE0/PE1 as SDA/SCL; PF7 and PA1 require software I²C. This is a
proposed development wiring assignment, not confirmation of electrical ownership.

## Existing recovered configuration

Evidence is in `recovered_exact/src/application_main.c` and
`docs/hardware_anchors.md`, corroborated by `sch/FIT-PM12-1.PDF`.

* PA1: original temperature SPI clock (ADT7311). Startup sets its direction to
  output through PORTA.DIRSET=0xFB and its output high through OUTSET=0xF3.
  PIN1CTRL=0x80 enables slew-rate limiting. Existing temperature transactions
  toggle this pin. Attaching I²C SCL therefore shares an actively driven clock,
  unless the board wiring has changed or devices are electrically isolated.
* PB6: original THS788 overtemperature alarm input. Startup sets PIN6CTRL=0x02
  (falling-edge sense) and PORTB.INT0MASK=0x40. Driving it as switch RESET would
  change pin ownership and can also interact with the existing interrupt path.
* PF7: startup sets PIN7CTRL=0x10 (input pull-down). A software-I²C SDA backend
  must replace that pull configuration appropriately; external bus pull-ups and
  voltage must be confirmed. Merely changing DIR does not remove the pull-down.

## Implementation decision

Use a software open-drain backend once sharing is resolved: pull LOW through
OUTCLR + DIRSET and release through DIRCLR, never drive a bus line HIGH.
Use per-bit port writes, bounded clock-stretch waits and explicit bus errors.
Keep switch reset optional until its model, polarity and pulse timing are known.
Do not silently disable the existing temperature driver or alarm interrupt.

No pin initialization or reset pulse has been added. The exact baseline,
`exact_asm`, golden reference and current development firmware are unchanged.
This note records static source/schematic evidence, not hardware validation.

Pending: actual board revision/rewiring and signal sharing, switch model,
bus voltage/pull-ups, first sensor and its 7-bit address.

## FIT_PM12 v1.1 evidence (2026-10-06)

User confirmed PA1/PB6 have NOT been disconnected from their legacy devices.
New supplied drawing: `FIT_PM12v1.1.PDF`, 35 pages, SHA256
`3bff65447a5b5fff46e856405f6b7edac444683c9ae6759fccd73d3cc5df3b70`.
It supplements the older drawing; golden/reference files remain unchanged.

* Page 5 explicitly maps PF7 → MCU_AFE_I2C_SDA,
  Tempsens_SCLK/PA1 → MCU_AFE_I2C_SCL and
  THS788_OT_ALARM/PB6 → MCU_I2C_SW_RSTn.
* Page 1 connects MCU SDA/SCL/RESET through R226/R227/R228, each marked
  `Undefined`; FPGA connections R229/R230/R231 are marked 10 ohms.
  Verify actual assembly/continuity and FPGA output ownership. `Undefined`
  alone is not proof of either a populated component or a missing component.
* Page 35 has IC1 and IC2, both TCA9548ARGER, supplied by P3V3.
  Upstream SDA/SCL pull-ups R224/R225 are 2.2 kohms; downstream pull-ups
  are 10 kohms. Both switches share I2C_SW_RSTn, active LOW.
* IC1 channels 0..7 serve AFE 1..8. IC2 channels 0..3 serve AFE 9..12;
  channels 4..6 serve Imeas 1..3; channel 7 has pull-ups but no named load.
* With only the drawn 10-kohm straps populated, IC1 A2:A0=000 gives 0x70
  and IC2 A2:A0=001 gives 0x71 (7-bit addresses). Optional straps marked
  `Undefined` mean actual addresses still depend on assembly.

TI source: https://www.ti.com/lit/ds/symlink/tca9548a.pdf
A channel mask is a single control byte, without a register-address byte.
A STOP is required after that byte for channel selection to become active.
RESET deselects all channels; a LOW shared alarm therefore also disables routing.

### Shared-pin policy for the development driver

1. FPGA must release SDA/SCL and must not hold RESET LOW while MCU owns the bus.
   Do not assume the currently loaded FPGA design does this.
2. Keep ADT7311 CS inactive throughout software I²C; do not overlap temperature
   transactions. Ensure SDA stays HIGH through legacy SPI clock transitions to
   avoid unintended I²C START/STOP conditions. Check ISR access before relying
   on main-loop serialization.
3. Keep PB6 an alarm input by default; never drive HIGH against an alarm output.
   Check RESET/alarm is HIGH before selecting a route and abort if asserted.
   A LOW event may reset both switches even if its interrupt is masked.
4. Default operation can select/deselect routes through I²C without generating
   a hardware-reset pulse. Any future deliberate PB6 LOW pulse needs confirmed
   THS788 output characteristics and treatment of the resulting alarm interrupt.
5. Switch channels must be reselected after any alarm/reset; do not trust a cached
   route. Initially select one downstream channel and deselect the other switch
   to avoid address collisions among repeated sensors.

These are static design constraints. No hardware transaction has been performed.
The development firmware has not yet been changed to drive the shared pins.

## Superseding physical state — handoff 2026-10-15

The previous shared-pin discussion describes the schematic and earlier state.
The user has now removed R226/R227/R228: MCU SDA/SCL/RESET are physically
isolated. FPGA is the sole current I²C master. Do not assume direct MCU reads
work. A future extension needs an explicitly defined FPGA bridge or an agreed
change of wiring/master ownership. No such transport is implemented here yet.

Both TCA9548A addresses 0x70/0x71 and three INA3221 devices at 0x40 on mux 0x71
channels 4/5/6 were physically validated by the diagnostic thread. RESET needs
HIGH bias; FPGA B26 pull-up restored operation after MCU isolation.

Authoritative received handoff: `handoffs/pm12-ina3221-2026-10-15.json`.
It records register reads, signed conversion, nominal shunts, raw evidence and
20 samples per monitor. In particular R15 is NOT fitted: P3V8 current is always
unavailable, and its observed bus input must not be labelled the actual 3.8-V
rail. TP1/TP2 also has no specified shunt/current conversion.

Keep the original exact baseline and six-byte development CLI gate pattern.
Do not import diagnostic PG/SPI hooks, alter PSU/IRQ protection, write monitor
configuration, or program hardware as part of this knowledge transfer. Save and
restore both mux masks, including error paths; select one INA branch at a time.
Physical MCU telemetry remains NOT_RUN. Future hardware operations must use the
identified PM12 device and lock specified in the handoff.

## Confirmed power-domain correction — 2026-10-15

The user confirmed a current hardware error: INA3221 and TCA9548A are supplied
by switched main P3V3 DC/DC, not the always-powered MCU P3V3_LDO domain.
RS232 is already supplied from P3V3_LDO. Consequently MCU/UART can remain alive
while the telemetry ICs cannot respond. Firmware must report unavailable
telemetry on missing ACK, never fabricate measured zero voltage/current or
weaken PSU protection to keep telemetry alive.

Next board revision must move INA3221 VS/VPU, TCA9548A VCC and associated bus
pull-ups into the MCU P3V3_LDO domain. Verify LDO current budget and back-powering
of unpowered AFE/FPGA branches before releasing the revised schematic. This
correction is recorded in the updated received handoff; no hardware changes or
programming were performed here. Independent telemetry during main P3V3 failure
cannot be achieved by firmware alone on the present wiring.
