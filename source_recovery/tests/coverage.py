#!/usr/bin/env python3
"""Non-overlapping original routine spans; excludes unverified C implementation bytes."""
import pathlib,json
root=pathlib.Path(__file__).resolve().parents[2]
rows=[
('0x08e4','0x098a','pm_fpga_settings_init','Settings initialization, 0x0FFF final unlock value'),
('0x098a','0x09de','pm_fpga_settings_reset','Channels and restart reason'),
('0x0c96','0x0d10','pm_system_deinit','Shutdown register/memory sequence'),
('0x0e88','0x0ee0','pm_console_dre','DRE ISR body; prologue/epilogue ABI not recovered'),
('0x0ee0','0x0f54','pm_console_rxc','RXC ISR body; prologue/epilogue ABI not recovered'),
('0x2130','0x214c','pm_scale_signed','Signed fixed-point scaling (old misleading fpga_is_ready label)'),
('0x214c','0x2174','pm_scale_unsigned','Staged unsigned saturation'),
('0x2174','0x2208','pm_ths_write24','THS write wire trace'),
('0x2208','0x22aa','pm_ths_read16','THS read, selector handling'),
('0x22aa','0x230e','pm_dac_send','DAC SPI control/header'),
('0x230e','0x2368','pm_fpga_write16','FPGA SPI write'),
('0x2368','0x23ca','pm_fpga_read16','FPGA SPI read, incoming R21 bits'),
('0x23ca','0x2486','pm_fpga_read_bc','Eight-byte read'),
('0x2486','0x24ce','pm_pll_write32','LSB-first PLL write'),
('0x24ce','0x2530','pm_pll_read32','PLL read request'),
('0x2598','0x25be','pm_adt7311_write8','ADT command + one byte'),
('0x25be','0x25ea','pm_adt7311_exchange16','ADT command + MSB-first word'),
('0x25ea','0x2608','pm_adt7311_clear','Four FF reset bytes'),
('0x2608','0x2634','pm_adt7311_byte','Bitwise exchange'),
('0x2634','0x26ac','pm_parse_integer','16-bit decimal parser'),
('0x26ac','0x26f8','pm_parse_hex','1..4 uppercase hexadecimal digits'),
('0x26f8','0x2720','pm_send_hex16','Four hex digits; historical name said 32bit'),
('0x2720','0x272e','pm_send_hex_digit','Full digit + send'),
('0x272e','0x281e','pm_send_decimal','Five entry points and shared formatter'),
('0x281e','0x2826','pm_send_crlf','CR LF'),
('0x2826','0x2836','pm_send_flash_string','NUL-terminated LPM string'),
('0x2836','0x28ac','pm_console_next','Two entry points: raw/folded receive'),
('0x28ac','0x2916','pm_console_send','UART direct/queued send'),
]
covered=set(); data=[]
for start,end,name,note in rows:
 a,b=int(start,0),int(end,0); region=set(range(a,b)); assert not covered&region; covered|=region
 data.append({'start':a,'end_exclusive':b,'original_bytes':b-a,'c_function':name,'notes':note})
baseline=json.loads((root/'docs/asm_audit.json').read_text())['symbolic_asm_bytes']
report={'coverage_measure':'original routine spans represented by functional C, INCLUDING original register/ISR scaffolding; NOT bytes replaced in running firmware','original_routine_bytes':len(covered),'original_symbolic_code_bytes':baseline,'percentage':100*len(covered)/baseline,'original_entry_points':33,'replaced_in_exact_firmware_bytes':0,'regions':data}
(root/'source_recovery/build/coverage.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'Functional C corresponds to {len(covered)} original routine-span bytes / {baseline} ({report["percentage"]:.2f}%). Original ABI integration remains pending.')
