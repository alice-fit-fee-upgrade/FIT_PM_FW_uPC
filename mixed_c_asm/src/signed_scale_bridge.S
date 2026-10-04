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
