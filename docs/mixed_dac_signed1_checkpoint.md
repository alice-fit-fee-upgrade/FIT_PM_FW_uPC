# Sixth original entry integrated: signed DAC command 1

Entry 0x2104 (`dac_set_value`) now uses the same compiled C preparation as
0x20EC with command 1. Its original 24-byte body is JMP plus FF padding.
All original inputs, output/scratch registers and final SPI/IRQ behavior
match in 65,536 comparisons over every input word with cycled channel and
SREG states. Build and layout checks pass; all 806 original symbols retain
their addresses. Maximum extra caller stack is 35 bytes. The separate exact
assembly baseline is unchanged. Full final regression is recorded separately
in mixed_dac_final_checkpoint.md when complete.

As in the other checkpoints, evidence comes from actual linked AVR opcodes
in a bounded custom functional interpreter with scripted peripheral inputs.
It does not validate cycle timing, asynchronous interrupts, the complete
firmware stack budget or physical module operation.
