#include "asm.h"
#include "gdt.h"
#include "display.h"
#include "pic.h"
#include "keyboard.h"
#include "fdd.h"
#include "idt.h"
#include "dma.h"
#include "fs.h"
#include "mm.h"
#include "heap.h"
#include "pm.h"


void main()
{
	init_display();
	init_gdt();
	init_idt();
	init_pic();
	init_keyboard();
	init_fdd();
	init_dma();
	init_fs();
	init_mm();
	init_heap();
	init_pm();

	sti();

	asm volatile ("mov		$0x23, %ax\n"
				  "mov		%ax, %ds\n"
				  "mov		%ax, %es\n"
				  "mov		%ax, %fs\n"
				  "mov		%ax, %gs\n"
				  "push		$0x23\n"
				  "push		$0x8fff0\n"
				  "pushfl\n"
				  "push		$0x1b\n"
				  "push		$0x10000\n"
				  "iret");
}
