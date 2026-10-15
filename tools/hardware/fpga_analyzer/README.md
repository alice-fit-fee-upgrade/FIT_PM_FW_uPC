# PM12 passive shared-clock analyzer

SRAM-only Kintex-7 instrument for the ADT/I2C shared-clock investigation.
This is a small event-compressed logic analyzer using the existing raw JTAG
transport; it does not require LiteX firmware or change the MCU FLASH.

Inputs only: SDA H22, SCL H21, RESET_N B26. Existing RESET pull-up and MCU SPI
slave are retained from the known passive image. No SDA/SCL output driver,
pull-up or pull-down is added. ADT CS/DIN/DOUT are not accessible through these
three pins, so this instrument records clock timing and bus levels, not full
ADT command/data decoding. SPI `CS` in this design is the MCU-to-FPGA chip
select, not the ADT chip select.

Two-stage synchronizers sample lines at MCLK1 (runner measures it with a host-timed JTAG counter; the first
physical attempt measured about 40 MHz). Event RAM holds 2048 entries: initial line
state at time zero, then each observed state change with a 32-bit cycle stamp.
Capture stops when full or after 120000000 cycles (about 3 s at 40 MHz). Events shorter than the
sampling aperture can be missed; asynchronous signals changing together may
appear on adjacent samples. Clock tolerance and host timing limit absolute
nanosecond accuracy. No amplitude, slew-rate or analog clamping measurement.

USER1 returns the report, USER2 selects RAM and toggles capture arm. Header
fields (bit offsets): magic 0, free-running ticks 32, lines 64, count 72,
active 88, elapsed 96, selected event timestamp 128 / line state 160,
selected RAM address 192. Read only once capture has stopped. Event bit order
is SDA/SCL/RESET_N (least significant first).

Build using the shared Vivado environment and `build.tcl`. `test_capture.v`
is simulation-only and must not be included in synthesis. Its behavioral
primitive/SPI/JTAG stubs test capture logic and report packing, not vendor
BSCAN operation or the inherited SPI implementation. Run Icarus with
`-g2012 -s tb pm12_analyzer.v test_capture.v`.

`run_capture.py --out <new-absolute-evidence-directory>` takes both board/ICE
leases, verifies current passive magic 504d5a02 and ADT ID C3, loads the
instrument into FPGA SRAM, captures a bounded TEMP console read with periodic
ISR traffic, exports CSV/VCD and restores the exact known passive image in a
finally block. It checks ADT/PSU/scratch-test after restoration. No MCU or
nonvolatile programming. The current helper uses existing host-specific
container, cable, UART and restore-payload locations explicitly.

The FPGA itself needs its power rails to remain alive. It cannot preserve a
BRAM trace through loss of its own supply. An unpowered-hub test requires the
hub supply to be independently absent while FPGA sampling remains powered;
ordinary idle traffic alone cannot validate that condition.
