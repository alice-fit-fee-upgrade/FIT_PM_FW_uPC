# Eighth integrated entry: CRC byte update at 0x17E4

Compiled C reproduces the original left-shifted 32-bit state with LSB-first
input-byte consumption and caller-supplied polynomial. It does not substitute a
library CRC convention. Original state register order is R18,R19,R16,R17;
polynomial order is R23,R24,R25,R26. R22 is consumed to zero and R20 ends at zero.
All remaining registers, including original R0/R1 and T/I, are preserved.
Final flags are Z=1, N=V=S=0, C=input bit 7, H=pre-final-shift state bit 27.

The first differential run caught swapped 16-bit halves in GNU C argument
packing. The bridge packing was corrected before validation or commit.
The C half-carry expression selects the high byte explicitly to avoid GCC
emitting a 27-bit shift loop; state/flag semantics remain unchanged.

Build/layout checks pass, retaining all 806 original symbol addresses.
Internal original labels are retained only as metadata; external references
are rejected. 77,864 direct comparisons passed over all bytes, CRC/polynomial
boundaries, individual bit bases, random full states and all incoming SREG
patterns. Another 280 original SPI stream callers passed, including 16/24-bit
address wraps. Registers, flags, SPI traces and physical return-stack balance
match. Extra stack peaks at 29 bytes directly and 32 through the stream caller.
No static SRAM or CRT is introduced; the exact baseline remains unchanged.

Evidence is functional execution of linked AVR opcodes in a bounded interpreter,
not cycle, asynchronous interrupt or hardware validation.
