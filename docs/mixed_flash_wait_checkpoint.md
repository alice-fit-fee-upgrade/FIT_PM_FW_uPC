# Twelfth integrated entry: FLASH busy polling at 0x173A

The first eight original bytes remain executed ASM: load 0x0640 into R25:R24
and repeat SBIW/BRNE for 1600 iterations. Layout checking compares these bytes
to golden. At 0x1742 a JMP invokes C for the original status-05 transaction:
assert CS, send 05, poll SPI ready, send 00, poll ready, read data, release CS.
The bridge restores complete final SPI status in R19, data in R24, R25=0 and
R16=10. Other registers and post-delay SREG remain unchanged. If data bit 0
is set, the original delay is re-entered; no timeout or alternative condition
is introduced. Internal polling labels remain address markers, not entries.

Build/layout checks passed, retaining all 806 original symbol addresses.
5,120 AVR comparisons passed over all even FLASH statuses, all ready SPI
statuses, all incoming SREG patterns, failed polls and 1/2/4 busy retries.
Registers, flags, exact ordered SPI traces and physical return-stack balance
match; extra stack is 15 bytes. Another 65,536 comparisons through the original
erase-command caller at 0x1710 passed after both its command helpers moved to C.
Exact ASM is unchanged. Original delay encoding is retained, but cycle-accurate
interrupt suppression and asynchronous/hardware behavior remain unvalidated.
