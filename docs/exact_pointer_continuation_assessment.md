# Exact pointer/register continuation: steps 387–433

**33 of 47 candidates accepted**, across **15 translation units**, moving another
**106 executable bytes from ASM to C**. Fourteen candidates were restored. Every
accepted candidate independently passed the clean root `make exact-check` and
GCC APP/NOAPP provenance audit. No behavioral-equivalence acceptance or emulator
runs were used. Data and padding are excluded from the C byte counts.

## The successful changes

| Original entry | Additional C bytes | Accepted operations |
|---|---:|---|
| DMA_CH1_vect_isr | 8 | Three X/Y stores and the limit high-byte X read |
| TCC0_OVF_vect_isr | 32 | Twelve Z stores, two Z loads, Y read/store |
| PORTD_INT0_vect_isr | 4 | SREG snapshot and restore |
| PORTB_INT0_vect_isr | 2 | SREG restore; exact IN remains ASM |
| PORTF_INT0_vect_isr | 2 | Queue address low-byte addition |
| USARTF0_DRE_vect_isr | 4 | Readiness byte read and low address ADD |
| USARTF0_RXC_vect_isr | 4 | Write-index read and low address ADD |
| fpga_data_exchange | 6 | Final high-byte read and two returns |
| fpga_firmware_update | 4 | Low remaining-length SUB and upper-byte OR |
| FUN_code_000b59 | 2 | FLASH programming buffer-byte read |
| eeprom_settings_save | 8 | Byte read/store, offset extraction, progress-flag clear |
| cli_get_integer | 10 | Partial-product ADDs, two byte copies, return |
| cli_send_uint16 | 4 | Initial terminator store and return |
| cli_get_next_char | 12 | Queue addressing, mode mask, two queue reads, return |
| cli_send_buf | 4 | Queue low address ADD and return |
| **Total** | **106** | |

The machine-readable deltas, including original byte addresses, are in
`exact_pointer_continuation_accepted_bytes.json`. Individual acceptance/rejection
logs are indexed in `exact_continuation_results_387_433.json`.

## Why explicit pointer operands worked

Several integer-address candidates cast a globally reserved word to a pointer.
GCC still created address temporaries: timer failures used R24 as an address
before reload failed for POINTER_REGS; DMA candidates expanded fixed regions.
The stronger word reservation that helped MOVW/ADIW did not solve these loads.

Typed pointers captured through the actual X/Y/Z operand class worked instead:

```c
{
    register uint8_t *source asm("r30");
    asm volatile("" : "=z" (source) : : "memory");
    r16 = *source;
    asm volatile("" : "+r" (r16) : : "memory");
}
```

The capture and barriers emit zero bytes; C emits the original LD. Stores use
an analogous pointer/value capture followed by a C store. Read-modify boundaries
also stop substitution of an equivalent value held in another register. These
are private register-entry adapters, not general GNU argument/result APIs.

Auto-increment/decrement helpers are retained where C expanded the sequence.
For UART TX/RX, separating the final plain read from earlier LD Z+ instructions
accepted the C read without retrying a whole pointer-update rewrite. Queue-index
polling uses volatile typed reads to retain both reads inside the IRQ window.

## Carry, borrow and mode masking

Five UART/console paths now calculate the low address byte in C. The original
upper-byte ADC remains a small exact helper, consuming the identical carry from
the compiler-generated ADD. Where required, original CLR stays ASM because LDI
zero would preserve different flags. No wider addition or relocation is accepted.

The decimal parser now adds the digit into R0 and another partial byte into R21
in C, and copies the appropriate product bytes with native C assignments. MUL,
ADC, live nonzero R1 and original carry/error branches retain exact ASM. The FPGA
remaining-length low SUB similarly feeds retained upper SBCs; a C OR feeds the
original BREQ/CLR sequence. The odd-start 24-bit increment candidate expanded
layout and remains three original immediate instructions with a C comment.

Console character folding preserves the original signed threshold checks while
C implements the mode-bit test and original 0x5F mask. Original behavior is kept;
there is no libc case-folding replacement or character/protocol correction.

## Small interrupt frames

The opt-in `legacy_interrupt_register_c.h` reserves the saved SREG byte in R31
without allocating SRAM. Its users have local R31 call-saved compiler profiles.
PUSH/POP/RETI remain exact helpers. PORTD uses C for both IN and OUT.

PORTB needs a smaller split. Rejected trial421 emitted:

```text
push r31
in   r16,0x3f
mov  r31,r16
push r16
```

The original is PUSH R31 / IN R31 / PUSH R16. The candidate expanded the region
and borrowed R16 before its original save; it was rejected. Holding interrupted
R16 live until PUSH caused GENERAL_REGS allocation failure in trial422. Trial423
therefore retains the exact IN in ASM and restores SREG from C. The rejected
object disassembly is saved as `exact_c_steps/step_421_object_disassembly.txt`.
The header documents the split and refuses no checking: every user still has to
pass complete image equality. No runtime or independent behavioral-test claim is
made for the rejected read.

## Restored candidates

- Integer-cast DMA/timer accesses: region expansion or POINTER_REGS allocation;
  explicit typed pointer operands subsequently resolved their accepted accesses.
- DAC SPI selection copies/constant setup: changed the fixed region, including
  the smaller copy-only retry. The original selection helper retains a commented
  C value equivalent, without new standalone functional-test evidence.
- PLL alarm flag set: layout/encoding changed even with an opaque status operand;
  it remains the original SBI with its C comment.
- Final saturating-DAC ADD/carry decision: expanded its region, including an
  existing-value retry and one local no-crossjumping adjustment. Exact ASM stays;
  the historical tested complete C alternative/bridge retains its separate scope.
- Odd-start FPGA remaining counter: global triple reservation changed layout.
- Full PORTB SREG read: extra IN/MOV; the liveness retry did not allocate.
- Dispatcher returns: the combined conversion changed fixed layout. Returns
  passed separately in FPGA exchange, console read/send, parser and formatter;
  the dispatcher exits keep their original RET with a C equivalent comment.

Conditional candidates not reached by the scripts are not counted as attempted.
There were no new compiler-syntax failures in these 47 complete-firmware trials.

## Current exact baseline

- Application entries: **89**, including ISRs, main and shared formatter entries.
- C_BINARY_EXACT: **5**; C_WITH_EXACT_ASM_HELPER: **83**; ASM_EXACT: **1**.
- Bootloader: **9 ASM procedures / 778 executable bytes**.
- C: **6342/10836 executable bytes (58.5271%)**; application **63.0543%**.
- ASM: **4494 bytes (41.4729%)**, including **3686 inline-helper bytes**.
- Compiler-generated AVR primitive subset: **394 bytes**; other C: **5948**.
- All **806 original text symbols** retain their byte addresses.
- Two final clean ELF/HEX/BIN builds are identical.
- Full FLASH: **139264 bytes**, **differing bytes = 0**.

Golden and rebuilt canonical SHA256:

```text
e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0
```

The original `exact_asm`, `reference`, `dump` and `asm_analysis` remain unchanged.
