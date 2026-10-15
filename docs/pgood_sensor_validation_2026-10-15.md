# PGOOD sensor diagnostics checkpoint — 2026-10-15

Board-tested firmware branch: `development/pgood-adt-readback`, implementation commit `6965a07`.
Canonical development FLASH SHA256: `a016c00d39617d106ca855c03a62170d918f2e9a93514e7962bdc0cb7017c23e`.
Physical readback matched the build. Boot, EEPROM, fuses, lock and signature memories remained unchanged.

UART commands `@INA 4`, `@INA 5`, `@INA 6` read the three INA3221 monitors; `@POWER` reads all three. Measurements use nominal shunts, without calibration. Route5 channel1 has no fitted R15: its floating voltage must not be interpreted as a supply measurement; current is NA. Route6 channel3 has no shunt.

The temperature ISR now requires EN_PSU and both PGOOD inputs before claiming the shared clock. The UART diagnostics retain powered guards and bus ownership exclusion. Tests covered 5376 unready GPIO/SREG/ownership contexts and 1536 ready contexts. The original native SPI byte timing is retained.

Physical console regression passed 43 checks. Of 33 common command responses, 27 were byte-identical; remaining differences concern help, diagnostic/state/temperature/scratch-counter values. State-changing legacy ON/OF were excluded because they write EEPROM D1. Stress testing passed 12 cycles: 174 successful monitor reads and 6 expected BUSY responses, with valid temperature, PSU ON and PLL LOCKED.

## Laboratory limitation

Original ADT startup configuration still executes before later PSU readiness. This release DOES NOT fix automatic startup configuration. After stable PGOOD, explicit `@TEMP RESTORE` was used and read back CONFIG=50, TCRIT=2300 and THIGH=1E00; THYST=05 and TLOW=0500 were preserved. The command is state-changing and its SENT reply requires subsequent `@TEMP ALL` verification. This checkpoint is a lab prerelease, not production/cold-boot acceptance.

The passive FPGA analyzer (USER1 magic504d5a03) was left resident in SRAM, capture stopped. Healthy powered captures do not establish that an unpowered I2C hub cannot clamp the shared clock. FPGA power-loss/fault-injection and the red LED cause remain unresolved.

Original recovered baseline is separate: root `make exact-check` must remain zero-difference, canonical SHA256 `e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`. Intentional development image is not binary-identical to that baseline.

Evidence: `docs/evidence/pgood-2026-10-15/`. Full local raw backups remain under `/home/codex-hil/artifacts/pm12-i2c-validation-2026-10-15/`. Only application PDI writes and FPGA SRAM loading were used; no chip erase.
