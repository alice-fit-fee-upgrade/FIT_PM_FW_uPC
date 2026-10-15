# Original PM TDC receiver review

Reviewed 2026-10-15, static source inspection only. No new bitstream was loaded.
Source: FIT_TCM_GW_FPGA at857b773eec28282b357d4668ab934e95f41b6fe7.
All source references below are under firmware/FT0/PM/. Extraction reproducible
with extract.py; no recovered MCU or original gateware modifications.

## Signal path

```
CFD event -> FPGA pin_capt -> coarse event/reference difference -> coarse FIFO
THS788 RDATA/RSTROBE + RCLK -> 7-bit serial receiver ------------------+
                                                                   |
                                           coarse correction + fine -> TDC12
```

fit.vhd:645–754 receives three differential TDCCLK/RCLKs through IBUFDS+BUFR
and four RDATA/RSTROBE pairs per THS788. There are12 TDCCHAN instances
(fit.vhd:1119–1144): channels1..4 use chip1 A..D;5..8 chip2;9..12 chip3.
Each also receives its CFD observation CGE*i as pin_in. Capturing only the
THS788 serial outputs omits the original coarse event measurement.

TDCCHAN.vhd:33–50 defines the interface. clk300, clk600 and clk600_90 are
per-chip clocks; tdcclk is the corresponding external THS788 result clock.
They must not be replaced with the ADC's320MHz capture clock.

pin_capt.vhd uses ISERDESE2 in OVERSAMPLE mode, with600MHz clock and its
quadrature phase; four samples plus previous-state logic encode C_BITS[2:0]
and C_STR. This is an actual edge-phase detector, not a two-flop GPIO sampler.
Its O output also passes the observed event into subsequent channel logic.

## Reference and coarse time

fit.vhd:1094–1117 instantiates three CLK600_pll + autophase loops, as well as
three pin_capt instances for MCLK1..3. fit.vhd:1879–1946 increments a4-bit
TDC_COU at clk300, captures BC_PER=TDC_COU & BC_BITS on the reference edge,
and detects changes of reference phase through BC_JUMP/HBC_JUMP. lock300
requires BOTH clock lock and autophase completion. Preserve these dependencies.

For each CFD rising event, TDCCHAN captures:

```
C_PER = tdc_count[3:0] concatenated with C_BITS[2:0]
C_TIME = (C_PER - bc_time) modulo128
FIFO input = C_TIME[6:1]
```

Thus coarse time is relative to the sampled PM reference, not an absolute
arrival counter. A Kasli phase sweep must be coherent with that reference;
two independent nominal40MHz clocks produce drift, not a fixed phase point.
Which clock is supplied to which board remains a physical setup decision.

The archived TDC_FIFO properties specify6-bit width, depth16, common-clock
shift-register implementation, Standard_FIFO (not FWFT), synchronous reset.
Write occurs on delayed CFD event unless full/reset/recovery blocks it. Read
occurs on synchronized FALLING RSTROBE in clk300. Output is subsequently read
by tdcclk logic: this is an original cross-clock timing contract, not an async
FIFO. Regenerated latency and clock relationships require simulation/timing
checks; an arbitrary replacement FIFO would change behavior.

If synchronized RSTROBE stays high while FIFO is nonempty, an incrementing
counter reaching40 asserts the FIFO reset/recovery path. It is not a fixed
40-event FIFO depth. At nominal300MHz this is about133ns of counted cycles;
reset/pipeline timing adds cycles. Full FIFO suppresses writes silently at
this interface; diagnostic dropped-event counters must observe that condition.

## Serial timing: why tdc_rdy is not a buffer write enable

On a rising tdcclk edge with falling RSTROBE, the VHDL schedules count=7.
Signal assignments use OLD values within the process. After a settled idle
count0, the following sequence applies:

| Rising tdcclk relative to detection | Old count | Action |
|---|---|---|
|0|0|Detect falling RSTROBE; schedule count7, ready0, raw_pending1|
|1|7|Shift bit0 into bit6|
|2|6|Shift bit1|
|3|5|Shift bit2|
|4|4|Shift bit3|
|5|3|Shift bit4|
|6|2|Shift bit5; schedule ready1|
|7|1|Shift bit6; serial word complete|
|8|0|Commit raw13 if raw_pending and not raw_lock; clear pending|

