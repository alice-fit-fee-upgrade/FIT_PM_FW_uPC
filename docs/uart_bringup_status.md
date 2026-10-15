# PM12 console bring-up — 2026-10-15

Received result: **PARTIAL**, not a passed console or firmware validation.
Evidence snapshot: `handoffs/pm12-uart-2026-10-15.json`.
Original captures: `/home/codex-hil/fit-bringup/evidence/pm12-uart-2026-10-15/`.

The Gembird/Prolific adapter is
`/dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller-if00-port0`
(VID:PID 067b:2303; ttyUSB29 at this test only), 115200 baud, 8N1.
Three RS commands produced consecutive fragments of the original startup banner,
not completed RS replies. RTS/DTR combinations did not drain pending output.
Host CTS=TRUE does not establish that MCU CTS is asserted.

FIT_PM12v1.1 PDF page 4, X6 RJ45 (schematic pin numbers, not a guessed plug view):

| X6 pin | Board signal | Direction at board |
|---|---|---|
| 2 | TX | Output |
| 3 | RX | Input |
| 5 | GND | Reference |
| 7 | CTS | Input |
| 8 | RTS | Output |

MCU PF1 is CTS input, active LOW after the RS232 receiver; PF0 is RTS output.
The older upstream README flow-control labels must not override this evidence.
`console_cts_interrupt.c` updates readiness at RAM 0x2004 and can start TX when
PF1 is LOW. `console_tx_interrupt.c` stops DRE IRQ when readiness is zero.
`console_rx_interrupt.c` has no direct TX restart operation. RX-coupled CTS or
missing handshake is an unproven hypothesis explaining the partial output.

Bring-up thread closed the serial port, deasserted RTS/DTR and released its lock.
No firmware, reset, PSU or flash changes were made. Do not add commands to a
potentially blocked TX queue. Next action requires the user's wiring information
(TX/RX/GND only versus RTS/CTS included) and verification of the host RTS to board
CTS path. That thread owns physical console investigation; coordinate through
the common PM12 lock before any later serial/programming operation.
