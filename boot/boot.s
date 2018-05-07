.global _start
.code16

_start:
	cli

# enable A20 line
	mov		$0x2401, %ax
	int		$0x15

# load kernel
	xor		%bx, %bx
	mov		%bx, %es
	mov		$0x500, %bx
	mov		$0x0212, %ax
	mov		$0x1c01, %cx
	mov		$0x0100, %dx
	int		$0x13
/*
	movw	$0x0212, %ax
	addw	$0x2000, %bx
	incb	%ch
	movb	$0x01, %cl
	movb	$0x00, %dh
	int		$0x13

	movw	$0x0212, %ax
	addw	$0x2400, %bx
	movb	$0x01, %dh
	int		$0x13

	movw	$0x0202, %ax
	addw	$0x2400, %bx
	incb	%ch
	movb	$0x00, %dh
	int		$0x13
*/

# load GDT
	lgdt	gdtr

# enter protected mode
	movl	%cr0, %eax
	orl		$0x1, %eax
	movl	%eax, %cr0

# initialize segment registers
	movw	$0x10, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss

# initialize stack pointer
	movl	$0xfff0, %esp

# jump to kernel
	ljmp	$0x8, $0x500


gdt:
	.quad	0x0000000000000000	# null segment descriptor
	.quad	0x00CF9A000000FFFF	# code segment descriptor
	.quad	0x00CF92000000FFFF	# data segment descriptor

gdtr:
	.word	gdtr - gdt - 1
	.long	gdt