The shift expression is `TDC <= rdata & TDC(6 downto1)`; the first captured
bit ends up in bit0 after7 shifts (LSB first). On FALLING tdcclk, tdc_rdy
copies internal ready. It can therefore assert between edges6 and7, before
the final data bit is captured. It stays high until the next event start;
it is not a one-cycle event pulse.

Original Channel.vhd:180–182 samples ready through the320MHz pipeline and
uses an edge detector at362. That original latency matters. A new BRAM writer
clocked directly by tdcclk must NOT just write on ready rise: it can capture
an incomplete fine word. Nor should it use raw-value-changed detection:
repeated identical timestamps are valid independent events.

Recommended diagnostic hook: observe the exact raw commit condition
(old count0 && raw_pending), with a dedicated event-valid pulse and latched
coarse/fine/processed snapshot. This would be a NEW instrumented copy/wrapper,
not a claim that the original entity already exposes raw_valid. Stage complete
records into a proper CDC FIFO before the IPbus capture domain. Buffer only
accepted events, count attempted/accepted/dropped separately, record valid
length and ARM/timeout status. Capture overflows must never look like PASS.

## Result construction

The serial result TDC is7bits. The output equations are:

```
raw13 = (coarse6 <<7) | fine7
correction = +1 if coarse6.bit0==1 and fine7[6:5]==00
             -1 if coarse6.bit0==0 and fine7[6:5]==11
              0 otherwise
processed12 = ((((coarse6 >>1) + correction) modulo32) <<7) | fine7
```

This boundary correction is part of the original receiver. Preserve it before
measuring a phase sweep; raw13 and processed12 are not interchangeable.
`tdc_raw_lock` suppresses updates of the raw diagnostic register only; it does
not stop processed12 or the ready signal. SPI readback exposes a held/latest
raw snapshot, not a history buffer (fit.vhd mapping0x40..0x4b).

Channel.vhd:354 subsequently subtracts CH_shift before trigger/event logic.
Capture before this subtraction and before trigger gating initially, so a
configuration threshold cannot hide received events. Store post-shift values
only as an additional explicitly labeled field if needed.

## Original-source variants

Compared local alice-fit-fpga upstream and FIT_TCM_GW_FPGA checkout:

* TDCCHAN.vhd identical SHA256
  dde8d417ef7770b329e853cb845d43b8d74e0259ea37f86647695c817f8bf833.
* pin_capt differs functionally. Upstream samples clk300 on falling clk600
  and gates on ena; selected upgrade version uses a clk300 toggle transported
  through clk300t1/t2, with a different str output path.

Do not combine one version's clock/phase assumptions with the other's capture
logic without verification. The extractor pins the selected revision, including
original autophase and CLK600_pll properties. Which version generated the
historical installed PM FPGA bitstream is not established by this comparison.

## Consequences for the requested0..24ns sweep

The requested25 phase points fit this architecture, but each point requires
stable common reference and settled original per-chip autophase. Use many events
per point, preferably4096 accepted events/channel as with ADC buffer depth.
Here depth counts EVENTS, not uniform ADC sample clocks. Start with a divided
pulse rate, preserving phase modulo25ns, before testing40MHz event throughput.
Store phase ID, reference-lock/autophase state, counters and the actual event
count alongside raw13 and processed12. Preserve discontinuities/wrap in raw
records; unwrap only in host analysis with the acquisition metadata.

MGT outputs cannot be routed arbitrarily to an EEM DIO pin. The proposed
DIO-RJ45-LVDS path calls for SelectIO OSERDES on those pins; exact serializer
rate/clock feasibility depends on the Kasli model, speed grade and routing.
The generator implementation is deliberately deferred until this receiver and
physical reference/CFD injection path have been validated. No physical sweep,
TDC data acquisition, HDL simulation or timing qualification is claimed here.
