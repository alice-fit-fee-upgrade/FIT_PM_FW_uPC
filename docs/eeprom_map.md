# EEPROM map

EEPROM is separate state, not part of linked FLASH. Supplied PM.eep and repo eeprom.hex
normalize identically to 2,048 bytes (0x000..0x7FF). Golden snapshot: reference/eeprom_golden.bin.
Data-space EEPROM window is 0x1000..0x17FF: distinguish mapped addresses from EEPROM offsets.
The firmware writes NVM_CTRLB = 0x08 at startup, enabling mapped EEPROM, then copies offset
0x000..0x0D1 inclusive (210 bytes) from Y=0x1000 to Z=0x2163, stopping at Z=0x2235.
Evidence: original code at word LAB_code_000895 (byte 0x112A), setup near word 0x0847.
RAM 0x2000..0x2442 inclusive is cleared beforehand; this is not an EEPROM default initialization.

| EEPROM offset | Size | Accesses | Inferred meaning | Confidence | Evidence |
|---|---:|---|---|---|---|
| 0x000..0x017 | 24 | Startup read; settings save write | 12 16-bit channel threshold/calibration words, RAM 0x2163..0x217A | Medium semantic; high copy range | Startup copy; FPGA register B0..BB hypothesis in README; calibration routine 0x1E7C |
| 0x018..0x023 | 12 | Startup read; save write | THS788 initial values, RAM 0x217B..0x2186 | Medium | THS788 reset/startup code, prior README; previous RAM ending 0x2182 is inconsistent with 12-byte span |
| 0x024..0x053 | 48 | Startup read; save write | FPGA range corrections, RAM 0x2187..0x21B6 | Medium | Setting routine 0x1DB6; register hypothesis 25..3C |
| 0x054..0x06B | 24 | Startup read; save write | Channel TDC settings, RAM 0x21B7..0x21CE | Medium | Setting routine 0x1E2E; README's 0x22CE end is a typo/hypothesis error |
| 0x06C..0x0CB | 96 | Startup read; save write | FPGA channel settings, RAM 0x21CF..0x222E | Medium | ADC-zero updates near word 0x100E, 8-byte/channel stride; FPGA register 80..AF hypothesis |
| 0x0CC | 1 | Startup read; save write | Gate byte, RAM 0x222F | High access/size; medium semantic | fpga_settings_init loads only R16 at 0x222F and clears R17 before FPGA register 00 write |
| 0x0CD..0x0CE | 2 | Startup read; save write | Trigger saturation word, RAM 0x2230..0x2231 | High access/size; medium semantic | Original LDS pair in fpga_settings_init before register 3D write |
| 0x0CF..0x0D0 | 2 | Startup read; save write | Board identifier word, RAM 0x2232..0x2233 | High access/size; medium semantic | Original LDS pair before register BD write |
| 0x0D1 | 1 | Explicit command write; startup read | Persisted power/configuration flag, RAM 0x2234 | Medium meaning; high access | Original mapped 0x10D1 write and subsequent value test |
| 0x0D2..0x7FF | 1838 | No direct access proved | Unassigned state | Low | Whole dump preserved; indirect/boot behavior not exhaustively path-proven |

Save path begins near word 0x0CD6 (byte 0x19AC): NVM_CMD 0x36 clears EEPROM page buffer,
Y=0x0FFF then ADIW before each mapped access, Z=0x2163..0x2234. Only changed bytes are
written to page buffer, dirty flag R19 gates commit helper word 0x0D07 (byte 0x1A0E).
Boundary checks commit pages while continuing across the entire 210-byte configuration window.
NVM_CMD 0x35 commits erase/write EEPROM page with NVM_ADDR0/1 from Y and CCP-protected
NVM_CTRLA execution. Individual D1 path uses the same buffer clear and page-commit sequence.
Do not turn those mapped pointer values into raw EEPROM offsets without checking NVM semantics.

Flash table at 0x2916..0x2961 contains PLL defaults; CDCE reset routine at 0x1A3E consumes
FLASH constants. No evidence that PLL register defaults are stored in EEPROM in this image.
THS/FPGA configuration inference is stronger than PLL inference but needs transaction tracing.
No proven EEPROM validity checksum or comprehensive default-on-erased-EEPROM path is established.
Protocol names WR/ON/OF are inherited hypotheses: dispatcher/dataflow corroborate settings-save and
power control paths; full command grammar and physical timing are not yet independently validated.

C-recovery correction: the earlier map inherited an incorrect two-byte gate assumption.
Tracing original settings initialization proves a one-byte gate and the word pairs above.
Consequently the D1 power-state byte does NOT overlap the actual board-ID word consumed by
FPGA settings initialization. It overlapped only the earlier unverified Board S/N hypothesis.
Original code remains untouched. Independently verified C in src/settings.c preserves these accesses
and the final 0x0FFF value written to register 7C (older README said 0xFFFF).
