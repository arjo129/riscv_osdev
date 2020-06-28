OUTPUT_ARCH("riscv")
/**
 * The entry point of our code will be labelled _entry in our
 * Assembly Code
 */ 
ENTRY(_entry)
/**
 * From the file qemu/hw/riscv/virt.c memory map we know that
 * DRAM is located at 0x80000000
 */ 
MEMORY
{
  ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 128M
}

PHDRS
{
  text PT_LOAD;
  data PT_LOAD;
  bss PT_LOAD;
}

SECTIONS
{
  /**
   * .text contains the executable code section
   */ 
  .text : {
     PROVIDE(_text_start = .);
     *(.text.init) *(.text .text.*)
     PROVIDE(_text_end = .);
  } >ram AT>ram :text /*Put the text inside the RAM*/

  /**
   * Globals
   */ 
  PROVIDE(_global_pointer = .);
  .rodata : {
    PROVIDE(_rodata_start = .);
    *(.rodata .rodata.*)
    PROVIDE(_rodata_end = .);
  } >ram AT>ram :text
  .data : {
    . = ALIGN(4096);
    PROVIDE(_data_start = .);
    *(.sdata .sdata.*) *(.data .data.*)
    PROVIDE(_data_end = .);
  } >ram AT>ram :data

  /**
   * The stack
   */  
  .bss : {
    PROVIDE(_bss_start = .);
    *(.sbss .sbss.*) *(.bss .bss.*)
    PROVIDE(_bss_end = .);
  } >ram AT>ram :bss

  PROVIDE(_memory_start = ORIGIN(ram));
  PROVIDE(_stack = _bss_end + 0x80000);
  PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));
  PROVIDE(_heap_start = _stack);
  PROVIDE(_heap_size = _memory_end - _stack);
 }
