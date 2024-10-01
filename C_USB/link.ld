ENTRY(main)

MEMORY{
  flash : ORIGIN = 0x00004000, LENGTH = 0x80000 - 0x4000
  ram :   ORIGIN = 0x20000000, LENGTH = 0x30000
}

SECTIONS{
  . = ALIGN(4);
  .text : {
    . = ALIGN(4);
    KEEP(*(.vector_table))
    *(.text* .text.*)
    . = ALIGN(4);
  } > flash

  . = ALIGN(4);
  .rodata : {
    . = ALIGN(4);
    *(.rodata* .rodata.*)
    . = ALIGN(4);
  } > flash
  
  . = ALIGN(4);
  data_start_flash = .;
  .data : {
    . = ALIGN(4);
    data_start_ram = .;
    . = ALIGN(4);
    *(.data* .data.*)
    . = ALIGN(4);
    data_end_ram = .;
    . = ALIGN(4);
  } > ram AT > flash
  . = ALIGN(4);
  
  .bss : {
    . = ALIGN(4);
    bss_start_ram = .;
    . = ALIGN(4);
    *(.bss* .bss.*)
    . = ALIGN(4);
    bss_end_ram = .;
    . = ALIGN(4);
  } > ram AT > flash
  
  . = ALIGN(4);
  /DISCARD/ : {
    *(.comment)
    *(.ARM.attributes)
  }

}