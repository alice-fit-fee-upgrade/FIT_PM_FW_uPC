#!/usr/bin/env python3
"""Ensure the verifier rejects corruption outside the allowed extension regions."""
import subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];BUILD=ROOT/'development/build'
p=BUILD/'development.bin';original=p.read_bytes()
try:
    for address in [0x100,0x12f0,0x3000,0x8000,0x20000]:
        corrupted=bytearray(original);corrupted[address]^=1;p.write_bytes(corrupted)
        result=subprocess.run([sys.executable,ROOT/'development/tools/check_layout.py'],capture_output=True,text=True)
        assert result.returncode!=0, ('unplanned corruption accepted',hex(address))
finally:p.write_bytes(original)
subprocess.run([sys.executable,ROOT/'development/tools/check_layout.py'],check=True)
print('PASS layout rejection: vectors, continuation, unused nonreserved FLASH, window end and boot')
