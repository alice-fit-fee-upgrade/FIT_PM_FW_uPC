#!/usr/bin/env bash
# Software-only build. No programmers, UART, JTAG or network calls.
set -euo pipefail
cd "$(dirname "$0")/../.."
unset PM_DAC_CANDIDATE PM_SCALING_CANDIDATE
avr-gcc --version
avr-ld --version
python3 --version
make clean
make exact-check
mkdir -p handoff-artifacts/exact
cp recovered_exact/build/reconstructed.{elf,hex} recovered_exact/build/flash_rebuilt.bin handoff-artifacts/exact/
make -C development clean
make development
mkdir -p handoff-artifacts/development
cp development/build/development.{elf,hex,bin,map,dis} handoff-artifacts/development/
make -C development clean
PM_DAC_CANDIDATE=1 make development
mkdir -p handoff-artifacts/dac
cp development/build/development.{elf,hex,bin,map,dis} handoff-artifacts/dac/
sha256sum reference/flash_golden.bin handoff-artifacts/exact/flash_rebuilt.bin handoff-artifacts/development/development.bin handoff-artifacts/dac/development.bin
find handoff-artifacts -type f ! -name SHA256SUMS -print0 | sort -z | xargs -0 sha256sum > handoff-artifacts/SHA256SUMS
