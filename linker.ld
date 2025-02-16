ENTRY(_start)

MEMORY
{
    TEXT   : ORIGIN = 0x100000, LENGTH = 1M
    DATA   : ORIGIN = 0x200000, LENGTH = 1M
    BSS    : ORIGIN = 0x300000, LENGTH = 1M
    STACK  : ORIGIN = 0x400000, LENGTH = 1M
}

SECTIONS
{
    /* Text section (code) */
    .text : 
    {
        *(.text)
    } > TEXT

    /* Data section (initialized data) */
    .data : 
    {
        *(.data)
    } > DATA

    /* BSS section (uninitialized data) */
    .bss :
    {
        *(.bss)
    } > BSS

    /* Stack section */
    .stack :
    {
        *(.stack)
    } > STACK
}
