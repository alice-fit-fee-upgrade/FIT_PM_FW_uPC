# Small native C recovery: steps 623–671

49 isolated candidates; 28 accepted, 21 restored. Acceptance was full canonical
FLASH equality after each accepted change, followed by a final clean root
`make exact-check`. Original reference trees were not modified.

Accepted code: six simple conditions in console interrupt/output code, eight
bit tests in timer overflow, seven in DMA, and seven in FPGA data exchange.
Fixed-register captures are zero-byte compiler barriers. Actual tests and jumps
are now generated from ordinary C conditions. They are counted as native C
only by the independent GCC APP/NOAPP provenance audit.

Rejected candidates: native zero assignments, increments, several console/status
branches and OS_task hex threshold. Historical CLR/INC encodings or branch layout
were not reproduced. The OS_task threshold exceeded its fixed region and linker
rejected the overlap (section/layout class F). These attempts were restored;
no behavior-equivalence claim or expensive emulator acceptance was substituted.

Native C: 6732 -> 6844 bytes (+112). Inline ASM: 3296 -> 3184 bytes.
Application C: 68.0453%; all executable bytes including boot: 63.1598%.
Classes remain 5 C_BINARY_EXACT, 83 C_WITH_EXACT_ASM_HELPER, 1 ASM_EXACT; boot 9 ASM.
All 806 original text symbols retain their addresses.

