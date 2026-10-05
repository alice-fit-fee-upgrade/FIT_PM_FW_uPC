#!/usr/bin/env python3
"""Rerun retained historical functional suites; record successes individually."""
from pathlib import Path
import subprocess,time,json,hashlib
r=Path(__file__).resolve().parents[2];rows=[]
subprocess.run(['make','-C','mixed_c_asm','all'],cwd=r,check=True)
names=['check_gpio_instructions','check_fpga_settings_contract','check_fpga_settings','check_system_deinit','check_system_init','check_pll_reset','check_pll_read','check_adt','check_fpga_stamp','check_hex_digit','check_hex16','check_crlf','check_layout','check_abi','check_unsigned_abi','check_callers','check_dac_edges','check_status_gate','check_crc_byte','check_flash_spi','check_flash_wait','check_fpga_state','check_status_led']
for name in names:
 t=time.monotonic();path=r/'docs'/'logical_c_test_logs'/f'{name}.log';path.parent.mkdir(exist_ok=True)
 with path.open('w') as f:p=subprocess.run(['python3',f'mixed_c_asm/tests/{name}.py'],cwd=r,stdout=f,stderr=subprocess.STDOUT)
 row={'suite':f'mixed_c_asm/tests/{name}.py','status':'PASS' if not p.returncode else 'FAIL','returncode':p.returncode,'elapsed_seconds':round(time.monotonic()-t,3),'log':str(path.relative_to(r)),'suite_sha256':hashlib.sha256((r/f'mixed_c_asm/tests/{name}.py').read_bytes()).hexdigest()};rows.append(row)
 (r/'docs/logical_c_historical_tests.json').write_text(json.dumps({'suites':rows,'complete':len(rows)==len(names),'mixed_flash_sha256':hashlib.sha256((r/'mixed_c_asm/build/flash_mixed.bin').read_bytes()).hexdigest()},indent=2)+'\n')
 print(name,row['status'],row['elapsed_seconds'],flush=True)

if any(row['returncode'] for row in rows): raise SystemExit(1)
