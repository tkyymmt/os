OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386:i386)

ENTRY(main)


SECTIONS
{
	.segment 0x10000 :
	{
	  *(.entry)
	  *(.text)
	  *(.rodata) *(.rodata.*)
	  *(.data)
	  *(.bss) *(COMMON)
	}
}
