#!/usr/bin/env python3
"""Replace only the original signed scaler body with an anchored JMP.
No golden data is embedded. All upstream and exact-source files stay untouched.
"""
from pathlib import Path
import subprocess
root=Path(__file__).resolve().parents[2]
baseline=root/'mixed_c_asm/build/application_exact.S'
subprocess.run(['python3',str(root/'tools/convert_analysis.py'),'--output',str(baseline)],check=True)
s=baseline.read_text()
start=s.index('fpga_is_ready:\n')
end=s.index('.org (0x10a6 << 1)',start)
body=s[start:end]
expected=['eor         R17,R17','mul         R18,R20','mulsu       R21,R19','add         R17,R0']
if not all(text in body for text in expected): raise ValueError('upstream scaler changed; audit required')
s=s[:start]+'''fpga_is_ready:
    /* Mixed build: original symbol/address retained; new C implementation. */
    jmp pm_scale_signed_bridge
; next original function retains its exact address
'''+s[end:]
(root/'mixed_c_asm/generated/application.S').write_text(s)
