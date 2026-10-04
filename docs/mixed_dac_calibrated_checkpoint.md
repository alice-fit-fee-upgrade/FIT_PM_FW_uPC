# Fourth original entry integrated: calibrated DAC preparation

Entry 0x20A6 now invokes compiled C `pm_dac_prepare_calibrated`. It scales
by 0x020C, adds the calibration word modulo 65536, complements the result,
and forms `(channel * 4 mod 256) | 3`. An ABI packet preserves original
R1:R0 (last unsigned partial product), R19=2, calibration in R21:R20,
value in R17:R16 and command in R22; all other registers remain unchanged
except the original Z output. The original SPI sender determines final SREG.

The bridge explicitly computes the original byte-wrapped RAM address:
`0x2163 + ((channel * 2) & 255)`. It reads the low byte then the high byte,
leaving Z at the high byte. It saves this Z through the GNU C call and
retains incoming T/I before calling the unchanged SPI sender. Reading the
calibration now precedes the pure C scaling rather than following the old
pure ASM scaler: ordered external reads/writes match, while timing and
asynchronous observation still require hardware validation.

Build and layout checks passed; all 806 original text-symbol addresses
remain fixed. Full four-caller comparisons passed 262,144 cases. Another
87,040 comparisons exercise all 256 channel bytes, boundary input values,
calibration values 0000/7FFF/8000/FFFF and all 65,536 calibration words with
input 1234. Registers, all SREG bits, SPI/IRQ traces, calibration-read order,
final Z and balanced three-byte-PC stack all match the original. Maximum
additional caller stack is 33 bytes. JSON evidence is saved alongside this file.

The exact assembly baseline remains separate and unchanged. These are
functional bounded-interpreter tests with scripted hardware, not cycle or
asynchronous interrupt equivalence or physical-device validation.
