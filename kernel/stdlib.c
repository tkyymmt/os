#include <stddef.h>
#include <stdint.h>

#include "heap.h"


void *malloc(size_t size)
{
	void *ptr;
	struct heap_blk_hdr *tmp;

	ptr = NULL;
	tmp = heap_blk_list;

	while (tmp != NULL) {
		if (tmp->in_use)
			goto next;

		if (tmp->size > size + sizeof(struct heap_blk_hdr)) {
			struct heap_blk_hdr *new;

			new = (struct heap_blk_hdr *)((uint32_t)tmp + sizeof(struct heap_blk_hdr) + size);
			new->next = tmp->next;
			new->prev = tmp;
			new->in_use = false;
			new->size = tmp->size - (size + sizeof(struct heap_blk_hdr));
			tmp->next = new;

			goto out;
		} else if (tmp->size == size) {
			goto out;
		} else {
			goto next;
		}

out:
		tmp->in_use = true;
		ptr = (void *)((uint32_t)tmp + sizeof(struct heap_blk_hdr));
		break;

next:
		tmp = tmp->next;
	}

	return ptr;
}

void free(void *ptr)
{
	struct heap_blk_hdr *hdr;

	hdr = (struct heap_blk_hdr *)((uint32_t)ptr - sizeof(struct heap_blk_hdr));
	hdr->in_use = false;

	if (hdr->next != NULL && !hdr->next->in_use) {
		hdr->size += (sizeof(struct heap_blk_hdr) + hdr->next->size);
		hdr->next = hdr->next->next;

		if (hdr->next != NULL)
			hdr->next->prev = hdr;
	}

	if (hdr->prev != NULL && !hdr->prev->in_use) {
		hdr->prev->next = hdr->next;
		hdr->prev->size += (sizeof(struct heap_blk_hdr) + hdr->size);

		if (hdr->next != NULL)
			hdr->next->prev = hdr->prev;
	}
}
