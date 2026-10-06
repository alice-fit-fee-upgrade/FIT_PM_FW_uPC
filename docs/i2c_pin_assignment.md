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
