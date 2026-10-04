/* Original register ABI at byte 0x2130, NOT the GNU C ABI.
 * Input:  R19:R18 coefficient, R21:R20 signed input.
 * Output: R17:R16 result; R1:R0 = final signed-high * coefficient-high.
 * All other registers unchanged. T/I unchanged. C/Z/N/V/S/H must match
 * the final original ADD, not compiler arithmetic or a conventional C ABI.
 */
.section .bridges,"ax",@progbits
.global pm_scale_signed_bridge
.type pm_scale_signed_bridge,@function
pm_scale_signed_bridge:
    in r0, 0x3f
    push r0
    /* GCC call-clobbered registers; inputs must be restored too. */
    push r18
    push r19
    push r20
    push r21
    push r22
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    clr r1
    movw r24, r18
    movw r22, r20
    call pm_scale_signed
    movw r16, r24
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18
    pop r0
    /* Restore T and I before rebuilding original output register/flag state. */
    out 0x3f, r0
    mulsu r21, r19
    /* Reconstruct pre-ADD high byte modulo 256, then repeat original ADD.
     * SUB flags are overwritten. ADD sets all six arithmetic flags exactly. */
    sub r17, r0
    add r17, r0
    ret
.size pm_scale_signed_bridge, .-pm_scale_signed_bridge

/* Unsigned entry 0x214c: same register preservation, with exact SREG
 * returned by the C ABI adapter. MUL reconstructs the original R1:R0. */
.global pm_scale_unsigned_bridge
.type pm_scale_unsigned_bridge,@function
pm_scale_unsigned_bridge:
    in r0, 0x3f
    push r0
    push r18
    push r19
    push r20
    push r21
    push r22
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    clr r1
    movw r24, r18
    movw r22, r20
    mov r20, r0
    call pm_scale_unsigned_abi
    movw r16, r22
    mov r0, r24
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18
    pop r1
    push r18
    push r0
    mul r19, r21
    pop r18
    out 0x3f, r18
    pop r18
    ret
.size pm_scale_unsigned_bridge, .-pm_scale_unsigned_bridge

/* Original inverse-DAC entry 0x20D0. Preserve all registers outside the
 * documented outputs and retain T before invoking the original SPI sender.
 * Its final OR/ORI sets Z/N/V/S, ADD sets C/H, and SEI sets I, independently
 * of arithmetic flags produced by preparation. */
.global pm_dac_inverse_bridge
.type pm_dac_inverse_bridge,@function
pm_dac_inverse_bridge:
    push r18
    in r18, 0x3f
    push r18
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    clr r1
    movw r24, r20
    call pm_dac_prepare_inverse
    movw r16, r18
    movw r0, r22
    mov r22, r24
    ldi r19, 2
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r18
    out 0x3f, r18
    pop r18
    call dac_send_value
    ret
.size pm_dac_inverse_bridge, .-pm_dac_inverse_bridge

/* Calibrated DAC preparation at 0x20A6. Z is an output, not preserved:
 * 0x2163 + ((channel * 2) mod 256) + 1. The two RAM reads remain ordered. */
.global pm_dac_calibrated_bridge
.type pm_dac_calibrated_bridge,@function
pm_dac_calibrated_bridge:
    push r18
    in r18, 0x3f
    push r18
    push r23
    push r24
    push r25
    push r26
    push r27
    movw r24, r20
    mov r18, r22
    add r18, r18
    ldi r30, 0x63
    ldi r31, 0x21
    eor r0, r0
    add r30, r18
    adc r31, r0
    ld r20, Z+
    ld r21, Z
    push r30
    push r31
    clr r1
    call pm_dac_prepare_calibrated
    movw r16, r18
    movw r0, r22
    mov r22, r24
    ldi r19, 2
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r18
    out 0x3f, r18
    pop r18
    call dac_send_value
    ret
.size pm_dac_calibrated_bridge, .-pm_dac_calibrated_bridge

.global pm_dac_signed2_bridge
.type pm_dac_signed2_bridge,@function
pm_dac_signed2_bridge:
    push r18
    in r18, 0x3f
    push r18
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    clr r1
    movw r24, r20
    ldi r20, 2
    call pm_dac_prepare_signed
    movw r16, r18
    movw r0, r22
    mov r22, r24
    ldi r19, 0x41
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r18
    out 0x3f, r18
    pop r18
    call dac_send_value
    ret
