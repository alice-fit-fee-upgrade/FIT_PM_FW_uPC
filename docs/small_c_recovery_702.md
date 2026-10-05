# Native C recovery: steps 672–702

31 candidates, 9 accepted, 22 restored. Each accepted candidate passed a full
canonical FLASH exact-check. The final clean root build also passed.

Accepted: four bit branches in the console dispatcher, two tests in decimal
formatting, two comparisons in system status, and the device selection comparison
in TDC host write. Fixed-register captures emit zero bytes. Ordinary C now
generates the original test and branch instructions at the same addresses.

Rejected pointer post-increment stores either failed register allocation or
changed region sizes (section/layout class F). Capturing the low byte explicitly
was also tried in three command handlers and rejected. Other comparisons did not
reproduce the existing encoding/layout. All rejected sources were restored.
No behavior-equivalence acceptance or expensive emulator substitute was used.

Native C: 6844 -> 6880 executable bytes.
Application C: 68.4033%; all executable bytes including boot: 63.4921%.
Inline ASM: 3148 bytes; all ASM including boot: 3956 bytes.
Classes remain 5 C_BINARY_EXACT, 83 C_WITH_EXACT_ASM_HELPER, 1 ASM_EXACT; boot 9 ASM.
All 806 original text symbols retain their addresses.

SHA256 golden and rebuilt: `e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`. Differing bytes: 0.
Original instruction bytes and explanatory C model hashes remain unchanged;
existing transition test evidence therefore remains applicable.

| Step | Source | Result | Evidence |
| --- | --- | --- | --- |
| 672 | console_dispatch.c | ACCEPTED: 0 differences | [step_672.log](exact_c_steps/step_672.log) |
| 673 | console_dispatch.c | ACCEPTED: 0 differences | [step_673.log](exact_c_steps/step_673.log) |
| 674 | console_dispatch.c | ACCEPTED: 0 differences | [step_674.log](exact_c_steps/step_674.log) |
| 675 | console_dispatch.c | ACCEPTED: 0 differences | [step_675.log](exact_c_steps/step_675.log) |
| 676 | application_main.c | RESTORED | [step_676_rejected.log](exact_c_steps/step_676_rejected.log) |
| 677 | application_main.c | RESTORED | [step_677_rejected.log](exact_c_steps/step_677_rejected.log) |
| 678 | console_decimal_formatter.c | ACCEPTED: 0 differences | [step_678.log](exact_c_steps/step_678.log) |
| 679 | system_status_display.c | ACCEPTED: 0 differences | [step_679.log](exact_c_steps/step_679.log) |
| 680 | system_status_display.c | ACCEPTED: 0 differences | [step_680.log](exact_c_steps/step_680.log) |
| 681 | tdc_channel_data_display.c | RESTORED | [step_681_rejected.log](exact_c_steps/step_681_rejected.log) |
| 682 | tdc_host_write.c | ACCEPTED: 0 differences | [step_682.log](exact_c_steps/step_682.log) |
| 683 | fpga_binary_programming_stream.c | RESTORED | [step_683_rejected.log](exact_c_steps/step_683_rejected.log) |
| 684 | flash_page_program.c | RESTORED | [step_684_rejected.log](exact_c_steps/step_684_rejected.log) |
| 685 | console_decimal_formatter.c | ACCEPTED: 0 differences | [step_685.log](exact_c_steps/step_685.log) |
| 686 | channel_adc_zero_command.c | RESTORED | [step_686_rejected.log](exact_c_steps/step_686_rejected.log) |
| 687 | channel_cfd_zero_command.c | RESTORED | [step_687_rejected.log](exact_c_steps/step_687_rejected.log) |
| 688 | channel_adc_delay_command.c | RESTORED | [step_688_rejected.log](exact_c_steps/step_688_rejected.log) |
| 689 | console_dispatch.c | RESTORED | [step_689_rejected.log](exact_c_steps/step_689_rejected.log) |
| 690 | console_dispatch.c | RESTORED | [step_690_rejected.log](exact_c_steps/step_690_rejected.log) |
| 691 | fpga_data_exchange.c | RESTORED | [step_691_rejected.log](exact_c_steps/step_691_rejected.log) |
| 692 | fpga_data_exchange.c | RESTORED | [step_692_rejected.log](exact_c_steps/step_692_rejected.log) |
| 693 | dma_channel_interrupt.c | RESTORED | [step_693_rejected.log](exact_c_steps/step_693_rejected.log) |
| 694 | dma_channel_interrupt.c | RESTORED | [step_694_rejected.log](exact_c_steps/step_694_rejected.log) |
| 695 | dma_channel_interrupt.c | RESTORED | [step_695_rejected.log](exact_c_steps/step_695_rejected.log) |
| 696 | dma_channel_interrupt.c | RESTORED | [step_696_rejected.log](exact_c_steps/step_696_rejected.log) |
| 697 | dma_channel_interrupt.c | RESTORED | [step_697_rejected.log](exact_c_steps/step_697_rejected.log) |
| 698 | dma_channel_interrupt.c | RESTORED | [step_698_rejected.log](exact_c_steps/step_698_rejected.log) |
| 699 | dma_channel_interrupt.c | RESTORED | [step_699_rejected.log](exact_c_steps/step_699_rejected.log) |
| 700 | channel_adc_zero_command.c | RESTORED | [step_700_rejected.log](exact_c_steps/step_700_rejected.log) |
| 701 | channel_cfd_zero_command.c | RESTORED | [step_701_rejected.log](exact_c_steps/step_701_rejected.log) |
| 702 | channel_adc_delay_command.c | RESTORED | [step_702_rejected.log](exact_c_steps/step_702_rejected.log) |
