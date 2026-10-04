# Checkpoint status

EXACT MATCH: YES. Differing bytes: 0. No unresolved FLASH reconstruction regions.

Remaining limits concern semantic recovery, not the exact binary build: source function names and
protocol meanings are partially inherited hypotheses; full EEPROM dynamic coverage and physical-board
validation remain pending; schematic/device-selector revision differences remain documented.
The separate C recovery now covers 33 original entries spanning 2,494 routine bytes.
Functional differential tests passed; C ABI/vector/timing integration remains pending, and
none of those routines replaces bytes in the exact firmware. See c_recovery.md.

A separate mixed C/ASM image now integrates the signed scaler at 0x2130, with original
register/SREG/stack contracts checked on compiled AVR opcodes. Remaining routines' ABI and
ISR integration, interrupt timing and hardware execution remain pending.
