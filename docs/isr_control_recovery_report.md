# Timer and DMA control/data recovery

Canonical FLASH EXACT MATCH: **YES**. Differing bytes: **0**.
This pass started from a2a2680 on recovery/binary-exact-c-asm, preserving the
accepted recovered source, original toolchain/profiles and all reference trees.

| Measure | Before | After |
|---|---:|---:|
| Timer ISR ASM | 338 B | 300 B |
| DMA ISR ASM | 236 B | 228 B |
| Application ASM | 3104 B | 3058 B |
| Inline ASM | 3074 B | 3028 B |
| Compiler-generated C | 6954 B | 7000 B |
| ASM helper intervals | 638 | 634 |
| Pure C application entries | 5 | 5 |

**46 executable bytes** moved from ASM to actual compiler-generated C:
7 local byte conditions (28 B) and 9 displacement loads (18 B).
C now accounts for **69.5963%** of application executable bytes and **64.5995%**
of all executable bytes. Application .S remains 30 B; bootloader ASM remains
778 B / 9 procedures. Data/constants remain 710 B and are excluded from coverage.
Application classifications remain 89 entries: C_BINARY_EXACT 5,
C_WITH_EXACT_ASM_HELPER 83, ASM_EXACT 1.

## Accepted constructions

Existing private registers are captured with zero-byte output constraints before
C reads them. Local conditions use `if (value == constant)` or
`if (value != 0)` with the actual local C target. Whole-image checks establish
that the original instructions, operands, flags and branch encodings remain
identical; neither the empty captures nor C-hosted ASM count as converted bytes.

The displacement loads use an existing pointer captured into its original
X/Y/Z register and an ordinary byte access `value = cursor[offset]`.
Unlike post-increment LD pairs, these loads need no extra pointer arithmetic.
Eight reviewed source substitutions cover nine instructions because one exact
source instruction occurs twice. No artificial volatile was added.

## Rejected constructions and retained helpers

34 control/clear trials accepted 7 substitutions; all 8 load trials were
accepted. Four additional increment trials were rejected. Source and manifest
were restored after each rejection, followed by a fresh complete-image check.
The retained arithmetic still depends on original flag behavior and encoding;
value-equivalent expressions are not sufficient acceptance evidence.

48 retained clear/increment/decrement instruction sites now use small named
macros in [legacy_flag_ops.h](../recovered_exact/src/legacy_flag_ops.h).
The macros retain the original register strings, clobbers and instruction order.
Their executable ASM reduction is **zero**. Their benefit is a shared explicit
contract: EOR-self sets Z and clears N/V/S while preserving C/H/T/I; INC/DEC
update Z/N/V/S while preserving C/H/T/I. These are private-register instruction
primitives, not ordinary GNU ABI functions. Existing register captures remain
required at the C boundary.

Value-only reference expressions are labeled REFERENCE_C_INFERRED. The separate
compiled register-state models passed **394944 instruction-transition cases**,
with **1084 operand shapes**. This test does not certify whole-path, asynchronous
IRQ or timing equivalence for an independently compiled plain-C implementation.
The byte-identical accepted substitutions require no exhaustive behavior tests.

## Reproduction

Reviewed substitutions, exact-check logs and acceptance inventories:

* [Control/clear trials](codegen_isr_control_trials/results.json)
* [Displacement-load trials](codegen_isr_data_trials/results.json)
* [Increment trials](codegen_isr_increment_trials/results.json)
* [Named flag-helper trials](codegen_isr_flag_helpers_trials/results.json)
* [Initial byte inventory](isr_control_baseline_inventory.json)
* [Final byte inventory](isr_control_final_inventory.json)

Run `make c-comment-check` for explanatory models and `make exact-check` for the
complete baseline. The latter clean-builds the image, compares all 139264 FLASH
bytes and independently audits GCC C/ASM provenance. All 806 original text
symbols preserve their addresses. exact_asm, reference, dump and asm_analysis
remain unchanged.

Golden SHA256 = rebuilt SHA256:
`e80e8612f7114caff6d7a2494ad0f51010182f2c2c83fdfb3047c9f8a34663c0`.
