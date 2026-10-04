# Original entry-point inventory and C progress

Generated from the current mixed ELF/BIN and verified original listing.
Regenerate with `make c-progress`; JSON is in mixed_c_asm/build/function_inventory.json.

Application: **89 original entries; 14 integrated C; 75 not integrated**.
Functional C recovery: **45 entries**, including 31 not yet integrated.
Boot: **9 ASM procedures**, all untouched. Total not integrated including boot: **84**.
This counts entry points, including 10 application ISRs, main and shared formatter
entries; it does not imply every entry requires or should receive a C replacement.
All original direct CALL targets and vector destinations are included.

Functional C corresponds to 2890 original routine-span bytes (26.67% of 10836),
including register/ISR scaffolding and eight retained timed ASM bytes. This broader
metric includes the mixed project; the portable library's historical 2494-byte
measure remains separate. Actual substituted entry spans cover 464 bytes (4.28%).
Timing-sensitive GPIO serializers and boot/NVM routines may remain ASM.
Inherited function names are hypotheses; addresses are byte addressed.

| Byte address | Original name | State |
|---|---|---|
| 0x01E2 | DMA_CH1_vect_isr | ASM; C not recovered |
| 0x046C | FUN_code_000236 | ASM; C not recovered |
| 0x049E | TCC0_OVF_vect_isr | ASM; C not recovered |
| 0x08E4 | fpga_settings_init | Functional C recovered; ASM entry |
| 0x098A | fpga_settings_reset | Functional C recovered; ASM entry |
| 0x09DE | FUN_code_0004ef | ASM; C not recovered |
| 0x0A4C | PORTD_INT0_vect_isr | ASM; C not recovered |
| 0x0A9C | FUN_code_00054e | Integrated C |
| 0x0ABC | PORTB_INT0_vect_isr | ASM; C not recovered |
| 0x0AE0 | PORTF_INT1_vect_isr | ASM; C not recovered |
| 0x0B68 | FUN_code_0005b4 | ASM; C not recovered |
| 0x0B96 | PORTE_INT0_vect_isr | ASM; C not recovered |
| 0x0C2A | PORTE_INT1_vect_isr | ASM; C not recovered |
| 0x0C7E | set_status_and_vd8_led | Integrated C |
| 0x0C96 | system_deinit | Functional C recovered; ASM entry |
| 0x0D10 | system_init | ASM; C not recovered |
| 0x0DDC | CDCE62005_control_rst | ASM; C not recovered |
| 0x0E20 | PORTF_INT0_vect_isr | ASM; C not recovered |
| 0x0E88 | USARTF0_DRE_vect_isr | Functional C recovered; ASM entry |
| 0x0EE0 | USARTF0_RXC_vect_isr | Functional C recovered; ASM entry |
| 0x0F54 | main | ASM; C not recovered |
| 0x115C | fpga_data_exchange | ASM; C not recovered |
| 0x12EA | cli_prompt_parse | ASM; C not recovered |
| 0x157A | unlock_programming | ASM; C not recovered |
| 0x15A6 | fpga_firmware_update | ASM; C not recovered |
| 0x1664 | FUN_code_000b32 | Integrated C |
| 0x167E | FUN_code_000b3f | ASM; C not recovered |
| 0x16B2 | FUN_code_000b59 | ASM; C not recovered |
| 0x1710 | FUN_code_000b88 | ASM; C not recovered |
| 0x171E | FUN_code_000b8f | Integrated C |
| 0x173A | FUN_code_000b9d | Integrated C; original delay prefix retained |
| 0x1772 | FUN_code_000bb9 | ASM; C not recovered |
| 0x17E4 | FUN_code_000bf2 | Integrated C |
| 0x1808 | FUN_code_000c04 | Integrated C |
| 0x1840 | FUN_code_000c20 | ASM; C not recovered |
| 0x188E | cli_send_ch_mean_amplitude | ASM; C not recovered |
| 0x18BE | cli_send_adc_baseline_dispersion | ASM; C not recovered |
| 0x1910 | cli_send_tdc_data | ASM; C not recovered |
| 0x19AE | eeprom_settings_save | ASM; C not recovered |
| 0x1A0E | FUN_code_000d07 | ASM; C not recovered |
| 0x1A3E | cdce62005_rst | ASM; C not recovered |
| 0x1A64 | cli_send_system_status | ASM; C not recovered |
| 0x1BE4 | alarms_clear | ASM; C not recovered |
| 0x1C62 | channels_read | ASM; C not recovered |
| 0x1CF2 | cli_send_channel_cdf_adc | ASM; C not recovered |
| 0x1D62 | fpga_set_trg_charge_lvls | ASM; C not recovered |
| 0x1D92 | fpga_set_trg_settings | ASM; C not recovered |
| 0x1DB6 | fpga_set_adc_range_corr | ASM; C not recovered |
| 0x1E2E | fpga_set_tdc_values | ASM; C not recovered |
| 0x1E7C | fpga_set_threshold_calibration | ASM; C not recovered |
| 0x1EE0 | FUN_code_000f70 | ASM; C not recovered |
| 0x1F3A | fpga_set_ch_adc_delay | ASM; C not recovered |
| 0x1F8E | fpga_set_ch_cfd_threshold | ASM; C not recovered |
| 0x1FEC | fpga_set_adc_zero | ASM; C not recovered |
| 0x204E | fpga_set_ch_cfd_zero | ASM; C not recovered |
| 0x20A6 | FUN_code_001053 | Integrated C |
| 0x20D0 | FUN_code_001068 | Integrated C |
| 0x20EC | dac_set_value_2 | Integrated C |
| 0x2104 | dac_set_value | Integrated C |
| 0x211C | FUN_code_00108e | Integrated C |
| 0x2130 | fpga_is_ready | Integrated C |
| 0x214C | FUN_code_0010a6 | Integrated C |
| 0x2174 | ths788_write | Functional C recovered; ASM entry |
| 0x2208 | ths788_read | Functional C recovered; ASM entry |
| 0x22AA | dac_send_value | Functional C recovered; ASM entry |
| 0x230E | fpga_msg_send_t2 | Functional C recovered; ASM entry |
| 0x2368 | fpga_msg_read_t1 | Functional C recovered; ASM entry |
| 0x23CA | FUN_code_0011e5 | Functional C recovered; ASM entry |
| 0x2486 | CDCE62005_send_control_settings | Functional C recovered; ASM entry |
| 0x24CE | FUN_code_001267 | Functional C recovered; ASM entry |
| 0x2530 | fpga_send_mcu_ts | ASM; C not recovered |
| 0x2598 | adt7311_8bit_rw | Functional C recovered; ASM entry |
| 0x25BE | adt7311_16bit_rw | Functional C recovered; ASM entry |
| 0x25EA | adt7311_faults_clr | Functional C recovered; ASM entry |
| 0x2608 | adt7311_byte_rw | Functional C recovered; ASM entry |
| 0x2634 | cli_get_integer | Functional C recovered; ASM entry |
| 0x26AC | cli_get_hex | Functional C recovered; ASM entry |
| 0x26F8 | cli_send_32bit_hex | Functional C recovered; ASM entry |
| 0x2720 | cli_send_digit_hex | Functional C recovered; ASM entry |
| 0x272E | FUN_code_001397 | Functional C recovered; ASM entry |
| 0x2736 | cli_send_temperature | Functional C recovered; ASM entry |
| 0x273E | FUN_code_00139f | Functional C recovered; ASM entry |
| 0x2746 | cli_send_int16 | Functional C recovered; ASM entry |
| 0x274E | cli_send_uint16 | Functional C recovered; ASM entry |
| 0x281E | cli_send_crlf | Functional C recovered; ASM entry |
| 0x2826 | cli_send_msg | Functional C recovered; ASM entry |
| 0x2836 | cli_get_next_byte | Functional C recovered; ASM entry |
| 0x283C | cli_get_next_char | Functional C recovered; ASM entry |
| 0x28AC | cli_send_buf | Functional C recovered; ASM entry |

Boot body entries (vector stubs excluded): 0x0201E2, 0x02023A, 0x0202A0, 0x0203D6, 0x0203E0, 0x0203F4, 0x02041C, 0x02042A, 0x02047C.
