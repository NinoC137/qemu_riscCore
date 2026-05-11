ENTRY(start)

SECTIONS {
  . = 0x80000000;
  PROVIDE(kernel_start = .);

  .text : ALIGN(4K) {
    PROVIDE(text_start = .);
    *(.init)
    *(.text)
    *(.text.*)
    PROVIDE(text_end = .);
  }

  .rodata : ALIGN(4K) {
    PROVIDE(rodata_start = .);
    *(.srodata)
    *(.srodata.*)
    *(.rodata)
    *(.rodata.*)
    PROVIDE(rodata_end = .);
  }

  .data : ALIGN(4K) {
    PROVIDE(data_start = .);
    *(.sdata)
    *(.sdata.*)
    *(.data)
    *(.data.*)
    PROVIDE(data_end = .);
  }

  .bss : ALIGN(4K) {
    PROVIDE(bss_start = .);
    *(.sbss)
    *(.sbss.*)
    *(.bss)
    *(.bss.*)
    *(COMMON)
    PROVIDE(bss_end = .);
  }

  . = ALIGN(16);
  PROVIDE(boot_stack_bottom = .);
  PROVIDE(stack_bottom = .);
  . += 16K;
  PROVIDE(boot_stack_top = .);
  PROVIDE(stack_top = .);

  . = ALIGN(4K);
  PROVIDE(kernel_end = .);
  PROVIDE(heap_start = .);

  PROVIDE(global_pointer = .);
}
