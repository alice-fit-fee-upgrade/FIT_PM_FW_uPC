/* Exact private ABI wrapper; C computes the values. ASM reconstructs live
 * R1:R0 and SREG because they are explicit outputs of the original routines.
 * Logical C: result=scale(coefficient,input); restore_inputs_and_banks();
 * restore_legacy_product_and_flags(); return result;
 * Tests execute this entire wrapper and compiled C, including GCC helpers. */
/* Original register ABI at byte 0x2130, NOT the GNU C ABI.
 * Input:  R19:R18 coefficient, R21:R20 signed input.
 * Output: R17:R16 result; R1:R0 = final signed-high * coefficient-high.
 * All other registers unchanged. T/I unchanged. C/Z/N/V/S/H must match
 * the final original ADD, not compiler arithmetic or a conventional C ABI.
 */
.section .text.pm_scaling_bridges,"ax",@progbits
.global pm_dev_scale_signed_bridge
.type pm_dev_scale_signed_bridge,@function
pm_dev_scale_signed_bridge:
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
    in r0, 0x38
    push r0
    in r0, 0x39
    push r0
    in r0, 0x3a
    push r0
    in r0, 0x3b
    push r0
    in r0, 0x3c
    push r0
    clr r1
    out 0x38, r1
    out 0x39, r1
    out 0x3a, r1
    out 0x3b, r1
    out 0x3c, r1
    movw r24, r18
    movw r22, r20
    call pm_dev_scale_signed
    movw r16, r24
    pop r0
    out 0x3c, r0
    pop r0
    out 0x3b, r0
    pop r0
    out 0x3a, r0
    pop r0
    out 0x39, r0
    pop r0
    out 0x38, r0
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
.size pm_dev_scale_signed_bridge, .-pm_dev_scale_signed_bridge

/* Unsigned entry 0x214c: same register preservation, with exact SREG
 * returned by the C ABI adapter. MUL reconstructs the original R1:R0. */
.global pm_dev_scale_unsigned_bridge
.type pm_dev_scale_unsigned_bridge,@function
pm_dev_scale_unsigned_bridge:
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
    in r0, 0x38
    push r0
    in r0, 0x39
    push r0
    in r0, 0x3a
    push r0
    in r0, 0x3b
    push r0
    in r0, 0x3c
    push r0
    clr r1
    out 0x38, r1
    out 0x39, r1
    out 0x3a, r1
    out 0x3b, r1
    out 0x3c, r1
    movw r24, r18
    movw r22, r20
    in r20, 0x3f
    call pm_dev_scale_unsigned_abi
    movw r16, r22
    pop r0
    out 0x3c, r0
    pop r0
    out 0x3b, r0
    pop r0
    out 0x3a, r0
    pop r0
    out 0x39, r0
    pop r0
    out 0x38, r0
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
.size pm_dev_scale_unsigned_bridge, .-pm_dev_scale_unsigned_bridge


/* Only the exhaustively covered application coefficients enter C.
 * Others replay the displaced original prefix and continue the original body.
 * Both entry registers and SREG are restored before either route. */
.macro guard_save
    push r0
    in r0, 0x3f
    push r0
.endm
.macro guard_restore
    pop r0
    out 0x3f, r0
    pop r0
.endm
.section .text.pm_scaling_dispatch,"ax",@progbits
.global pm_scaling_signed_entry
pm_scaling_signed_entry:
    guard_save
    cpi r18, 0x88
    brne .Lsigned_original
    cpi r19, 0x41
    brne .Lsigned_original
    guard_restore
    jmp pm_dev_scale_signed_bridge
.Lsigned_original:
    guard_restore
    clr r17
    mul r18, r20
    jmp 0x2134
.global pm_scaling_unsigned_entry
pm_scaling_unsigned_entry:
    guard_save
    cpi r19, 2
    brne .Lunsigned_original
    cpi r18, 0x0c
    breq .Lunsigned_c
    cpi r18, 0x72
    brne .Lunsigned_original
.Lunsigned_c:
    guard_restore
    jmp pm_dev_scale_unsigned_bridge
.Lunsigned_original:
    guard_restore
    clr r17
    mul r18, r20
    jmp 0x2150
.global pm_scaling_patch_table
pm_scaling_patch_table:
    jmp pm_scaling_signed_entry
    jmp pm_scaling_unsigned_entry