.size pm_dac_signed2_bridge, .-pm_dac_signed2_bridge

.global pm_dac_signed1_bridge
.type pm_dac_signed1_bridge,@function
pm_dac_signed1_bridge:
    push r18
    in r18, 0x3f
    push r18
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    clr r1
    movw r24, r20
    ldi r20, 1
    call pm_dac_prepare_signed
    movw r16, r18
    movw r0, r22
    mov r22, r24
    ldi r19, 0x41
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r18
    out 0x3f, r18
    pop r18
    call dac_send_value
    ret
.size pm_dac_signed1_bridge, .-pm_dac_signed1_bridge

/* Status gate 0x211C: R16 becomes the full status byte. All other
 * registers (including arbitrary incoming R1) remain unchanged on the
 * ready path. C is cleared there; failure calls the original message
 * sender with Z=0x2AB4 and sets C afterwards. */
.global pm_status_gate_bridge
.type pm_status_gate_bridge,@function
pm_status_gate_bridge:
    push r0
    push r1
    in r0, 0x3f
    push r0
    push r18
    push r19
    push r20
    push r21
    push r22
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    lds r16, 0x2157
    mov r24, r16
    clr r1
    call pm_status_gate_allowed
    tst r24
    breq .Lstatus_gate_failed
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18
    pop r0
    out 0x3f, r0
    pop r1
    pop r0
    clc
    ret
.Lstatus_gate_failed:
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18
    pop r0
    out 0x3f, r0
    pop r1
    pop r0
    clc
    ldi r30, 0xb4
    ldi r31, 0x2a
    call cli_send_msg
    sec
    ret
.size pm_status_gate_bridge, .-pm_status_gate_bridge

/* CRC byte at 0x17E4. State order: R18,R19,R16,R17 (low to high).
 * Polynomial: R23,R24,R25,R26. Input R22 is consumed to zero; R20=0.
 * Every other register is preserved, including original R0/R1. */
.global pm_crc_byte_bridge
.type pm_crc_byte_bridge,@function
pm_crc_byte_bridge:
    push r0
    push r1
    in r0, 0x3f
    push r0
    push r21
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    push r23
    push r24
    push r25
    push r26
    push r22
    movw r22, r18
    movw r24, r16
    pop r16
    pop r21
    pop r20
    pop r19
    pop r18
    clr r1
    call pm_crc_byte_abi
    movw r16, r20
    mov r0, r22
    clr r20
    clr r22
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r21
    pop r1
    bst r1, 6
    bld r0, 6
    bst r1, 7
    bld r0, 7
    out 0x3f, r0
    pop r1
    pop r0
    ret
.size pm_crc_byte_bridge, .-pm_crc_byte_bridge

.global pm_flash_init_bridge
.type pm_flash_init_bridge,@function
pm_flash_init_bridge:
    push r0
    push r1
    in r0, 0x3f
    push r0
    push r18
    push r19
    push r20
    push r21
    push r22
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    clr r1
    call pm_flash_spi_init
    ldi r16, 0x50
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18
    pop r0
    out 0x3f, r0
    pop r1
    pop r0
    ret
.size pm_flash_init_bridge, .-pm_flash_init_bridge

.global pm_flash_enable_bridge
.type pm_flash_enable_bridge,@function
pm_flash_enable_bridge:
    push r0
    push r1
    in r0, 0x3f
    push r0
    push r18
    push r20
    push r21
    push r22
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    clr r1
    call pm_flash_write_enable
    mov r19, r24
    ldi r16, 0x10
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r18
    pop r0
    out 0x3f, r0
    pop r1
    pop r0
    ret
.size pm_flash_enable_bridge, .-pm_flash_enable_bridge

.global pm_flash_address_bridge
.type pm_flash_address_bridge,@function
pm_flash_address_bridge:
    push r0
    push r1
    in r0, 0x3f
    push r0
    push r18
    push r20
    push r21
    push r22
    push r23
    push r24
    push r25
    push r26
    push r27
    push r30
    push r31
    mov r18, r20
    mov r20, r21
    mov r24, r16
    clr r1
    call pm_flash_send_address
    mov r19, r24
    ldi r17, 0x10
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r18
    pop r0
    out 0x3f, r0
    pop r1
    pop r0
    ret
.size pm_flash_address_bridge, .-pm_flash_address_bridge
