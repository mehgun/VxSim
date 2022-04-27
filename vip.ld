_vxStart = ENTRY_SYMBOL + _VX_LMA_START;
ENTRY(_start)



SECTIONS
{
    .text (_VX_VMA_START) : AT(_VX_VMA_START + _VX_LMA_START)
    {
    _VX_START_TEXT = .;




    *(.text.entry)

    *(.text)

    *(.text.exit)

    *(.text.fast)
    *(.text.init)
    *(.text.unlikely .text.*_unlikely) *(.text.hot .text.hot.*)
    *(.text.*) *(.stub) *(.gnu.warning) *(.gnu.linkonce.t*)



    *(.text.startup) KEEP(*(.init)) KEEP(*(.fini))

    *(.sfpr)
    *(.glink)
    __wrs_kernel_rodata_start = .;
    *(.glue_7t) *(.glue_7) *(.rdata)
    *(.mips16.fn.*) *(.mips16.call.*) *(.reginfo)
    *(.rodata) *(.rodata.*) *(.gnu.linkonce.r*) *(.rodata1)
  }
  . = ALIGN(16);
  .ARM.extab : { *(.ARM.extab) *(.ARM.extab.*) *(.gnu.linkonce.armextab.*) }
  __exidx_start = .;
  .ARM.exidx : { *(.ARM.exidx) *(.ARM.exidx.*) *(.gnu.linkonce.armexidx.*) }
  __exidx_end = .;

  .eh_frame_hdr : {
    *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*)
  }
  .eh_frame : {
    __EH_FRAME_BEGIN__ = .;
    KEEP (*(.eh_frame)) *(.eh_frame.*)
    LONG (0)
  }
  . = ALIGN(16);
  .gcc_except_table : { *(.gcc_except_table .gcc_except_table.*) }
  .wrs_build_vars : {
    *(.wrs_build_vars)
  }
  . = ALIGN(16);
  .sdata2 : {
    _SDA2_BASE_ = . + 0x7ff0 ;
    *(.sdata2) *(.sbss2)
  }




  .toc ALIGN(0x100):
  {
      _TOC_BASE_ = . + 0x8000;
    *(.toc)
  }
  . = ALIGN(16);
   __wrs_kernel_rodata_end = .;
  etext = .; _etext = .;
  . = ALIGN(0x10000);
  . = ALIGN(_VX_DATA_ALIGN);
  .data :

  {
    _VX_START_DATA = .;
    *(.data)
    *(.data.fast)
    *(.data.init)
    . = ALIGN(16);
    wrs_kernel_data_set_start = .; _wrs_kernel_data_set_start = .;
    *(.data.set)
    wrs_kernel_data_set_end = .; _wrs_kernel_data_set_end = .;
    *(.data.*) *(.gnu.linkonce.d*) SORT(CONSTRUCTORS) *(.data1)
    . = ALIGN(8);

    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);

    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))


    *(.got.plt) *(.got) *(.dynamic)
    *(.got2)
    *(.lit8) *(.lit4)
  }




  . = ALIGN(16);


  .tdata : {
    *(.tdata) *(.tdata.*)
  }
  . = ALIGN(16);
  .tbss : {
    *(.tbss) *(.tbss.*)
  }





  . = ALIGN(16);
  _gp = ALIGN(16) + 0x7ff0;

  .sdata :
  {
    __global_pointer$ = . + 0x800;
    _SDA_BASE_ = . + 0x7ff0 ;
    *(.sdata) *(.sdata.*)
  }




  . = ALIGN(16);







  . += _VX_LMA_START;
  _TEXTLOCORE_LOAD_ADDR = .;


  .text.locore :
  AT(_TEXTLOCORE_LOAD_ADDR)
  {
    *(.text_locore)



  }
  . = ALIGN(_VX_DATA_ALIGN);


  .data.locore :
  {
    *(.data_locore)
  }
  . = . - _VX_LMA_START;



  edata = .; _edata = .;

  .sbss :
  {
    *(.sbss) *(.sbss.*) *(.scommon)
  }




  . = ALIGN(16);

  .bss :
  {
    *(.dynbss) *(.bss) *(.bss.*) *(COMMON) *(.gnu.linkonce.b*)
  }




  . = ALIGN(16);


  end = .; _end = .;

  PROVIDE (wrs_kernel_text_start = ADDR(.text));
  PROVIDE (_wrs_kernel_text_start = wrs_kernel_text_start);

  PROVIDE (__wrs_eh_frame_hdr_start = ADDR(.eh_frame_hdr));
  PROVIDE (__wrs_eh_frame_hdr_end = ADDR(.eh_frame_hdr) + SIZEOF(.eh_frame_hdr));

  PROVIDE (__wrs_eh_frame_start = ADDR(.eh_frame));
  PROVIDE (__wrs_eh_frame_end = ADDR(.eh_frame) + SIZEOF(.eh_frame));

  PROVIDE (wrs_kernel_text_end = ADDR(.text) + SIZEOF(.text));
  PROVIDE (_wrs_kernel_text_end = wrs_kernel_text_end);

  PROVIDE (wrs_kernel_data_start = ADDR(.data));
  PROVIDE (_wrs_kernel_data_start = wrs_kernel_data_start);

  PROVIDE (wrs_kernel_data_set_start = ADDR(.data_set));
  PROVIDE (_wrs_kernel_data_set_start = wrs_kernel_data_set_start);

  PROVIDE (wrs_kernel_data_set_end = ADDR(.data_set) + SIZEOF(.data_set));
  PROVIDE (_wrs_kernel_data_set_end = wrs_kernel_data_set_end);

  PROVIDE (wrs_kernel_tls_data_start = SIZEOF (.tdata) ? ABSOLUTE (ADDR (.tdata)) : SIZEOF (.tbss) ? ABSOLUTE (ADDR (.tbss)) : 0);
  PROVIDE (_wrs_kernel_tls_data_start = wrs_kernel_tls_data_start);
  PROVIDE (wrs_kernel_tls_data_size = SIZEOF (.tdata));
  PROVIDE (_wrs_kernel_tls_data_size = wrs_kernel_tls_data_size);


  PROVIDE (wrs_kernel_tls_bss_start = SIZEOF (.tbss) ? ABSOLUTE (ADDR (.tbss)) : 0);
  PROVIDE (_wrs_kernel_tls_bss_start = wrs_kernel_tls_bss_start);
  PROVIDE (wrs_kernel_tls_bss_end = wrs_kernel_tls_bss_start + SIZEOF(.tbss));
  PROVIDE (_wrs_kernel_tls_bss_end = wrs_kernel_tls_bss_end);

  PROVIDE (wrs_kernel_tls_bss_offset = wrs_kernel_tls_bss_start - wrs_kernel_tls_data_start);
  PROVIDE (_wrs_kernel_tls_bss_offset = wrs_kernel_tls_bss_offset);
  PROVIDE (wrs_kernel_tls_mem_size = wrs_kernel_tls_bss_end ? wrs_kernel_tls_bss_end - wrs_kernel_tls_data_start : wrs_kernel_tls_data_size);
  PROVIDE (_wrs_kernel_tls_mem_size = wrs_kernel_tls_mem_size);
  PROVIDE (wrs_kernel_tls_data_align = ALIGNOF (.tdata) > ALIGNOF (.tbss) ? ALIGNOF (.tdata) : ALIGNOF (.tbss));
  PROVIDE (_wrs_kernel_tls_data_align = wrs_kernel_tls_data_align);

  PROVIDE (wrs_kernel_data_end = edata);
  PROVIDE (_wrs_kernel_data_end = wrs_kernel_data_end);

  PROVIDE (wrs_kernel_bss_start = ADDR(.sbss));
  PROVIDE (_wrs_kernel_bss_start = wrs_kernel_bss_start);

  PROVIDE (wrs_kernel_bss_end = end);
  PROVIDE (_wrs_kernel_bss_end = wrs_kernel_bss_end);

  /DISCARD/ :
  {
  *(.note)
  *(.comment)
  *(.pdr)
  }

  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }




  .debug 0 : { *(.debug) }
  .line 0 : { *(.line) }

  .debug_srcinfo 0 : { *(.debug_srcinfo) }
  .debug_sfnames 0 : { *(.debug_sfnames) }

  .debug_aranges 0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }

  .debug_info 0 : { *(.debug_info) }
  .debug_abbrev 0 : { *(.debug_abbrev) }
  .debug_line 0 : { *(.debug_line) }
  .debug_frame 0 : { *(.debug_frame) }
  .debug_str 0 : { *(.debug_str) }
  .debug_loc 0 : { *(.debug_loc) }
  .debug_macinfo 0 : { *(.debug_macinfo) }

  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames 0 : { *(.debug_varnames) }
}
