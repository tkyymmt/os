/*
.macro	exc_handler lbl, func
.global	\lbl
.extern \func
\lbl:
	pushal
	push	%ds
	push	%es
	push	%fs
	push	%gs

	mov		%ss, %ax
	mov		%ax, %ds
	mov		%ax, %es
	mov		%ax, %fs
	mov		%ax, %gs

	call	\func

	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal

	iret
.endm

exc_handler divide_by_zero do_divide_by_zero
*/

.macro	exc_handler_err lbl, func
.global	\lbl
.extern \func
\lbl:
	pushal
	push	%ds
	push	%es
	push	%fs
	push	%gs

	mov		%ss, %ax
	mov		%ax, %ds
	mov		%ax, %es
	mov		%ax, %fs
	mov		%ax, %gs

	call	\func

	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal

	add		$4, %esp

	iret
.endm

exc_handler_err page_fault do_page_fault


.extern	send_eoi

.macro	irq_handler lbl, func, irq_nr
.global	\lbl
.extern	\func
\lbl:
	pushal
	push	%ds
	push	%es
	push	%fs
	push	%gs

	mov		%ss, %ax
	mov		%ax, %ds
	mov		%ax, %es
	mov		%ax, %fs
	mov		%ax, %gs

	call	\func

	push	\irq_nr

	call	send_eoi

	add		$4, %esp

	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal

	iret
.endm

irq_handler irq_fdd do_fdd 6


.global syscall
.extern syscalls

syscall:
	pusha
	push	%ds
	push	%es
	push	%fs
	push	%gs

	mov		%ss, %edi
	mov		%edi, %ds
	mov		%edi, %es
	mov		%edi, %fs
	mov		%edi, %gs

	call	*syscalls(, %ebx, 4)
	mov		%eax, (0xffd8)

	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popa

	iret
