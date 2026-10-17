/* Ordinary C owns all DAC MMIO and the48-write update loop.
 * Assembly only preserves private/GNU ABI boundaries and final CPU flags.
 * C reference: save_legacy_state(); dac_send(control,value);
 * restore_state_and_original_flags(); return;
 * Entire wrappers, C and original callees execute in simulator tests. */
.macro banks_save_zero
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
 .endm
.macro banks_restore
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
 .endm
.section .text.pm_dev_dac_bridge,"ax",@progbits
.global pm_dev_dac_send_entry
.type pm_dev_dac_send_entry,@function
pm_dev_dac_send_entry:
    push r22
    push r23
    push r24
    push r0
    in r0,0x3f
    push r0
    cli
    push r1
    push r18
    push r19
    push r20
    push r21
    push r25
    push r26
    push r27
    push r30
    push r31
    push r22
    banks_save_zero
    mov r24,r22
    movw r22,r16
    call pm_dev_dac_send_locked
    banks_restore
    pop r22
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r21
    pop r20
    pop r19
    pop r18
    pop r1
    pop r0
    bst r0,6
    pop r0
    /* Original final header operations reconstruct C/H/N/V/S/Z.
     * Inputs remain stacked until after SEI, exactly as the old ABI requires. */
    mov r23,r22
    swap r22
    andi r22,0xc0
    lsl r23
    andi r23,6
    or r22,r23
    ori r22,0x10
    ldi r22,7
    sei
    pop r24
    pop r23
    pop r22
    ret
.size pm_dev_dac_send_entry,.-pm_dev_dac_send_entry
.global pm_dev_dac_apply_entry
.type pm_dev_dac_apply_entry,@function
pm_dev_dac_apply_entry:
    push r0
    push r1
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
    in r0,0x3f
    push r0
    banks_save_zero
    call pm_dev_dac_apply_stored
    banks_restore
    pop r0
    bst r0,6
    /* Original loop terminates with CPI12,12; last sender sets I=1. */
    ldi r24,12
    cpi r24,12
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
    pop r0
    ret
/* Standard GNU adapter: preserve callee-saved R16/R17; legacy routines
 * intentionally leave dirty R1, which GNU C requires to be zero. */
.size pm_dev_dac_apply_entry,.-pm_dev_dac_apply_entry
.global pm_dev_dac_set_legacy
.type pm_dev_dac_set_legacy,@function
pm_dev_dac_set_legacy:
    push r16
    push r17
    mov r18,r20
    movw r20,r24
    cpi r18,0
    breq .Lthreshold
    cpi r18,1
    breq .Lcfd_zero
    cpi r18,2
    breq .Ladc_zero
    call FUN_code_001068
    rjmp .Ldone
.Lthreshold:
    call FUN_code_001053
    rjmp .Ldone
.Lcfd_zero:
    call dac_set_value_2
    rjmp .Ldone
.Ladc_zero:
    call dac_set_value
.Ldone:
    clr r1
    pop r17
    pop r16
    ret
.size pm_dev_dac_set_legacy,.-pm_dev_dac_set_legacy
.global pm_dac_patch_table
pm_dac_patch_table:
    jmp pm_dev_dac_send_entry
    jmp pm_dev_dac_apply_entry
