OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386:i386)

ENTRY(main)


SECTIONS
{
	.text 0x500 : { *(.text.startup) *(.text) }
	.rodata	: { *(.rodata) *(.rodata.*) }
	.data : { *(.data) }
	.bss : { *(.bss) *(COMMON) }

	end_of_kernel_image = .;
}
