# Seventh integrated entry: status guard at 0x211C

The original 20-byte routine now calls compiled C to test status bit 4 at RAM
0x2157. The bridge returns the complete status byte in R16, preserves all other
registers (including arbitrary R0/R1), restores incoming SREG, and clears C.
If the bit is absent it calls the unchanged original message sender with
Z=0x2AB4 ("FPGA not ready\r\n") and sets C afterwards. All existing message,
UART queue and interrupt-enable behavior is retained at the function boundary.

Build/layout checks passed: all 806 original text-symbol addresses remain fixed,
with no new static SRAM. 70,656 actual linked-AVR comparisons passed, including
all 256 status bytes times all 256 incoming SREG patterns, plus queued/busy/wrap/
full UART scenarios. Registers, flags, ordered memory/UART/IRQ traces and balanced
three-byte return stack match original execution. Extra stack peaks at 18 bytes.
Full-queue consumption is scripted; cycle/asynchronous interrupt/hardware behavior
remains unvalidated. The exact assembly baseline is retained unchanged.
