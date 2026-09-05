ENTRY(boot)

SECTIONS {
    . = 0x80200000;

    /*
     * Text section comes first.
     * Boot function must be at the start of the program memroy because that's
     * where SBI moves the program counter.
     */
    .text : {
        KEEP(*(.text.boot));
        *(.text .text.*);
    }

    /*
     * Read-only data.
     * i.e. Constants.
     */
    .rodata : ALIGN(4) {
        *(.rodata .rodata.*);
    }

    /*
     * Read-write data.
     * i.e. Variables.
     */
    .data : ALIGN(4) {
        *(.data .data.*);
    }

    /*
     * Read-write data that's initially set to zero.
     * i.e. Variables.
     */
    .bss : ALIGN(4) {
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }

    /*
     * 128KB stack.
     */
    . = ALIGN(4);
    . += 128 * 1024;
    __stack_top = .;
}
