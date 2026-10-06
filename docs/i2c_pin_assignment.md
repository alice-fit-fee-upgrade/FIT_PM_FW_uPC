# Proposed I²C wiring — pending board ownership confirmation

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
