#include <stdbool.h>
#include <stdint.h>


struct pte {
	unsigned int flags : 9;
	unsigned int ignored : 3;
	unsigned int pf_nr : 20;
} __attribute__((packed));

struct pde {
	unsigned int flags : 9;
	unsigned int ignored : 3;
	unsigned int pt_addr : 20;
} __attribute__((packed));


static struct pte pg_table[256] __attribute__((aligned(4096)));
static struct pde pg_directory __attribute__((aligned(4096)));

static uint8_t pg_frame_bitmap[32];


static bool is_pf_set(int pf_nr)
{
	int idx, shift;

	idx = pf_nr / 8;
	shift = pf_nr % 8;

	return pg_frame_bitmap[idx] >> shift & 1;
}

static void set_pf_bitmap(int pf_nr)
{
	int idx, shift;

	idx = pf_nr / 8;
	shift = pf_nr % 8;

	pg_frame_bitmap[idx] |= 1 << shift;
}

static void clear_pf_bitmap(int pf_nr)
{
	int idx, shift;

	idx = pf_nr / 8;
	shift = pf_nr % 8;

	pg_frame_bitmap[idx] &= ~(1 << shift);
}

void alloc_page(uint32_t vaddr)
{
	int pf_nr;
	struct pte *pte;

	for (pf_nr = 16; pf_nr < 144; ++pf_nr) {
		if (!is_pf_set(pf_nr)) {
			set_pf_bitmap(pf_nr);
			break;
		}
	}

	pte = &pg_table[vaddr / 4096];

	pte->flags |= 1;
	pte->pf_nr = pf_nr;
}

static void invalidate_pages()
{
	asm volatile("movl	%cr3, %eax");
	asm volatile("movl	%eax, %cr3");
}

void take_pages(uint8_t pf_nrs[])
{
	int idx = 0;

	for (int i = 16; i < 144; i++, idx++) {
		if (pg_table[i].flags & 1) {
			pg_table[i].flags &= ~1;
			pf_nrs[idx] = pg_table[i].pf_nr;
		} else {
			pf_nrs[idx] = 0;
		}
	}

	invalidate_pages();
}

void set_pages(uint8_t pf_nrs[])
{
	int idx = 0;

	for (int i = 16; i < 144; i++, idx++) {
		if (pg_table[i].flags & 1)
			clear_pf_bitmap(pg_table[i].pf_nr);

		if (pf_nrs[idx]) {
			pg_table[i].flags |= 1;
			pg_table[i].pf_nr = pf_nrs[idx];
			set_pf_bitmap(pg_table[i].pf_nr);
		}
	}

	invalidate_pages();
}

void init_mm()
{
	// initialize page table
	for (int i = 0; i < 256; ++i) {
		if (i < 16 || i >= 144) {
			pg_table[i].flags = 0x103; // G, R/W, P
			pg_table[i].ignored = 0;
			pg_table[i].pf_nr = i;
			set_pf_bitmap(i);
		} else {
			pg_table[i].flags = 6; // U/S, R/W
			pg_table[i].ignored = 0;
			// clear_pf_bitmap(i);
		}
	}

	// initialize page directory
	pg_directory.flags = 7;
	pg_directory.ignored = 0;
	pg_directory.pt_addr = (uint32_t)pg_table >> 12 & 0xfffff;

	// not to update global pages
	asm volatile("movl	%%cr4, %%eax\n\t"
				 "orl	$0x80, %%eax\n\t"
				 "movl	%%eax, %%cr4"
				 ::: "%eax", "cc");

	// install page directory
	asm volatile("movl	%0, %%cr3" :: "r" (&pg_directory));

	// enable paging
	asm volatile("movl	%%cr0, %%eax\n\t"
				 "orl	$0x80000000, %%eax\n\t"
				 "movl	%%eax, %%cr0\n"
				 ::: "%eax", "cc");
}
