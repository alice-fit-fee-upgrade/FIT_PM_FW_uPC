# Historical experiment: PLL, temperature transactions and console recovery

This 27-entry tree is NOT the accepted baseline: its FLASH differs from PM.hex.
The user subsequently required binary identity for every accepted C conversion.
See recovered_exact and docs/exact_c_checkpoint.md for the current baseline.

Eight more original entries execute compiled C, in device_reads.c and
console_output.c. Each replacement was built and checked against actual original
AVR execution before adding the next. Exact ASM and original analyses are untouched.

| Original byte range | Implementation | Focused comparisons | Additional tested stack |
|---|---|---|---|
| 0x24CE..0x252F | pm_pll_read_c | 49152 | 31 |
| 0x2598..0x25BD | pm_adt8_c | 67072 | 27 |
| 0x25BE..0x25E9 | pm_adt16_c | 67072 | 29 |
| 0x25EA..0x2607 | pm_adt_faults_c | 67072 | 25 |
| 0x2530..0x2597 | pm_fpga_stamp_c | 32768 | 23 |
| 0x2720..0x272D | pm_hex_digit_c | 70656 | 17 |
| 0x26F8..0x271F | pm_hex16_c | 75776 | 32 |
| 0x281E..0x2825 | pm_crlf_c | 20480 | 28 |

## Original behavior preserved

PLL read first sends control word 0x0000008E through the unchanged original
serializer, with no added CLI/SEI. It then asserts PF4, sends four zero bytes,
polls SPIC_STATUS bit 7 for each byte, reads SPIC_DATA and releases PF4. The
little-endian response returns in R16..R19. Other registers are preserved; final
SREG is (incoming & 0xE1) | 2. Tests cover every byte in each response position,
all incoming flags, all ready status values, and bounded poll delays.

ADT transaction wrappers keep the byte primitive at 0x2608 in ASM. Eight-bit
transactions send command then data and discard the response, preserving all GPRs.
Sixteen-bit transactions send command, high data, low data; received low/high
bytes return in R20/R21, while command and transmit data registers are restored.
Fault clearing sends four FF bytes and leaves R16=0x10, R17=last raw GPIO sample,
R18=0. An adapter captures the primitive's final SREG before C operations can
change it. Tests compare all ordered CS, data, clock writes and GPIO reads,
including noise in non-data GPIO bits. Within-byte instructions are unchanged;
inter-byte timing and asynchronous behavior have not been physically validated.

The FPGA timestamp entry sends SPIC_CTRL=0xD1, asserts PD0, sends 3D 40 and the
original two FLASH words at 0x2B92..0x2B95, high byte first, then releases PD0.
It uses actual LPM instructions, not a copied table. Every register is restored;
final SREG is (incoming & 0xC0) | 2. Tests cover all flags and all SPI-ready bytes.

HEX digit masks the incoming value to four bits and emits uppercase ASCII.
The original name cli_send_32bit_hex is misleading: it prints four digits of
R17:R16, preserving all registers. GNU-callable UART adapters retain the original
sender at 0x28AC and its CLI/SEI, queue and full-buffer behavior. Tests also capture
ASCII and full SREG at entry to that original sender, preventing an apparently
correct final state from hiding wrong flags at an intermediate call. Every
16-bit input is checked, with additional all-flag cases in direct, queued, busy,
wrapped and full-buffer modes. CRLF delegates to the original FLASH-string sender
at 0x2826 using address 0x2984; it returns Z=0x2987 exactly as before.

The HEX16 test initially had a wrong trace-field index in its independent ASCII
assertion (SREG instead of ASCII). Correcting that assertion required no firmware
change; the full original/rebuilt trace comparison already agreed.

## Progress and limits

89 application entries: 27 integrated C, 62 still ASM. Functional C is available
for 48 entries, including 21 awaiting integration; 41 lack recovered C. Nine boot
procedures remain ASM, giving 71 unintegrated entries including boot. Counts
include main, original interrupts and shared formatter entry points.
Substituted original spans total 1484 bytes, 13.70% of 10836 symbolic code bytes.
Broader functional recovery covers 3266 span bytes, 30.14%. These measures include
retained ASM and ABI scaffolding, rather than pure emitted-C bytes.

All 806 original text symbols retain their addresses. Bridges occupy 2268 bytes
at 0x3000; C/helpers occupy 2464 at 0x4000. Layout permits changes only in the
replaced regions and those additions. No new static SRAM, data, BSS or CRT is
introduced; original vectors, strings, tables, startup and boot are unchanged.
Maximum additional tested stack across all integrated callers remains 35 bytes.
Whole-firmware stack budget, cycles, SPL suppression, asynchronous interrupts and
physical-device validation remain pending. No device is programmed.

## Validation evidence

Full mixed-check was interrupted during check_adt.py immediately on the strategy
change. It completed GPIO fixtures, the original settings audit, FPGA settings,
system init/deinit, PLL reset and PLL read for this image; remaining suites were
not completed. The log is mixed_recovery_reads_validation_interrupted.log.
The focused counts above belong to earlier per-step builds, not a completed
regression or reproducibility proof for the final experimental image.
No exhaustive behavioral tests were continued after the strategy change.
The portable C library is unchanged; its previous 390601-case result is historical.
Golden-only audits and synthetic opcode fixtures are counted separately from
actual original-versus-compiled-C execution comparisons.

Mixed replacements use anchored JMPs (difference class E), FF padding in vacated
routine regions (G), and bridges/C in previously erased FLASH (F). Independent
exact ASM retains zero mismatches; its golden SHA256 is
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.
