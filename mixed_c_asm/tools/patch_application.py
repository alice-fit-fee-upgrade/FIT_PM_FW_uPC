#!/usr/bin/env python3
"""Replace eleven audited original routine bodies with anchored JMPs.
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
start=s.index('dac_set_value:')
end=s.index('.org (0x108e << 1)',start)
assert 'ori         R22,0x1' in s[start:end]
s=s[:start]+'''dac_set_value:
    jmp pm_dac_signed1_bridge
'''+s[end:]
start=s.index('FUN_code_00108e:')
end=s.index('.org (0x1098 << 1)',start)
assert 'lds         R16,0x2157' in s[start:end] and 'sbrc        R16,0x4' in s[start:end]
s=s[:start]+'''FUN_code_00108e:
    jmp pm_status_gate_bridge
'''+s[end:]
start=s.index('FUN_code_000bf2:')
end=s.index('.org (0x0c04 << 1)',start)
body=s[start:end]
assert 'eor         R18,R23' in body and 'adc         R17,R17' in body
labels=[('LAB_code_000bf3',0x17e6),('LAB_code_000bfb',0x17f6),('LAB_code_000bfd',0x17fa),('LAB_code_000c01',0x1802)]
for name,address in labels:
 assert s[:start].count(name)+s[end:].count(name)==0,('external CRC entry',name)
replacement='FUN_code_000bf2:\n    jmp pm_crc_byte_bridge\n'
# Preserve original internal label addresses as non-callable padding markers.
for name,address in labels:
 if address==0x17e6: replacement+=f'.set {name}, FUN_code_000bf2 + 2\n'
 else: replacement+=f'.org 0x{address:x}, 0xff\n{name}:\n'
s=s[:start]+replacement+s[end:]
start=s.index('FUN_code_000b32:')
end=s.index('.org (0x0b3f << 1)',start)
assert 'sts         SPIE_CTRL,R16' in s[start:end] and 'ldi         R16,0x50' in s[start:end]
s=s[:start]+'''FUN_code_000b32:
    jmp pm_flash_init_bridge
'''+s[end:]
start=s.index('FUN_code_000b8f:')
end=s.index('.org (0x0b9d << 1)',start)
body=s[start:end]
assert 'ldi         R16,0x6' in body and 'LAB_code_000b95:' in body
assert s[:start].count('LAB_code_000b95')+s[end:].count('LAB_code_000b95')==0
s=s[:start]+'''FUN_code_000b8f:
    jmp pm_flash_enable_bridge
.org (0x0b95 << 1), 0xff
LAB_code_000b95:
'''+s[end:]
start=s.index('FUN_code_000c04:')
end=s.index('.org (0x0c20 << 1)',start)
body=s[start:end]
assert 'sts         SPIE_DATA,R22' in body and 'sts         SPIE_DATA,R20' in body
labels=[('LAB_code_000c09',0x1812),('LAB_code_000c0f',0x181e),('LAB_code_000c15',0x182a),('LAB_code_000c1b',0x1836)]
replacement='FUN_code_000c04:\n    jmp pm_flash_address_bridge\n'
for name,address in labels:
 assert s[:start].count(name)+s[end:].count(name)==0,('external SPI poll entry',name)
 replacement+=f'.org 0x{address:x}, 0xff\n{name}:\n'
s=s[:start]+replacement+s[end:]
s='.global dac_send_value\n.global cli_send_msg\n'+s
(root/'mixed_c_asm/generated/application.S').write_text(s)
