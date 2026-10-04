# GPIO and FPGA state recovery: 14 integrated application entries

Two more original routines now route to compiled C, with the original addresses
and all 806 original text symbols retained:

| Original byte range | C implementation | Original span |
|---|---|---|
| 0x0A9C..0x0ABB | pm_fpga_set_state | 32 bytes |
| 0x0C7E..0x0C95 | pm_status_led | 24 bytes |

The state transition writes its incoming FPGA state to RAM 0x215B, clears PORTB
bit 7 via OUTCLR, and reads THS state at 0x2159. Only states zero and five are
changed to four. The assembly adapter restores all original registers except
R16, and reproduces the exact final AND/CPI flags, including preserved C/H when
the old state was zero. Incoming R0/R1 and T/I are preserved.

The status/LED routine tests GPIOR0 bit 1, clears bit 0 atomically with CBI,
and writes PORTA OUTSET or OUTCLR. Ordering of the atomic clear and LED write
is branch-dependent and matches the original. GNU AVR GCC emits real SBIS/CBI
instructions from the C volatile register expressions. R16 returns one; all
other registers and all incoming SREG bits are preserved by the adapter.

## Differential verification

The FPGA state suite checks all 256 old-state bytes and all 256 incoming SREG
patterns directly and through original FLASH-deinitialization caller 0x167E:
131072 actual linked-AVR comparisons. The caller additionally checks GPIO bit
clear and ordered CLI/SEI effects. The LED suite checks all 256 GPIO bytes and
all 256 SREG patterns: 65536 compiled-AVR comparisons. Both compare all 32
registers, all flags, ordered RAM/MMIO/IRQ effects and balanced three-byte return
stacks. Additional tested stack is 17 bytes direct state, 20 through its caller,
and 18 for LED. Global maximum remains 35 bytes through earlier DAC callers.

Low-I/O interpreter support now includes SBIC/SBIS and CBI/SBI. Independent
architecture fixtures check 16- and 32-bit skip lengths, every byte and bit for
atomic bit updates, and flag preservation (16392 synthetic fixtures, separate
from compiled-firmware case counts). Atomicity is represented as one GPIO update
in firmware comparisons; asynchronous interrupt execution and cycles are not modeled.

## Progress and layout

89 application entry points: 14 integrated C, 75 remain ASM. Functional C is
available for 45 entries, including 31 awaiting integration; 44 have no recovered
C. Nine boot procedures remain ASM, giving 84 nonintegrated entries including
boot. Entries include ISRs and shared formatters, not independent source bodies.

Substituted original spans total 464 bytes (4.28% of 10836 original symbolic
code bytes); this includes eight retained delay bytes and ABI scaffolding.
Bridges occupy 1130 bytes at 0x3000 and C/helpers 1390 at 0x4000. All 2919 changed
byte positions lie in permitted replacement regions or previously erased
additions. No new static SRAM, initialized data, BSS or CRT is introduced.
Original vectors, other instructions, tables, strings, startup and boot retain
exact bytes. The separate exact ASM source remains bit-exact to golden FLASH.

The next integrations are FPGA settings initialization/reset. Their ABI audit
is in fpga_settings_abi_audit.md; initialization's unusual unpreserved R28 must
be retained. Hardware, cycle timing, nested asynchronous interrupts, SPL timing
and full-firmware stack headroom remain unvalidated. No device programming occurs.

## Final validation

`make mixed-check` passed its full clean-build firmware regression: 2587488
compiled-AVR comparisons. `make c-check` passed 390601 portable C comparisons.
The additional architecture fixtures and golden-only settings audit were run
separately and are included in the updated mixed-check recipe for subsequent runs.
Reference SHA256 verification and exact-check passed; golden FLASH cmp still has
zero differences. A second clean build reproduced identical ELF, HEX and BIN
hashes, including tested mixed BIN SHA256
`c9d2c0c803939704ddb6a4c2513322bb6d7877a7e77ed68f921cde116c22232e`.

Full logs and result snapshots are saved as mixed_recovery_gpio_*. The
reproducibility JSON separates actual compiled-firmware comparisons from
synthetic architecture fixtures and the original-only settings audit.

## Intentional mixed-image differences

The unchanged exact ASM image has no differences. In the separate mixed image,
the two new entry JMPs are class E address/encoding changes; bridge/C additions
are class F layout changes in previously erased space; vacated original routine
bodies are class G padding. These are intentional replacements, not unresolved
exact-reconstruction failures. All 2919 differing positions are permitted by
check_layout.py, and instruction/byte reports are retained. Functional equality
is supported by the differential tests above, not by equal disassembly mnemonics.
