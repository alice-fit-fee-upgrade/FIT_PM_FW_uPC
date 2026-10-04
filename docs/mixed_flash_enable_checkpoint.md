# Tenth integrated entry: FLASH command 06 at 0x171E

C preserves original CS assertion, SPIE_DATA=06, unbounded polling of status
bit 7 and CS release, returning the complete final status byte in R19. R16=10;
all other registers and all original SREG bits are restored by the bridge.
98,304 linked AVR comparisons passed: every ready-status byte 80..FF, all
256 incoming SREG patterns, and 0/1/3 unsuccessful status polls. MMIO order
and physical return-stack balance match. Extra stack is 17 bytes; all 806
original symbol addresses are fixed and no static SRAM is added. Hardware
SPI generates clock edges; cycle/async interrupt/physical timing remain
unvalidated. The original exact assembly baseline is unchanged.
