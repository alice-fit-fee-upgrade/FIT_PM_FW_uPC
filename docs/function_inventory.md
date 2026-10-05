# Accepted binary-exact source inventory

Generated from recovered_exact and verified against complete canonical PM.hex.
Regenerate with `make c-progress`; root `make exact-check` is the acceptance gate.

| Measure | Count |
|---|---|
| Application entries | 89 |
| C_BINARY_EXACT | 5 |
| C_WITH_EXACT_ASM_HELPER | 83 |
| ASM_EXACT | 1 |
| Bootloader ASM procedures | 9 |

Counts include 10 original ISRs, main and shared formatter entries. All 806 original
text symbols retain their addresses. Remaining ASM is reviewed in
[easy_conversion_assessment.md](easy_conversion_assessment.md); its classification
does not claim that larger further C recovery is impossible.

Of 10836 executable bytes, compiler-generated C accounts for **6178 (57.0137%)** and
ASM for **4658 (42.9863%)**, including **3850 inline ASM helper bytes**.
For the 10058 application executable bytes, C is **61.4237%** and ASM **38.5763%**.
Of C bytes, 394 are compiler-generated AVR primitives; 5784 are other C
operations. This subset is checked independently using the original instruction
index and GCC assembly provenance. All 778 boot executable bytes are ASM. Data (710 bytes) and erased/padding regions
are excluded. C-hosted function regions total 10028 bytes; this includes helpers
and is not pure C coverage. `exact_c_provenance.json` independently checks every
accepted helper range using GCC APP/NOAPP markers and GNU subsection ordering.

