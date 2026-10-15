/* Reference C: if (!try_claim_temperature()) { thermal_pending=true; return
 * through the original ISR epilogue; } original_temperature_read(); release();
 * Preserve R16 and all SREG bits during claim/defer. Busy ISR never spins.
 * Replay the displaced LDI/LDI and EOR/SBRS at their original successors.
 * The full legacy timer body, calls and peripheral timing remain in place.
 */
.section .text.pm_temperature_guards,"ax",@progbits
.global pm_temperature_claim
.global pm_timer_at_077e
pm_temperature_claim:
pm_timer_at_077e:
 push r16
 in r16,0x3f
 push r16
 cli
 in r16,0x0f
 andi r16,0x43
 brne .Ldefer
 cbi 0x0f,7
 sbi 0x0f,1
 pop r16
 out 0x3f,r16
 pop r16
.global pm_timer_instruction_077e
pm_timer_instruction_077e:
 ldi r16,0x50
 ldi r17,0xff
 jmp 0x0782
.Ldefer:
 sbi 0x0f,7
 in r16,0x0e
 cpi r16,255
 breq .Lcounted
 inc r16
 out 0x0e,r16
.Lcounted:
 pop r16
 out 0x3f,r16
 pop r16
 jmp 0x08be
.global pm_temperature_release
.global pm_timer_at_0788
pm_temperature_release:
pm_timer_at_0788:
 cbi 0x0f,1
 /* TCE0 compare channels C/D are disabled diagnostic storage. Snapshot the
  * three sensor alarm bits and retain their OR since reset. No SPI/PSU write.
  * ANDI/OR preserve C/H/T/I; the displaced EOR below restores Z/N/V/S.
  * R16 is preserved and original R17 is overwritten by that EOR anyway. */
 push r16
 mov r17,r20
 andi r17,7
 sts 0x0a2c,r17
 lds r16,0x0a2e
 or r16,r17
 sts 0x0a2e,r16
 pop r16
.global pm_timer_instruction_0788
pm_timer_instruction_0788:
 eor r17,r17
 sbrs r20,2
 jmp 0x078c
 jmp 0x078e
.global pm_temperature_patch_table
pm_temperature_patch_table:
 jmp pm_temperature_claim
 jmp pm_temperature_release
.global pm_timer_at_08be
.set pm_timer_at_08be,0x08be
.global pm_timer_at_0666
.set pm_timer_at_0666,0x0666
.global pm_timer_at_0692
.set pm_timer_at_0692,0x0692
.global pm_timer_at_05ea
.set pm_timer_at_05ea,0x05ea
