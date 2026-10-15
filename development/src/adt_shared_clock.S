/* Readable value equivalent (REFERENCE_C_INFERRED for the private flag ABI):
 * for (unsigned bit=0;bit<8;bit++) {
 *     MOSI = (data & 0x80) != 0; delay_us(1);
 *     SCLK = 0; data <<= 1; delay_us(1);
 *     data |= MISO ? 1 : 0;
 *     SCLK = 1; delay_us(1);
 * }
 * return data;
 *
 * Development only: PA1 is now loaded by the additional I2C wiring. Preserve
 * the legacy R16 argument/result, R18=0, R17 final port snapshot, T=last RX bit,
 * C/H from the last shift and Z/N/V/S from the final counter DEC.
 * No other GPR is touched. DIN settles before the falling edge; DOUT is sampled
 * during LOW, before the final rising edge can release it. Each delay is
 * exactly 32 CPU cycles at 32 MHz. The original entry address is retained.
 */
.macro PHASE_DELAY
 ldi r17,10
1: dec r17
 brne 1b
 nop
 nop
.endm
.section .text.pm_adt_shared_byte,"ax",@progbits
.global pm_adt_shared_byte
pm_adt_shared_byte:
 ldi r18,8
.Lbit:
 ldi r17,8
 sbrc r16,7
 sts 0x0605,r17
 sbrs r16,7
 sts 0x0606,r17
 PHASE_DELAY
 ldi r17,2
 sts 0x0606,r17
 add r16,r16
 PHASE_DELAY
 lds r17,0x0608
 bst r17,2
 bld r16,0
 ldi r17,2
 sts 0x0605,r17
 PHASE_DELAY
 lds r17,0x0608
 dec r18
 brne .Lbit
 ret
.global pm_adt_shared_patch
pm_adt_shared_patch:
 jmp pm_adt_shared_byte
