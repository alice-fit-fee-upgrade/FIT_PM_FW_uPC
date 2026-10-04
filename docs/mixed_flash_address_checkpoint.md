# Eleventh integrated entry: command/address SPI transaction at 0x1808

C retains CS assertion, command byte, three address bytes high/middle/low,
and the original status-bit-7 polling after each transfer. CS intentionally
remains asserted on return. The complete final SPI status returns in R19;
R17=10, while command/address inputs and every other register remain unchanged.
All original SREG bits are restored. Internal old polling labels retain only
their addresses; external references to them are rejected.

65,536 linked AVR comparisons passed across all command bytes and initial
SREG patterns, varied address bytes, complete ready statuses and 0..3 failed
polls. Registers, flags, ordered MMIO effects and return-stack balance match.
Extra stack is 17 bytes. All 806 original code symbols stay fixed with no
new static SRAM. Hardware SPI clock generation remains peripheral-driven;
cycle/async interrupt/physical-module validation is pending. Exact ASM stays
unchanged.
