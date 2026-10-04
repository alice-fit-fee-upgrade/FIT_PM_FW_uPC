# Hardware anchors and semantic limits

The supplied FIT_PM12 PDF and repository sch/FIT-PM12-1.PDF are inventoried independently.
The supplied drawing carries WUT ISE 2024; original screenshot is dated 2023 and recovered banner
says INR PM12. A newer schematic is semantic evidence, not permission to alter original code.
The repo README mentions N25Q032 whereas the supplied hardware description specifies S25FL064L;
retain this revision disagreement until board inspection confirms the fitted part.

PORT register blocks in ATxmega data space: A 0x0600, B 0x0620, C 0x0640, D 0x0660,
E 0x0680, F 0x06A0. Offsets DIR 0, DIRSET 1, DIRCLR 2; OUT 4, OUTSET 5, OUTCLR 6, IN 8.
Each pin's bit mask follows its port number. These offsets correlate symbolic definitions with
original LDS/STS addresses; a register write alone does not prove attached-device semantics.

| Signal/group | MCU correlation | Original-code evidence | Confidence |
|---|---|---|---|
| ADT7311 Tempsens_SCLK/DOUT/DIN/CS | PA1/PA2/PA3/PA4 | byte exchange 0x2608 toggles OUT bit1, sends bit7 on bit3, samples IN bit2; CS bit4 routines 0x2598/0x25BE | High pin/transaction inference |
| THS HCLK/HDATA/HSTROBE1..3/RESET/OT_ALARM | PB0/PB1/PB2..4/PB5/PB6 | write/read 0x2174/0x2208; PB initialization 0x0D10 | Medium/high; verify polarity/timing |
| PLL MOSI/MISO/CLK | PC5/PC6/PC7 SPIC | code SPIC accesses; shared DAC SPI bus | High MCU peripheral, medium device ownership |
| PLL LE/SYNC/LOCK | PF4/PF5/PF6 | GPIO masks and startup; CDCE routine 0x1A3E reads FLASH table | Medium, board revision matters |
| FPGA PROGRAM_B/INIT_B/DONE | PD1/PD2/PD3 | startup output changes and status tests | Medium/high pin map |
| Config MOSI/DIN/FCS/CCLK | PE5/PE6/PE4/PE7 SPIE | SPIE routine near 0x1808, firmware-update path 0x15A6 | Medium; shared FPGA/serial-FLASH topology |
| Console Rx/Tx/RTS/CTS | PF2/PF3/PF1/PF0 USARTF0 | vector slots 119/120, handlers 0x0EE0/0x0E88, GPIO flow-control | High USART identity; polarity requires tracing |
| EN_PSU/IN_PWR_OK/P1V2LDO_PG | PE2/PE1/PE3 | OUTCLR/SET bit2, IN tests and startup status | Medium/high |
| PA LEDs | PA0, PA5, PA6, PA7 | original masks 01/20/40/80; supplied labels LED_CLK_IN/ERR/Module_ready differ from older README colors | Medium semantics |

Names in upstream are retained, not globally accepted as verified. FPGA custom control/status routines
at 0x230E/0x2368 and RAM 0x2157..0x2162 have static evidence, but full protocol/state-machine semantics,
THS register meanings and peripheral transaction timing remain development work. No hardware execution
was available to confirm startup, configuration or UART behavior. Bit equality is the checkpoint evidence.
