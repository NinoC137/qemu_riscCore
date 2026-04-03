ENTRY(start)

SECTIONS {
  . = 0x80000000;

  .text : ALIGN(4K) {
    *(.init)
    *(.text)
    *(.text.*)
  }

  .rodata : ALIGN(16) {
    *(.rodata)
    *(.rodata.*)
  }

  .data : ALIGN(16) {
    *(.data)
    *(.data.*)
  }

  .bss : ALIGN(16) {
    PROVIDE(bss_start = .);
    *(.bss)
    *(.bss.*)
    *(COMMON)
    PROVIDE(bss_end = .);
  }

  . = ALIGN(16);
  . += 4096;
  PROVIDE(stack_top = .);

  . = ALIGN(16);
  PROVIDE(global_pointer = .);
}
