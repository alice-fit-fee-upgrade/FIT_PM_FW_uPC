# Separate diagnostic candidate f9869c9 — physical validation handoff

Bring-up reports application-only programming and basic UART PASS on PM12 with
Atmel-ICE J41800043065, signature 1E9742. This is the separate diagnostic checkout
`f9869c9`, **not** recovery release `pm12-dev-001-timer-relocated` and not proof
of its timer relocation or future C implementation. No patches were imported.

Raw snapshot: `handoffs/pm12-diagnostic-f9869c9-hardware-2026-10-15.json`.
Original evidence directory:
`/home/codex-hil/fit-bringup/evidence/pm12-fw-validation-2026-10-15/`.

Reported pre-program FLASH matched the canonical original SHA256
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.
Candidate HEX SHA256:
`93c010b4a13323259453b2b19b2154e73ba6f96968d4140cb778592881e0ac9d`.
Full readback matches the diagnostic candidate BIN, SHA256
`56fe5ccb19340b05a8810d93beb5cb496d24e7c1f7e0418828923b30ad2806d9`.
Application pages were programmed through APP_PAGE without chip erase. Evidence
reports boot, EEPROM, fuse0/1/2/4/5, lock, user/production/device signatures
unchanged. The physical fuse4 is reported F3; historical supplied ELF value E3
is retained as historical evidence. Do not replace golden files or fuse values.

Observed responses: @PING/@HELP/@HW, ADT7311 ID C3 and temperature 30.0 C,
three THS788 register 83 reads of 8010, CDCE62005 read 80009008. These observations
are not a complete peripheral/fault regression. RS initially reported PLL NOT
LOCKED; a register read alone is not proof of a locked clock. @PGTIME state 00
is NOT_STARTED after PDI, so this is not a successful PG timing measurement.

MCU remains on that diagnostic image. Bring-up owns ongoing passive-FPGA/SPI
walking-bit tests; results were pending at this handoff. Do not open UART,
programmer or FPGA concurrently. MCU INA/I²C and shared PA1 ownership protection
remain unimplemented. No hardware operations were performed by this recovery
thread while receiving this evidence.
