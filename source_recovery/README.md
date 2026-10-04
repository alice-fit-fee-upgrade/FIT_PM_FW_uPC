# Controlled ASM-to-C recovery: first isolated candidate

The exact ASM remains the golden source reconstruction. No C is linked into exact_asm.
`hex_digit.c` recovers the arithmetic prefix of `cli_send_digit_hex` at byte 0x2720..0x2728.
`python3 source_recovery/check_hex_digit.py` checks all 256 byte inputs against arithmetic of
verified original opcodes. AVR compilation: `avr-gcc -mmcu=atxmega128a3u -Os -Wall -Wextra
-Werror -c source_recovery/hex_digit.c -o /tmp/hex_digit.o`.

This candidate has a C ABI, not the original R16 ABI. Sending, SREG effects and register preservation
are not recovered by this prefix. It is not a drop-in firmware replacement. Before integration:
add an ABI wrapper, verify caller-visible flags/registers, pin the region, and run exact-check.
If GCC cannot preserve the original encoding, keep assembly in the exact project and develop a
separate behavior-validated C branch. No similar-but-different firmware is presented as exact.
