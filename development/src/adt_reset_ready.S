/* REFERENCE_C_INFERRED:
 *     legacy_adt_serial_reset();
 *     delay_us(600); // ADT7311 specifies >=500 us before another address.
 * This startup-only adapter preserves the exact reset callee, all its return
 * registers and SREG. It adds no register values or GPIO operations.
 * CLK_PER/CPU is 32 MHz here; the counted loop alone takes 19199 cycles.
 * Canonical recovered firmware remains unchanged; only development CALL 1100
 * is redirected to this adapter, before the original configuration writes.
 */
.section .text.pm_adt_reset_ready,"ax",@progbits
.global pm_adt_reset_ready
pm_adt_reset_ready:
 call adt7311_faults_clr
 push r0
 in r0,0x3f
 push r0
 push r16
 push r17
 ldi r16,lo8(4800)
 ldi r17,hi8(4800)
.global pm_adt_reset_delay_loop
pm_adt_reset_delay_loop:
 subi r16,1
 sbci r17,0
 brne pm_adt_reset_delay_loop
 pop r17
 pop r16
 pop r0
 out 0x3f,r0
 pop r0
 ret
.global pm_adt_reset_patch
pm_adt_reset_patch:
 call pm_adt_reset_ready
