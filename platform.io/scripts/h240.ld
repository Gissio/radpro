/* Rad Pro H240 application linker script. License: MIT */

ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 40K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

__stack_top = ORIGIN(RAM) + LENGTH(RAM);
__stack_bottom = __stack_top - 1536;

SECTIONS
{
    .isr_vector :
    {
        __image_start = .;
        KEEP(*(.isr_vector))
    } > FLASH

    .text :
    {
        *(.text .text.*)
        *(.rodata .rodata.*)
        . = ALIGN(4);
    } > FLASH

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > FLASH
    .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } > FLASH

    .data :
    {
        . = ALIGN(4);
        __data_start = .;
        *(.data .data.*)
        . = ALIGN(4);
        __data_end = .;
    } > RAM AT > FLASH
    __data_load = LOADADDR(.data);

    .image_crc :
    {
        . = ALIGN(4);
        __image_crc = .;
        LONG(0xffffffff)
    } > FLASH

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss .bss.* COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > RAM

    /DISCARD/ : { *(.note.GNU-stack .eh_frame*) }
}

ASSERT(__image_start == ORIGIN(FLASH), "H240 vectors must start at the flash base")
ASSERT(__bss_end <= __stack_bottom, "H240 data and stack exceed the 8 KiB SRAM budget")
ASSERT(__image_crc + 4 <= ORIGIN(FLASH) + LENGTH(FLASH), "H240 image exceeds the application flash window")
