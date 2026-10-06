/* REFERENCE_C_INFERRED (value-level adapters):
 * getc: save_native_state(); legacy_read(); value = R16;
 *       restore_native_state_except_R24(); return value;
 * putc: save_native_state(); R16 = value; legacy_send(); restore_native_state();
 * Actual decoded register/flag save/restore paths are checked with adversarial
 * substituted callees by tests/check_bridge.py; not a whole UART/timing test.
 * Legacy -> GNU ABI gate and GNU -> legacy queue adapters.
 * Value-level reference (INFERRED): fetch original first command byte;
 * if (byte != '@') continue original dispatcher; else dispatch_extensions();
 * Save/restore models are tested against the decoded linked instructions.
 * The initial ANDI/STS/CALL replay preserves the legacy CR counter semantics.
 */
.section .extension.entry,"ax",@progbits
.global pm_extension_entry
pm_extension_entry:
    andi r16, 0x7f
    sts 0x2005, r16
    call cli_get_next_char
    push r0
    in r0, 0x3f
    push r0
    cpi r16, '@'
    breq .Lnew_command
    pop r0
    out 0x3f, r0
    pop r0
    jmp 0x12f4
.Lnew_command:
    push r1
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push r16
    push r17
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
    push r28
    push r29
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
    call pm_extension_dispatch
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
    pop r29
    pop r28
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
    pop r17
    pop r16
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop r7
    pop r6
    pop r5
    pop r4
    pop r3
    pop r2
    pop r1
    pop r0
    out 0x3f, r0
    pop r0
    ret

/* JMP+NOP is assembled/relocated by GNU binutils, then copied only into the
 * six displaced bytes in the development ELF's CLI section. Never golden. */
.section .text.pm_patch_template,"ax",@progbits
.global pm_patch_template
pm_patch_template:
    jmp pm_extension_entry
    nop

.section .text.pm_console_getc,"ax",@progbits
.global pm_console_getc
pm_console_getc:
    push r0
    push r1
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push r16
    push r17
    push r18
    push r19
    push r20
    push r21
    push r22
    push r23
    push r25
    push r26
    push r27
    push r28
    push r29
    push r30
    push r31
    in r0, 0x3f
    push r0
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
    call cli_get_next_char
    mov r24, r16
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
    pop r0
    out 0x3f, r0
    pop r31
    pop r30
    pop r29
    pop r28
    pop r27
    pop r26
    pop r25
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18
    pop r17
    pop r16
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop r7
    pop r6
    pop r5
    pop r4
    pop r3
    pop r2
    pop r1
    pop r0
    ret

.section .text.pm_console_putc,"ax",@progbits
.global pm_console_putc
pm_console_putc:
    push r0
    push r1
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push r16
    push r17
    push r18
    push r19
    push r20
    push r21
    push r22
    push r23
    push r25
    push r26
    push r27
    push r28
    push r29
    push r30
    push r31
    in r0, 0x3f
    push r0
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
    mov r16, r24
    call cli_send_buf
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
    pop r0
    out 0x3f, r0
    pop r31
    pop r30
    pop r29
    pop r28
    pop r27
    pop r26
    pop r25
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18
    pop r17
    pop r16
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop r7
    pop r6
    pop r5
    pop r4
    pop r3
    pop r2
    pop r1
    pop r0
    ret

