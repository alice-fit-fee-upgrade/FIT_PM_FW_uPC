# Forensic inventory

Full machine-readable inventory: `forensic_inventory.json`. All upstream tracked files, including Ghidra project/database files, C, drivers, listings, makefiles and schematics, are inventoried below. Existing firmware/analysis files were not modified; root Makefile gains an isolated exact-check target.

Acquisition provenance is a user/repository assertion, not a verified chain of custody. Golden FLASH uses the supplied `1-PM.hex`, not an assumed-equivalent repo dump.

Target: ATxmega128A3U, PDI, user-reported signature 0x1E9742. Screenshot selects ATxmega128A3 (without U); this naming discrepancy is retained. Canonical FLASH is 0x22000 bytes: 128 KiB application plus 8 KiB boot; EEPROM 2 KiB; user signature 512 bytes. Device reference: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8386-8-and-16-bit-AVR-Microcontroller-ATxmega64A3U-128A3U-192A3U-256A3U_datasheet.pdf

| Supplied/reference | Comparison | Text identical | Canonical identical | Different bytes |
|---|---|---|---|---|
| reference/originals/1-PM.hex | dump/flash.hex | False | True | 0 |
| reference/originals/4-PM.eep | dump/eeprom.hex | False | True | 0 |
| reference/originals/3-PM_sign.hex | dump/usersig.hex | False | True | 0 |
| reference/originals/3-PM_sign.hex | reference/originals/5-PM_sign.hex | True | True | 0 |

Repository flash HEX is shorter/sparse; supplied FLASH includes the full space. Both describe the same canonical FLASH only if the comparison above says True. Record checksums, EOF, record sizes, extended segment/linear addressing and conflicting overlaps are validated by `tools/memory_image.py`. Type 02 uses segment << 4; type 04 uses upper word << 16. Missing cells become FF. Addresses in this inventory are byte addresses.

## Original ELF

`file` reports ELF32 AVR EXEC, but there is no .text/application/boot section and no executable code. It is a device-memory programming container, 992 bytes, stripped, no symbols, entry 0, flags 0, four LOAD segments. `avr-readelf`, `avr-objdump`, `avr-nm` outputs are preserved alongside this document.

| Section | Address | Length | Memory |
|---|---|---|---|
| .signature | 0x840000 | 3 | Device ID, stored 42 97 1E |
| .lock | 0x830000 | 1 | Lock bits |
| .fuse | 0x820000 | 6 | Fuses, includes unused index 3 |
| .user_signature | 0x850000 | 512 | User signature |

## Every input file