Final SHA256 (golden and rebuilt): `e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.
Differing bytes: 0. Instruction bytes and explanatory C model hashes are
unchanged, so existing transition validation remains applicable; no historical
exhaustive emulator suites were rerun.

## Individual results

| Step | Source | Result | Evidence |
| --- | --- | --- | --- |
| 623 | console_hex_digit.c | RESTORED | [step_623_rejected.log](exact_c_steps/step_623_rejected.log) |
| 624 | console_cts_interrupt.c | RESTORED | [step_624_rejected.log](exact_c_steps/step_624_rejected.log) |
| 625 | power_porte_interrupt.c | RESTORED | [step_625_rejected.log](exact_c_steps/step_625_rejected.log) |
| 626 | console_decimal_formatter.c | RESTORED | [step_626_rejected.log](exact_c_steps/step_626_rejected.log) |
| 627 | eeprom_page_commit.c | RESTORED | [step_627_rejected.log](exact_c_steps/step_627_rejected.log) |
| 628 | eeprom_settings_store.c | RESTORED | [step_628_rejected.log](exact_c_steps/step_628_rejected.log) |
| 629 | console_hex_parser.c | RESTORED | [step_629_rejected.log](exact_c_steps/step_629_rejected.log) |
| 630 | console_rx_interrupt.c | RESTORED | [step_630_rejected.log](exact_c_steps/step_630_rejected.log) |
| 631 | console_character_read.c | RESTORED | [step_631_rejected.log](exact_c_steps/step_631_rejected.log) |
| 632 | channel_settings_display.c | RESTORED | [step_632_rejected.log](exact_c_steps/step_632_rejected.log) |
| 633 | system_status_display.c | RESTORED | [step_633_rejected.log](exact_c_steps/step_633_rejected.log) |
| 634 | console_tx_interrupt.c | ACCEPTED: 0 differences | [step_634.log](exact_c_steps/step_634.log) |
| 635 | console_tx_interrupt.c | RESTORED | [step_635_rejected.log](exact_c_steps/step_635_rejected.log) |
| 636 | console_cts_interrupt.c | RESTORED | [step_636_rejected.log](exact_c_steps/step_636_rejected.log) |
| 637 | console_cts_interrupt.c | ACCEPTED: 0 differences | [step_637.log](exact_c_steps/step_637.log) |
| 638 | console_rx_interrupt.c | RESTORED | [step_638_rejected.log](exact_c_steps/step_638_rejected.log) |
| 639 | console_rx_interrupt.c | ACCEPTED: 0 differences | [step_639.log](exact_c_steps/step_639.log) |
| 640 | console_character_read.c | RESTORED | [step_640_rejected.log](exact_c_steps/step_640_rejected.log) |
| 641 | console_flash_message.c | ACCEPTED: 0 differences | [step_641.log](exact_c_steps/step_641.log) |
| 642 | console_character_send.c | ACCEPTED: 0 differences | [step_642.log](exact_c_steps/step_642.log) |
| 643 | console_character_send.c | RESTORED | [step_643_rejected.log](exact_c_steps/step_643_rejected.log) |
| 644 | console_character_send.c | ACCEPTED: 0 differences | [step_644.log](exact_c_steps/step_644.log) |
| 645 | console_character_send.c | RESTORED | [step_645_rejected.log](exact_c_steps/step_645_rejected.log) |
| 646 | system_status_display.c | RESTORED | [step_646_rejected.log](exact_c_steps/step_646_rejected.log) |
| 647 | system_status_display.c | RESTORED | [step_647_rejected.log](exact_c_steps/step_647_rejected.log) |
| 648 | system_status_display.c | RESTORED | [step_648_rejected.log](exact_c_steps/step_648_rejected.log) |
| 649 | timer_overflow_interrupt.c | ACCEPTED: 0 differences | [step_649.log](exact_c_steps/step_649.log) |
| 650 | timer_overflow_interrupt.c | ACCEPTED: 0 differences | [step_650.log](exact_c_steps/step_650.log) |
| 651 | timer_overflow_interrupt.c | ACCEPTED: 0 differences | [step_651.log](exact_c_steps/step_651.log) |
| 652 | timer_overflow_interrupt.c | ACCEPTED: 0 differences | [step_652.log](exact_c_steps/step_652.log) |
| 653 | timer_overflow_interrupt.c | ACCEPTED: 0 differences | [step_653.log](exact_c_steps/step_653.log) |
| 654 | timer_overflow_interrupt.c | ACCEPTED: 0 differences | [step_654.log](exact_c_steps/step_654.log) |
| 655 | timer_overflow_interrupt.c | ACCEPTED: 0 differences | [step_655.log](exact_c_steps/step_655.log) |
| 656 | timer_overflow_interrupt.c | RESTORED | [step_656_rejected.log](exact_c_steps/step_656_rejected.log) |
| 657 | timer_overflow_interrupt.c | ACCEPTED: 0 differences | [step_657.log](exact_c_steps/step_657.log) |
| 658 | dma_channel_interrupt.c | ACCEPTED: 0 differences | [step_658.log](exact_c_steps/step_658.log) |
| 659 | dma_channel_interrupt.c | ACCEPTED: 0 differences | [step_659.log](exact_c_steps/step_659.log) |
| 660 | dma_channel_interrupt.c | ACCEPTED: 0 differences | [step_660.log](exact_c_steps/step_660.log) |
| 661 | dma_channel_interrupt.c | ACCEPTED: 0 differences | [step_661.log](exact_c_steps/step_661.log) |
| 662 | dma_channel_interrupt.c | ACCEPTED: 0 differences | [step_662.log](exact_c_steps/step_662.log) |
| 663 | dma_channel_interrupt.c | ACCEPTED: 0 differences | [step_663.log](exact_c_steps/step_663.log) |
| 664 | dma_channel_interrupt.c | ACCEPTED: 0 differences | [step_664.log](exact_c_steps/step_664.log) |
| 665 | fpga_data_exchange.c | ACCEPTED: 0 differences | [step_665.log](exact_c_steps/step_665.log) |
| 666 | fpga_data_exchange.c | ACCEPTED: 0 differences | [step_666.log](exact_c_steps/step_666.log) |
| 667 | fpga_data_exchange.c | ACCEPTED: 0 differences | [step_667.log](exact_c_steps/step_667.log) |
| 668 | fpga_data_exchange.c | ACCEPTED: 0 differences | [step_668.log](exact_c_steps/step_668.log) |
| 669 | fpga_data_exchange.c | ACCEPTED: 0 differences | [step_669.log](exact_c_steps/step_669.log) |
| 670 | fpga_data_exchange.c | ACCEPTED: 0 differences | [step_670.log](exact_c_steps/step_670.log) |
| 671 | fpga_data_exchange.c | ACCEPTED: 0 differences | [step_671.log](exact_c_steps/step_671.log) |
