/* Instrumentation only. Original PG countdown/fault branches stay intact.
 * TCE0 (unused by recovered firmware) runs at clk_per/1024, no IRQ/output.
 * CCA stores elapsed ticks; CCB low: 0 unstarted,1 waiting,2 PG observed,
 * 3 original PG timeout,4 PG already high,5 other shutdown; CCB high: OVF.
 * Measurement stops at the original GPIO ISR, so IRQ latency is included.
 * No new SRAM, vectors, power decisions, PLL profile or GPIO directions. */
.section .text.pm_pg_hooks,"ax",@progbits
.macro SAVE
 push r0
 in r0,0x3f
 push r0
 push r16
 push r17
 cli
.endm
.macro RESTORE
 pop r17
 pop r16
 pop r0
 out 0x3f,r0
 pop r0
.endm
.global pm_pg_init_hook
pm_pg_init_hook:
 SAVE
 ldi r16,0
 out 0x0e,r16
 out 0x0f,r16
 ldi r16,0
 sts 0x0a00,r16
 sts 0x0a01,r16
 sts 0x0a03,r16
 sts 0x0a04,r16
 sts 0x0a06,r16
 sts 0x0a07,r16
 sts 0x0a21,r16
 sts 0x0a20,r16
 sts 0x0a29,r16
 sts 0x0a28,r16
 sts 0x0a2b,r16
 sts 0x0a2a,r16
 /* Disabled C/D compare channels: last/sticky ADT alarm bits. */
 sts 0x0a2d,r16
 sts 0x0a2c,r16
 sts 0x0a2f,r16
 sts 0x0a2e,r16
 ldi r16,0xff
 sts 0x0a27,r16
 sts 0x0a26,r16
 sts 0x0a0c,r16
 RESTORE
 sts 0x0800,r16
 jmp 0x106e
.global pm_pg_start_hook
pm_pg_start_hook:
 SAVE
 ldi r16,0
 sts 0x0a00,r16
 sts 0x0a21,r16
 sts 0x0a20,r16
 sts 0x0a29,r16
 sts 0x0a28,r16
 sts 0x0a2b,r16
 ldi r16,0xff
 sts 0x0a0c,r16
 ldi r16,1
 sts 0x0a2a,r16
 lds r17,0x0688
 sbrc r17,3
 rjmp .Lalready_high
 ldi r16,7
 sts 0x0a00,r16
 rjmp .Lstart_done
.Lalready_high:
 ldi r16,4
 sts 0x0a2a,r16
.Lstart_done:
 RESTORE
 sts 0x0685,r16
 jmp 0x0666
/* Caller supplies result in r16; every gate saved r0/r16/r17/SREG. */
pm_pg_record:
 push r16
 ldi r16,0
 sts 0x0a00,r16
 lds r16,0x0a20
 lds r17,0x0a21
 sts 0x0a29,r17
 sts 0x0a28,r16
 lds r16,0x0a0c
 andi r16,1
 sts 0x0a2b,r16
 pop r16
 sts 0x0a2a,r16
 ret
.global pm_pg_irq_hook
pm_pg_irq_hook:
 SAVE
 lds r16,0x0a2a
 cpi r16,1
 brne .Lirq_done
 lds r16,0x0684
 sbrs r16,2
 rjmp .Lirq_done
 lds r16,0x0688
 sbrs r16,3
 rjmp .Lirq_done
 ldi r16,2
 rcall pm_pg_record
.Lirq_done:
 RESTORE
 jmp PORTE_INT0_vect_isr
.macro OFF name, continuation, result
.global \name
\name:
 /* Cut power first; telemetry does not precede the original shutdown. */
 sts 0x0686,r16
 SAVE
 lds r16,0x0a2a
 cpi r16,1
 brne 1f
 ldi r16,\result
 rcall pm_pg_record
1:
 RESTORE
 jmp \continuation
.endm
OFF pm_pg_timeout_hook,0x0692,3
OFF pm_pg_thermal_hook,0x05ea,5
OFF pm_pg_alarm_hook,0x0ad8,5
OFF pm_pg_input_loss_hook,0x0bd0,5
OFF pm_pg_retry_hook,0x0c10,5
OFF pm_pg_manual_hook,0x144a,5
OFF pm_pg_restart_hook,0x1554,5
OFF pm_pg_reset_hook,0x1c42,5
.section .text.pm_pg_patch_table,"ax",@progbits
.global pm_pg_patch_table
pm_pg_patch_table:
 jmp pm_pg_init_hook
 jmp pm_pg_start_hook
 jmp pm_pg_irq_hook
 jmp pm_pg_timeout_hook
 jmp pm_pg_thermal_hook
 jmp pm_pg_alarm_hook
 jmp pm_pg_input_loss_hook
 jmp pm_pg_retry_hook
 jmp pm_pg_manual_hook
 jmp pm_pg_restart_hook
 jmp pm_pg_reset_hook
