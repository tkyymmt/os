#ifndef HEAP_H
#define HEAP_H


#include <stddef.h>
#include <stdbool.h>


struct heap_blk_hdr {
	struct heap_blk_hdr *next;
	struct heap_blk_hdr *prev;
	bool in_use;
	size_t size; // without header size
};


extern struct heap_blk_hdr *heap_blk_list;


extern void init_heap();


#endif // HEAP_H
