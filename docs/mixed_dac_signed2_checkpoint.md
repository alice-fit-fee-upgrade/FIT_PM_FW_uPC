# Fifth original entry integrated: signed DAC command 2

Byte 0x20EC (`dac_set_value_2`) now invokes `pm_dac_prepare_signed` in C
with command 2. It preserves the original signed coefficient 0x4188,
XOR of the result with 0x8000, modulo-byte channel multiplication by four,
and command bits. The packet returns the original MULSU result as a uint16
bit pattern, explicitly interpreting the input high byte as signed without
relying on implementation-defined conversion to int8_t. The bridge restores
R19=0x41, R1:R0, original input R21:R20, result R17:R16 and command R22.
Remaining registers and T/I are retained before the unchanged SPI routine,
which determines all six final arithmetic flags and enables interrupts.

Build and layout checks passed with all 806 original symbol addresses fixed.
65,536 comparisons at this entry passed for every input word, cycling all
12 channel indices, all 256 initial SREG patterns and randomized registers.
The 87,040 DAC boundary/calibration regression comparisons also passed.
Maximum additional stack in the new signed caller is 35 bytes. Evidence is
saved in mixed_dac_signed2_*.json. The second signed bridge is prepared but
its original entry 0x2104 is still ASM at this checkpoint.

Tests execute actual linked AVR instructions in a bounded functional model.
Timing, asynchronous interrupts, overall firmware stack budget and physical
module behavior remain unvalidated; the separate exact ASM is unchanged.
