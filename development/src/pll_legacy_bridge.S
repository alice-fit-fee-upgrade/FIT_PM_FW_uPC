/* Legacy FUN_code_001267 -> ordinary GNU ABI.
 * Original returns R19:R16, preserves other GPRs/bank registers, leaves
 * SREG Z=1,N=V=S=0 and preserves C,H,T,I. C routine owns the SPI transaction.
 * C reference: save_abi_clobbers(); result=pm_pll_read_status_c();
 * restore_abi_clobbers(); legacy_result=result; restore_legacy_flags();
 * Decoded full-call validation is tests/check_pll_legacy.py. */
.section .text.pm_pll_read_legacy_adapter,"ax",@progbits
.global pm_pll_read_legacy_adapter
pm_pll_read_legacy_adapter:
    push r0
    push r1
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
    call pm_pll_read_status_c
    movw r16, r22
    movw r18, r24
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
    pop r20
    andi r20, 0xe1
    ori r20, 2
    out 0x3f, r20
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
    pop r1
    pop r0
    ret
.section .text.pm_pll_patch_table,"ax",@progbits
.global pm_pll_patch_table
pm_pll_patch_table:
    jmp pm_pll_read_legacy_adapter

/* GNU test/diagnostic entry: call the ORIGINAL symbol, including its JMP gate.
 * Preserve GNU callee-saved R16/R17; legacy returns four bytes in R16..R19. */
.section .text.pm_pll_read_legacy_native,"ax",@progbits
.global pm_pll_read_legacy_native
pm_pll_read_legacy_native:
    push r16
    push r17
    call FUN_code_001267
    movw r22, r16
    movw r24, r18
    pop r17
    pop r16
    ret