| Path | Bytes | SHA256 | Address range / record types | Memory | Format |
|---|---:|---|---|---|---|
| .gitignore | 21 | `e3881c0fafea776df46f74a8f8ee35a13fdd16c4c7af9f6a4a18722b22fe837c` | not memory-addressed;  | not a memory image | ASCII text |
| Makefile | 7710 | `4922e2b1378cb59a9dd05b25817234d1fdfef6055fd25c51cd027bf616bb3dcf` | not memory-addressed;  | not a memory image | makefile script, ASCII text |
| README.md | 9248 | `ef9aea8cc49e865ba080f437e35fb15b2709b0701a940d845716908ae8e12327` | not memory-addressed;  | not a memory image | ASCII text |
| asm_analysis/ATxmega128A3Udef.inc | 256931 | `883bb3d3fe0be0683967499e6c1c197271c9d775191e2120c7420ebb9c8846d1` | not memory-addressed;  | not a memory image | C source, ASCII text |
| asm_analysis/Makefile | 649 | `7fd69b2705c1ad6f2029592cbb4380f1e4fe7024aa24526d1e6915a543a37a62` | not memory-addressed;  | not a memory image | makefile script, ASCII text |
| asm_analysis/main.S | 282395 | `aa99daadd2705460e4134f5e71bf3eaf600ca81160b0698254589307c7682232` | not memory-addressed;  | not a memory image | C source, ASCII text |
| avr_compiler.h | 5310 | `04ed9b1a5ca4c4b0be1976a7c6f9125bf317df228e093509089c75d99267b3dc` | not memory-addressed;  | not a memory image | C source, ASCII text |
| console.c | 2400 | `d938ac4436aa55b423d940412130f8430b14b29e58db727b1db44c14e29f9e04` | not memory-addressed;  | not a memory image | C source, ASCII text |
| console.h | 208 | `35362eea96fae0cc0354b472c6f15d4583f43f8d4bd38a84c9e571a8cc78f68d` | not memory-addressed;  | not a memory image | C source, ASCII text |
| doc/PM12 module.png | 714121 | `525fdf94fd8f2b6552b8cee75750124b4b23ed35094570f25e6a788ac7f7b2d9` | not memory-addressed;  | not a memory image | PNG image data, 1728 x 970, 8-bit/color RGBA, non-interlaced |
| drivers/TC_driver.c | 15205 | `8bc0737f62340efd566bfede12e806c593542a6429b814ecf13ebce03c194565` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/TC_driver.h | 16600 | `69b152f34e489e762bcffdfeb15de2dc727f46140cdc7995ae71993e791d083f` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/adt7311_driver.c | 3340 | `7296fae508ce5a8e13b3f31271e5fd771f3645f0655e2cd469ae25297c9a5b3d` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/adt7311_driver.h | 3050 | `efc7459e340791f0a6fe4e974c5582ad460431d040bfe4809c6dcfdc498ecf22` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/clksys_driver.c | 11005 | `35a07557fdbc34e9bb59423d4b2e99d2afaebce5ad51ab45203b6744b3fc6d12` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/clksys_driver.h | 5389 | `5610ce53dab6d0fff71e2ea96952aa39b37b66d2355f0ff98bf5d9a68d42290d` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/eeprom_driver.c | 10939 | `2c3fef451107c524beb9bdaabb5fd14942b497ca30c9d262784893ebde250a8f` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/eeprom_driver.h | 5601 | `e711c3fb79bedd02eddccd49a892f8806eb4ae7223b68b39e2a76c2bdeb06246` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/pmic_driver.c | 3854 | `81c37fab8a1de289685bf9afeeb6b397ab18c7c82234daab5bebdc808e5b171c` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/pmic_driver.h | 5612 | `c67ca977cab7ec4c0d649b2b6d720db66115a606d6e0681c12100c6fda53fe33` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/port_driver.c | 7146 | `71b858832047ea5583441b614411fc5cd405aebe0fcf4d43cdbddf8e4ba19159` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/port_driver.h | 10946 | `1116afaf430c66362e61e4c85e1847a60b6b5450d0ae76e70c458068905806c3` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/spi_driver.c | 14100 | `9f3e71d6993fb2fe7050789015325e2f150357b9e8e0dbaf16f1e1770ab46f10` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/spi_driver.h | 8026 | `8fd37f8562d7209e06243c69168f011d9047feeaffc7ed4f624bc7ad56a840e9` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/twi_master_driver.c | 12817 | `21af9858a7fef6a9964f753245bb4022bd4ca13995bfba2a9a45c83261421d6c` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/twi_master_driver.h | 5973 | `47625815e2c8d8d5c3046671ba1dabdbc0beb691e1a6052c6ad4b2fb5ad2dba1` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/usart_driver.c | 12843 | `61a5814135f6507a50ad76cc077e46775ea39403b9e3db00dc9a7e2839ec7542` | not memory-addressed;  | not a memory image | C source, ASCII text |
| drivers/usart_driver.h | 11431 | `7923c10d79e62b7c6f4e303911b90c009287efad5f16c5f7e6eb2e0d6d268559` | not memory-addressed;  | not a memory image | C source, ASCII text |
| dump/application.hex | 26680 | `23f805bb58b36f8b74254209d7f7a0c77aea6fe32fde8a8a0e36d8cc83f6c27d` | [0, 11227]; {0: 351, 1: 1} | FLASH | ASCII text |
| dump/apptable.hex | 12 | `9e2df0a1190a1205c098889c455e5b76c4df18b5ccac2b7605da1575f05b64c5` | None; {1: 1} | FLASH | ASCII text |
| dump/boot.hex | 3000 | `c1b6f29b04d903748a7872fe2525c5ba1aceff4e06bae2097dce1c4593ded969` | [0, 1253]; {0: 40, 1: 1} | FLASH | ASCII text |
| dump/data.hex | 12 | `9e2df0a1190a1205c098889c455e5b76c4df18b5ccac2b7605da1575f05b64c5` | None; {1: 1} | data-space dump; interpretation unverified | ASCII text |
| dump/eeprom.hex | 4876 | `196a669f62825c6737fb6644b6dcbbd432e72bbb4ddebfb1dd0651f286d2f355` | [0, 2047]; {0: 64, 1: 1} | EEPROM | ASCII text |
| dump/flash.hex | 314328 | `44fe28b45a0bfaaf64d534b41b10c1e24cf61e971eb0d7ab5d6a95a4b5dae842` | [0, 132325]; {0: 4136, 4: 2, 1: 1} | FLASH | ASCII text |
| dump/fuse0.hex | 26 | `24f66141b6c78ee3e1d77e2b654d69791249dba323f2b646b48902b28ff55014` | [0, 0]; {0: 1, 1: 1} | fuses | ASCII text |
| dump/fuse1.hex | 26 | `c3f2ee75459ee15fe6bf5a6f5ad95766f1477ffe75fb9d867f540e1f36d39b3f` | [0, 0]; {0: 1, 1: 1} | fuses | ASCII text |
| dump/fuse2.hex | 26 | `b9bd9767cbb1ebcc2bd9e143dd30c651d71257df3af29cc9e93381844f1649a4` | [0, 0]; {0: 1, 1: 1} | fuses | ASCII text |
| dump/fuse4.hex | 26 | `be578724b135365b3cc24456b9abdd90f145101c5d47a93b8593c1e3c2f78ba2` | [0, 0]; {0: 1, 1: 1} | fuses | ASCII text |
| dump/fuse5.hex | 26 | `ac7b5ae64e317049b67c1eee204935781008a055622d84477a6353eb03513d4b` | [0, 0]; {0: 1, 1: 1} | fuses | ASCII text |
| dump/lock.hex | 26 | `24f66141b6c78ee3e1d77e2b654d69791249dba323f2b646b48902b28ff55014` | [0, 0]; {0: 1, 1: 1} | lock bits | ASCII text |
| dump/signature.hex | 30 | `dfc1f8e8d0e78c7c6b19a4205089ec15fbf03ab615321d34b6f09054a952f32a` | [0, 2]; {0: 1, 1: 1} | device signature | ASCII text |
| dump/usersig.hex | 1228 | `14af3a00d567ff72dd78733dcf409f4126d1a8e7ae26fe482729ebff2a9a0726` | [0, 511]; {0: 16, 1: 1} | user signature | ASCII text |
| fpga.c | 2773 | `c2f3d1398b7a37d22bcbe584ba224e38e08183404f11965102047f2609139e79` | not memory-addressed;  | not a memory image | C source, ASCII text |
| fpga.h | 1901 | `6e5649b35aae9fcd8ce9a0f506783283c5e09e0132790ff5fee693a86ea28747` | not memory-addressed;  | not a memory image | C source, ASCII text |
| ghidra/Ghidra/Processors/Atmel/data/languages/avr8.ldefs | 2900 | `6b391c92b4673d27358144a7dc710948fa84fe4a79ba19b844559fa9b42c7c0c` | not memory-addressed;  | not a memory image | XML 1.0 document, ASCII text |
| ghidra/Ghidra/Processors/Atmel/data/languages/avr8xmega.pspec | 73107 | `5389cefad7e82e32d295bdc3d2cbcce5bf759c0091af29b264346040e91b66f9` | not memory-addressed;  | not a memory image | XML 1.0 document, ASCII text |
| ghidra/Ghidra/Processors/Atmel/data/languages/avr8xmega128a3.pspec | 57205 | `9ecdcfe5cc040c04f3a29c5fbc9b41dc0a1118bf5fdfdf60947986dd40f1443e` | not memory-addressed;  | not a memory image | XML 1.0 document, ASCII text |
| ghidra/PM_n.gpr | 0 | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | not memory-addressed;  | not a memory image | empty |
| ghidra/PM_n.rep/idata/00/00000001.prp | 481 | `d54a9c24029d6acd3feeaa2156ded79c874f676fb5c9d2ec439c7bf24c5ea3e4` | not memory-addressed;  | not a memory image | XML 1.0 document, ASCII text |
| ghidra/PM_n.rep/idata/00/~00000001.db/db.25.gbf | 1998848 | `4dd27a196b502a8039055b111f6f4151201a8e69e7be15d66ef99a93525fbd45` | not memory-addressed;  | not a memory image | data |
| ghidra/PM_n.rep/idata/~index.bak | 101 | `31376bd0a1b047d6e56548ef6c3caddb1846c33b184ecd98920d2bec9578761a` | not memory-addressed;  | not a memory image | ASCII text |
| ghidra/PM_n.rep/idata/~index.dat | 101 | `31376bd0a1b047d6e56548ef6c3caddb1846c33b184ecd98920d2bec9578761a` | not memory-addressed;  | not a memory image | ASCII text |
| ghidra/PM_n.rep/project.prp | 158 | `740bbe288178df70a043fa30394e15c2a36cd380d2c500538bb4bb251645d993` | not memory-addressed;  | not a memory image | XML 1.0 document, ASCII text |
| ghidra/PM_n.rep/projectState | 498 | `faddab86feb665a1c0a5e597d8bd096317cc7efdd490f87150e1a7fa44624b57` | not memory-addressed;  | not a memory image | XML 1.0 document, ASCII text, with CRLF line terminators |
| ghidra/PM_n.rep/user/00/00000001.prp | 510 | `01dff771048291979d29356a053e0a79f149bb0bc44f80a207c2e5b8b0d51920` | not memory-addressed;  | not a memory image | XML 1.0 document, ASCII text |
| ghidra/PM_n.rep/user/00/~00000001.db/db.22.gbf | 81920 | `76950c42767d14a1cacc731b72010230b7cb66f605cdb2243ccbc47d4fc2b54d` | not memory-addressed;  | not a memory image | data |
| ghidra/PM_n.rep/user/~index.bak | 122 | `35c817c8aaa7c8dde7ac43aec122ff8e91e3bae47f002ce29a5432e214528733` | not memory-addressed;  | not a memory image | ASCII text |
| ghidra/PM_n.rep/user/~index.dat | 122 | `6dbc9d241d4fac2bea4a0afe09a20f3946767d011f65409279a595bdc5be12f8` | not memory-addressed;  | not a memory image | ASCII text |
| ghidra/PM_n.rep/user/~journal.bak | 136 | `195462bdf289952dc1e97442dc3439d842372b3ee515df5793e2f42eb67590bc` | not memory-addressed;  | not a memory image | ASCII text |
| ghidra/PM_n.rep/versioned/~index.bak | 59 | `0a5034997e47499d758115fa580ad21d96557d2a32c89f2263bcfd3fa8a9a6e5` | not memory-addressed;  | not a memory image | ASCII text |
| ghidra/PM_n.rep/versioned/~index.dat | 59 | `0a5034997e47499d758115fa580ad21d96557d2a32c89f2263bcfd3fa8a9a6e5` | not memory-addressed;  | not a memory image | ASCII text |
| io.c | 4357 | `ab97cbe06ca15ad8aefb623dfa5d0da5297554b72f2af81ee359e37047d162ac` | not memory-addressed;  | not a memory image | C source, ASCII text |
| io.h | 1252 | `fc3a5efedf82f36f33bc8ca9030cfc16257cc4c257574c5750f51939a25ea030` | not memory-addressed;  | not a memory image | C source, ASCII text |
| main.c | 2327 | `0934f0dcc0816e9d57b2a429814c0db96b720ab72003ed6b8fa174d10b27e2d0` | not memory-addressed;  | not a memory image | C source, ASCII text |
| sch/FIT-PM12-1.PDF | 5645207 | `0c90f12eee27cde1945e2a74fd3ac1a9cd9d443054f5c0d99e08953558f5cdf0` | not memory-addressed;  | not a memory image | PDF document, version 1.4, 110 page(s) |
| system.c | 2896 | `dcbee7a52879bd384e4c65d0f580de52d0ec8ea55676897132e543979d3fb7d5` | not memory-addressed;  | not a memory image | C source, ASCII text |
| system.h | 1089 | `849c6d14ec35bc2bd488ce3349ac00b302146297366b1b05193f9f0481eb0b47` | not memory-addressed;  | not a memory image | C source, ASCII text |
| ths788.c | 473 | `83248248347e6dbd4bb6c359c95f65d1b88aa5f3064d31be795462a71d371f41` | not memory-addressed;  | not a memory image | C source, ASCII text |
| ths788.h | 1287 | `aedd054f4b1038b1462a1bc41ebfa47f1922a87e2634a1dbd423d33ad0e3696a` | not memory-addressed;  | not a memory image | C source, ASCII text |
| timer.c | 10768 | `25cb79bc61fe3ebe1be15dc30dabb99d0850c63d0bce71e4638c1539dabadb24` | not memory-addressed;  | not a memory image | C source, ASCII text |
| timer.h | 281 | `07de9cc9115227ae69a6915da433ddf22ed42b27a9f9ab0d38aa958b6bec5865` | not memory-addressed;  | not a memory image | C source, ASCII text |
| reference/originals/1-PM-Zrzut-ekranu-2023-11-11-131139.jpg | 106966 | `f2b2db47f46416afd4b75d13c6778445d82f305eab0fa668bf8a7d197deed505` | not memory-addressed;  | not a memory image | JPEG image data, JFIF standard 1.01, aspect ratio, density 1x1, segment length 16, baseline, precision 8, 1280x1107, components 3 |
| reference/originals/1-PM.hex | 391727 | `334804bf1a52e564c106ef7ab5b3ea356846a1efe75ecac3829321e5a722159e` | [0, 139263]; {0: 8704, 2: 2, 1: 1} | FLASH | ASCII text, with CRLF line terminators |
| reference/originals/2-FIT_PM12-1-.PDF | 6851374 | `8a33ca4d4325eea999347ee7f4c26173cb28356f5cb61e6d38a9852a0513de78` | not memory-addressed;  | not a memory image | PDF document, version 1.4, 46 page(s) |
| reference/originals/2-PM_Atxmega128.elf | 992 | `c62bfd28b80441fd6ce854aa10a9491790bcb5a78b2542f36f52642ae5c62a4b` | not memory-addressed;  | ELF multi-memory container | ELF 32-bit LSB executable, Atmel AVR 8-bit, version 1 (GNU/Linux), statically linked, stripped |
| reference/originals/3-PM_sign.hex | 1453 | `768d3bc5b3fdbb978b96180b7a9509dc49741cc742509ece97eb11e0f4a7dc5a` | [0, 511]; {0: 32, 1: 1} | user signature | ASCII text, with CRLF line terminators |
| reference/originals/4-PM.eep | 5773 | `2bf9f5cf4b9784bbe5d8822758331599b316e819fedebfe2e7d250d7ef5fbde6` | [0, 2047]; {0: 128, 1: 1} | EEPROM | ASCII text, with CRLF line terminators |
| reference/originals/5-PM_sign.hex | 1453 | `768d3bc5b3fdbb978b96180b7a9509dc49741cc742509ece97eb11e0f4a7dc5a` | [0, 511]; {0: 32, 1: 1} | user signature | ASCII text, with CRLF line terminators |

## Additional cross-checks

- Repo application.hex is region-relative (base 0x0); canonical region matches supplied FLASH: True.
- Repo boot.hex is region-relative (base 0x20000); canonical region matches supplied FLASH: True.
- Repo apptable.hex has EOF only, no payload; it does not establish that the corresponding hardware space is erased.
- Repo data.hex has EOF only, no payload; it does not establish that the corresponding hardware space is erased.
- Supplied and repo schematic PDFs differ at file level; both are separately hashed. No PDF semantic identity is assumed.
- ELF user signature matches both supplied HEX files. Lock and defined fuses match repo dumps. Device signature has REVERSED BYTE ORDER: ELF 42 97 1E versus repo HEX 1E 97 42; both describe displayed ID 0x1E9742, but these payloads are not byte-identical.
