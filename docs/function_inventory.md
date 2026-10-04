# Accepted binary-exact source inventory

Generated from recovered_exact and verified against the complete canonical PM.hex.
Regenerate the build report with `make c-progress`. `make exact-check` is the acceptance gate.

| Measure | Count |
|---|---|
| Application entries | 89 |
| C_BINARY_EXACT | 2 |
| C_WITH_EXACT_ASM_HELPER | 12 |
| ASM_EXACT | 75 |
| Bootloader ASM procedures | 9 |

Counts include 10 original ISRs, main and shared formatter entries. All 806 original
code/data text symbols retain their addresses. Unconverted entries are verified exact
ASM; their classification does not claim that further clean C recovery is impossible.

Of 10836 executable bytes, compiler-generated C accounts for **548 (5.0572%)** and
ASM for **10288 (94.9428%)**. The latter includes **216 inline ASM helper bytes**.
For the 10058 application executable bytes alone, C is **5.4484%** and ASM **94.5516%**.
All 778 boot executable bytes are ASM. Data (710 bytes) and erased/padding regions
are excluded. C-hosted function regions total 764 bytes; this larger number includes
helpers and is not reported as pure C coverage.

Golden and rebuilt canonical SHA256:
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`. **Differing bytes: 0**.

| Byte address | Original symbol | Classification |
|---|---|---|
| 0x01E2 | DMA_CH1_vect_isr | ASM_EXACT |
| 0x046C | FUN_code_000236 | ASM_EXACT |
| 0x049E | TCC0_OVF_vect_isr | ASM_EXACT |
| 0x08E4 | fpga_settings_init | ASM_EXACT |
| 0x098A | fpga_settings_reset | ASM_EXACT |
| 0x09DE | FUN_code_0004ef | ASM_EXACT |
| 0x0A4C | PORTD_INT0_vect_isr | ASM_EXACT |
| 0x0A9C | FUN_code_00054e | ASM_EXACT |
| 0x0ABC | PORTB_INT0_vect_isr | ASM_EXACT |
| 0x0AE0 | PORTF_INT1_vect_isr | ASM_EXACT |
| 0x0B68 | FUN_code_0005b4 | C_WITH_EXACT_ASM_HELPER |
| 0x0B96 | PORTE_INT0_vect_isr | ASM_EXACT |
| 0x0C2A | PORTE_INT1_vect_isr | ASM_EXACT |
| 0x0C7E | set_status_and_vd8_led | C_BINARY_EXACT |
| 0x0C96 | system_deinit | C_WITH_EXACT_ASM_HELPER |
| 0x0D10 | system_init | C_WITH_EXACT_ASM_HELPER |
| 0x0DDC | CDCE62005_control_rst | ASM_EXACT |
| 0x0E20 | PORTF_INT0_vect_isr | ASM_EXACT |
| 0x0E88 | USARTF0_DRE_vect_isr | ASM_EXACT |
| 0x0EE0 | USARTF0_RXC_vect_isr | ASM_EXACT |
| 0x0F54 | main | ASM_EXACT |
| 0x115C | fpga_data_exchange | ASM_EXACT |
| 0x12EA | cli_prompt_parse | ASM_EXACT |
| 0x157A | unlock_programming | ASM_EXACT |
| 0x15A6 | fpga_firmware_update | ASM_EXACT |
| 0x1664 | FUN_code_000b32 | C_BINARY_EXACT |
| 0x167E | FUN_code_000b3f | C_WITH_EXACT_ASM_HELPER |
| 0x16B2 | FUN_code_000b59 | ASM_EXACT |
| 0x1710 | FUN_code_000b88 | C_WITH_EXACT_ASM_HELPER |
| 0x171E | FUN_code_000b8f | C_WITH_EXACT_ASM_HELPER |
| 0x173A | FUN_code_000b9d | ASM_EXACT |
| 0x1772 | FUN_code_000bb9 | ASM_EXACT |
| 0x17E4 | FUN_code_000bf2 | ASM_EXACT |
| 0x1808 | FUN_code_000c04 | C_WITH_EXACT_ASM_HELPER |
| 0x1840 | FUN_code_000c20 | ASM_EXACT |
| 0x188E | cli_send_ch_mean_amplitude | ASM_EXACT |
| 0x18BE | cli_send_adc_baseline_dispersion | ASM_EXACT |
| 0x1910 | cli_send_tdc_data | ASM_EXACT |
| 0x19AE | eeprom_settings_save | ASM_EXACT |
| 0x1A0E | FUN_code_000d07 | ASM_EXACT |
| 0x1A3E | cdce62005_rst | ASM_EXACT |
| 0x1A64 | cli_send_system_status | ASM_EXACT |
| 0x1BE4 | alarms_clear | ASM_EXACT |
| 0x1C62 | channels_read | ASM_EXACT |
| 0x1CF2 | cli_send_channel_cdf_adc | ASM_EXACT |
| 0x1D62 | fpga_set_trg_charge_lvls | ASM_EXACT |
| 0x1D92 | fpga_set_trg_settings | ASM_EXACT |
| 0x1DB6 | fpga_set_adc_range_corr | ASM_EXACT |
| 0x1E2E | fpga_set_tdc_values | ASM_EXACT |
| 0x1E7C | fpga_set_threshold_calibration | ASM_EXACT |
| 0x1EE0 | FUN_code_000f70 | ASM_EXACT |
| 0x1F3A | fpga_set_ch_adc_delay | ASM_EXACT |
| 0x1F8E | fpga_set_ch_cfd_threshold | ASM_EXACT |
| 0x1FEC | fpga_set_adc_zero | ASM_EXACT |
| 0x204E | fpga_set_ch_cfd_zero | ASM_EXACT |
| 0x20A6 | FUN_code_001053 | ASM_EXACT |
| 0x20D0 | FUN_code_001068 | ASM_EXACT |
| 0x20EC | dac_set_value_2 | ASM_EXACT |
| 0x2104 | dac_set_value | ASM_EXACT |
| 0x211C | FUN_code_00108e | ASM_EXACT |
| 0x2130 | fpga_is_ready | ASM_EXACT |
| 0x214C | FUN_code_0010a6 | ASM_EXACT |
| 0x2174 | ths788_write | ASM_EXACT |
| 0x2208 | ths788_read | ASM_EXACT |
| 0x22AA | dac_send_value | ASM_EXACT |
| 0x230E | fpga_msg_send_t2 | ASM_EXACT |
| 0x2368 | fpga_msg_read_t1 | ASM_EXACT |
| 0x23CA | FUN_code_0011e5 | ASM_EXACT |
| 0x2486 | CDCE62005_send_control_settings | C_WITH_EXACT_ASM_HELPER |
| 0x24CE | FUN_code_001267 | ASM_EXACT |
| 0x2530 | fpga_send_mcu_ts | ASM_EXACT |
| 0x2598 | adt7311_8bit_rw | C_WITH_EXACT_ASM_HELPER |
| 0x25BE | adt7311_16bit_rw | C_WITH_EXACT_ASM_HELPER |
| 0x25EA | adt7311_faults_clr | C_WITH_EXACT_ASM_HELPER |
| 0x2608 | adt7311_byte_rw | ASM_EXACT |
| 0x2634 | cli_get_integer | ASM_EXACT |
| 0x26AC | cli_get_hex | ASM_EXACT |
| 0x26F8 | cli_send_32bit_hex | ASM_EXACT |
| 0x2720 | cli_send_digit_hex | ASM_EXACT |
| 0x272E | FUN_code_001397 | ASM_EXACT |
| 0x2736 | cli_send_temperature | ASM_EXACT |
| 0x273E | FUN_code_00139f | ASM_EXACT |
| 0x2746 | cli_send_int16 | ASM_EXACT |
| 0x274E | cli_send_uint16 | ASM_EXACT |
| 0x281E | cli_send_crlf | C_WITH_EXACT_ASM_HELPER |
| 0x2826 | cli_send_msg | ASM_EXACT |
| 0x2836 | cli_get_next_byte | ASM_EXACT |
| 0x283C | cli_get_next_char | ASM_EXACT |
| 0x28AC | cli_send_buf | ASM_EXACT |

Boot body entries (vector stubs excluded): 0x0201E2, 0x02023A, 0x0202A0, 0x0203D6, 0x0203E0, 0x0203F4, 0x02041C, 0x02042A, 0x02047C.

Historical behavioral integration counts are archived separately in
mixed_recovery_reads_function_inventory.md; they are not current baseline C counts.
