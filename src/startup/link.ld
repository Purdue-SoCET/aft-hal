MEMORY
{
    RAM : ORIGIN = 0x8400, LENGTH = 64K
}

ENTRY(_start);

SECTIONS
{

    .text :
    {
        *(.text .text.*)
    } > RAM

    .data :
    {
        *(.data .data.* .rodata .rodata.* .eh_frame .eh_frame.*);
    } > RAM
    
    PROVIDE(_edata = .);

    PROVIDE(_sbss = .);
    .bss : 
    {
        *(.bss)
    } > RAM
    PROVIDE(_ebss = .);
    PROVIDE(_end = .);
    PROVIDE(__global_pointer$ = .);
    PROVIDE(__stack_top = ORIGIN(RAM) + LENGTH(RAM));
}