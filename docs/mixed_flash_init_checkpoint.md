# Ninth integrated entry: FLASH hardware-SPI initialization at 0x1664

C writes the original four registers in order: PORTD_OUTCLR=2,
PORTE_OUTSET=0x10, PORTE_DIRSET=0xB0, SPIE_CTRL=0x50. The bridge restores
all registers except the original R16 output 0x50, and all eight original
SREG bits. Original R0/R1 are preserved without a zero-register assumption.
65,536 actual AVR comparisons passed across randomized register states and
all incoming SREG patterns; MMIO write order, return PCs and SP match.
Extra stack is 18 bytes. All 806 original symbols stay fixed, with no new
static SRAM. C command helpers are prepared but not yet routed from the
remaining original entries at this checkpoint. Physical/timing validation
remains pending; exact ASM is unchanged.
