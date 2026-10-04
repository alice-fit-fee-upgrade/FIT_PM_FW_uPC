OUTPUT_FORMAT("elf32-avr")
OUTPUT_ARCH(avr:106)
ENTRY(reset)
MEMORY { flash (rx) : ORIGIN = 0, LENGTH = 0x22000 }
SECTIONS {
 reset = 0;
 INCLUDE generated/sections.ld
}
INCLUDE generated/assertions.ld
