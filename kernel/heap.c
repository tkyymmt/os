#include <stdint.h>

#include "heap.h"


struct heap_blk_hdr *heap_blk_list;


extern int end_of_kernel_image;


void init_heap()
{
	heap_blk_list = (struct heap_blk_hdr *)&end_of_kernel_image;
	heap_blk_list->next = NULL;
	heap_blk_list->prev = NULL;
	heap_blk_list->in_use = false;
	heap_blk_list->size = 0x10000 - (uint32_t)heap_blk_list - sizeof(struct heap_blk_hdr);
}
