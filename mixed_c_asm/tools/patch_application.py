#!/usr/bin/env python3
"""Replace only the two original scaler bodies with an anchored JMP.
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
start=s.index('FUN_code_0010a6:')
end=s.index('.org (0x10ba << 1)',start)
body=s[start:end]
# Only the two branches inside this routine may target its internal label.
assert s[:start].count('LAB_code_0010b7') + s[end:].count('LAB_code_0010b7') == 0
if 'mul         R19,R21' not in body or 'LAB_code_0010b7:' not in body:
 raise ValueError('unsigned scaler changed; audit required')
s=s[:start]+'''FUN_code_0010a6:
    jmp pm_scale_unsigned_bridge
.org (0x10b7 << 1), 0xff
LAB_code_0010b7:
    /* Internal original saturation label retained for address audit. */
'''+s[end:]
start=s.index('FUN_code_001068:')
end=s.index('.org (0x1076 << 1)',start)
body=s[start:end]
assert 'ldi         R18,0x72' in body and 'movw        R20,R18' in body
s=s[:start]+'''FUN_code_001068:
    jmp pm_dac_inverse_bridge
'''+s[end:]
start=s.index('FUN_code_001053:')
end=s.index('.org (0x1068 << 1)',start)
body=s[start:end]
assert 'ldi         R18,0xc' in body and 'com         R17' in body
s=s[:start]+'''FUN_code_001053:
    jmp pm_dac_calibrated_bridge
'''+s[end:]
start=s.index('dac_set_value_2:')
end=s.index('.org (0x1082 << 1)',start)
assert 'ori         R22,0x2' in s[start:end]
s=s[:start]+'''dac_set_value_2:
    jmp pm_dac_signed2_bridge
'''+s[end:]
s='.global dac_send_value\n'+s
(root/'mixed_c_asm/generated/application.S').write_text(s)
