# Board-qualified migration order

Keep root exact recovery immutable. Each deliberate development replacement gets
a narrowly allowlisted old-entry jump into0x4000..0x7fff, its own simulation,
physical A/B record and recoverable application-only checkpoint. There is no
requirement to replace working bootloader code.

| Function group | Simulation gate | Available PM12 acceptance | Decision |
|---|---|---|---|
| PLL register8 read0x24ce | Full compiled AVR vs original: registers, flags, SPI trace | `@PLL LEGACY` through old entry vs bounded `@PLL`, full map, lock, sensor/SPI regression | Local PLL PASS, system ADT regression; rolled back, not accepted |
| PLL four-byte write0x2486 | All byte values/flag/ABI contracts and LE framing | First exercise only a read-selector command; later unchanged normal-init profile and full readback | Next bounded transport candidate; no EEPROM-copy tests |
| PLL normal initialization | Original ten-word sequence including calibration order and state/IRQ contract | Controlled MCU startup, raw map comparison, lock and independently measured outputs | Wait for defined clock measurement/trace acceptance before replacing initialization |
| Console command decoding | Parser bounds, legacy forwarding, queue/ABI behavior | Read-only legacy commands and new sensor commands over same UART | Additive parser already isolated; leave legacy handler bodies |
| ADT temperature transport | Bit-cell waveform/order, signed conversion, ownership/power failure model | ID/config/readback and repeated concurrent I2C/temperature reads | Resolve existing startup/THYST observation separately; avoid combining fixes with migration |
| GPIO LEDs/status formatting | Register/flag/output model | Observable LEDs and UART under explicitly defined normal/alarm stimuli | Only cases that can actually be forced and measured |
| INA3221/I2C diagnostics | Bus framing, NACK/timeout, ownership and conversions | PGOOD-qualified routes4/5/6 and restored mux masks | Already additive C; keep independent of FPGA readiness |
| TDC calibration, production FPGA/flash update, external protocol fault paths | Needs the corresponding peripheral/FPGA stimulus model | This standalone diagnostic PM12 does not cover all production stimuli | Defer until tester/emulator is available |
| Bootloader | Existing exact baseline | No migration required | Retain ASM |

The decoded AVR interpreter is a function/state model with explicit SPI/GPIO
models, not a full ATxmega peripheral or asynchronous-interrupt emulator.
Physical normal-state readback is useful evidence but must not be described as
full thermal fault, power failure, PLL jitter or production link qualification.
