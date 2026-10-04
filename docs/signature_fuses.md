# Signature, fuses, lock and runtime scope

The original ELF is a programming-state container. Raw sections, verified with objdump:

| Space | Bytes | Evidence |
|---|---|---|
| Device signature | 42 97 1E | ELF .signature; repo signature.hex agrees; UI displays 0x1E9742 |
| Lock | FF | ELF .lock; repo lock.hex agrees |
| Fuses indices 0..5 | FF 00 FE FF E3 E8 | ELF .fuse; repo per-byte dumps agree at defined indices |
| User signature | 512 bytes FF | Both supplied PM_sign.hex files, repo usersig.hex and ELF .user_signature agree |

Raw fuse index 3 is FF and absent from per-fuse repo dump files; retain this reserved position.
Screenshot directly shows fuse0 FF, fuse1 00, fuse2 FE and fuse4 E3. Fuse5 value E8 comes from ELF,
not the cropped raw-register table. UI semantic observations: application reset, BOD continuously
active, 64 ms startup, JTAG disabled, watchdog lock unchecked, EEPROM-save unchecked, BOD 3.4 V.
Do not infer fuse bytes solely by interpreting checked boxes (many fuse fields are active-low).
Screenshot device selection says ATxmega128A3, while requested target is ATxmega128A3U.
No hardware identity is changed on that basis.

The device-signature byte order in the programming ELF is 42 97 1E, unlike the presentation order
1E9742; these are container bytes, not instructions and not a FLASH region. Device signature is
not a user-defined authentication signature. The 512-byte user signature is erased state.
No production calibration-row dump was supplied; do not conflate it with the three-byte device ID.

Static scan of explicit NVM_CMD writes finds application commands 0x36, 0x35 (EEPROM buffer/page),
0x26 (application FLASH erase); boot commands include 0x23, 0x2F, 0x2A, 0x05 and 0x0A in programming
operations. No explicit signature-row read command was identified in recovered code. This is static
evidence, not proof about every indirect entry or debugger-invoked operation. Runtime SPM/ELPM code
is retained unchanged. Fuse settings do affect reset destination, BOD/watchdog behavior, debugging
and EEPROM retention on chip erase; they must be managed separately from FLASH reconstruction.
No fuse/lock programming or hardware action was performed.
