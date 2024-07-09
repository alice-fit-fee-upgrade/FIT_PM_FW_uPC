# FIT_PM_FW_uPC

## Firmware timestamp & version
The build timestamp (32-bit) is located at the very end of the application code (e.g.: 0x2b7c), not far from the " Flash TImestamp:" string.  
The timestamp can be easily translated to firmware version YMD.Hm using the following rule:  
- Y: years, counting from 2020, so 2021 is 1,
- M: month
- D: day
- H: hour
- m: minutes  
All the values are coded using the following alphabet:  
> 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvw

---
## Hardware 
Some details about the peripherals the MCU ATxmega128a3 is communicating with.

| PIN 	| LABEL 	| PORT	| MODE			| PERIPHERAL 	|
|:---:	|:---:  	|:---:	| :---:			|:---:        	
|63  	|DA27_SCLK	|PA1 	| SPI CLK		|**ADT7311WTRZ Temperature Sensor**| 
|64	|DA27_DOUT	|PA2 	| SPI MISO		|		| 
|1	|DA27_DIN	|PA3  	| SPI MOSI		| 		|
|2	|DA27_CS	|PA4	| SPI CS		| 		|
|62	|		|PA0 	| OUT LED VD8 (red)	|**LEDS**	|
|3	|		|PA5 	| OUT LED VD12 (blue)	|		|
|4	|		|PA6 	| OUT LED VD13 (red)	|		|
|5	|		|PA7 	| OUT LED VD14 (green)	|		|
|6	|HCLK		|PB0	| OUT			|**THS788PFD (x3)**|
|7	|HDATA		|PB1	| IN/OUT		|		|
|8	|HSTROBE1	|PB2	| OUT CS		|		|
|9	|HSTROBE2	|PB3	| OUT CS		|		|
|10	|HSTROBE3	|PB4	| OUT CS		|		|		
|11	|THS788_RESET	|PB5	| OUT RST		|		|
|12	|THS788_OT_ALARM|PB6	| INT0 (overtemperature alarm)	|	|
|16	|SYNC_A		|PC0	| ???			|**  **|
|17	|SYNC_B		|PC1	| ???			|		|
|18	|SYNC_C		|PC2	| ???			| 		|
|19	|DD21_PWRDNB	|PC3	| ???			| **CDCF5801ADBQ(multiple)**|
|21 	|DA22_DIN	|PC5 	| SPI MOSI		| **CDCE62005RGZT & DAC8554IPW(multiple)**|
|22 	|DD3_SPI_MISO	|PC6 	| SPI MISO 		| **CDCE62005RGZT**|
|23 	|DD3_SPI_CLK	|PC7 	| SPI SCK		|		|
|50	|DD3_SPI_LE	|PF4	| SPI CS 		| 		|
|51	|DD3_SYNC	|PF5	| OUT 			|		|
|54	|DD3_PLL_LOCK	|PF6	| IN			|		|
|36 	|Xmega_PE0	|PE0 	| ??? 			| **FPGA**	|
|26 	|Xmega_PD0	|PD0 	| ??? 			|		|
|27 	|Xmega_PD1	|PD1 	| OUT (PROGRAM_B_0) 	|		|
|28	|Xmega_PD2	|PD2 	| ??? (INIT_B_0)	|		|
|29	|DD13D_AB	|PD3	| ??? (DONE_0)		|		|
|20	|DD18_60_12	|PC4	| ???			| 		|
|9	|Xmega_PB7	|PB7	| ???			|		|
|30	|DD28A_OUT	|PD4	| IN ??? 		| **HDMI13**	|
|31	|DD28C_OUT	|PD5 	| IN ???		| **HDMI16**	|
|32	|NC		|PD6	| OUT ???		| **HDMI18**	|
|33	|DD28B_OUT	|PD7	| IN ???		| **HDMI15**	|
|37	|DA21_RST_N	|PE1	| INT (nrst)		|**LTC2906ITS8**|
|38	|DD11A_A	|PE2	| OUT (enable)		|**Voltage enable**	|
|39	|DA7_PGOOD	|PE3	| IN  (outa & outb)	|**Voltage monitor** 	|
|40	|DD9_S  	|PE4	| SPI CS		|**N25Q032A11EF840 Serial Flash Memory**| 
|41	|DD9_DQ0	|PE5	| SPI MOSI		|		|
|42	|DD9_DQ1	|PE6	| SPI MISO		|		|
|43	|DD9_C		|PE7	| SPI CLK		|		|
|46	|DD7_T2IN	|PF0	| UART CTS		|**RJ45 Connector**|
|47	|DD7_R2OUT	|PF1	| UART RTS (INT0)	|		|
|48	|DD7_R1OUT	|PF2	| UART Rx 		|		|
|49	|DD7_T1IN	|PF3	| UART Tx		|		|
|55	|NC		|PF7	| ---		


---  
## UART F0 commands
| CMD 		| DESC |
|:---: 		|:---: |
| CA\r		| |
| CP\r		| |
| PA		| Flash ATxmega with new firmware |
| PC		| Flash default CDCE62005 settings|
| PF		| Flash FPGA with new firmware |
| RA		| |
| RC		| |
| RF		| |
| RS 		| Read status |
| RT		| |
| RZ		| |
| SCL		| |
| SCS		| |
| SCT		| |
| SCR		| |
| SD		| |
| SL		| |
| SO		| |
| SS		| |
| ST		| |
| SV		| |
| WR 		| Write settings to EEPROM|

