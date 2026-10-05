# Native entry frame recovery: steps 614–622

All nine candidates were accepted after individual full FLASH exact-checks.
Two shared formatter entries and seven ISRs now use GCC-generated entry saves.
The original restoring shared tail or RETI tail remains exact ASM.

The local -fcall-saved register profile exposes the save contract;
-fno-ipa-pure-const prevents noreturn inference from eliminating the entry save.
This is a private ABI boundary, not a conventional GNU interrupt declaration.

Native C increased from 6714 to 6732 executable bytes (+18); inline ASM
decreased from 3314 to 3296 bytes. Application C: 66.9318%; including boot: 62.1262%.
Final complete FLASH comparison: zero differing bytes; SHA256 both:
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`. All 806 original text symbols retain their addresses.

Instruction bytes are unchanged, so the existing compiled logical C models and
validation evidence remain applicable. Exhaustive historical tests were not rerun.
Per-candidate build and comparison logs: docs/exact_c_steps/step_614.log through
step_622.log. The result index is exact_frame_call_results_590.json.
