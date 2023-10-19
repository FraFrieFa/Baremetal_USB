ENTRY(main)

MEMORY{
  flash : ORIGIN = 0x00004000, LENGTH = 0x80000 - 0x4000
  ram :   ORIGIN = 0x20000000, LENGTH = 0x30000
}

MCLK_AHBMASK = 0x40000810;
MCLK_APBBMASK = 0x40000818;
OSCCTRL_EVCTRL = 0x40001000;
OSCCTRL_DFLLCTRLA = 0x4000101C;
OSCCTRL_DFLLCTRLB = 0x40001020;
GCLK_CTRLA = 0x40001C00;
GCLK_GENCTRL0 = 0x40001C20;
GCLK_GENCTRL1 = 0x40001C24;
GCLK_PCHCTRL0 = 0x40001C80;
GCLK_PCHCTRL10 = 0x40001CA8;

USB_CTRLA = 0x41000000;
USB_CTRLB = 0x41000008;
USB_DADD = 0x4100000A;

USB_INTENSET = 0x41000018;

PA_DIR = 0x41008000;
PA_OUT = 0x41008010;
PA_PMUX12 = 0x4100803C;
PA_PINCFG24 = 0x41008058;
PA_PINCFG25 = 0x41008059;



NVIC_ISER2 = 0xE000E108;


SECTIONS{
  . = ALIGN(4);
  .text : {
    . = ALIGN(4);
    KEEP(startup.o(.vector_table))
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