Golden and rebuilt canonical SHA256:
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`. **Differing bytes: 0**.

| Byte address | Original symbol | Classification |
|---|---|---|
| 0x01E2 | DMA_CH1_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x046C | FUN_code_000236 | C_WITH_EXACT_ASM_HELPER |
| 0x049E | TCC0_OVF_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x08E4 | fpga_settings_init | C_WITH_EXACT_ASM_HELPER |
| 0x098A | fpga_settings_reset | C_WITH_EXACT_ASM_HELPER |
| 0x09DE | FUN_code_0004ef | C_WITH_EXACT_ASM_HELPER |
| 0x0A4C | PORTD_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x0A9C | FUN_code_00054e | C_WITH_EXACT_ASM_HELPER |
| 0x0ABC | PORTB_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x0AE0 | PORTF_INT1_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x0B68 | FUN_code_0005b4 | C_WITH_EXACT_ASM_HELPER |
| 0x0B96 | PORTE_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x0C2A | PORTE_INT1_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x0C7E | set_status_and_vd8_led | C_BINARY_EXACT |
| 0x0C96 | system_deinit | C_WITH_EXACT_ASM_HELPER |
| 0x0D10 | system_init | C_WITH_EXACT_ASM_HELPER |
| 0x0DDC | CDCE62005_control_rst | C_WITH_EXACT_ASM_HELPER |
| 0x0E20 | PORTF_INT0_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x0E88 | USARTF0_DRE_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x0EE0 | USARTF0_RXC_vect_isr | C_WITH_EXACT_ASM_HELPER |
| 0x0F54 | main | C_WITH_EXACT_ASM_HELPER |
| 0x115C | fpga_data_exchange | C_WITH_EXACT_ASM_HELPER |
| 0x12EA | cli_prompt_parse | C_WITH_EXACT_ASM_HELPER |
| 0x157A | unlock_programming | C_WITH_EXACT_ASM_HELPER |
| 0x15A6 | fpga_firmware_update | C_WITH_EXACT_ASM_HELPER |
| 0x1664 | FUN_code_000b32 | C_BINARY_EXACT |
| 0x167E | FUN_code_000b3f | C_WITH_EXACT_ASM_HELPER |
| 0x16B2 | FUN_code_000b59 | C_WITH_EXACT_ASM_HELPER |
| 0x1710 | FUN_code_000b88 | C_WITH_EXACT_ASM_HELPER |
| 0x171E | FUN_code_000b8f | C_BINARY_EXACT |
| 0x173A | FUN_code_000b9d | C_WITH_EXACT_ASM_HELPER |
| 0x1772 | FUN_code_000bb9 | C_WITH_EXACT_ASM_HELPER |
| 0x17E4 | FUN_code_000bf2 | C_WITH_EXACT_ASM_HELPER |
| 0x1808 | FUN_code_000c04 | C_BINARY_EXACT |
| 0x1840 | FUN_code_000c20 | C_WITH_EXACT_ASM_HELPER |
| 0x188E | cli_send_ch_mean_amplitude | C_WITH_EXACT_ASM_HELPER |
| 0x18BE | cli_send_adc_baseline_dispersion | C_WITH_EXACT_ASM_HELPER |
| 0x1910 | cli_send_tdc_data | C_WITH_EXACT_ASM_HELPER |
| 0x19AE | eeprom_settings_save | C_WITH_EXACT_ASM_HELPER |
| 0x1A0E | FUN_code_000d07 | C_WITH_EXACT_ASM_HELPER |
| 0x1A3E | cdce62005_rst | C_WITH_EXACT_ASM_HELPER |
| 0x1A64 | cli_send_system_status | C_WITH_EXACT_ASM_HELPER |
| 0x1BE4 | alarms_clear | C_WITH_EXACT_ASM_HELPER |
| 0x1C62 | channels_read | C_WITH_EXACT_ASM_HELPER |
| 0x1CF2 | cli_send_channel_cdf_adc | C_WITH_EXACT_ASM_HELPER |
| 0x1D62 | fpga_set_trg_charge_lvls | C_WITH_EXACT_ASM_HELPER |
| 0x1D92 | fpga_set_trg_settings | C_WITH_EXACT_ASM_HELPER |
| 0x1DB6 | fpga_set_adc_range_corr | C_WITH_EXACT_ASM_HELPER |
| 0x1E2E | fpga_set_tdc_values | C_WITH_EXACT_ASM_HELPER |
| 0x1E7C | fpga_set_threshold_calibration | C_WITH_EXACT_ASM_HELPER |
| 0x1EE0 | FUN_code_000f70 | C_WITH_EXACT_ASM_HELPER |
| 0x1F3A | fpga_set_ch_adc_delay | C_WITH_EXACT_ASM_HELPER |
| 0x1F8E | fpga_set_ch_cfd_threshold | C_WITH_EXACT_ASM_HELPER |
| 0x1FEC | fpga_set_adc_zero | C_WITH_EXACT_ASM_HELPER |
| 0x204E | fpga_set_ch_cfd_zero | C_WITH_EXACT_ASM_HELPER |
| 0x20A6 | FUN_code_001053 | C_WITH_EXACT_ASM_HELPER |
| 0x20D0 | FUN_code_001068 | C_WITH_EXACT_ASM_HELPER |
| 0x20EC | dac_set_value_2 | C_WITH_EXACT_ASM_HELPER |
| 0x2104 | dac_set_value | C_WITH_EXACT_ASM_HELPER |
| 0x211C | FUN_code_00108e | C_WITH_EXACT_ASM_HELPER |
| 0x2130 | fpga_is_ready | C_WITH_EXACT_ASM_HELPER |
| 0x214C | FUN_code_0010a6 | C_WITH_EXACT_ASM_HELPER |
| 0x2174 | ths788_write | C_WITH_EXACT_ASM_HELPER |
| 0x2208 | ths788_read | C_WITH_EXACT_ASM_HELPER |
| 0x22AA | dac_send_value | C_WITH_EXACT_ASM_HELPER |
| 0x230E | fpga_msg_send_t2 | C_WITH_EXACT_ASM_HELPER |
| 0x2368 | fpga_msg_read_t1 | C_WITH_EXACT_ASM_HELPER |
| 0x23CA | FUN_code_0011e5 | C_WITH_EXACT_ASM_HELPER |
| 0x2486 | CDCE62005_send_control_settings | C_BINARY_EXACT |
| 0x24CE | FUN_code_001267 | C_WITH_EXACT_ASM_HELPER |
| 0x2530 | fpga_send_mcu_ts | C_WITH_EXACT_ASM_HELPER |
| 0x2598 | adt7311_8bit_rw | C_WITH_EXACT_ASM_HELPER |
| 0x25BE | adt7311_16bit_rw | C_WITH_EXACT_ASM_HELPER |
| 0x25EA | adt7311_faults_clr | C_WITH_EXACT_ASM_HELPER |
| 0x2608 | adt7311_byte_rw | C_WITH_EXACT_ASM_HELPER |
| 0x2634 | cli_get_integer | C_WITH_EXACT_ASM_HELPER |
| 0x26AC | cli_get_hex | C_WITH_EXACT_ASM_HELPER |
| 0x26F8 | cli_send_32bit_hex | C_WITH_EXACT_ASM_HELPER |
| 0x2720 | cli_send_digit_hex | C_WITH_EXACT_ASM_HELPER |
| 0x272E | FUN_code_001397 | C_WITH_EXACT_ASM_HELPER |
| 0x2736 | cli_send_temperature | C_WITH_EXACT_ASM_HELPER |
| 0x273E | FUN_code_00139f | C_WITH_EXACT_ASM_HELPER |
| 0x2746 | cli_send_int16 | ASM_EXACT |
| 0x274E | cli_send_uint16 | C_WITH_EXACT_ASM_HELPER |
| 0x281E | cli_send_crlf | C_WITH_EXACT_ASM_HELPER |
| 0x2826 | cli_send_msg | C_WITH_EXACT_ASM_HELPER |
| 0x2836 | cli_get_next_byte | C_WITH_EXACT_ASM_HELPER |
| 0x283C | cli_get_next_char | C_WITH_EXACT_ASM_HELPER |
| 0x28AC | cli_send_buf | C_WITH_EXACT_ASM_HELPER |

Boot body entries (vector stubs excluded): 0x0201E2, 0x02023A, 0x0202A0, 0x0203D6, 0x0203E0, 0x0203F4, 0x02041C, 0x02042A, 0x02047C.

Historical behavioral counts are archived in mixed_recovery_reads_function_inventory.md;
they do not contribute to the accepted baseline counts.
