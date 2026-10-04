# Forensic/reconstruction tools

All tools use Python 3 standard library; disassembly tools invoke existing AVR binutils.

- `forensic_inventory.py`: inventory all tracked upstream inputs and preserved attachments,
  hash them, validate HEX records and compare independently normalized memory.
- `memory_image.py INPUT OUTPUT --size 0x22000`: strict byte-address Intel HEX normalization.
  Optional offset supports explicitly shifted address spaces. Never point OUTPUT at a golden snapshot.
- `convert_analysis.py`: deterministic minimal conversion; does not read golden bytes.
- `recover_boot.py reference/flash_golden.bin /tmp/boot.S`: one-time symbolic forensic extraction.
  To validate provenance, compare output to `exact_asm/src/boot.S`. Not part of the build.
- `compare_flash.py GOLDEN REBUILT --report REPORT`: hashes, byte regions, extent/non-erased/union
  statistics, failure exit on mismatch. `--report-only` lets make produce instruction evidence before cmp.
- `instruction_diff.py GOLDEN REBUILT`: canonical-image disassembly windows, target/operand classification.
- `audit_asm.py`: verify Ghidra-derived label addresses, instruction coverage and vector slots;
  expects exact_asm to be built. Emits docs/asm_audit.json and instruction_index.json.
- `test_reconstruction.py`: six behavior tests; run with `python3 tools/test_reconstruction.py`.

Historical repo-relative boot.hex offsets are not physical absolute FLASH addresses;
this is independently documented in the inventory. Never merge unrelated memory spaces by filename.
