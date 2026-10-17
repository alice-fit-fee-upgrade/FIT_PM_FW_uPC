# DAC8554 control and stored-setting update recovery

Branch `development/dac-c-simulation`, based on arithmetic checkpoint9c52fb1.
This pass performs no board access/programming/reset and writes no EEPROM.
Sources are optional development candidates, not the binary-exact baseline.

## Readback: console settings are not DAC register reads

DAC8554 has an input-only serial interface: DIN, SCLK and SYNC. It has no SDO/
MISO/readback pin or register-read operation. TI datasheet SLAS431B, pin table
and serial interface section: https://www.ti.com/lit/ds/symlink/dac8554.pdf.
The PM12 DAC sheet likewise wires MOSI→DIN11, clock→SCLK10, SYNC→pin9.
The shared MCU SPI MISO line serving other devices does not provide DAC readback.

Original console `RF`+CR prints four stored CFD/ADC settings per channel from
RAM0x21cf..0x222e, plus gate/saturation fields. See
`recovered_exact/src/channel_cdf_adc_display.c`. It reads neither the DAC SPI
registers nor analog output voltage. `RC` displays other channel settings and
THS information; it is not DAC readback either. Successful transmission cannot
be established by comparing the RAM shadow alone.

## Two replacements

| Original entry | New implementation | Original executable body |
|---|---|---|
| 0x22aa `dac_send_value` | `pm_dev_dac_send_locked` plus private-ABI entry | First4 bytes JMP;0x22ae..0x230d retained |
| 0x09de `FUN_code_0004ef` | `pm_dev_dac_apply_stored` plus private-ABI entry | First4 bytes JMP;0x09e2..0x0a4b retained |

The sender configures SPIC_CTRL=d5, selects one of three SYNC groups, writes
header/high-byte/low-byte, polls the original completion flag after each byte,
and raises all three SYNC lines. It retains original indefinite polling,
controller state and explicit CLI/SEI policy; no timeout/configuration fix or
readback is invented. The ordinary C `_locked` entry requires exclusive SPI
ownership and IRQs already masked. The legacy adapter enforces the original
CLI/SEI sequence, saves GNU-clobbered state/banks and reconstructs live flags.

The updater reads96 bytes from the existing RAM settings table. For channels
0..11 it applies, in order:

1. CFD threshold, including the original calibration offset read.
2. CFD zero.
3. ADC zero.
4. ADC delay.

This produces48 independent24-bit SPI frames. It is a sequential update, not
an atomic/simultaneous latch of all48 outputs. No EEPROM save, default settings,
power transition or sensor configuration is added. Existing callers (including
the original timer path) reach the new function through the old entry. No new
state-changing UART command is installed in this pass.

The four legacy preparation wrappers remain in place. Their two arithmetic
cores use the previously qualified optional C scaling routes. The C updater
calls the wrappers through one small GNU/private ABI adapter which preserves
callee-saved R16/R17 and restores GNU R1=0 after dirty legacy arithmetic. The
updater's outer adapter preserves all original GPRs and the original final
CPI12,12 flags/T/I contract. This is not a claim that every original DAC-related
wrapper has become plain C.

## Reproduce and scope

```
make dac-check
make exact-check
```

`PM_DAC_CANDIDATE=1` includes both DAC and arithmetic candidates in the existing
reviewed FLASH window0x4000..0x7fff. The normal development build excludes them.
Only their four-byte entry gates are additionally allowlisted; the remainder
of each body and all original symbols/bootloader remain protected. RAM globals
and CRT data initialization remain rejected. The PLL hardware trial runner
rejects these simulation-only images.

Tests execute the full linked AVR C, ABI adapters, original preparation wrappers
and GCC helpers. SPI completion is scripted, no C arithmetic/transport is stubbed.
All32 GPRs, all8 SREG flags, banks, physical3-byte return stack and complete
MMIO/explicit IRQ traces are compared against golden original instructions.

Sender matrix: all65536 data words for each of four outputs in each of three
representative SYNC groups (AFE indices0,4,8), plus every256 control bytes ×
every256 initial SREG values ×two extreme words. Other device-address bits
are covered by the control matrix; this is NOT every header×word×flag Cartesian
combination. Poll delays0..3 and nonzero unrelated status bits are exercised.

Updater matrix: eight96-byte settings images ×all256 initial SREG values,
48 frames each. Calibration RAM uses a deterministic image, and all ordered
settings/calibration reads, transmitted bytes, polling and IRQ events must agree.
This is not exhaustive over all possible96-byte RAM/calibration images.

Arithmetic regression separately repeats complete supported16-bit input-domain,
GNU numeric-API and four-caller boundary checks on the combined linked image.

Neither simulator test proves analog voltages, electrical framing/timing,
asynchronous IRQ scheduling or full-system stack headroom. Physical acceptance
needs a monitored SPI capture and DAC/AFE voltage measurements, with original
settings restored. The previous PLL trial's ADT startup regression remains a
separate blocker to deployment. No release is produced as hardware-qualified.

## Recorded result2026-10-17

**SIMULATION_QUALIFIED_ONLY: PASS.**
[Machine-readable summary](validation-2026-10-17/summary.json), raw logs and
per-suite reports record917504 sender comparisons,2048 full48-frame update
comparisons (98304 frames), and1188864 arithmetic/API/caller regressions on
the same combined image:2108416 comparisons total. Normal development checks
and root exact-check passed; the original baseline still differs by0 bytes.

Observed additional stack below function entry: sender3→24 bytes, updater
24→68 bytes. Original values were measured by the final smoke matrix; new
values also occur in the full matrix. Caller return frames, surrounding ISR
frames and asynchronous nesting are outside these per-entry bounds. This
additional stack/latency must be included in physical system qualification.

Combined canonical SHA256:
`f98cae124c45c272935c1c22c5c70ab4fa082aba518a6d8c38f0c2256becfa69`.
The two DAC replacements add496 extension bytes relative to the scaling
candidate:162 bytes emitted from C and334 bytes of ABI/patch assembly.
The original202 body bytes after the two entry JMPs remain in place.
Original exact-C coverage is unchanged; these are deliberate development
replacements, not binary-identical baseline conversions.

Header declarations and debug function-size metadata were integrated afterward
and the entire canonical FLASH rebuilt identically to the fully tested image.
The default opt-out build was restored and verified byte-identical to pre-pass
experimental default9ece53e7…, with no DAC/scaling gates. Final logs distinguish
full matrices from smoke checks; neither is a hardware PASS.

No physical MCU/FPGA, DAC outputs, EEPROM or other board state was accessed
or changed in this pass. No hardware-qualified release was created.
