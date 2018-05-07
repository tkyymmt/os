#include <stdint.h>

#include "mm.h"


/*
#include "stdio.h"
void do_divide_by_zero()
{
	printf("EXCEPTION: divide_by_zero");
	asm ("hlt");
}
*/

void do_page_fault()
{
	uint32_t vaddr;

	asm volatile("movl	%%cr2, %0" : "=r" (vaddr));

	alloc_page(vaddr);
} 
