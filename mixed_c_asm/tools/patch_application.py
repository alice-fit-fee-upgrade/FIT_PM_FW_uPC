#!/usr/bin/env python3
"""Replace sixteen audited original routine bodies with anchored JMPs.
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
start=s.index('FUN_code_000b9d:')
end=s.index('.org (0x0bb9 << 1)',start)
body=s[start:end]
timed_end=body.index('\tldi         R16,0x10')
assert 'ldi         R24,0x40' in body[:timed_end] and 'sbiw        R24,0x1' in body[:timed_end]
replacement=body[:timed_end]+'    jmp pm_flash_wait_bridge\n'
for name,address in [('LAB_code_000ba7',0x174e),('LAB_code_000bad',0x175a)]:
 assert s[:start].count(name)+s[end:].count(name)==0,('external FLASH polling entry',name)
 replacement+=f'.org 0x{address:x}, 0xff\n{name}:\n'
s=s[:start]+replacement+s[end:]
start=s.index('FUN_code_00054e:')
end=s.index('.org (0x055e << 1)',start)
body=s[start:end]
assert 'sts         0x215b,R16' in body and 'cpi         R16,0x5' in body
assert s[:start].count('LAB_code_00055a')+s[end:].count('LAB_code_00055a')==0
s=s[:start]+"FUN_code_00054e:\n    jmp pm_fpga_state_bridge\n.org (0x055a << 1), 0xff\nLAB_code_00055a:\n"+s[end:]
start=s.index('set_status_and_vd8_led:')
end=s.index('.org (0x064b << 1)',start)
body=s[start:end]
assert 'sbic        GPIO_GPIOR0,0x1' in body and body.count('cbi         GPIO_GPIOR0,0x0')==2
assert s[:start].count('LAB_code_000646')+s[end:].count('LAB_code_000646')==0
s=s[:start]+"set_status_and_vd8_led:\n    jmp pm_status_led_bridge\n.org (0x0646 << 1), 0xff\nLAB_code_000646:\n"+s[end:]
start=s.index('fpga_settings_reset:')
end=s.index('.org (0x04ef << 1)',start)
body=s[start:end]
assert 'ldi         R18,0xbe' in body and 'lds         R16,0x2441' in body
replacement='fpga_settings_reset:\n    jmp pm_fpga_settings_reset_bridge\n'
for name,address in [('LAB_code_0004cd',0x99a),('LAB_code_0004d8',0x9b0)]:
 assert s[:start].count(name)+s[end:].count(name)==0,('external settings entry',name)
 replacement+=f'.org 0x{address:x}, 0xff\n{name}:\n'
s=s[:start]+replacement+s[end:]
start=s.index('fpga_settings_init:')
end=s.index('.org (0x04c5 << 1)',start)
body=s[start:end]
assert 'push        ZL' in body and 'ldi         R17,0xf' in body
replacement='fpga_settings_init:\n    jmp pm_fpga_settings_init_bridge\n'
for name,address in [('LAB_code_000488',0x910),('LAB_code_000494',0x928)]:
 assert s[:start].count(name)+s[end:].count(name)==0,('external settings entry',name)
 replacement+=f'.org 0x{address:x}, 0xff\n{name}:\n'
s=s[:start]+replacement+s[end:]
s='.global fpga_msg_send_t2\n.global dac_send_value\n.global cli_send_msg\n.global FUN_code_000b9d\n'+s
(root/'mixed_c_asm/generated/application.S').write_text(s)
