OUTPUT_FORMAT("elf32-avr")
OUTPUT_ARCH(avr:106)
ENTRY(reset)
MEMORY {
 flash (rx) : ORIGIN = 0, LENGTH = 0x22000
 ram (rw!x) : ORIGIN = 0x802000, LENGTH = 0x2000
}
SECTIONS {
 reset = 0;
 .application 0 : { KEEP(*(.application)) } > flash
 .bridges 0x3000 : { KEEP(*(.bridges)) } > flash
 .text 0x4000 : { *(.text*) } > flash
 .data : { *(.data*) *(.rodata*) } > ram
 .bss (NOLOAD) : { *(.bss*) *(COMMON) } > ram
 .boot 0x20000 : { KEEP(*(.boot)) } > flash
 /DISCARD/ : { *(.comment) *(.note*) }
}
ASSERT(SIZEOF(.application) == 0x2bdc, "original application extent changed")
ASSERT(SIZEOF(.bridges) <= 0x1000, "bridge exceeds pinned space")
ASSERT(ADDR(.text) + SIZEOF(.text) <= 0x1e000, "C overlaps application-table area")
ASSERT(SIZEOF(.data) == 0, "new initialized SRAM needs startup/placement audit")
ASSERT(SIZEOF(.bss) == 0, "new BSS overlaps original RAM state")
ASSERT(SIZEOF(.boot) <= 0x2000, "boot overflow")
