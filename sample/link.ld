OUTPUT_ARCH("riscv:rv32")
ENTRY(_start)

MEMORY{
    ROM : ORIGIN = 0x0000, LENGTH = 16k
    RAM : ORIGIN = 0x0400, LENGTH = 16k
}

SECTIONS{
    . = 0x0000;

    .text.init : {
		. = ALIGN(4);
    	*(.text.init)
	} > ROM

    .text : {
        . = ALIGN(4);
        *(.text)
    } > ROM

    .data : {
        . = ALIGN(4);
        *(.data)
    } > RAM

    .bss : {
        . = ALIGN(4);
        *(.bss)
    } > RAM

    .rodata : {
        . = ALIGN(4);
        *(.rodata)
    } > RAM
}
