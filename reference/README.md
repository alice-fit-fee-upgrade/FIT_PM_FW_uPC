# Protected reference snapshots

`originals/` preserves user attachments byte-for-byte with original upload prefixes.
FLASH: `1-PM.hex`; EEPROM: `4-PM.eep`; user signature: `3-PM_sign.hex`.
The duplicate `5-PM_sign.hex` is retained independently. Device-memory ELF, fuse screenshot
and supplied schematic are also retained. Original acquisition provenance is stated by the user.

Canonical images use byte-address spaces, validated Intel HEX, missing cells FF:
FLASH 0x22000, EEPROM 0x800, user signature 0x200. Normalization never merges memory spaces.
`SHA256SUMS` covers all original attachments and all canonical golden images. Check:
`cd reference && sha256sum -c SHA256SUMS`.
Files have write permissions removed. exact-check verifies hashes before compiling; normal builds
never generate or write references. To change the forensic baseline, add a new versioned snapshot
and document provenance; never replace these files or silently refresh their hashes.
