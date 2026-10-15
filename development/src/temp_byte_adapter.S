/* REFERENCE_C_INFERRED: return legacy_adt_byte(tx);
 * Native R24 argument/result -> legacy R16. Legacy cell clobbers R17/R18
 * and SREG only. Save native callee-saved R16/R17; R1 remains zero.
 * This adapter adds no sensor-register writes or reset sequence. */
.section .text.pm_diag_legacy_temp_byte,"ax",@progbits
.global pm_diag_legacy_temp_byte
pm_diag_legacy_temp_byte:
 push r16
 push r17
 mov r16,r24
 call adt7311_byte_rw
 mov r24,r16
 pop r17
 pop r16
 ret
