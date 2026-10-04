OUTPUT_FORMAT("elf32-avr")
OUTPUT_ARCH(avr:106)
ENTRY(reset)
MEMORY { flash (rx) : ORIGIN = 0, LENGTH = 0x22000 }
SECTIONS {
 reset = 0;
 .application 0 : { KEEP(*(.application)) } > flash
 .boot 0x20000 : { KEEP(*(.boot)) } > flash
 /DISCARD/ : { *(.comment) *(.note*) }
}
ASSERT(SIZEOF(.application) <= 0x20000, "application overflow")
ASSERT(SIZEOF(.boot) <= 0x2000, "boot overflow")
