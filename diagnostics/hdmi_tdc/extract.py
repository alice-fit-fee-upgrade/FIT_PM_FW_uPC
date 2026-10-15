#!/usr/bin/env python3
"""Stage unmodified upstream blocks and explicitly non-standalone PM excerpts."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

REV = '857b773eec28282b357d4668ab934e95f41b6fe7'
FILES = [
 'firmware/FT0/PM/hdl/PM12_pkg.vhd',
 'firmware/FT0/PM/hdl/trigger.vhd',
 'firmware/FT0/PM/hdl/TDCCHAN.vhd',
 'firmware/FT0/PM/hdl/pin_capt.vhd',
 'firmware/FT0/PM/hdl/fit.vhd',
 'firmware/FT0/PM/ipcore_properties/TDC_FIFO.txt',
 'firmware/FT0/PM/ipcore_properties/PLL320.txt',
 'firmware/FT0/PM/xdc/ios.xdc',
 'firmware/FT0/PM/xdc/Timing.xdc',
 'firmware/FT0/TCM_v1/hdl/HDMIRX.vhd',
 'firmware/FT0/TCM_v1/hdl/pm-spi.vhd',
 'firmware/FT0/TCM_v1/hdl/tcm_side.vhd',
 'firmware/FT0/TCM_v1/ipcore_properties/COUNTER_FIFO.txt',
]

def main():
 p = argparse.ArgumentParser(description=__doc__)
 p.add_argument('repository', type=Path)
 p.add_argument('--out', type=Path, default=Path(__file__).parent/'generated')
 a = p.parse_args()
 result = {'repository': 'https://github.com/alice-fit-fee-upgrade/FIT_TCM_GW_FPGA',
           'revision': REV, 'status': 'SOURCE_EXTRACTION_ONLY_NOT_HARDWARE_VALIDATED', 'files': []}
 for name in FILES:
  # Read pinned git objects, never possibly modified working-tree files.
  raw = subprocess.check_output(['git', '-C', str(a.repository), 'show', REV+':'+name])
  dest = a.out/name
  dest.parent.mkdir(parents=True, exist_ok=True)
  dest.write_bytes(raw)
  result['files'].append({'path': name, 'size': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()})
 fit = (a.out/FILES[4]).read_bytes().splitlines(keepends=True)
 # This is a verbatim process body, with original register-map dependencies.
 start = next(i for i,l in enumerate(fit) if l.strip() == b'process (HSCKI, HSELI, RESET)')
 end = next(i for i in range(start,len(fit)) if fit[i].strip() == b'end process;')+1
 name = 'pm_hdmi_spi_slave_process.vhd.fragment'
 (a.out/name).write_bytes(b''.join(fit[start:end]))
 result['slave_process'] = {'path': name, 'source': FILES[4], 'first_line': start+1, 'last_line': end,
  'standalone': False, 'note': 'Keep read map, side effects, flags and CDC dependencies; not generic MCU SPI.'}
 (a.out/'manifest.json').write_text(json.dumps(result, indent=2)+'\n')
 print('Extracted',len(FILES),'pinned upstream files and PM HDMI SPI process; no hardware access.')

if __name__ == '__main__': main()
