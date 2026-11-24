/* Rad Pro H240 SRAM application page loader. License: MIT */

ENTRY(flash_page_start)

MEMORY
{
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
    .text :
    {
        flash_page_image_start = .;
        KEEP(*(.text.flash_page_start))
        *(.text .text.*)
        *(.rodata .rodata.*)
        . = ALIGN(4);
        flash_page_image_end = .;
    } > SRAM

    .mailbox (NOLOAD) : ALIGN(4)
    {
        KEEP(*(.mailbox))
    } > SRAM

    .stack (NOLOAD) : ALIGN(8)
    {
        . += 512;
        flash_page_stack_top = .;
    } > SRAM

    .data (NOLOAD) :
    {
        *(.data .data.* .bss .bss.*)
        *(COMMON)
    } > SRAM

    .input 0x20001000 (NOLOAD) :
    {
        flash_page_input = .;
        . += 4096;
        flash_page_input_end = .;
    } > SRAM

    /DISCARD/ : { *(.ARM.exidx* .ARM.extab*) }
}

ASSERT(ADDR(.text) == ORIGIN(SRAM), "unexpected flash page load address")
ASSERT(SIZEOF(.mailbox) == 48, "flash page mailbox must contain twelve words")
ASSERT(SIZEOF(.data) == 0, "flash page loader must have no data initialization")
ASSERT(flash_page_stack_top <= flash_page_input, "flash page loader overlaps input SRAM")
ASSERT(flash_page_input_end == ORIGIN(SRAM) + LENGTH(SRAM), "unexpected flash page input end")
